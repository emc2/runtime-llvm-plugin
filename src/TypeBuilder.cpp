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
