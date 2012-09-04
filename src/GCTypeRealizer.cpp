/* Copyright (c) 2012 Eric McCorkle. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define __STDC_LIMIT_MACROS 1
#define __STDC_CONSTANT_MACROS 1
#include <string.h>
#include "TypeBuilder.h"
#include "GCTypeRealizer.h"
#include "llvm/DerivedTypes.h"

GCTypeRealizer::GCTypeRealizer(llvm::Module& M) : M(M) {}

bool GCTypeRealizer::begin(const StructGCType* const gcty,
			   TypeBuilder*& ctx,
			   TypeBuilder*&) {
  ctx = new StructTypeBuilder(gcty);

  return true;
}

bool GCTypeRealizer::begin(const FuncPtrGCType* const gcty,
			   TypeBuilder*& ctx,
			   TypeBuilder*&) {
  ctx = new FuncPtrTypeBuilder(gcty);

  return true;
}

bool GCTypeRealizer::begin(const ArrayGCType* const gcty,
			   TypeBuilder*& ctx,
			   TypeBuilder*&) {
  ctx = new ArrayTypeBuilder(gcty);

  return true;
}

void GCTypeRealizer::end(const StructGCType*,
			 TypeBuilder*& ctx,
			 TypeBuilder*& parent) {
  parent->add(ctx->build(M));
  delete ctx;
  ctx = NULL;
}

void GCTypeRealizer::end(const FuncPtrGCType*,
			 TypeBuilder*& ctx,
			 TypeBuilder*& parent) {
  parent->add(ctx->build(M));
  delete ctx;
  ctx = NULL;
}

void GCTypeRealizer::end(const ArrayGCType*,
			 TypeBuilder*& ctx,
			 TypeBuilder*& parent) {
  parent->add(ctx->build(M));
  delete ctx;
  ctx = NULL;
}

// Native pointers are essentially opaque values to us
void GCTypeRealizer::visit(const NativePtrGCType* const gcty,
			   TypeBuilder*& ctx) {
  llvm::Type* const llvmty =
    llvm::PointerType::getUnqual(gcty->getElemTy());
  ctx->add(llvmty);
}

void GCTypeRealizer::visit(const GCPtrGCType* const gcty,
			   TypeBuilder*& ctx) {
  // XXX parameterize the generator by the GC type we want to generate
  llvm::Type* const ptrty = llvm::PointerType::getUnqual(gcty->getElemTy());
  llvm::Type* const llvmty = llvm::ArrayType::get(ptrty, 2);

  ctx->add(llvmty);
}

// Primitive types are likewise opaque values
void GCTypeRealizer::visit(const PrimGCType* const gcty,
			   TypeBuilder*& ctx) {
  ctx->add(gcty->getLLVMType());
}
