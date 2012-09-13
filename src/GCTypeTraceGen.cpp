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

#include "GCTypeTraceGen.h"

bool GCTypeCopyGenVisitor::begin(const StructGCType*) {
  idxstate.pushIndex();

  return true;
}

// Create a loop that iterates the elements of the array, and then
// push the loop index variable.  Note: this assumes arrays are short,
// or else not being clustered.
bool GCTypeCopyGenVisitor::begin(const ArrayGCType* gcty) {
  llvm::Context& C = BB->getContext();
  llvm::Function* const F = BB->getParent();
  llvm::BasicBlock* const loopBB = llvm::BasicBlock::Create(C, "", F, BB);
  llvm::PHINode* const loopidx;

  // Polish off the old block
  llvm::BranchInst::Create(loopBB, BB);
  // Make the loop block the insert block
  BB = loopBB;

  // Push the 
  idxstate.pushIndex(loopidx);

  return true;
}

bool GCTypeCopyGenVisitor::begin(const FuncPtrGCType*) {
  const llvm::Value* const idxsrc = getIndexedSrc();
  const llvm::Value* const idxdst = getIndexedDst();

  visit(gcty, idxsrc, idxdst);

  return false;
}


void GCTypeCopyGenVisitor::end(const StructGCType*) {
  idxstate.popIndex();
}

void GCTypeCopyGenVisitor::end(const ArrayGCType* gcty) {
  llvm::BasicBlock* const newBB = llvm::BasicBlock::Create(C, "", F, loopBB);
  llvm::PHINode* const loopidx =
    llvm::cast<llvm::PHINode*>(idxstate.popIndex());
  // XXX not done here
  const llvm::Value* const inc =
    LLVM::AddInst();
  const llvm::Value* const len;
  const llvm::Value* const cond = llvm::ICmpInst(BB, inc, len);

  // Add the incoming value for this block (the incremented variable)
  loopidx->addIncoming(inc, BB);
  // Add the end of the loop
  llvm::BranchInst::Create(BB, newBB, cond, BB);
  // Set the new block as the insert point
  BB = newBB;
}

void GCTypeCopyGenVisitor::visit(const NativePtrGCType* const gcty) {
  const llvm::Value* const idxsrc = getIndexedSrc();
  const llvm::Value* const idxdst = getIndexedDst();

  visit(gcty, idxsrc, idxdst);
}

void GCTypeCopyGenVisitor::visit(const GCPtrGCType* const gcty) {
  const llvm::Value* const idxsrc = getIndexedSrc();
  const llvm::Value* const idxdst = getIndexedDst();

  visit(gcty, idxsrc, idxdst);
}

void GCTypeCopyGenVisitor::visit(const PrimGCType* const gcty) {
  const llvm::Value* const idxsrc = getIndexedSrc();
  const llvm::Value* const idxdst = getIndexedDst();

  visit(gcty, idxsrc, idxdst);
}
