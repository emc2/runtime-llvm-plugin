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
#include "TypeBuilder.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Module.h"

StructTypeBuilder::StructTypeBuilder(unsigned nfields,
				     const llvm::StringRef tyname,
				     bool packed) :
  TypeBuilder(), field(0), tyname(tyname), packed(packed),
  nfields(nfields), fields(new llvm::Type*[nfields]) {}

StructTypeBuilder::StructTypeBuilder(const StructGCType* const gcty,
				     const llvm::StringRef tyname) :
  TypeBuilder(), field(0), tyname(tyname), packed(gcty->isPacked()),
  nfields(gcty->numFields()), fields(new llvm::Type*[gcty->numFields()]) {}

StructTypeBuilder::~StructTypeBuilder() {
  delete[] fields;
}

void StructTypeBuilder::add(llvm::Type* const ty) {
  fields[field++] = ty;
}

llvm::Type* StructTypeBuilder::build(llvm::Module& M) {
  llvm::ArrayRef<llvm::Type*> fieldarr(fields, nfields);
  llvm::StructType* outty = M.getTypeByName(tyname);

  if(NULL == outty)
    outty = llvm::StructType::create(M.getContext(), tyname);

  outty->setBody(fieldarr, packed);
}

ArrayTypeBuilder::ArrayTypeBuilder(const unsigned length) :
  length(length), elemty(NULL) {}

ArrayTypeBuilder::ArrayTypeBuilder(const ArrayGCType* const gcty) :
  length(gcty->getNumElems()), elemty(NULL) {}

void ArrayTypeBuilder::add(llvm::Type* const ty) {
  elemty = ty;
}

llvm::Type* ArrayTypeBuilder::build(llvm::Module&) {
  return llvm::ArrayType::get(elemty, length);
}

FuncPtrTypeBuilder::FuncPtrTypeBuilder(const unsigned nparams,
				       const bool vararg) :
  nparams(nparams), vararg(vararg), retty(NULL), param(0),
  params(new llvm::Type*[nparams]) {}

FuncPtrTypeBuilder::FuncPtrTypeBuilder(const FuncPtrGCType* gcty) :
  nparams(gcty->numParams()), vararg(gcty->isVararg()), retty(NULL),
  param(0), params(new llvm::Type*[gcty->numParams()]) {}

FuncPtrTypeBuilder::~FuncPtrTypeBuilder() {
  delete[] params;
}

void FuncPtrTypeBuilder::add(llvm::Type* const ty) {
  if(NULL != retty)
    params[param++] = ty;
  else
    retty = ty;
}

llvm::Type* FuncPtrTypeBuilder::build(llvm::Module&) {
  llvm::ArrayRef<llvm::Type*> paramarr(params, nparams);
  llvm::Type* functy = llvm::FunctionType::get(retty, paramarr, vararg);

  return llvm::PointerType::getUnqual(functy);
}
