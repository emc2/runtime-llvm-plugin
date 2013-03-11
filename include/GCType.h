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
    flags = (flags & ~0x1c) | ((typeID & 0x7) << 2);
  }

  inline void setMutability(unsigned mutability) {
    flags = (flags & ~0x3) | (mutability & 0x3);
  }

protected:
  GCType(unsigned typeID,
	 unsigned mutability = MutableID) : flags(0) {
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
  inline unsigned mutability() const {
    return (flags & 0x3);
  }

  inline unsigned getTypeID() const {
    return (flags >> 2) & 0x7;
  }

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
   * be passed in as the parent argument in the visitor functions
   * instead of the visitor's default argument.
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
  llvm::Type *const TypeRef;

  PrimGCType(llvm::Type* TypeRef,
	     unsigned mutability = MutableID)
    : GCType(PrimTypeID, mutability), TypeRef(TypeRef) {}

  static const PrimGCType* unitGCTy;
public:

  inline llvm::Type* getLLVMType() const { return TypeRef; }

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

  static inline PrimGCType* narrow(GCType* const ty) {
    if(GCType::PrimTypeID == ty->getTypeID()) {
      return static_cast<PrimGCType*>(ty);
    } else
      return NULL;
  }

  static inline const PrimGCType* narrow(const GCType* const ty) {
    if(GCType::PrimTypeID == ty->getTypeID()) {
      return static_cast<const PrimGCType*>(ty);
    } else
      return NULL;
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
    : GCType(ArrayTypeID, mutability), nelems(nelems), Elem(Elem) {}
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

  static inline ArrayGCType* narrow(GCType* const ty) {
    if(GCType::ArrayTypeID == ty->getTypeID()) {
      return static_cast<ArrayGCType*>(ty);
    } else
      return NULL;
  }

  static inline const ArrayGCType* narrow(const GCType* const ty) {
    if(GCType::ArrayTypeID == ty->getTypeID()) {
      return static_cast<const ArrayGCType*>(ty);
    } else
      return NULL;
  }

};

class PtrGCType : public GCType {
protected:
  llvm::Type* const Inner;

  PtrGCType(unsigned typeID,
	    llvm::Type* Inner,
	    unsigned mutability = MutableID)
    : GCType(typeID, mutability), Inner(Inner) {}

public:
  inline llvm::Type* getElemTy() const { return Inner; }
};

/*!
 * This class represents a pointer to something not contained in the
 * GC system.
 *
 * \brief A basic (non-GC) pointer.
 */
class NativePtrGCType : public PtrGCType {
private:
  NativePtrGCType(llvm::Type* Inner,
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

  static inline NativePtrGCType* narrow(GCType* const ty) {
    if(GCType::NativePtrTypeID == ty->getTypeID()) {
      return static_cast<NativePtrGCType*>(ty);
    } else
      return NULL;
  }

  static inline const NativePtrGCType* narrow(const GCType* const ty) {
    if(GCType::NativePtrTypeID == ty->getTypeID()) {
      return static_cast<const NativePtrGCType*>(ty);
    } else
      return NULL;
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
  const unsigned mobility;

  GCPtrGCType(llvm::Type* Inner,
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
    FinalizerPtrID,
    PhantomPtrID
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

  static inline GCPtrGCType* narrow(GCType* const ty) {
    if(GCType::GCPtrTypeID == ty->getTypeID()) {
      return static_cast<GCPtrGCType*>(ty);
    } else
      return NULL;
  }

  static inline const GCPtrGCType* narrow(const GCType* const ty) {
    if(GCType::GCPtrTypeID == ty->getTypeID()) {
      return static_cast<const GCPtrGCType*>(ty);
    } else
      return NULL;
  }

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
    : GCType(StructTypeID, mutability), fieldtys(fieldtys),
      nfields(nfields), packed(packed) {}
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
    : GCType(FuncPtrTypeID, mutability), resty(resty),
      paramtys(paramtys), nparams(nparams), vararg(vararg) {}
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
