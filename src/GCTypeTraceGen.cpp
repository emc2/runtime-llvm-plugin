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

#define __STDC_LIMIT_MACROS 1
#define __STDC_CONSTANT_MACROS 1
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "GCType.h"
#include "GCTypeTraceGen.h"

void getSrcDst(llvm::BasicBlock* const BB,
	       struct IndexState& ctx,
	       llvm::Value*& src,
	       llvm::Value*& dst) {
  if(NULL == ctx.loopidx) {
    llvm::LLVMContext& C = BB->getContext();
    llvm::IntegerType* const int32ty = llvm::Type::getInt32Ty(C);
    llvm::Value* indexes[2] = {
      llvm::ConstantInt::get(int32ty, 0, false),
      llvm::ConstantInt::get(int32ty, ctx.idx++, false)
    };

    src = llvm::GetElementPtrInst::Create(ctx.src, indexes, "", BB);
    dst = llvm::GetElementPtrInst::Create(ctx.dst, indexes, "", BB);
  }
  else {
    src = ctx.src;
    dst = ctx.dst;
  }
}

bool GCTypeCopyGCTraceGen::begin(const StructGCType* const gcty,
				 struct IndexState& ctx,
				 struct IndexState& parent) {
  getSrcDst(BB, parent, ctx.src, ctx.dst);
  ctx.loopidx = NULL;
  ctx.idx = 0;

  return descend(gcty);
}

// Create a loop that iterates the elements of the array, and then
// push the loop index variable.  Note: this assumes arrays are short,
// or else not being clustered.
bool GCTypeCopyGCTraceGen::begin(const ArrayGCType* const gcty,
				 struct IndexState& ctx,
				 struct IndexState& parent) {
  bool out = descend(gcty);

  if(out) {
    llvm::LLVMContext& C = BB->getContext();
    llvm::Function* const F = BB->getParent();
    llvm::BasicBlock* const loopBB = llvm::BasicBlock::Create(C, "", F, BB);
    llvm::IntegerType* const int64ty = llvm::Type::getInt64Ty(C);
    llvm::Value* const constzero = llvm::ConstantInt::get(int64ty, 0, false);
    llvm::PHINode* const loopidx =
      llvm::PHINode::Create(int64ty, 2, "", loopBB);

    // Wire in the incoming zero
    loopidx->addIncoming(constzero, BB);
    // Polish off the old block
    llvm::BranchInst::Create(loopBB, BB);
    // Make the loop block the insert block
    BB = loopBB;

    // Create the new contex
    llvm::IntegerType* const int32ty = llvm::Type::getInt32Ty(C);

    if(NULL == parent.loopidx) {
      llvm::Value* indexes[3] = {
	llvm::ConstantInt::get(int32ty, 0, false),
	llvm::ConstantInt::get(int32ty, parent.idx++, false),
	loopidx
      };

      ctx.src = llvm::GetElementPtrInst::Create(parent.src, indexes,
						"", loopBB);
      ctx.dst = llvm::GetElementPtrInst::Create(parent.dst, indexes,
						"", loopBB);
    }
    else {
      llvm::Value* indexes[2] = {
	llvm::ConstantInt::get(int32ty, 0, false),
	loopidx
      };

      ctx.src = llvm::GetElementPtrInst::Create(parent.src, indexes,
						"", loopBB);
      ctx.dst = llvm::GetElementPtrInst::Create(parent.dst, indexes,
						"", loopBB);
    }

    ctx.loopidx = loopidx;
    ctx.idx = 0;
  }

  return out;
}

bool GCTypeCopyGCTraceGen::begin(const FuncPtrGCType* const gcty,
				 struct IndexState&,
				 struct IndexState& parent) {
  llvm::Value* src;
  llvm::Value* dst;

  getSrcDst(BB, parent, src, dst);
  visit(gcty, src, dst);

  return false;
}


void GCTypeCopyGCTraceGen::end(const ArrayGCType* gcty,
			       struct IndexState& ctx,
			       struct IndexState&) {
  llvm::LLVMContext& C = BB->getContext();
  llvm::Function* const F = BB->getParent();
  llvm::BasicBlock* const newBB = llvm::BasicBlock::Create(C, "", F, BB);
  llvm::PHINode* const loopidx = ctx.loopidx;
  llvm::IntegerType* const int64ty = llvm::Type::getInt64Ty(C);
  llvm::Value* const constone = llvm::ConstantInt::get(int64ty, 1, false);
  llvm::Value* const inc =
    llvm::BinaryOperator::CreateAdd(loopidx, constone, "", BB);
  // Get the array's length
  // XXX need to make provisions for non-clustered, yet variable
  // sized arrays.
  llvm::Value* const len =
    llvm::ConstantInt::get(int64ty, gcty->getNumElems(), false);
  llvm::Value* const cond =
    new llvm::ICmpInst(*BB, llvm::CmpInst::ICMP_ULT, inc, len, "");

  // Add the incoming value for this block (the incremented variable)
  loopidx->addIncoming(inc, BB);
  // Add the end of the loop
  llvm::BranchInst::Create(BB, newBB, cond, BB);
  // Set the new block as the insert point
  BB = newBB;
}

void GCTypeCopyGCTraceGen::visit(const NativePtrGCType* const gcty,
				 struct IndexState& ctx) {
  llvm::Value* src;
  llvm::Value* dst;

  getSrcDst(BB, ctx, src, dst);
  visit(gcty, src, dst);
}

void GCTypeCopyGCTraceGen::visit(const GCPtrGCType* const gcty,
				 struct IndexState& ctx) {
  llvm::Value* src;
  llvm::Value* dst;

  getSrcDst(BB, ctx, src, dst);
  visit(gcty, src, dst);
}

void GCTypeCopyGCTraceGen::visit(const PrimGCType* const gcty,
				 struct IndexState& ctx) {
  llvm::Value* src;
  llvm::Value* dst;

  getSrcDst(BB, ctx, src, dst);
  visit(gcty, src, dst);
}

// XXX dummy implementation for testing
bool GCTypeCopyGCTraceGen::descend(const StructGCType* ty) {
  return true;
}

// XXX dummy implementation for testing
bool GCTypeCopyGCTraceGen::descend(const ArrayGCType* ty) {
  return true;
}

// XXX dummy implementation for testing
void GCTypeCopyGCTraceGen::visit(const NativePtrGCType* gcty,
                                 const llvm::Value* src,
                                 const llvm::Value* dst) {}

// XXX dummy implementation for testing
void GCTypeCopyGCTraceGen::visit(const GCPtrGCType* gcty,
                                 const llvm::Value* src,
                                 const llvm::Value* dst) {}

// XXX dummy implementation for testing
void GCTypeCopyGCTraceGen::visit(const PrimGCType* gcty,
                                 const llvm::Value* src,
                                 const llvm::Value* dst) {}

// XXX dummy implementation for testing
void GCTypeCopyGCTraceGen::visit(const FuncPtrGCType* gcty,
                                 const llvm::Value* src,
                                 const llvm::Value* dst) {}
