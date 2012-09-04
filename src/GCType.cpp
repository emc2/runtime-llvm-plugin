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
#include "GCType.h"
#include "metadata.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Metadata.h"
#include "llvm/Constants.h"

static const llvm::Type* getType(const llvm::Module& M,
				 const llvm::MDString* desc) {
  const llvm::StringRef name = desc->getString();
  const llvm::Type* const innerty = M.getTypeByName(name);

  if(NULL == innerty)
    return llvm::StructType::create(M.getContext(), name);
  else
    return innerty;
}

const GCType* GCType::get(const llvm::Module& M,
			  const llvm::MDNode* md,
			  unsigned mutability) {
  unsigned tag =
    llvm::cast<llvm::ConstantInt>(md->getOperand(0))->getZExtValue();

  switch(tag) {
  default: return NULL;
  case GC_MD_TYPE_FUNC: return FuncPtrGCType::get(M, md, mutability);
  case GC_MD_TYPE_STRUCT: return StructGCType::get(M, md, mutability);
  case GC_MD_TYPE_ARRAY: return ArrayGCType::get(M, md, mutability);
  case GC_MD_TYPE_NATIVEPTR: return NativePtrGCType::get(M, md, mutability);
  case GC_MD_TYPE_GCPTR: return GCPtrGCType::get(M, md, mutability);
  case GC_MD_TYPE_INT: return PrimGCType::getInt(M, md, mutability);
  case GC_MD_TYPE_FLOAT: return PrimGCType::getFloat(M, md, mutability);
  case GC_MD_TYPE_NAMED: return PrimGCType::getNamed(M, md, mutability);
  case GC_MD_TYPE_UNIT: return PrimGCType::getUnit();
  }

}

// Format: GC_MD_FUNC vararg retty paramty*
const FuncPtrGCType* FuncPtrGCType::get(const llvm::Module& M,
					const llvm::MDNode* md,
					unsigned mutability) {
  const unsigned vararg =
    llvm::cast<llvm::ConstantInt>(md->getOperand(1))->getZExtValue();
  const unsigned operands = md->getNumOperands();
  const GCType** const paramtys = new const GCType*[operands - 3];
  const llvm::MDNode* rettydesc =
    llvm::cast<llvm::MDNode>(md->getOperand(2));
  const GCType* retty = GCType::get(M, rettydesc);

  for(unsigned i = 3; i < operands; i++) {
    const llvm::MDNode* paramdesc =
      llvm::cast<llvm::MDNode>(md->getOperand(i));

    paramtys[i - 3] = GCType::get(M, paramdesc, mutability);
  }

  return new FuncPtrGCType(retty, paramtys, operands - 3, vararg, mutability);
}

// Format: GC_MD_STRUCT packed { mutability, field }+
const StructGCType* StructGCType::get(const llvm::Module& M,
				      const llvm::MDNode* md,
				      unsigned mutability) {
  const unsigned packed =
    llvm::cast<llvm::ConstantInt>(md->getOperand(1))->getZExtValue();
  const unsigned operands = md->getNumOperands();
  const GCType** const fieldtys = new const GCType*[operands - 2];

  for(unsigned i = 2; i < operands; i++) {
    const llvm::MDNode* fielddesc =
      llvm::cast<llvm::MDNode>(md->getOperand(i));
    const unsigned mutability =
      llvm::cast<llvm::ConstantInt>(fielddesc->getOperand(0))->getZExtValue();
    const llvm::MDNode* typedesc =
      llvm::cast<llvm::MDNode>(fielddesc->getOperand(1));

    fieldtys[i - 2] = GCType::get(M, typedesc, mutability);
  }

  return new StructGCType(fieldtys, operands - 2, packed, mutability);
}

// Format: GC_MD_ARRAY inner [size]
const ArrayGCType* ArrayGCType::get(const llvm::Module& M,
				    const llvm::MDNode* md,
				    unsigned mutability) {
  const llvm::MDNode* inner =
    llvm::cast<llvm::MDNode>(md->getOperand(1));
  const GCType* const innerty = GCType::get(M, inner, mutability);

  if(3 == md->getNumOperands()) {
    const unsigned size =
      llvm::cast<llvm::ConstantInt>(md->getOperand(2))->getZExtValue();

    return new ArrayGCType(innerty, size, mutability);
  }
  else
    return new ArrayGCType(innerty, 0, mutability);
}

// Format: GC_MD_NATIVEPTR inner
const NativePtrGCType* NativePtrGCType::get(const llvm::Module& M,
					    const llvm::MDNode* md,
					    unsigned mutability) {
  const llvm::MDString* inner =
    llvm::cast<llvm::MDString>(md->getOperand(1));
  const llvm::Type* innerty = getType(M, inner);

  return new NativePtrGCType(innerty, mutability);
}

// Format: GC_MD_GCPTR mobility ptrclass inner
const GCPtrGCType* GCPtrGCType::get(const llvm::Module& M,
				    const llvm::MDNode* md,
				    unsigned mutability) {
  const unsigned mobility =
    llvm::cast<llvm::ConstantInt>(md->getOperand(1))->getZExtValue();
  const unsigned ptrclass =
    llvm::cast<llvm::ConstantInt>(md->getOperand(2))->getZExtValue();
  const llvm::MDString* inner =
    llvm::cast<llvm::MDString>(md->getOperand(3));  
  const llvm::Type* innerty = getType(M, inner);

  return new GCPtrGCType(innerty, mutability, mobility, ptrclass);
}

// Format: GC_MD_INT size
/*
const PrimGCType* PrimGCType::get(llvm::LLVMContext &C,
				  const llvm::MDNode* md,
				  unsigned mutability) {

}
*/
// Format: GC_MD_UNIT
const PrimGCType* PrimGCType::getUnit() {
  if(NULL == unitGCTy)
    unitGCTy = new PrimGCType(NULL, ImmutableID);;

  return unitGCTy;
}

// XXX Probably unique these types like LLVM does

const PrimGCType* PrimGCType::getNamed(const llvm::Module& M,
				       const llvm::MDNode* const md,
				       const unsigned mutability) {
  const llvm::MDString* const name =
    llvm::cast<llvm::MDString>(md->getOperand(1));
  const llvm::Type* const ty = getType(M, name);

    return new PrimGCType(ty, mutability);
}

// Format: GC_MD_INT size
const PrimGCType* PrimGCType::getInt(const llvm::Module& M,
				     const llvm::MDNode* const md,
				     const unsigned mutability) {
  const unsigned size =
    llvm::cast<llvm::ConstantInt>(md->getOperand(1))->getZExtValue();
  const llvm::Type* const ty = llvm::Type::getIntNTy(M.getContext(), size);

  return new PrimGCType(ty, mutability);
}

// Format: GC_MD_FLOAT size
const PrimGCType* PrimGCType::getFloat(const llvm::Module& M,
				       const llvm::MDNode* const md,
				       const unsigned mutability) {
  llvm::LLVMContext& C = M.getContext();
  const unsigned size =
    llvm::cast<llvm::ConstantInt>(md->getOperand(1))->getZExtValue();

  switch(size) {
  case 16: return new PrimGCType(llvm::Type::getHalfTy(C), mutability);
  case 32: return new PrimGCType(llvm::Type::getFloatTy(C), mutability);
  case 64: return new PrimGCType(llvm::Type::getDoubleTy(C), mutability);
  case 128: return new PrimGCType(llvm::Type::getFP128Ty(C), mutability);
  }

}

// Visitor functions
void ArrayGCType::accept(GCTypeVisitor& v) const {
  const bool descend = v.begin(this);

  if(descend)
    Elem->accept(v);

  v.end(this);
}

void StructGCType::accept(GCTypeVisitor& v) const {
  const bool descend = v.begin(this);

  if(descend)
    for(unsigned i = 0; i < nfields; i++)
      fieldtys[i]->accept(v);

  v.end(this);
}

void FuncPtrGCType::accept(GCTypeVisitor& v) const {
  const bool descend = v.begin(this);

  if(descend) {
    resty->accept(v);

    for(unsigned i = 0; i < nparams; i++)
      paramtys[i]->accept(v);
  }

  v.end(this);
}

void NativePtrGCType::accept(GCTypeVisitor& v) const {
  v.visit(this);
}

void GCPtrGCType::accept(GCTypeVisitor& v) const {
  v.visit(this);
}

void PrimGCType::accept(GCTypeVisitor& v) const {
  v.visit(this);
}

const char* const GCType::mutabilityStrs[] =
  { "immutable", "mutable", "writeonce" };

const PrimGCType* PrimGCType::unitGCTy = NULL;

const char* const GCPtrGCType::mobilityStrs[] =
  { "mobile", "immobile" };

const char* const GCPtrGCType::ptrClassStrs[] =
  { "strong", "soft", "weak", "phantom", "final" };
