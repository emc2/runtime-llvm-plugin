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
#ifndef _GC_TYPE_H_
#define _GC_TYPE_H_

#include "llvm/Type.h"
#include "llvm/Metadata.h"

class GCTypeVisitor;

/*!
 * This is the base type of a shadow hierarchy which represents the
 * additional GC information for types.
 *
 * This class cannot be directly instantiated.
 *
 * \brief A type representing the extra GC info in some types.
 */
class GCType {
private:
  unsigned flags;

  inline void setMutability(unsigned mutability) const {
    flags = (flags & ~0x1) | mutability;
  }

protected:
  GCType(unsigned mutability = MutableID) {
    setMutability(mutability);
  }

public:

  enum {
    ImmutableID,
    MutableID,
    WriteOnceID,
    NumMutabilityIDs
  };

  /*!
   * This function returns the mutability of the entire type.
   *
   * \brief Get the mutability.
   */
  inline unsigned mutability() const { return (flags & ~0x1); }

  virtual void accept(GCTypeVisitor* v) = 0;
  template <typename T> virtual void accept(GCTypeContextVisitor<T>*, T&) = 0;

  static const GCType* get(LLVMContext &C,
			   const llvm::MDNode* md,
			   unsigned mutability = MutableID);
};

/*!
 * This class represents a primitive type in a GC object.
 *
 * \brief A primitive type.
 */
class PrimGCType : public GCType{
private:
  const llvm::Type *const TypeRef;

  AtomicGCType(const llvm::Type* TypeRef,
	       unsigned mutability = MutableID)
    : TypeRef(TypeRef), GCType(mutability) {}

  static GCType* unitGCTy = NULL;
public:
  virtual void accept(GCTypeVisitor* v);
  template <typename T> virtual void accept(GCTypeContextVisitor<T>*, T&);

  static const PrimGCType* getUnit();
  static const PrimGCType* getInt(LLVMContext &C,
				  const llvm::MDNode* md,
				  unsigned mutability);
  static const PrimGCType* getFloat(LLVMContext &C,
				    const llvm::MDNode* md,
				    unsigned mutability);
  static const PrimGCType* getNamed(LLVMContext &C,
				    const llvm::MDNode* md,
				    unsigned mutability);

};

/*!
 * This class represents an array in a GC object.  Unlike LLVM arrays,
 * GC arrays may be of indeterminant size.
 *
 * \brief An array in a GC type
 */
class ArrayGCType : public GCType {
private:
  const unsigned nelems;
  const GCType* const Elem;

  ArrayGCType(const GCType* Elem,
	      unsigned nelems = 0,
	      unsigned mutability = MutableID)
    : GCType(mutability), Elem(Elem), nelems(nelems) {}
public:
  virtual void accept(GCTypeVisitor* v);
  template <typename T> virtual void accept(GCTypeContextVisitor<T>*, T&);

  inline const GCType* getElemTy() const { return Elem; }
  inline bool isSized() const { return nelems != 0; }
  inline unsigned getNumElems() const { return nelems; }

  static const ArrayGCType* get(LLVMContext &C,
				const llvm::MDNode* md,
				unsigned mutability);
};

class PtrGCType : public GCType {
private:
  const llvm::Type* const Inner;

protected:
  PtrGCType(const llvm::Type* Inner,
	    unsigned mutability = MutableID)
    : GCType(mutability), Inner(Inner) {}

public:
  inline const GCType* getElemTy() const { return Inner; }
};

/*!
 * This class represents a pointer to something not contained in the
 * GC system.
 *
 * \brief A basic (non-GC) pointer.
 */
class NativePtrGCType : public PtrGCType {
private:
  NativePtrGCType(const llvm::Type* Inner,
		  unsigned mutability = MutableID)
    : PtrGCType(Inner, mutability) {}

public:
  virtual void accept(GCTypeVisitor* v);
  template <typename T> virtual void accept(GCTypeContextVisitor<T>*, T&);

  static const NativePtrGCType* get(LLVMContext &C,
				    const llvm::MDNode* md,
				    unsigned mobility);
};

/*!
 * This class represents a pointer to a garbage-collected object.
 *
 * \brief A pointer to a GC object.
 */
class GCPtrGCType : public PtrGCType {
private:
  const unsigned ptrclass;

  GCPtrGCType(const llvm::Type* Inner,
	      unsigned mutability = MutableID,
	      unsigned mobility = MobileID,
	      unsigned ptrclass = StrongPtrID)
    : PtrGCType(Inner, mutability, mobility), ptrclass(ptrclass) {}
public:
  virtual void accept(GCTypeVisitor* v);
  template <typename T> virtual void accept(GCTypeContextVisitor<T>*, T&);

  static const GCPtrGCType* get(LLVMContext &C,
				const llvm::MDNode* md,
				unsigned mutability = MutableID,
				unsigned mobility = MobileID,
				unsigned ptrclass = StrongPtrID);

  enum {
    StrongPtrID,
    SoftPtrID,
    WeakPtrID,
    PhantomPtrID,
    FinalPtrID
  };
};


/*!
 * This class represents a structure type.
 *
 * \brief A structure.
 */
class StructGCType : public GCType {
private:
  const GCType* const * const fieldtys;
  const unsigned nfields;
  const bool packed;

  StructGCType(const GCType* const * fieldtys,
	       unsigned nfields,
	       bool packed = false,
	       unsigned mutability = MutableID)
    : GCType(mutability), nfields(nfields), fieldtys(fieldtys),
      fieldflags(fieldflags), packed(packed) {}
public:
  virtual void accept(GCTypeVisitor* v);
  template <typename T> virtual void accept(GCTypeContextVisitor<T>*, T&);

  inline bool isPacked() const { return packed; }
  inline unsigned numFields() const { return nfields; }

  static const StructGCType* get(LLVMContext &C,
				 const llvm::MDNode* md,
				 unsigned mutability = MutableID);
};

/*!
 * This class represents a function pointer type.
 *
 * \brief A function pointer.
 */
class FuncPtrGCType : public GCType {
private:
  const GCType* const resty;
  const GCType* const * const paramtys;
  const unsigned nparams;
  const bool vararg;

  FuncPtrGCType(const GCType* resty,
		const GCType* const * paramtys,
		unsigned nparams,
		bool vararg = false,
		unsigned mutability = MutableID)
    : GCType(mutability), nfields(nfields), fieldtys(fieldtys),
      fieldflags(fieldflags), packed(packed) {}
public:
  virtual void accept(GCTypeVisitor*);
  template <typename T> virtual void accept(GCTypeContextVisitor<T>*, T&);

  inline bool isVararg() const { return vararg; }
  inline unsigned numParams() const { return nparams; }
  inline const GCType* resultTy() const { return resty; }

  static const FuncPtrGCType* get(LLVMContext &C,
				  const llvm::MDNode* md,
				  unsigned mutability = MutableID);
};

#endif
