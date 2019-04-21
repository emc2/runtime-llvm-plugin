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
#include "TypeRealizer.h"
#include "llvm/IR/DerivedTypes.h"

/*
TypeBuilder* TypeRealizer::initial(const GenType* ty) {
  // If we're building a type that has a direct GC representation (ie
  // a structure or an array), then set the top-level context to NULL,
  // which will result in end for those types writing their result
  // contexts into the top-level.
  switch(ty->getTypeID()) {
  case GenType::StructTypeID:
  case GenType::ArrayTypeID:
    return NULL;
  // Otherwise, just wrap the type in a single-field structure.
  default:
    // XXX possibly need to name the structure
    return new StructTypeBuilder(1, "", false);
  }
}
*/
bool TypeRealizer::begin(const StructGenType* const gcty,
			   TypeBuilder*& ctx,
			   TypeBuilder*&) {
  ctx = new StructTypeBuilder(gcty);

  return true;
}

bool TypeRealizer::begin(const FuncPtrGenType* const gcty,
                         TypeBuilder*& ctx,
                         TypeBuilder*&) {
  ctx = new FuncPtrTypeBuilder(gcty);

  return true;
}

bool TypeRealizer::begin(const ArrayGenType* const gcty,
                         TypeBuilder*& ctx,
                         TypeBuilder*&) {
  ctx = new ArrayTypeBuilder(gcty);

  return true;
}

// Both structures and arrays might just be handed down to their
// parents.
void TypeRealizer::end(const StructGenType*,
                       TypeBuilder*& ctx,
                       TypeBuilder*& parent) {
  if(NULL != parent) {
    llvm::Type* ty = ctx->build(M);
    parent->add(ty);
    delete ctx;
    ctx = NULL;
  }
  else {
    parent = ctx;
    ctx = NULL;
  }
}

void TypeRealizer::end(const ArrayGenType*,
                       TypeBuilder*& ctx,
                       TypeBuilder*& parent) {
  if(NULL != parent) {
    llvm::Type* ty = ctx->build(M);
    parent->add(ty);
    delete ctx;
    ctx = NULL;
  }
  else {
    parent = ctx;
    ctx = NULL;
  }
}

void TypeRealizer::end(const FuncPtrGenType*,
                       TypeBuilder*& ctx,
                       TypeBuilder*& parent) {
  llvm::Type* ty = ctx->build(M);
  parent->add(ty);
  delete ctx;
  ctx = NULL;
}

// Native pointers are essentially opaque values to us
void TypeRealizer::visit(const NativePtrGenType* const gcty,
                         TypeBuilder*& ctx) {
  llvm::Type* const llvmty =
    llvm::PointerType::getUnqual(gcty->getElemTy());
  ctx->add(llvmty);
}

void TypeRealizer::visit(const GCPtrGenType* const gcty,
                         TypeBuilder*& ctx) {
  // XXX parameterize the generator by the GC type we want to generate
  llvm::Type* const ptrty = llvm::PointerType::getUnqual(gcty->getElemTy());
  llvm::Type* llvmty;

  if(params.doublePtrs)
    llvmty = llvm::ArrayType::get(ptrty, 2);
  else
    llvmty = ptrty;

  ctx->add(llvmty);
}

// Primitive types are likewise opaque values
void TypeRealizer::visit(const PrimGenType* const gcty,
                          TypeBuilder*& ctx) {
  llvm::Type* const ty = gcty->getLLVMType();
  ctx->add(ty);
}

const llvm::Type* TypeRealizer::realize(const GenType* const ty,
                                        const llvm::StringRef name) {
  TypeBuilder* builder = new StructTypeBuilder(1, name, true);
  ty->accept(*this, builder);
  const llvm::Type* const out = builder->build(M);
  delete builder;

  return out;
}
