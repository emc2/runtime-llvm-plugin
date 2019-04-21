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
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Module.h"

StructTypeBuilder::StructTypeBuilder(unsigned nfields,
				     const llvm::StringRef tyname,
				     bool packed) :
  field(0), nfields(nfields), fields(new llvm::Type*[nfields]),
  tyname(tyname), packed(packed) {}

StructTypeBuilder::StructTypeBuilder(const StructGenType* const gcty,
				     const llvm::StringRef tyname) :
  TypeBuilder(), field(0), nfields(gcty->numFields()),
  fields(new llvm::Type*[gcty->numFields()]),
  tyname(tyname), packed(gcty->isPacked()) {}

StructTypeBuilder::~StructTypeBuilder() {
  delete[] fields;
}

void StructTypeBuilder::add(llvm::Type* const ty) {
  fields[field++] = ty;
}

llvm::Type* StructTypeBuilder::build(llvm::Module& M) {
  llvm::ArrayRef<llvm::Type*> fieldarr(fields, nfields);
  llvm::StructType* outty;

  if("" != tyname) {
    outty = M.getTypeByName(tyname);

    if(NULL == outty)
      outty = llvm::StructType::create(M.getContext(), fieldarr,
				       tyname, packed);
    else
      outty->setBody(fieldarr, packed);
  }
  else
    outty = llvm::StructType::create(M.getContext(), fieldarr);

  return outty;
}

ArrayTypeBuilder::ArrayTypeBuilder(const unsigned length) :
  elemty(NULL), length(length) {}

ArrayTypeBuilder::ArrayTypeBuilder(const ArrayGenType* const gcty) :
  elemty(NULL), length(gcty->getNumElems()) {}

void ArrayTypeBuilder::add(llvm::Type* const ty) {
  elemty = ty;
}

llvm::Type* ArrayTypeBuilder::build(llvm::Module&) {
  return llvm::ArrayType::get(elemty, length);
}

FuncPtrTypeBuilder::FuncPtrTypeBuilder(const unsigned nparams,
				       const bool vararg) :
  param(0), nparams(nparams), params(new llvm::Type*[nparams]),
  retty(NULL), vararg(vararg) {}

FuncPtrTypeBuilder::FuncPtrTypeBuilder(const FuncPtrGenType* gcty) :
  param(0), nparams(gcty->numParams()),
  params(new llvm::Type*[gcty->numParams()]),
  retty(NULL), vararg(gcty->isVararg()) {}

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
