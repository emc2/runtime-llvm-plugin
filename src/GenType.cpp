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
#include <stdio.h>
#include <stdlib.h>
#include "GenType.h"
#include "metadata.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"
#include <llvm/Support/raw_os_ostream.h>

static inline unsigned getMDIntArg(const llvm::MDNode* const md,
                                   const unsigned idx) {
  const llvm::ConstantAsMetadata* const mdconst =
    llvm::cast<llvm::ConstantAsMetadata>(md->getOperand(idx));

  return llvm::cast<llvm::ConstantInt>(mdconst->getValue())->getZExtValue();
}

static llvm::Type* getType(const llvm::Module& M,
			   const llvm::MDString* const desc) {
  const llvm::StringRef name = desc->getString();
  llvm::Type* const innerty = M.getTypeByName(name);

  if(NULL == innerty)
    return llvm::StructType::create(M.getContext(), name);
  else
    return innerty;
}

static inline GenType::Mutability decodeMutability(const unsigned mut) {
  switch(mut) {
  default:
    fprintf(stderr, "Bad mutability code %d\n", mut);
    abort();
  case TYPE_MUT_MUTABLE: return GenType::Mutable;
  case TYPE_MUT_WRITEONCE: return GenType::WriteOnce;
  case TYPE_MUT_IMMUTABLE: return GenType::Immutable;
  }
}

const GenType* GenType::get(const llvm::Module& M,
                            const llvm::MDNode* md,
                            unsigned mut = TYPE_MUT_MUTABLE) {
  return GenType::get(M, md, decodeMutability(mut));
}

const GenType* GenType::get(const llvm::Module& M,
                            const llvm::MDNode* const md,
                            const Mutability mut) {
  const unsigned tag = getMDIntArg(md, 0);

  switch(tag) {
  default: return NULL;
  case GEN_TYPE_FUNC: return FuncPtrGenType::get(M, md, mut);
  case GEN_TYPE_STRUCT: return StructGenType::get(M, md, mut);
  case GEN_TYPE_ARRAY: return ArrayGenType::get(M, md, mut);
  case GEN_TYPE_NATIVEPTR: return NativePtrGenType::get(M, md, mut);
  case GEN_TYPE_GCPTR: return GCPtrGenType::get(M, md, mut);
  case GEN_TYPE_INT: return PrimGenType::getInt(M, md, mut);
  case GEN_TYPE_FLOAT: return PrimGenType::getFloat(M, md, mut);
  case GEN_TYPE_NAMED: return PrimGenType::getNamed(M, md, mut);
  case GEN_TYPE_UNIT: return PrimGenType::getUnit();
  }

}

bool GenType::operator==(const GenType& other) const {
  if(getTypeID() == other.getTypeID())
    switch(other.getTypeID()) {
    case FuncPtrTypeID:
      return *FuncPtrGenType::narrow(this) == (FuncPtrGenType&)other;
    case GCPtrTypeID:
      return *GCPtrGenType::narrow(this) == (GCPtrGenType&)other;
    case ArrayTypeID:
      return *ArrayGenType::narrow(this) == (ArrayGenType&)other;
    case StructTypeID:
      return *StructGenType::narrow(this) == (StructGenType&)other;
    case PrimTypeID:
      return *PrimGenType::narrow(this) == (PrimGenType&)other;
    case NativePtrTypeID:
      return *NativePtrGenType::narrow(this) == (NativePtrGenType&)other;
    }
  return false;
}

// Format: GEN_TYPE_FUNC vararg retty paramty*
const FuncPtrGenType* FuncPtrGenType::get(const llvm::Module& M,
                                          const llvm::MDNode* const md,
                                          const Mutability mut) {
  const unsigned vararg = getMDIntArg(md, 1);
  const unsigned operands = md->getNumOperands();
  const GenType** const paramtys = new const GenType*[operands - 3];
  const llvm::MDNode* const rettydesc =
    llvm::cast<llvm::MDNode>(md->getOperand(2));
  const GenType* const retty = GenType::get(M, rettydesc);

  for(unsigned i = 3; i < operands; i++) {
    const llvm::MDNode* const paramdesc =
      llvm::cast<llvm::MDNode>(md->getOperand(i));

    paramtys[i - 3] = GenType::get(M, paramdesc);
  }

  return new FuncPtrGenType(retty, paramtys, operands - 3, vararg, mut);
}

// Format: GEN_TYPE_STRUCT packed { mutability, field }+
const StructGenType* StructGenType::get(const llvm::Module& M,
                                        const llvm::MDNode* const md,
                                        const Mutability mut) {
  const unsigned packed = getMDIntArg(md, 1);
  const unsigned operands = md->getNumOperands();
  const GenType** const fieldtys = new const GenType*[operands - 2];

  for(unsigned i = 2; i < operands; i++) {
    const llvm::MDNode* const fielddesc =
      llvm::cast<llvm::MDNode>(md->getOperand(i));
    const unsigned mutability = getMDIntArg(md, 0);
    const llvm::MDNode* const typedesc =
      llvm::cast<llvm::MDNode>(fielddesc->getOperand(1));

    fieldtys[i - 2] = GenType::get(M, typedesc, mutability);
  }

  return new StructGenType(fieldtys, operands - 2, packed, mut);
}

// Format: GEN_TYPE_ARRAY inner [size]
const ArrayGenType* ArrayGenType::get(const llvm::Module& M,
                                      const llvm::MDNode* const md,
                                      const Mutability mut) {
  const llvm::MDNode* const inner =
    llvm::cast<llvm::MDNode>(md->getOperand(1));
  const GenType* const innerty = GenType::get(M, inner, mut);

  if(3 == md->getNumOperands()) {
    const unsigned size = getMDIntArg(md, 2);

    return new ArrayGenType(innerty, size, mut);
  }
  else
    return new ArrayGenType(innerty, 0, mut);
}

// Format: GEN_TYPE_NATIVEPTR inner
const NativePtrGenType* NativePtrGenType::get(const llvm::Module& M,
                                              const llvm::MDNode* const md,
                                              const Mutability mut) {
  const llvm::MDString* const inner =
    llvm::cast<llvm::MDString>(md->getOperand(1));
  llvm::Type* const innerty = getType(M, inner);

  return new NativePtrGenType(innerty, mut);
}

static inline GCPtrGenType::Mobility decodeMobility(const unsigned mut) {
  switch(mut) {
  default:
    fprintf(stderr, "Bad mobility code %d\n", mut);
    abort();
  case PTR_MOB_MOBILE: return GCPtrGenType::Mobile;
  case PTR_MOB_IMMOBILE: return GCPtrGenType::Immobile;
  }
}

static inline GCPtrGenType::PtrClass decodePtrClass(const unsigned mut) {
  switch(mut) {
  default:
    fprintf(stderr, "Bad pointer class code %d\n", mut);
    abort();
  case PTRCLASS_STRONG: return GCPtrGenType::StrongPtr;
  case PTRCLASS_SOFT: return GCPtrGenType::SoftPtr;
  case PTRCLASS_WEAK: return GCPtrGenType::WeakPtr;
  case PTRCLASS_FINALIZER: return GCPtrGenType::FinalizerPtr;
  case PTRCLASS_PHANTOM: return GCPtrGenType::PhantomPtr;
  }
}

// Format: GEN_TYPE_GCPTR mutability mobility ptrclass inner
const GCPtrGenType* GCPtrGenType::get(const llvm::Module& M,
                                      const llvm::MDNode* const md,
                                      const Mutability mut) {
  const unsigned mobilitycode = getMDIntArg(md, 1);
  const unsigned ptrclasscode = getMDIntArg(md, 2);
  const Mobility mobility = decodeMobility(mobilitycode);
  const PtrClass ptrclass = decodePtrClass(ptrclasscode);
  const llvm::MDString* const inner =
    llvm::cast<llvm::MDString>(md->getOperand(3));
  llvm::Type* const innerty = getType(M, inner);

  return new GCPtrGenType(innerty, mut, mobility, ptrclass);
}

// Format: GC_MD_INT size
/*
const PrimGCType* PrimGCType::get(llvm::LLVMContext &C,
				  const llvm::MDNode* md,
				  unsigned mutability) {

}
*/
// Format: GEN_TYPE_UNIT
const PrimGenType* PrimGenType::getUnit() {
  if(NULL == unitGenTy)
    unitGenTy = new PrimGenType(NULL, Immutable, NULL, NULL);

  return unitGenTy;
}

// XXX Probably should unique these types like LLVM does

const PrimGenType* PrimGenType::getNamed(const llvm::Module& M,
                                         const llvm::MDNode* const md,
                                         const Mutability mut) {
  const llvm::MDString* const name =
    llvm::cast<llvm::MDString>(md->getOperand(1));
  llvm::Type* const ty = getType(M, name);
  llvm::Function* const accessFunc = llvm::cast<llvm::Function>
    (llvm::cast<llvm::ValueAsMetadata>(md->getOperand(2))->getValue());
  llvm::Function* const modifyFunc = mut == Immutable ? NULL :
    llvm::cast<llvm::Function>(llvm::cast<llvm::ValueAsMetadata>
                               (md->getOperand(3))->getValue());

  return new PrimGenType(ty, mut, accessFunc, modifyFunc);
}

// Format: GEN_TYPE_INT size accessor modifier?
const PrimGenType* PrimGenType::getInt(const llvm::Module& M,
                                       const llvm::MDNode* const md,
                                       const Mutability mut) {
  const unsigned size = getMDIntArg(md, 1);
  llvm::Type* const ty = llvm::Type::getIntNTy(M.getContext(), size);
  llvm::Function* const accessFunc = llvm::cast<llvm::Function>
    (llvm::cast<llvm::ValueAsMetadata>(md->getOperand(2))->getValue());
  llvm::Function* const modifyFunc = mut == Immutable ? NULL :
    llvm::cast<llvm::Function>(llvm::cast<llvm::ValueAsMetadata>
                               (md->getOperand(3))->getValue());

  return new PrimGenType(ty, mut, accessFunc, modifyFunc);
}

// Format: GEN_TYPE_FLOAT size accessor modifier?
const PrimGenType* PrimGenType::getFloat(const llvm::Module& M,
                                         const llvm::MDNode* const md,
                                         const Mutability mut) {
  llvm::LLVMContext& C = M.getContext();
  const unsigned size = getMDIntArg(md, 1);
  llvm::Function* const accessFunc = llvm::cast<llvm::Function>
    (llvm::cast<llvm::ValueAsMetadata>(md->getOperand(2))->getValue());
  llvm::Function* const modifyFunc = mut == Immutable ? NULL :
    llvm::cast<llvm::Function>(llvm::cast<llvm::ValueAsMetadata>
                               (md->getOperand(3))->getValue());

  switch(size) {
  default: return NULL;
  case 16: return new PrimGenType(llvm::Type::getHalfTy(C), mut,
                                  accessFunc, modifyFunc);
  case 32: return new PrimGenType(llvm::Type::getFloatTy(C), mut,
                                  accessFunc, modifyFunc);
  case 64: return new PrimGenType(llvm::Type::getDoubleTy(C), mut,
                                  accessFunc, modifyFunc);
  case 128: return new PrimGenType(llvm::Type::getFP128Ty(C), mut,
                                   accessFunc, modifyFunc);
  }

}

// Visitor functions
void ArrayGenType::accept(GenTypeVisitor& v) const {
  const bool descend = v.begin(this);

  if(descend)
    elem->accept(v);

  v.end(this);
}

void StructGenType::accept(GenTypeVisitor& v) const {
  const bool descend = v.begin(this);

  if(descend)
    for(unsigned i = 0; i < nfields; i++)
      fieldtys[i]->accept(v);

  v.end(this);
}

void FuncPtrGenType::accept(GenTypeVisitor& v) const {
  const bool descend = v.begin(this);

  if(descend) {
    retty->accept(v);

    const bool params = v.beginParams(this);

    if(params)
      for(unsigned i = 0; i < nparams; i++)
        paramtys[i]->accept(v);

    v.endParams(this);
  }

  v.end(this);
}

void NativePtrGenType::accept(GenTypeVisitor& v) const {
  v.visit(this);
}

void GCPtrGenType::accept(GenTypeVisitor& v) const {
  v.visit(this);
}

void PrimGenType::accept(GenTypeVisitor& v) const {
  v.visit(this);
}

std::ostream& GenType::print(std::ostream& stream) const {
  llvm::raw_os_ostream rstream(stream);
  GenTypePrintVisitor visitor(rstream);
  visitor.print(this);

  return stream;
}

const char* const GenType::mutabilityStrs[] =
  { "immutable", "mutable", "writeonce" };

const PrimGenType* PrimGenType::unitGenTy = NULL;

const char* const GCPtrGenType::mobilityStrs[] =
  { "mobile", "immobile" };

const char* const GCPtrGenType::ptrClassStrs[] =
  { "strong", "soft", "weak", "phantom", "final" };
