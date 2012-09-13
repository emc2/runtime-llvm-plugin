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
#ifndef _GC_TYPE_TRACE_GEN_H_
#define _GC_TYPE_TRACE_GEN_H_

#include "GCTypeVisitors.h"
#include "llvm/Module.h"
#include "llvm/Value.h"
#include "llvm/Instructions.h"

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
public:
  IndexState();

  void pushIndex(const llvm::Value* v);
  void pushIndex();
  const llvm::Value* popIndex();
  void nextField();
  llvm::ArrayRef<const llvm::Value*> indexes() const;
};

class GCTypeCopyGenVisitor : public GCTypeVisitor {
private:
  struct IndexState idxstate;
  const llvm::Value* const src;
  const llvm::Value* const dst;

  const llvm::Value* getIndexedSrc();
  const llvm::Value* getIndexedDst();
protected:
  const llvm::Value* const gcctx;
  const llvm::BasicBlock* BB;
public:
  GCTypeCopyGenVisitor(const llvm::Value* src,
		       const llvm::Value* dst,
		       const llvm::Value* gcctx,
		       const llvm::BasicBlock* BB);

  virtual bool begin(const StructGCType*);
  virtual bool begin(const FuncPtrGCType*);
  virtual bool begin(const ArrayGCType*);

  virtual void end(const StructGCType*);
  virtual void end(const ArrayGCType*);

  virtual void visit(const NativePtrGCType*);
  virtual void visit(const GCPtrGCType*);
  virtual void visit(const PrimGCType*);

  virtual void visit(const NativePtrGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
  virtual void visit(const GCPtrGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
  virtual void visit(const PrimGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
  virtual void visit(const FuncPtrGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
};

/*!
 * This is a subclass of GCTypeVisitor which creates GC copy code for
 * a given GC type.  This is designed to fill in the body of a
 * function with instructions that copy an object from the old heap to
 * the new heap.
 *
 * This is used in the implementation of copying garbage collection.
 *
 * \brief A visitor which creates copy code.
 */
class GCTypeCopyCodeGen : public GCTypeCopyGenVisitor {
public:
  virtual void visit(const NativePtrGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
  virtual void visit(const GCPtrGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
  virtual void visit(const PrimGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
  virtual void visit(const FuncPtrGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
};

/*!
 * This is a subclass of GCTypeVisitor which creates GC syncronization
 * code for a given GC type.  This is designed to fill in the body of
 * a function with instructions that propagate changes from the source
 * heap to the destination.
 *
 * This is used in the implementation of concurrent and semispace
 * garbage collection.
 *
 * \brief A visitor which creates copy code.
 */
class GCTypeSyncCodeGen  : public GCTypeCopyGenVisitor {
public:
  virtual void visit(const NativePtrGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
  virtual void visit(const GCPtrGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
  virtual void visit(const PrimGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
  virtual void visit(const FuncPtrGCType* gcty,
		     const llvm::Value* src,
		     const llvm::Value* dst) = 0;
};

#endif
