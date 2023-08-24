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
#ifndef _GEN_TYPE_H_
#define _GEN_TYPE_H_

#include <stdio.h>
#include <stdlib.h>
#include "llvm/IR/Type.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"
#include "GenTypeVisitors.h"
#include <iostream>

/*!
 * This is the base type of a shadow hierarchy which represents
 * generated types.  This is used to implement a number of features,
 * including tags, GC, and reflection.
 *
 * This class cannot be directly instantiated.
 *
 * \brief A type representing generated types.
 */
class GenType {
public:
  /*!
   * Used for the type ID field.
   *
   * \brief Enumeration for identifying kinds of generated types.
   */
  enum TypeID {
    /*!
     * \brief Type ID for structures.
     */
    StructTypeID,
    /*!
     * \brief Type ID for function pointers.
     */
    FuncPtrTypeID,
    /*!
     * \brief Type ID for primitive types.
     */
    PrimTypeID,
    /*!
     * \brief Type ID for pointers to GC'ed objects.
     */
    GCPtrTypeID,
    /*!
     * \brief Type ID for pointers to non-GC'ed objects.
     */
    NativePtrTypeID,
    /*!
     * \brief Type ID for arrays.
     */
    ArrayTypeID
  };

  /*!
   * This can represent the mutability of both individual fields as
   * well as whole objects.
   *
   * \brief Enumeration for describing mutability.
   */
  enum Mutability {
    /*!
     * Immutable objects/fields.  Cannot be changed.
     */
    Immutable,
    /*!
     * Objects/fields which are mutable.
     */
    Mutable,
    /*!
     * Objects/fields which can only be written once.  Typically used
     * for fields that are initialized at some point and thereafter
     * immutable.
     */
    WriteOnce
  };

private:
  static inline unsigned makeFlags(const TypeID typeID,
                                   const Mutability mut) {
    return ((typeID & 0x7) << 2) | (mut & 0x3);
  }

protected:
  /*!
   * \brief Single word used to store both Type ID and Mutability.
   */
  const unsigned flags;

  /*!
   * \brief Initialize with a specific type ID and mutability.
   * \param typeID The type ID.
   * \param mut The mutability.
   */
  GenType(const TypeID typeID,
          const Mutability mut) :
    flags(makeFlags(typeID, mut)) {}

public:
  /*!
   * Array for converting Mutability into a string description.  This
   * contains a string at each index corresponding to a Mutability
   * value which describes that mutability in readable text.
   *
   * \brief Array of text descriptions of Mutability values.
   */
  static const char* const mutabilityStrs[];

  /*!
   * \brief Print this GenType to a stream.
   * \param stream Stream to which to print.
   */
  std::ostream& print(std::ostream& stream) const;

  /*!
   * \brief Get the mutability.
   * \return The mutability.
   */
  inline unsigned mutability() const {
    return (flags & 0x3);
  }

  /*!
   * \brief Get the type ID, which indicates what subclass this is.
   * \return The type ID.
   */
  inline TypeID getTypeID() const {
    return static_cast<TypeID>((flags >> 2) & 0x7);
  }

  /*!
   * \brief Get the mutability.
   * \return A readable string representation of the mutability.
   */
  inline const char* mutabilityName() const {
    return mutabilityStrs[mutability()];
  }

  /*!
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mut The mutability of the type.
   */
  static const GenType* get(const llvm::Module& M,
                            const llvm::MDNode* md,
                            Mutability mut);

  /*!
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mut The mutability of the type.
   */
  static const GenType* get(const llvm::Module& M,
                            const llvm::MDNode* md,
                            unsigned mut);

  /*!
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GenTypeVisitor& v) const = 0;

  /*!
   * This function runs a visitor on this type. The ctx argument will
   * be passed in as the parent argument in the visitor functions
   * instead of the visitor's default argument.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param ctx The context argument to pass in.
   */
  template <typename T> void accept(GenTypeCtxVisitor<T>& v,
				    T& ctx) const;

  /*!
   * \brief Structural comparison of two GenTypes.
   * \param other The GenType against which to compare.
   * \return Whether this GenType is structurally equal to other.
   */
  bool operator==(const GenType& other) const;

  /*!
   * \brief Structural comparison of two GenTypes.
   * \param other The GenType against which to compare.
   * \return Whether this GenType is structurally inequal to other.
   */
  inline bool operator!=(const GenType& other) const {
    return !(*this == other);
  }

};

inline std::ostream& operator<<(std::ostream& stream, const GenType& gcty) {
  return gcty.print(stream);
}

/*!
 * \brief A primitive type.
 */
class PrimGenType : public GenType {
private:
  /*!
   * \brief The underlying LLVM type.
   */
  llvm::Type* const typeRef;

  /*!
   * This should take one argument: a pointer to the top-level type,
   * and return a value of this type.
   *
   * \brief Accessor function.
   */
  llvm::Function* const getFunc;

  /*!
   * This should be null if the mutability is Immutable.
   *
   * \brief Modifier function.
   */
  llvm::Function* const setFunc;

  /*!
   * \brief Initialize with an underlying type and mutability.
   * \param typeRef The underlying LLVM type.
   * \param mut The mutability.
   */
  PrimGenType(llvm::Type* const typeRef,
              const Mutability mut,
              llvm::Function* const getFunc,
              llvm::Function* const setFunc) :
    GenType(PrimTypeID, mut), typeRef(typeRef),
    getFunc(getFunc), setFunc(setFunc) {}

  /*!
   * \brief A distinguished element for representing unit types.
   */
  static const PrimGenType* unitGenTy;
public:

  /*!
   * \brief Structural comparison of two PrimGenTypes.
   * \param other The PrimGenType against which to compare.
   * \return Whether this PrimGenType is structurally equal to other.
   */
  inline bool operator==(const PrimGenType& other) const {
    return typeRef == other.typeRef && flags == other.flags;
  }

  /*!
   * \brief Get the underlying LLVM type.
   * \return The underlying LLVM type.
   */
  inline llvm::Type* getLLVMType() const { return typeRef; }

  /*!
   * This always returns the same object.
   *
   * \brief Get the type representing unit.
   */
  static const PrimGenType* getUnit();

  /*!
   * This function builds a type from metadata.  It assumes the
   * metadata's type tag is GC_MD_INT, and the metadata node is
   * properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mut The mutability of the type.
   */
  static const PrimGenType* getInt(const llvm::Module& M,
                                   const llvm::MDNode* md,
                                   Mutability mut);

  /*!
   * This function assumes the metadata's type tag is GEN_TYPE_FLOAT,
   * and the metadata node is properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mut The mutability of the type.
   */
  static const PrimGenType* getFloat(const llvm::Module& M,
                                     const llvm::MDNode* md,
                                     Mutability mut);

  /*!
   * This function assumes the metadata's type tag is GEN_TYPE_NAMED,
   * and the metadata node is properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mut The mutability of the type.
   */
  static const PrimGenType* getNamed(const llvm::Module& M,
                                     const llvm::MDNode* md,
                                     Mutability mut);

  /*!
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GenTypeVisitor& v) const;

  /*!
   * The ctx argument will be passed in as the parent argument in the
   * visitor functions.
   *
   * \brief Run a context visitor on this type.
   * \param v The visitor to run.
   * \param ctx The context argument to pass in.
   */
  template<typename T> inline void accept(GenTypeCtxVisitor<T>& v,
					  T& ctx) const {
    v.visit(this, ctx);
  }

  /*!
   * This will return null if the GenType is not in fact a
   * PrimGenType.
   *
   * \brief Narrow a GenType to a PrimGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline PrimGenType* narrow(GenType* const ty) {
    if(GenType::PrimTypeID == ty->getTypeID()) {
      return static_cast<PrimGenType*>(ty);
    } else
      return NULL;
  }

  /*!
   * This will return null if the GenType is not in fact a
   * PrimGenType.
   *
   * \brief Narrow a GenType to a PrimGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline const PrimGenType* narrow(const GenType* const ty) {
    if(GenType::PrimTypeID == ty->getTypeID()) {
      return static_cast<const PrimGenType*>(ty);
    } else
      return NULL;
  }
};

/*!
 * Unlike LLVM arrays, these arrays may be of indeterminant size.
 *
 * \brief An array in a generated type
 */
class ArrayGenType : public GenType {
private:
  /*!
   * \brief The number of elements in the array.
   */
  const unsigned nelems;
  /*!
   * \brief The type of array elements.
   */
  const GenType* const elem;

  /*!
   * \brief Initialize from an element type, number of elements, and
   *        mutability.
   * \param elem The element type.
   * \param nelems The number of elements (defaults to 0, indicating unsized).
   * \param The mutability (defaults to mutable).
   */
  ArrayGenType(const GenType* elem,
               const unsigned nelems = 0,
               const Mutability mutability = Mutable) :
    GenType(ArrayTypeID, mutability), nelems(nelems), elem(elem) {}
public:
  /*!
   * \brief Structural comparison of two ArrayGenTypes.
   * \param other The ArrayGenType against which to compare.
   * \return Whether this ArrayGenType is structurally equal to other.
   */
  inline bool operator==(const ArrayGenType& other) const {
    return nelems == other.nelems && *elem == *other.elem &&
      flags == other.flags;
  }

  /*!
   * \brief Get the element type.
   * \return The element type.
   */
  inline const GenType* getElemTy() const { return elem; }

  /*!
   * \brief Check if this array has a definite size (other than 0).
   * \return Whether this array has a number of elements other than 0.
   */
  inline bool isSized() const { return nelems != 0; }

  /*!
   * \brief Get the number of elements in the array.
   * \return The number of elements in the array.
   */
  inline unsigned getNumElems() const { return nelems; }

  /*!
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GenTypeVisitor& v) const;

  /*!
   * The parent argument will be passed in as the parent argument in
   * the visitor functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param parent The context argument to pass in.
   */
  template<typename T> inline void accept(GenTypeCtxVisitor<T>& v,
					  T& parent) const {
    T ctx;
    const bool descend = v.begin(this, ctx, parent);

    if(descend)
      elem->accept<T>(v, ctx);

    v.end(this, ctx, parent);
  }

  /*!
   * This function assumes the metadata's type tag is GEN_TYPE_ARRAY,
   * and the metadata node is properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mutability The mutability of the type.
   */
  static const ArrayGenType* get(const llvm::Module& M,
                                 const llvm::MDNode* md,
                                 Mutability mutability);

  /*!
   * This will return null if the GenType is not in fact an
   * ArrayGenType.
   *
   * \brief Narrow a GenType to a ArrayGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline ArrayGenType* narrow(GenType* const ty) {
    if(GenType::ArrayTypeID == ty->getTypeID()) {
      return static_cast<ArrayGenType*>(ty);
    } else
      return NULL;
  }

  /*!
   * This will return null if the GenType is not in fact an
   * ArrayGenType.
   *
   * \brief Narrow a GenType to a ArrayGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline const ArrayGenType* narrow(const GenType* const ty) {
    if(GenType::ArrayTypeID == ty->getTypeID()) {
      return static_cast<const ArrayGenType*>(ty);
    } else
      return NULL;
  }
};

/*!
 * \brief Base class for generated pointer types.
 */
class PtrGenType : public GenType {
protected:
  /*!
   * \brief The pointed-to type.
   */
  llvm::Type* const inner;

  /*!
   * \brief Initialize from a type ID, an inner type, and mutability.
   * \param typeID The type ID.
   * \param inner The pointed-to type.
   * \param mut The mutability.
   */
  PtrGenType(const TypeID typeID,
             llvm::Type* const inner,
             const Mutability mut = Mutable) :
    GenType(typeID, mut), inner(inner) {}

public:
  /*!
   * \brief Get the pointed-to type.
   * \return The pointed-to type.
   */
  inline llvm::Type* getElemTy() const { return inner; }

  /*!
   * This attempts to narrow a pointer to a GenType to an PtrGenType.
   * This will return null if the GenType is not in fact an
   * PtrGenType.
   *
   * \brief Narrow a GenType to a PtrGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline PtrGenType* narrow(GenType* const ty) {
    const TypeID typeID = ty->getTypeID();

    if(GenType::NativePtrTypeID == typeID || GenType::GCPtrTypeID == typeID) {
      return static_cast<PtrGenType*>(ty);
    } else
      return NULL;
  }

  /*!
   * This attempts to narrow a pointer to a GenType to an PtrGenType.
   * This will return null if the GenType is not in fact an
   * PtrGenType.
   *
   * \brief Narrow a GenType to a PtrGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline const PtrGenType* narrow(const GenType* const ty) {
    const TypeID typeID = ty->getTypeID();

    if(GenType::NativePtrTypeID == typeID || GenType::GCPtrTypeID == typeID) {
      return static_cast<const PtrGenType*>(ty);
    } else
      return NULL;
  }
};

/*!
 * This class represents a pointer to something not contained in the
 * GC system.
 *
 * \brief A basic (non-GC) pointer.
 */
class NativePtrGenType : public PtrGenType {
private:
  /*!
   * \brief Initialize from an inner type and mutability.
   * \param inner The pointed-to type.
   * \param mut The mutability.
   */
  NativePtrGenType(llvm::Type* const inner,
                   const Mutability mut = Mutable)
    : PtrGenType(NativePtrTypeID, inner, mut) {}

public:
  /*!
   * \brief Structural comparison of two NativePtrGenTypes.
   * \param other The NativePtrGenType against which to compare.
   * \return Whether this NativePtrGenType is structurally equal to other.
   */
  inline bool operator==(const NativePtrGenType& other) const {
    return inner == other.inner && flags == other.flags;
  }

  /*!
   * This function runs a visitor on this type.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GenTypeVisitor& v) const;

  /*!
   * This function runs a visitor on this type. The ctx argument will
   * be passed in as the parent argument in the visitor functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param ctx The context argument to pass in.
   */
  template<typename T> inline void accept(GenTypeCtxVisitor<T>& v,
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
  static const NativePtrGenType* get(const llvm::Module& M,
                                     const llvm::MDNode* md,
                                     Mutability mutability);

  /*!
   * This attempts to narrow a pointer to a GenType to a NativePtrGenType.
   * This will return null if the GenType is not in fact an
   * NativePtrGenType.
   *
   * \brief Narrow a GenType to a NativePtrGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline NativePtrGenType* narrow(GenType* const ty) {
    if(GenType::NativePtrTypeID == ty->getTypeID()) {
      return static_cast<NativePtrGenType*>(ty);
    } else
      return NULL;
  }

  /*!
   * This attempts to narrow a pointer to a GenType to a NativePtrGenType.
   * This will return null if the GenType is not in fact an
   * NativePtrGenType.
   *
   * \brief Narrow a GenType to a NativePtrGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline const NativePtrGenType* narrow(const GenType* const ty) {
    if(GenType::NativePtrTypeID == ty->getTypeID()) {
      return static_cast<const NativePtrGenType*>(ty);
    } else
      return NULL;
  }
};

/*!
 * This class represents a pointer to a garbage-collected object.
 *
 * \brief A pointer to a GC object.
 */
class GCPtrGenType : public PtrGenType {
public:
  /*!
   * This enumeration represents pointer mobility classes.  This
   * indicates whether the garbage collector is permitted to move a
   * pointer, and how.
   *
   * \brief Enumeration for pointer mobility classes.
   */
  enum Mobility {
   /*!
    * This represents a pointer which can be moved arbitrarily by the
    * garbage collector.
    *
    * \brief Mobile pointer.
    */
    Mobile,
    /*!
     * This represents a pointer which the garbage collector cannot
     * move.  This is suitable for I/O buffers, among other things.
     *
     * \brief Immobile pointer.
     */
    Immobile
  };

  static const char* const mobilityStrs[];

  enum PtrClass {
    StrongPtr,
    SoftPtr,
    WeakPtr,
    FinalizerPtr,
    PhantomPtr
  };

  static const char* const ptrClassStrs[];

private:
  /*!
   * \brief The pointer class.
   */
  const PtrClass ptrclass;

  /*!
   * \brief The mobility class.
   */
  const Mobility mobility;

  GCPtrGenType(llvm::Type* const Inner,
               const Mutability mutability = Mutable,
               const Mobility mobility = Mobile,
               const PtrClass ptrclass = StrongPtr) :
    PtrGenType(GCPtrTypeID, Inner, mutability),
    ptrclass(ptrclass), mobility(mobility) {}
public:

  /*!
   * \brief Structural comparison of two GCPtrGenTypes.
   * \param other The GCPtrGenType against which to compare.
   * \return Whether this GCPtrGenType is structurally equal to other.
   */
  inline bool operator==(const GCPtrGenType& other) const {
    return inner == other.inner && flags == other.flags &&
      ptrclass == other.ptrclass && mobility == other.mobility;
  }

  inline PtrClass getPtrClass() const { return ptrclass; }

  inline const char* getPtrClassName() const {
    return ptrClassStrs[getPtrClass()];
  }

  inline Mobility getMobility() const { return mobility; }

  inline const char* getMobilityName() const {
    return mobilityStrs[getMobility()];
  }

  /*!
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GenTypeVisitor& v) const;

  /*!
   * The ctx argument will be passed in as the parent argument in the
   * visitor functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param ctx The context argument to pass in.
   */
  template<typename T> inline void accept(GenTypeCtxVisitor<T>& v,
					  T& ctx) const {
    v.visit(this, ctx);
  }

  /*!
   * This function assumes the metadata's type tag is GC_MD_GC_PTR,
   * and the metadata node is properly formatted
   *
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mutability The mutability of the type.
   */
  static const GCPtrGenType* get(const llvm::Module& M,
                                 const llvm::MDNode* md,
                                 Mutability mutability = Mutable);

  /*!
   * This will return null if the GenType is not in fact an
   * GCPtrGenType.
   *
   * \brief Narrow a GenType to a GCPtrGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline GCPtrGenType* narrow(GenType* const ty) {
    if(GenType::GCPtrTypeID == ty->getTypeID()) {
      return static_cast<GCPtrGenType*>(ty);
    } else
      return NULL;
  }

  /*!
   * This will return null if the GenType is not in fact an
   * GCPtrGenType.
   *
   * \brief Narrow a GenType to a GCPtrGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline const GCPtrGenType* narrow(const GenType* const ty) {
    if(GenType::GCPtrTypeID == ty->getTypeID()) {
      return static_cast<const GCPtrGenType*>(ty);
    } else
      return NULL;
  }

};


/*!
 * \brief A class representing a structure.
 */
class StructGenType : public GenType {
private:
  const GenType* const * const fieldtys;
  const unsigned nfields;
  const bool packed;

  StructGenType(const GenType* const * fieldtys,
                const unsigned nfields,
                const bool packed = false,
                const Mutability mutability = Mutable) :
    GenType(StructTypeID, mutability), fieldtys(fieldtys),
    nfields(nfields), packed(packed) {}
public:
  /*!
   * \brief Structural comparison of two StructGenTypes.
   * \param other The StructGenType against which to compare.
   * \return Whether this StructGenType is structurally equal to other.
   */
  inline bool operator==(const StructGenType& other) const {
    if(packed == other.packed && nfields == other.nfields) {
      for(unsigned i = 0; i < nfields; i++)
        if(*fieldtys[i] != *other.fieldtys[i])
          return false;
      return true;
    } else
      return false;
  }

  /*!
   * \brief Indicate whether this is a packed structure.
   * \return Whether this is a packed structure.
   */
  inline bool isPacked() const { return packed; }

  /*!
   * \brief Get the number of fields in this structure.
   * \return The number of fields in this structure.
   */
  inline unsigned numFields() const { return nfields; }

  /*!
   * \brief Get the type of a particular field.
   * \param idx Index of the field whose type to get.
   * \return The type of the field at index idx.
   * \invariant idx < numFields()
   */
  inline const GenType* fieldTy(unsigned idx) const { return fieldtys[idx]; }

  /*!
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GenTypeVisitor& v) const;

  /*!
   * The parent argument will be passed in as the parent argument in
   * the visitor functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param parent The context argument to pass in.
   */
  template<typename T> inline void accept(GenTypeCtxVisitor<T>& v,
					  T& parent) const {
    T ctx;
    const bool descend = v.begin(this, ctx, parent);

    if(descend)
      for(unsigned i = 0; i < nfields; i++)
	fieldtys[i]->accept<T>(v, ctx);

    v.end(this, ctx, parent);
  }

  /*!
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mut The mutability of the type.
   * \pre The metadata node md is properly formatted.
   * \pre The metadata node md has the type tag GEN_TYPE_STRUCT.
   */
  static const StructGenType* get(const llvm::Module& M,
                                  const llvm::MDNode* md,
                                  Mutability mut = Mutable);

  /*!
   * This will return null if the GenType is not in fact an
   * StructGenType.
   *
   * \brief Narrow a GenType to a StructGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline StructGenType* narrow(GenType* const ty) {
    if(GenType::StructTypeID == ty->getTypeID()) {
      return static_cast<StructGenType*>(ty);
    } else
      return NULL;
  }

  /*!
   * This will return null if the GenType is not in fact an
   * StructGenType.
   *
   * \brief Narrow a GenType to a StructGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline const StructGenType* narrow(const GenType* const ty) {
    if(GenType::StructTypeID == ty->getTypeID()) {
      return static_cast<const StructGenType*>(ty);
    } else
      return NULL;
  }
};

/*!
 * This class represents a function pointer type.
 *
 * \brief A function pointer.
 */
class FuncPtrGenType : public GenType {
private:
  /*!
   * \brief The return type.
   */
  const GenType* const retty;

  /*!
   * \brief The parameter types.
   */
  const GenType* const * const paramtys;

  /*!
   * \brief The number of parameters.
   */
  const unsigned nparams;

  /*!
   * \brief Whether the function has variable arguments.
   */
  const bool vararg;

  /*!
   * \brief Initialize from return type, parameter types, variable
   *        argument, and mutability.
   * \param retty The return type.
   * \param paramtys The parameter types.
   * \param nparams The number of parameter types.
   * \param vararg Whether this is a vararg function.
   * \param mut The mutability (defaults to Mutable).
   * \post Takes possession of the array passed to paramtys.
   */
  FuncPtrGenType(const GenType* const retty,
                 const GenType* const * const paramtys,
                 const unsigned nparams,
                 const bool vararg = false,
                 const Mutability mut = Mutable) :
    GenType(FuncPtrTypeID, mut), retty(retty),
    paramtys(paramtys), nparams(nparams), vararg(vararg) {}
public:
  /*!
   * \brief Structural comparison of two FuncPtrGenTypes.
   * \param other The FuncPtrGenType against which to compare.
   * \return Whether this FuncPtrGenType is structurally equal to other.
   */
  inline bool operator==(const FuncPtrGenType& other) const {
    if(vararg == other.vararg && nparams == other.nparams &&
       *retty == *other.retty) {
      for(unsigned i = 0; i < nparams; i++)
        if(*paramtys[i] != *other.paramtys[i])
          return false;
      return true;
    } else
      return false;
  }

  /*!
   * \brief Indicate whether this is a variable argument function.
   * \return Whether this is a variable argument function.
   */
  inline bool isVararg() const { return vararg; }

  /*!
   * \brief Get the number of parameters.
   * \return The number of parameters.
   */
  inline unsigned numParams() const { return nparams; }

  /*!
   * \brief Get the return type.
   * \return The return type.
   */
  inline const GenType* returnTy() const { return retty; }

  /*!
   * \brief Get the type of a given parameter.
   * \param idx The index of the parameter whose type to get.
   * \return The type of the parameter at index idx.
   * \invariant idx < numParams()
   */
  inline const GenType* paramTy(const unsigned idx) const {
    return paramtys[idx];
  }

  /*!
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   */
  virtual void accept(GenTypeVisitor&) const;

  /*!
   * The parent argument will be passed in as the parent argument in
   * the visitor functions.
   *
   * \brief Run a visitor on this type.
   * \param v The visitor to run.
   * \param parent The context argument to pass in.
   */
  template<typename T> inline void accept(GenTypeCtxVisitor<T>& v,
					  T& parent) const {
    T ctx;
    const bool descend = v.begin(this, ctx, parent);

    if(descend) {
      retty->accept<T>(v, ctx);

      const bool params = v.beginParams(this, ctx);

      if(params)
	for(unsigned i = 0; i < nparams; i++)
	  paramtys[i]->accept<T>(v, ctx);

      v.endParams(this, ctx);
    }

    v.end(this, ctx, parent);
  }

  /*!
   * \brief Construct a type from metadata.
   * \param M The module in which to build the type.
   * \param md The metadata from which to build the type.
   * \param mut The mutability of the type.
   * \pre The metadata node md is properly formatted.
   * \pre The metadata node md's type tag is GEN_TYPE_FUNC.
   */
  static const FuncPtrGenType* get(const llvm::Module& M,
                                   const llvm::MDNode* md,
                                   Mutability mut = Mutable);

  /*!
   * This will return null if the GenType is not in fact an
   * FuncPtrGenType.
   *
   * \brief Narrow a GenType to a FuncPtrGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline FuncPtrGenType* narrow(GenType* const ty) {
    if(GenType::FuncPtrTypeID == ty->getTypeID()) {
      return static_cast<FuncPtrGenType*>(ty);
    } else
      return NULL;
  }

  /*!
   * This will return null if the GenType is not in fact an
   * FuncPtrGenType.
   *
   * \brief Narrow a GenType to a FuncPtrGenType.
   * \param ty Pointer to narrow.
   * \return A pointer to ty, or null.
   */
  static inline const FuncPtrGenType* narrow(const GenType* const ty) {
    if(GenType::FuncPtrTypeID == ty->getTypeID()) {
      return static_cast<const FuncPtrGenType*>(ty);
    } else
      return NULL;
  }
};

template <typename T> void GenType::accept(GenTypeCtxVisitor<T>& v,
                                           T& ctx) const {
  switch(getTypeID()) {
  case PrimTypeID:
    static_cast<const PrimGenType*>(this)->accept(v, ctx);
    break;
  case StructTypeID:
    static_cast<const StructGenType*>(this)->accept(v, ctx);
    break;
  case ArrayTypeID:
    static_cast<const ArrayGenType*>(this)->accept(v, ctx);
    break;
  case FuncPtrTypeID:
    static_cast<const FuncPtrGenType*>(this)->accept(v, ctx);
    break;
  case NativePtrTypeID:
    static_cast<const NativePtrGenType*>(this)->accept(v, ctx);
    break;
  case GCPtrTypeID:
    static_cast<const GCPtrGenType*>(this)->accept(v, ctx);
    break;
  }
}

#endif
