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
#ifndef _GC_OBJECT_H_
#define _GC_OBJECT_H_

#include "llvm/Pass.h"

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
    flags = (flags & ~0x6) | (mutability << 1);
  }

  inline void setMobility(unsigned mobility) const {
    flags = (flags & 0x1) | mobility;
  }

protected:

  GCType(unsigned mutability = MutableID,
	 unsigned mobility = MobileID) {
    setMutability(mutability);
    setMobility(mobility);
  }

public:

  enum {
    MobileID,
    ImmobileID,
    NumMobilityIDs
  };

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
  inline unsigned mutability() const { return (flags & ~0x6) >> 1; }

  /*!
   * This function returns the mobility of the entire type.
   *
   * \brief Get the mobility.
   */
  inline unsigned mobility() const { return (flags & 0x1); }
};

/*!
 * This class represents a primitive type in a GC object.
 *
 * \brief A primitive type.
 */
class PrimGCType : public GCType{
private:
  const llvm::Type *const TypeRef;

  GCType(const llvm::Type* TypeRef,
       	 unsigned mutability = MutableID,
	 unsigned mobility = MobileID)
    : TypeRef(TypeRef), GCType(mutability, mobility) {}

  static inline const PrimGCType* get(Context &C,
				      const llvm::Type* TypeRef,
				      unsigned mutability = MutableID,
				      unsigned mobility = MobileID) {
    const PrimGCType* ty = C.TyMap[TypeRef];

    if(NULL == ty) {
      ty = new PrimGCType(TypeRef, mutability, mobility);
      C.TyMap[TypeRef] = ty;
    }

    return ty;
  }

public:
  static inline const GCType* getIntNTy(Context &C,
					unsigned N,
					unsigned mutability = MutableID,
					unsigned mobility = MobileID) {
    get(C, llvm::Type::getIntNTy(C.getContext(), N), mutability, mobility);
  }

  static inline const GCType* getInt1Ty(Context &C,
					unsigned mutability = MutableID,
					unsigned mobility = MobileID) {
    get(C, llvm::Type::getInt1Ty(C.getContext()), mutability, mobility);
  }

  static inline const GCType* getInt8Ty(Context &C,
					unsigned mutability = MutableID,
					unsigned mobility = MobileID) {
    get(C, llvm::Type::getInt8Ty(C.getContext()), mutability, mobility);
  }

  static inline const GCType* getInt16Ty(Context &C,
					 unsigned mutability = MutableID,
					 unsigned mobility = MobileID) {
    get(C, llvm::Type::getInt16Ty(C.getContext()), mutability, mobility);
  }

  static inline const GCType* getInt32Ty(Context &C,
					 unsigned mutability = MutableID,
					 unsigned mobility = MobileID) {
    get(C, llvm::Type::getInt32Ty(C.getContext()), mutability, mobility);
  }

  static inline const GCType* getInt64Ty(Context &C,
					 unsigned mutability = MutableID,
					 unsigned mobility = MobileID) {
    get(C, llvm::Type::getInt64Ty(C.getContext()), mutability, mobility);
  }

  static inline const GCType* getHalfTy(Context &C,
					unsigned mutability = MutableID,
					unsigned mobility = MobileID) {
    get(C, llvm::Type::getHalfTy(C.getContext()), mutability, mobility);
  }

  static inline const GCType* getFloatTy(Context &C,
					 unsigned mutability = MutableID,
					 unsigned mobility = MobileID) {
    get(C, llvm::Type::getFloatTy(C.getContext()), mutability, mobility);
  }

  static inline const GCType* getDoubleTy(Context &C,
					  unsigned mutability = MutableID,
					  unsigned mobility = MobileID) {
    get(C, llvm::Type::getDoubleTy(C.getContext()), mutability, mobility);
  }

  static inline const GCType* getFP128Ty(Context &C,
					 unsigned mutability = MutableID,
					 unsigned mobility = MobileID) {
    get(C, llvm::Type::getFP128Ty(C.getContext()), mutability, mobility);
  }
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
	      unsigned mutability = MutableID,
	      unsigned mobility = MobileID)
    : GCType(mutability, mobility), Elem(Elem), nelems(nelems) {}

public:
  inline const GCType* getElemTy() const { return Elem; }
  inline bool isSized() const { return nelems != 0; }
  inline unsigned getNumElems() const { return nelems; }

  static inline const ArrayGCType* get(Context &C,
				       const GCType* Elem,
				       unsigned nelems = 0,
				       const llvm::Type* TypeRef = NULL,
				       unsigned mutability = MutableID,
				       unsigned mobility = MobileID) {
    const ArrayGCType* ty;

    if(NULL != TypeRef) {
      ty = C.TyMap[TypeRef];

      if(NULL == ty) {
	ty = new ArrayGCType(Elem, NElems, mutability, mobility);
	C.TyMap[TypeRef] = ty;
      }
    }

    return ty;
  }
};

class PtrGCType : public GCType {
private:
  const llvm::Type* const Inner;

protected:
  PtrGCType(const llvm::Type* Inner,
	    unsigned mutability = MutableID,
	    unsigned mobility = MobileID)
    : GCType(mutability, mobility), Inner(Inner) {}

public:
  inline const GCType* getInnerTy() const { return Inner; }
};

/*!
 * This class represents a pointer to something not contained in the
 * GC system.
 *
 * \brief A basic (non-GC) pointer.
 */
class BasicPtrGCType : public PtrGCType {
private:
  BasicPtrGCType(const llvm::Type* Inner,
		 unsigned mutability = MutableID,
		 unsigned mobility = MobileID)
    : PtrGCType(Inner, mutability, mobility) {}

public:
  static inline const BasicPtrGCType* get(Context &C,
					  const llvm::Type* Inner,
					  const llvm::Type* TypeRef = NULL,
					  unsigned mutability = MutableID,
					  unsigned mobility = MobileID) {
    const BasicPtrGCType* ty;

    if(NULL != TypeRef) {
      ty = C.TyMap[TypeRef];

      if(NULL == ty) {
	ty = new BasicPtrGCType(Inner, mutability, mobility);
	C.TyMap[TypeRef] = ty;
      }
    }

    return ty;
  }
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
	      unsigned ptrclass = StrongPtrID,
	      unsigned mutability = MutableID,
	      unsigned mobility = MobileID)
    : PtrGCType(Inner, mutability, mobility), ptrclass(ptrclass) {}
public:

  enum {
    StrongPtrID,
    SoftPtrID,
    WeakPtrID,
    PhantomPtrID,
    FinalPtrID
  };

  static inline const GCPtrGCType* get(Context &C,
				       const llvm::Type* Inner,
				       unsigned ptrclass,
				       const llvm::Type* TypeRef = NULL,
				       unsigned mutability = MutableID,
				       unsigned mobility = MobileID) {
    const GCPtrGCType* ty;

    if(NULL != TypeRef) {
      ty = C.TyMap[TypeRef];

      if(NULL == ty) {
	ty = new GCPtrGCType(Inner, ptrclass, mutability, mobility);
	C.TyMap[TypeRef] = ty;
      }
    }

    return ty;
  }
};

#endif
