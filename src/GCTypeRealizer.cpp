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
