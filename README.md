# runtime-llvm-plugin

LLVM Plugins to generate runtime system glue from the output of the
Simple IR code generator.


## Roadmap

We get an LLVM Module with the following:

* All GC'ed types are opaque types.

* GC Alloc is an intrinsic function, as is GC manipulation.

* For now, assume that GC safepoints and GC logging are already there.
* We can add them later.

* Accesses to fields of GC objects use accessor functions that are
  declared, but not defined.

* Globals representing the GC type information have been declared, but
  not defined.



Translation goes roughly like this:

* Parse the metadata into usable structures. (Done, mostly)

* Translate all GC types into a concrete representation.  Note:
  sometimes this involves translating pointers to GC objects into
  double-pointers. (Partially implemented)

* Generate the GC trace functions, headers, and type information
  structures.  Replace all of the opaque types with the concrete
  types.  (Partially implemented)

* Generate root-set arrays for globals.  Make sure that all local
  pointers to GC objects are also in the root set.

* Generate the accessor functions.  We can mark these "always inline"
  and let LLVM take care of the inlining for us.

* Rewrite GC alloc intrinsics to use a raw allocator and fill in the
  GC header data.

* Do any optimizations, like elimination of duplicated logging.



Following this, we have a program in the following format:

* A safepoint function that takes and returns an opaque context object.

* A raw GC allocator function that hands back pointers to N bytes,
  taking an opaque context object as an argument.

* (Possibly) a GC logging function that takes a pointer to an object
  header and an opaque context object.

* The object headers have pointers to type descriptors, which in turn
  describes the format of the objects enough to parse them.

None of this depends in any way on the actual types of objects
anymore.  The garbage collectors have all the information they need in
the type descriptors.  Given a pointer to an object, the collector can
trace it.

The GC allocator works entirely on information in the context
structure (probably by bumping pointers) to get a raw i8 pointer,
which it then casts to the right type pointer, fills in the object
header, and hands off to the program.

The logging and root set functions work entirely on object headers and
contexts.

The safepoint function works entirely on the context.

Therefore, we can simply define the context type and the logging,
allocation, and safepoint function in standard LLVM IR and let LLVM
take care of inlining them.

If the runtime system is completely done in LLVM (or compiled into
it), then we can merge the entire program with the runtime system and
let the optimizer have a field day.  Make sure we get the right
volatility, capture, and other flags set right though!!


# Future

## Closures:

It might be possible to wrangle LLVM to do GC-allocated stack frames.
There is a "naked" flag for functions, which omits the usual
entry/exit bits.  There are also calling conventions.

An intrinsic could be used to set the frame, which is passed as an
argument to the function (this also does currying rather nicely).

A pass could rewrite functions with particular calling conventions
into frame allocation, and change their call-sites to pass in the
frame.  Alternately, mark the functions "naked", rename them, and
build wrapper functions.

This practically begs for an optimization pass to look at call graphs
and flatten frames (Appel has a paper about this).

You can also rewrite recursive functions that have no escaping local
state to use classic stacks, which you can allocate from GC.

With this scheme, it then becomes ridiculously easy to implement
first-class continuations using LLVM noreturn functions.  A
continuation is just a function and a frame.

It makes sense in this scheme to rewrite first-class function pointers
as function + frame pointer structures.  Curried functions can be
implemented by stashing args in the frame, and outputting wrapper
functions which pull them out and then call the original function.


## Transactional memory:

Commit and rollback strategies could be implemented using this
compilation style.  With safepoints, it's also possible to do
combinations that previously weren't possible (eg. eager writes, lazy
conflict detection)

The intrinsics should take a RISC approach: give the basic operations
one uses to implement TM.  Don't try to look like TM.
