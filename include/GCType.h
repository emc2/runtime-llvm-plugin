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
#include "llvm/Module.h"
#include "GCTypeVisitors.h"

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

  inline void setTypeID(unsigned typeID) {
    flags = (flags & ~0xe) | ((typeID & 0x7) << 1);
  }

  inline void setMutability(unsigned mutability) {
    flags = (flags & ~0x1) | mutability;
  }

  inline unsigned getTypeID() const {
    return (flags >> 1) & 0x7;
  }

protected:
  GCType(unsigned typeID,
	 unsigned mutability = MutableID) {
    setTypeID(typeID);
    setMutability(mutability);
  }

public:
  enum {
    StructTypeID,
    FuncPtrTypeID,
    PrimTypeID,
    GCPtrTypeID,
    NativePtrTypeID,
    ArrayTypeID
  };

  enum {
    ImmutableID,
    MutableID,
    WriteOnceID,
    NumMutabilityIDs
  };

  static const char* const mutabilityStrs[];

  /*!
   * This function returns the mutability of the entire type.
   *
   * \brief Get the mutability.
   */
  inline unsigned mutability() const { return (flags & ~0x1); }

  /*!
   * This function returns the string representation of the mutability
   * of the entire type.
   *
   * \brief Get the mutability.
   */
  inline const char* mutabilityName() const {
    return mutabilityStrs[mutability()];
  }

  /*!
   * This function runs a visitor on this type.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GCTypeVisitor& v) const = 0;

  /*!
   * This function builds a type from metadata.
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mutability The mutability of the type.
   */
  static const GCType* get(const llvm::Module& M,
			   const llvm::MDNode* md,
			   unsigned mutability = MutableID);

  /*!
   * This function runs a visitor on this type. The ctx argument will
   * be passed in as the parent argument in the visitor functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param ctx The context argument to pass in.
   */
  template <typename T> void accept(GCTypeContextVisitor<T>& v,
				    T& ctx) const;
};

/*!
 * This class represents a primitive type in a GC object.
 *
 * \brief A primitive type.
 */
class PrimGCType : public GCType {
private:
  const llvm::Type *const TypeRef;

  PrimGCType(const llvm::Type* TypeRef,
	     unsigned mutability = MutableID)
    : TypeRef(TypeRef), GCType(PrimTypeID, mutability) {}

  static const PrimGCType* unitGCTy;
public:

  inline const llvm::Type* getLLVMType() const { return TypeRef; }

  /*!
   * This function runs a visitor on this type.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GCTypeVisitor& v) const;

  /*!
   * This function returns the type representing the unit type.  This
   * is uniqued.
   *
   * \brief Get the type representing unit.
   */
  static const PrimGCType* getUnit();

  /*!
   * This function builds a type from metadata.  It assumes the
   * metadata's type tag is GC_MD_INT, and the metadata node is
   * properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mutability The mutability of the type.
   */
  static const PrimGCType* getInt(const llvm::Module& M,
				  const llvm::MDNode* md,
				  unsigned mutability);

  /*!
   * This function builds a type from metadata.  It assumes the
   * metadata's type tag is GC_MD_FLOAT, and the metadata node is
   * properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mutability The mutability of the type.
   */
  static const PrimGCType* getFloat(const llvm::Module& M,
				    const llvm::MDNode* md,
				    unsigned mutability);

  /*!
   * This function builds a type from metadata.  It assumes the
   * metadata's type tag is GC_MD_NAMED, and the metadata node is
   * properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mutability The mutability of the type.
   */
  static const PrimGCType* getNamed(const llvm::Module& M,
				    const llvm::MDNode* md,
				    unsigned mutability);

  /*!
   * This function runs a visitor on this type. The ctx argument will
   * be passed in as the parent argument in the visitor functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param ctx The context argument to pass in.
   */
  template<typename T> inline void accept(GCTypeContextVisitor<T>& v,
					  T& ctx) const {
    v.visit(this, ctx);
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
	      unsigned mutability = MutableID)
    : GCType(ArrayTypeID, mutability), Elem(Elem), nelems(nelems) {}
public:
  /*!
   * This function runs a visitor on this type.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GCTypeVisitor& v) const;

  inline const GCType* getElemTy() const { return Elem; }
  inline bool isSized() const { return nelems != 0; }
  inline unsigned getNumElems() const { return nelems; }

  /*!
   * This function runs a visitor on this type. The parent argument
   * will be passed in as the parent argument in the visitor
   * functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param parent The context argument to pass in.
   */
  template<typename T> inline void accept(GCTypeContextVisitor<T>& v,
					  T& parent) const {
    T ctx;
    const bool descend = v.begin(this, ctx, parent);

    if(descend)
      Elem->accept<T>(v, ctx);

    v.end(this, ctx, parent);
  }

  /*!
   * This function builds a type from metadata.  It assumes the
   * metadata's type tag is GC_MD_ARRAY, and the metadata node is
   * properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mutability The mutability of the type.
   */
  static const ArrayGCType* get(const llvm::Module& M,
				const llvm::MDNode* md,
				unsigned mutability);
};

class PtrGCType : public GCType {
protected:
  const llvm::Type* const Inner;

  PtrGCType(unsigned typeID,
	    const llvm::Type* Inner,
	    unsigned mutability = MutableID)
    : GCType(typeID, mutability), Inner(Inner) {}

public:
  inline const llvm::Type* getElemTy() const { return Inner; }
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
    : PtrGCType(NativePtrTypeID, Inner, mutability) {}

public:

  /*!
   * This function runs a visitor on this type.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GCTypeVisitor& v) const;

  /*!
   * This function runs a visitor on this type. The ctx argument will
   * be passed in as the parent argument in the visitor functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param ctx The context argument to pass in.
   */
  template<typename T> inline void accept(GCTypeContextVisitor<T>& v,
					  T& ctx) const {
    v.visit(this, ctx);
  }

  /*!
   * This function builds a type from metadata.  It assumes the
   * metadata's type tag is GC_MD_NATIVE_PTR, and the metadata node is
   * properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mutability The mutability of the type.
   */
  static const NativePtrGCType* get(const llvm::Module& M,
				    const llvm::MDNode* md,
				    unsigned mutability);
};

/*!
 * This class represents a pointer to a garbage-collected object.
 *
 * \brief A pointer to a GC object.
 */
class GCPtrGCType : public PtrGCType {
private:
  const unsigned ptrclass;
  const unsigned mobility;

  GCPtrGCType(const llvm::Type* Inner,
	      unsigned mutability = MutableID,
	      unsigned mobility = MobileID,
	      unsigned ptrclass = StrongPtrID)
    : PtrGCType(GCPtrTypeID, Inner, mutability),
      ptrclass(ptrclass), mobility(mobility) {}
public:
  enum {
    MobileID,
    ImmobileID
  };

  static const char* const mobilityStrs[];

  enum {
    StrongPtrID,
    SoftPtrID,
    WeakPtrID,
    PhantomPtrID,
    FinalPtrID
  };

  static const char* const ptrClassStrs[];

  inline unsigned getPtrClass() const { return ptrclass; }

  inline const char* getPtrClassName() const {
    return ptrClassStrs[getPtrClass()];
  }

  inline unsigned getMobility() const { return mobility; }

  inline const char* getMobilityName() const {
    return mobilityStrs[getMobility()];
  }

  /*!
   * This function runs a visitor on this type.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GCTypeVisitor& v) const;

  /*!
   * This function runs a visitor on this type. The ctx argument will
   * be passed in as the parent argument in the visitor functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param ctx The context argument to pass in.
   */
  template<typename T> inline void accept(GCTypeContextVisitor<T>& v,
					  T& ctx) const {
    v.visit(this, ctx);
  }

  /*!
   * This function builds a type from metadata.  It assumes the
   * metadata's type tag is GC_MD_GC_PTR, and the metadata node is
   * properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mutability The mutability of the type.
   */
  static const GCPtrGCType* get(const llvm::Module& M,
				const llvm::MDNode* md,
				unsigned mutability = MutableID);
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
    : GCType(StructTypeID, mutability), nfields(nfields),
      fieldtys(fieldtys), packed(packed) {}
public:

  /*!
   * This function runs a visitor on this type.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GCTypeVisitor& v) const;

  inline bool isPacked() const { return packed; }
  inline unsigned numFields() const { return nfields; }

  /*!
   * This function runs a visitor on this type. The parent argument will
   * be passed in as the parent argument in the visitor functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param parent The context argument to pass in.
   */
  template<typename T> inline void accept(GCTypeContextVisitor<T>& v,
					  T& parent) const {
    T ctx;
    const bool descend = v.begin(this, ctx, parent);

    if(descend)
      for(unsigned i = 0; i < nfields; i++)
	fieldtys[i]->accept<T>(v, ctx);

    v.end(this, ctx, parent);
  }

  /*!
   * This function builds a type from metadata.  It assumes the
   * metadata's type tag is GC_MD_STRUCT, and the metadata node is
   * properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mutability The mutability of the type.
   */
  static const StructGCType* get(const llvm::Module& M,
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
    : GCType(FuncPtrTypeID, mutability), nparams(nparams),
      paramtys(paramtys), resty(resty), vararg(vararg) {}
public:

  /*!
   * This function runs a visitor on this type.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GCTypeVisitor&) const;

  inline bool isVararg() const { return vararg; }
  inline unsigned numParams() const { return nparams; }
  inline const GCType* resultTy() const { return resty; }

  /*!
   * This function runs a visitor on this type. The parent argument will
   * be passed in as the parent argument in the visitor functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param parent The context argument to pass in.
   */
  template<typename T> inline void accept(GCTypeContextVisitor<T>& v,
					  T& parent) const {
    T ctx;
    const bool descend = v.begin(this, ctx, parent);

    if(descend) {
      resty->accept<T>(v, ctx);

      const bool params = v.beginParams(this, ctx);

      if(params) {
	for(unsigned i = 0; i < nparams; i++)
	  paramtys[i]->accept<T>(v, ctx);

	v.endParams(this, ctx);
      }

      v.end(this, ctx, parent);
    }

  }

  /*!
   * This function builds a type from metadata.  It assumes the
   * metadata's type tag is GC_MD_FUNC, and the metadata node is
   * properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mutability The mutability of the type.
   */
  static const FuncPtrGCType* get(const llvm::Module& M,
				  const llvm::MDNode* md,
				  unsigned mutability = MutableID);
};

template <typename T> void GCType::accept(GCTypeContextVisitor<T>& v,
					  T& ctx) const {
  switch(getTypeID()) {
  case PrimTypeID:
    static_cast<const PrimGCType*>(this)->accept(v, ctx);
    break;
  case StructTypeID:
    static_cast<const StructGCType*>(this)->accept(v, ctx);
    break;
  case ArrayTypeID:
    static_cast<const ArrayGCType*>(this)->accept(v, ctx);
    break;
  case FuncPtrTypeID:
    static_cast<const FuncPtrGCType*>(this)->accept(v, ctx);
    break;
  case NativePtrTypeID:
    static_cast<const NativePtrGCType*>(this)->accept(v, ctx);
    break;
  case GCPtrTypeID:
    static_cast<const GCPtrGCType*>(this)->accept(v, ctx);
    break;
  }
}

#endif
