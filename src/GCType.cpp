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

#include "GCObject.h"
#include "llvm/Metadata.h"
#include "llvm/Constants.h"

const GCType* GCType::get(LLVMContext &C,
			  const llvm::MDNode* md,
			  unsigned mutability) {
  unsigned tag = cast<llvm::ConstantInt>(md->getOperand(0))->getZExtValue();

  switch(tag) {
  default: return NULL;
  case GC_MD_FUNC: return FuncPtrGCType::get(C, md, mutability);
  case GC_MD_STRUCT: return StructGCType::get(C, md, mutability);
  case GC_MD_ARRAY: return ArrayGCType::get(C, md, mutability);
  case GC_MD_NATIVEPTR: return NativePtrGCType::get(C, md, mutability);
  case GC_MD_GCPTR: return GCPtrGCType::get(C, md, mutability);
  case GC_MD_INT: return PrimGCType::getInt(C, md, mutability);
  case GC_MD_FLOAT: return PrimGCType::getFloat(C, md, mutability);
  case GC_MD_NAMED: return PrimGCType::getNamed(C, md, mutability);
  case GC_MD_UNIT: return PrimGCType::getUnit();
  }

}

// Format: GC_MD_FUNC vararg retty paramty*
const FuncPtrGCType* FuncPtrGCType::get(LLVMContext &C,
					const llvm::MDNode* md,
					unsigned mutability) {
  const unsigned vararg =
    cast<llvm::ConstantInt>(md->getOperand(1))->getZExtValue();
  const unsigned operands = md->getNumOperands();
  const GCType** const paramtys = new GCType*[operands - 3];
  const llvm::MDNode* rettydesc =
    cast<llvm::MDNode>(md->getOperand(2))->getZExtValue();
  const GCType* retty = GCType::get(rettydesc);

  for(unsigned i = 3; i < operands; i++) {
    const llvm::MDNode* paramdesc = cast<llvm::MDNode>(md->getOperand(i));

    fieldtys[i - 3] = GCType::get(paramdesc, mutability);
  }

  return new FuncPtrGCType(retty, paramtys, operands - 3, vararg, mutability);
}

// Format: GC_MD_STRUCT packed { mutability, field }+
const StructGCType* StructGCType::get(LLVMContext &C,
				      const llvm::MDNode* md,
				      unsigned mutability) {
  const unsigned packed =
    cast<llvm::ConstantInt>(md->getOperand(1))->getZExtValue();
  const unsigned operands = md->getNumOperands();
  const GCType** const fieldtys = new GCType*[operands - 2];

  for(unsigned i = 2; i < operands; i++) {
    const llvm::MDNode* fielddesc = cast<llvm::MDNode>(md->getOperand(i));
    const unsigned mutability =
      cast<llvm::ConstantInt>(md->getOperand(0))->getZExtValue();
    const llvm::MDNode* typedesc = cast<llvm::MDNode>(md->getOperand(1));

    fieldtys[i - 2] = GCType::get(typedesc, mutability);
  }

  return new StructGCType(fieldtys, operands - 2, packed, mutability);
}

// Format: GC_MD_ARRAY inner [size]
const ArrayGCType* ArrayGCType::get(LLVMContext &C,
				    const llvm::MDNode* md,
				    unsigned mutability) {
  const llvm::MDNode* inner = cast<llvm::MDNode>(md->getOperand(1));
  const GCType* const innerty = GCType::create(inner, mutability);

  if(3 == md->getNumOperands()) {
    const unsigned size =
      cast<llvm::ConstantInt>(md->getOperand(2))->getZExtValue();

    return new ArrayGCType(inner, size, mutability);
  }
  else
    return new ArrayGCType(inner, 0, mutability);
}

// Format: GC_MD_NATIVEPTR inner
const NativePtrGCType* NativePtrGCType::get(LLVMContext &C,
					    const llvm::MDNode* md,
					    unsigned mutability) {
  const llvm::MDNode* inner = cast<llvm::MDNode>(md->getOperand(1));
  const llvm::Type* innerty = getType(md);

  return new NativePtrGCType(innerty, mutability)
}

// Format: GC_MD_GCPTR mobility ptrclass inner
const GCPtrGCType* GCPtrGCType::get(LLVMContext &C,
				    const llvm::MDNode* md,
				    unsigned mutability,
				    unsigned mobility,
				    unsigned ptrclass) {
  const unsigned mobility =
    cast<llvm::ConstantInt>(md->getOperand(1))->getZExtValue();
  const unsigned ptrclass =
    cast<llvm::ConstantInt>(md->getOperand(2))->getZExtValue();
  const llvm::MDNode* inner = cast<llvm::MDNode>(md->getOperand(3));

}

// Format: GC_MD_INT size
const PrimGCType* PrimGCType::get(LLVMContext &C,
				  const llvm::MDNode* md,
				  unsigned mutability) {

}

// Format: GC_MD_UNIT
const PrimGCType* PrimGCType::getUnit() {
  if(NULL == unitGCTy)
    unitGCTy = new PrimGCType(NULL, ImmutableID);;

  return unitGCTy;
}

// Visitor functions
void ArrayGCType::accept(GCTypeVisitor* const v) {
  const bool descend = v->begin(this);

  if(descend)
    Elem->accept(v);

  v->end(this);
}

void NativePtrGCType::accept(GCTypeVisitor* const v) {
  const bool descend = v->begin(this);

  if(descend)
    Elem->accept(v);

  v->end(this);
}

void StructGCType::accept(GCTypeVisitor* const v) {
  const bool descend = v->begin(this);

  if(descend)
    for(unsigned i = 0; i < nfields; i++)
      fields[i]->accept(v);

  v->end(this);
}

void FuncPtrGCType::accept(GCTypeVisitor* const v) {
  const bool descend = v->begin(this);

  if(descend) {
    retty->accept(v);

    for(unsigned i = 0; i < nparams; i++)
      params[i]->accept(v);
  }

  v->end(this);
}

void GCPtrGCType::accept(GCTypeVisitor* const v) {
  v->visit(this);
}

void PrimGCType::accept(GCTypeVisitor* const v) {
  v->visit(this);
}

template<typename T>
void ArrayGCType::accept(GCTypeVisitor* const v, T& parent) {
  T ctx;
  const bool descend = v->begin<T>(this, ctx, parent);

  if(descend)
    Elem->accept<T>(v, ctx);

  v->end<T>(this, ctx, parent);
}

template<typename T>
void NativePtrGCType::accept(GCTypeVisitor* const v, T& parent) {
  T ctx;
  const bool descend = v->begin<T>(this, ctx, parent);

  if(descend)
    Elem->accept<T>(v, ctx);

  v->end<T>(this, ctx, parent);
}

template<typename T>
void StructGCType::accept(GCTypeVisitor* const v, T& parent) {
  T ctx;
  const bool descend = v->begin<T>(this, ctx, parent);

  if(descend)
    for(unsigned i = 0; i < nfields; i++)
      fields[i]->accept<T>(v, ctx);

  v->end<T>(this, ctx, parent);
}

template<typename T>
void FuncPtrGCType::accept(GCTypeVisitor* const v, T& parent) {
  T ctx;
  const bool descend = v->begin<T>(this, ctx, parent);

  if(descend) {
    retty->accept<T>(v, ctx);

    for(unsigned i = 0; i < nparams; i++)
      params[i]->accept<T>(v, ctx);
  }

  v->end<T>(this, ctx, parent);
}

template<typename T>
void GCPtrGCType::accept(GCTypeContextVisitor* const v, T& ctx) {
  v->visit<T>(this, ctx);
}

template<typename T>
void PrimGCType::accept(GCTypeContextVisitor* const v, T& ctx) {
  v->visit<T>(this, ctx);
}
