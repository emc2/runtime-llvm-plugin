/* Copyright (c) 2012 Eric McCorkle.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */
#ifndef _TRACE_GENERATOR_H_
#define _TRACE_GENERATOR_H_

#include "GenTypeVisitors.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Instructions.h"

/* These visitors generate code for garbage collection tracing,
 * copying, and synchronization.
 *
 * We have the notion of a "mark" function, which takes a pointer to a
 * garbage collected object, and ensures that it will eventually be
 * visited by the GC trace, either by enqueueing it, or by directly
 * recursing.
 *
 * Tracing code is for non-copying garbage collection.  Trace
 * functions will call the mark function on every GC pointer in the
 * object.
 *
 * Copy code is for copying garbage collection.  Copy functions will
 * copy every scalar field over to the destination heap, and call the
 * mark function on every GC pointer in the object.
 *
 * Synchronization code is for concurrent garbage collection with
 * write barriers (ie semispace or concurrent).  This function takes
 * an index at which to synchronize, and ensures that the new heap
 * reflects the old.
 */

struct IndexState {
  llvm::Value* src;
  llvm::Value* dst;
  llvm::PHINode* loopidx;
  unsigned idx;
};

/*!
 * This class is an abstract base class for code generation in copying
 * GC.  It implements the work of traversing a type and generating the
 * necessary pointer arithmetic and loops.  It exposes four new visit
 * functions, each of which takes a source and destination pointer.
 *
 * Subclasses should implement the four new visit functions with code
 * generation for a particular case.
 *
 * \brief A base class for generating code for copying GC.
 */
class CopyGCTraceGen : public GenTypeCtxVisitor<struct IndexState> {
protected:
  /*!
   * \brief The current GC context value.
   */
  llvm::Value* const gcctx;

  /*!
   * \brief The current basic block.
   */
  llvm::BasicBlock* BB;
public:
  /*!
   * Initialize with a source value, destination value, context value,
   * and basic block.
   *
   * \brief Initialize with source and destination pointers, context,
   *        and basic block LLVM values.
   * \param src The source pointer LLVM value.
   * \param dst The destination pointer LLVM value.
   * \param gccty The GC context LLVM value.
   * \param BB The LLVM basic block.
   */
  CopyGCTraceGen(const llvm::Value* src,
                 const llvm::Value* dst,
                 const llvm::Value* gcctx,
                 const llvm::BasicBlock* BB);

  // These functions will implement generation of the "skeleton" of
  // getelementptr and loops that will traverse the type.
  virtual const llvm::Value* initial(const GenType* ty);

  virtual bool begin(const StructGenType* ty, struct IndexState&,
		     struct IndexState&);
  virtual bool begin(const FuncPtrGenType* ty, struct IndexState&,
		     struct IndexState&);
  virtual bool begin(const ArrayGenType* ty, struct IndexState&,
		     struct IndexState&);

  virtual void end(const ArrayGenType* ty, struct IndexState&,
		   struct IndexState&);

  virtual void visit(const NativePtrGenType* ty, struct IndexState&);
  virtual void visit(const GCPtrGenType* ty, struct IndexState&);
  virtual void visit(const PrimGenType* ty, struct IndexState&);

  virtual bool descend(const StructGenType* ty) = 0;
  virtual bool descend(const ArrayGenType* ty) = 0;

  /*!
   * This function should generate copy code for a native pointer
   * field, given a source and destination value.  The source and
   * destination are indexed pointer values that can be directly
   * dereferenced to get the desired address for reading and writing.
   *
   * \brief Generate code for a native pointer.
   * \param gcty The type of the field to copy.
   * \param src An LLVM value with the source address.
   * \param dst An LLVM value with the destination address.
   */
  virtual void visit(const NativePtrGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;

  /*!
   * This function should generate copy code for a GC pointer field,
   * given a source and destination value.  The source and destination
   * are indexed pointer values that can be directly dereferenced to
   * get the desired address for reading and writing.
   *
   * \brief Generate code for a GC pointer.
   * \param gcty The type of the field to copy.
   * \param src An LLVM value with the source address.
   * \param dst An LLVM value with the destination address.
   */
  virtual void visit(const GCPtrGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;

  /*!
   * This function should generate copy code for a function pointer
   * field, given a source and destination value.  The source and
   * destination are indexed pointer values that can be directly
   * dereferenced to get the desired address for reading and writing.
   *
   * \brief Generate code for a function pointer.
   * \param gcty The type of the field to copy.
   * \param src An LLVM value with the source address.
   * \param dst An LLVM value with the destination address.
   */
  virtual void visit(const FuncPtrGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
  /*!
   * This function should generate copy code for a primitive type
   * field, given a source and destination value.  The source and
   * destination are indexed pointer values that can be directly
   * dereferenced to get the desired address for reading and writing.
   *
   * \brief Generate code for a primitive type.
   * \param gcty The type of the field to copy.
   * \param src An LLVM value with the source address.
   * \param dst An LLVM value with the destination address.
   */
  virtual void visit(const PrimGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
};

/*!
 * This is a subclass of GenTypeVisitor which creates GC copy code for
 * a given GC type.  This is designed to fill in the body of a
 * function with instructions that copy an object from the old heap to
 * the new heap.
 *
 * This is used in the implementation of copying garbage collection.
 *
 * \brief A visitor which creates copy code.
 */
class CopyCodeGen : public CopyGCTraceGen {
public:
  virtual bool descend(const StructGenType* ty);
  virtual bool descend(const ArrayGenType* ty);
  virtual void visit(const NativePtrGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst);
  virtual void visit(const GCPtrGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst);
  virtual void visit(const PrimGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst);
  virtual void visit(const FuncPtrGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst);
};

/*!
 * This is a subclass of GenTypeVisitor which creates GC syncronization
 * code for a given GC type.  This is designed to fill in the body of
 * a function with instructions that propagate changes from the source
 * heap to the destination.
 *
 * This is used in the implementation of concurrent and semispace
 * garbage collection.
 *
 * \brief A visitor which creates copy code.
 */
class SyncCodeGen  : public CopyGCTraceGen {
public:
  virtual bool descend(const StructGenType* ty);
  virtual bool descend(const ArrayGenType* ty);
  virtual void visit(const NativePtrGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst);
  virtual void visit(const GCPtrGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst);
  virtual void visit(const PrimGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst);
  virtual void visit(const FuncPtrGenType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst);
};

#endif
