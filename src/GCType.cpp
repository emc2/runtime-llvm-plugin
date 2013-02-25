/* Copyright (c) 2013 Eric McCorkle.
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
#include "GCType.h"
#include "metadata.h"
#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Metadata.h"
#include "llvm/Module.h"

static llvm::Type* getType(const llvm::Module& M,
			   const llvm::MDString* desc) {
  const llvm::StringRef name = desc->getString();
  llvm::Type* const innerty = M.getTypeByName(name);

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
  llvm::Type* innerty = getType(M, inner);

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
  llvm::Type* innerty = getType(M, inner);

  return new GCPtrGCType(innerty, mutability, mobility, ptrclass);
}

// Format: GC_MD_INT size
/*
const PrimGCType* PrimGCType::get(llvm::LLVMContext &C,
				  const llvm::MDNode* md,
				  unsigned mutability) {

}
*/
// Format: GC_MD_TYPE_UNIT
const PrimGCType* PrimGCType::getUnit() {
  if(NULL == unitGCTy)
    unitGCTy = new PrimGCType(NULL, ImmutableID);;

  return unitGCTy;
}

// XXX Probably should unique these types like LLVM does

const PrimGCType* PrimGCType::getNamed(const llvm::Module& M,
				       const llvm::MDNode* const md,
				       const unsigned mutability) {
  const llvm::MDString* const name =
    llvm::cast<llvm::MDString>(md->getOperand(1));
  llvm::Type* const ty = getType(M, name);

    return new PrimGCType(ty, mutability);
}

// Format: GC_MD_TYPE_INT size
const PrimGCType* PrimGCType::getInt(const llvm::Module& M,
				     const llvm::MDNode* const md,
				     const unsigned mutability) {
  const unsigned size =
    llvm::cast<llvm::ConstantInt>(md->getOperand(1))->getZExtValue();
  llvm::Type* const ty = llvm::Type::getIntNTy(M.getContext(), size);

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
  default: return NULL;
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
