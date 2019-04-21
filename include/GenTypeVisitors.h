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
#ifndef _GEN_TYPE_VISITOR_H_
#define _GEN_TYPE_VISITOR_H_

#include <llvm/Support/raw_ostream.h>

class GenType;
class PrimGenType;
class ArrayGenType;
class NativePtrGenType;
class GCPtrGenType;
class FuncPtrGenType;
class StructGenType;

/*!
 * This implements a visitor-like pattern for generated types.  It
 * serves as the basis for implementing all the other visitors.
 *
 * \brief Basic visitor class for generated types.
 */
class GenTypeVisitor {
public:

  /*!
   * This is called when beginning to visit a structure.  If the
   * function returns true, it will continue to visit all the fields.
   * Otherwise, it will stop.
   *
   * If the function returns true, then all the fields of the
   * structure will be visited in order, followed by a call to end.
   *
   * \brief Begin a structure.
   * \param ty The type being visited.
   * \return Whether or not to visit the fields.
   */
  virtual bool begin(const StructGenType* ty);

  /*!
   * This is called when beginning to visit a function pointer.  If
   * the function returns true, it will continue to visit all the
   * parameters.  Otherwise, it will stop.
   *
   * If the function returns true, then the return type will be
   * visited, then beginParams will be called, which will choose
   * whether or not to visit the parameters.
   *
   * \brief Begin a function pointer.
   * \param ty The type being visited.
   * \return Whether or not to visit the return type and parameters.
   */
  virtual bool begin(const FuncPtrGenType* ty);

  /*!
   * This is called when beginning to visit an array.  If the function
   * returns true, it will continue to visit the element type.
   * Otherwise, it will stop.
   *
   * \brief Begin an array type.
   * \param ty The type being visited.
   * \return Whether or not to visit the return type and parameters.
   */
  virtual bool begin(const ArrayGenType* ty);

  /*!
   * This is called after visiting all the fields in a structure.
   * This will not be called if begin returned false.
   *
   * \brief End a structure type.
   * \param ty The type being visited.
   */
  virtual void end(const StructGenType* ty);

  /*!
   * This is called after visiting the return type and parameters of a
   * function pointer.  This will not be called if begin returned
   * false.
   *
   * \brief End a function pointer type.
   * \param ty The type being visited.
   */
  virtual void end(const FuncPtrGenType* ty);

  /*!
   * This is called after visiting the element type of an
   * function pointer.  This will not be called if begin returned
   * false.
   *
   * \brief End a function pointer type.
   * \param ty The type being visited.
   */
  virtual void end(const ArrayGenType* ty);

  /*!
   * This is called when visiting a native pointer.  This is a leaf
   * node in a type, as native pointer types contain an LLVM type as
   * their element type.
   *
   * \brief Visit a native pointer.
   * \param ty The type being visited.
   */
  virtual void visit(const NativePtrGenType* ty);

  /*!
   * This is called when visiting a GC pointer.  This is a leaf
   * node in a type, as GC pointer types contain an LLVM type as
   * their element type.
   *
   * \brief Visit a native pointer.
   * \param ty The type being visited.
   */
  virtual void visit(const GCPtrGenType* ty);

  /*!
   * This is called when visiting a native type.  This is a leaf node
   * in a type.
   *
   * \brief Visit a primitive type.
   * \param ty The type being visited.
   */
  virtual void visit(const PrimGenType* ty);

  /*!
   * This is called when beginning to visit the parameters of a
   * function pointer.  If the function returns true, it will continue
   * to visit all the parameters.  Otherwise, it will stop.
   *
   * If the function returns true, then the parameters will be visited
   * in order, followed by a call to endParams.  Otherwise, the
   * visitor will not visit the parameters.
   *
   * \brief Begin visiting a function pointer's parameters.
   * \param ty The type being visited.
   * \return Whether or not to visit the parameters.
   */
  virtual bool beginParams(const FuncPtrGenType* ty);

  /*!
   * This is called after visiting the parameters of a function
   * pointer.  This will not be called if beginParams returned false.
   *
   * \brief End a function pointer type.
   * \param ty The type being visited.
   */
  virtual void endParams(const FuncPtrGenType* ty);
};

/*!
 * This implements a visitor which processes types with a hierarchical
 * context argument.  This can be used in conjunction with a builder
 * to construct trees without having to heap-allocate builders.
 *
 * \brief Contextual visitor class for generated types.
 */
template<typename T> class GenTypeCtxVisitor {
public:
  /*!
   * This is called when beginning to visit a structure.  If the
   * function returns true, it will continue to visit all the fields.
   * Otherwise, it will stop.
   *
   * If the function returns true, then all the fields of the
   * structure will be visited in order, followed by a call to end.
   *
   * The ctx argument is created as a stack variable, which will last
   * as long as the current type is being visited.
   *
   * \brief Begin a structure.
   * \param ty The type being visited.
   * \param ctx The context argument being created for this type.
   * \param parent The context argument for the parent type.
   * \return Whether or not to visit the fields.
   */
  virtual bool begin(const StructGenType* ty,
                     T& ctx,
                     T& parent) {
    return true;
  }

  /*!
   * This is called when beginning to visit a function pointer.  If
   * the function returns true, it will continue to visit all the
   * parameters.  Otherwise, it will stop.
   *
   * If the function returns true, then the return type will be
   * visited, then beginParams will be called, which will choose
   * whether or not to visit the parameters.
   *
   * The ctx argument is created as a stack variable, which will last
   * as long as the current type is being visited.
   *
   * \brief Begin a function pointer.
   * \param ty The type being visited.
   * \param ctx The context argument being created for this type.
   * \param parent The context argument for the parent type.
   * \return Whether or not to visit the return type and parameters.
   */
  virtual bool begin(const FuncPtrGenType* ty,
                     T& ctx,
                     T& parent) {
    return true;
  }

  /*!
   * This is called when beginning to visit an array.  If the function
   * returns true, it will continue to visit the element type.
   * Otherwise, it will stop.
   *
   * The ctx argument is created as a stack variable, which will last
   * as long as the current type is being visited.
   *
   * \brief Begin an array type.
   * \param ty The type being visited.
   * \param ctx The context argument being created for this type.
   * \param parent The context argument for the parent type.
   * \return Whether or not to visit the return type and parameters.
   */
  virtual bool begin(const ArrayGenType* ty,
                     T& ctx,
                     T& parent) {
    return true;
  }

  /*!
   * This is called after visiting all the fields in a structure.
   * This will not be called if begin returned false.
   *
   * The ctx argument will be destroyed after this call ends.
   *
   * \brief End a structure type.
   * \param ctx The context argument for this type.
   * \param parent The context argument for the parent type.
   * \param ty The type being visited.
   */
  virtual void end(const StructGenType* ty,
                   T& ctx,
                   T& parent) {}

  /*!
   * This is called after visiting the return type and parameters of a
   * function pointer.  This will not be called if begin returned
   * false.
   *
   * The ctx argument will be destroyed after this call ends.
   *
   * \brief End a function pointer type.
   * \param ctx The context argument for this type.
   * \param parent The context argument for the parent type.
   * \param ty The type being visited.
   */
  virtual void end(const FuncPtrGenType* ty,
                   T& ctx,
                   T& parent) {}

  /*!
   * This is called after visiting the element type of an
   * function pointer.  This will not be called if begin returned
   * false.
   *
   * The ctx argument will be destroyed after this call ends.
   *
   * \brief End a function pointer type.
   * \param ctx The context argument for this type.
   * \param parent The context argument for the parent type.
   * \param ty The type being visited.
   */
  virtual void end(const ArrayGenType* ty,
                   T& ctx,
                   T& parent) {}

  /*!
   * This is called when visiting a native pointer.  This is a leaf
   * node in a type, as native pointer types contain an LLVM type as
   * their element type.
   *
   * \brief Visit a native pointer.
   * \param ty The type being visited.
   * \param parent The context for the parent.
   */
  virtual void visit(const NativePtrGenType* ty,
                     T& parent) {}

  /*!
   * This is called when visiting a GC pointer.  This is a leaf
   * node in a type, as GC pointer types contain an LLVM type as
   * their element type.
   *
   * \brief Visit a native pointer.
   * \param ty The type being visited.
   * \param parent The context for the parent.
   */
  virtual void visit(const GCPtrGenType* ty,
                     T& parent) {}

  /*!
   * This is called when visiting a native type.  This is a leaf node
   * in a type.
   *
   * \brief Visit a primitive type.
   * \param ty The type being visited.
   * \param parent The context for the parent.
   */
  virtual void visit(const PrimGenType* ty,
                     T& parent) {}

  /*!
   * This is called when beginning to visit the parameters of a
   * function pointer.  If the function returns true, it will continue
   * to visit all the parameters.  Otherwise, it will stop.
   *
   * If the function returns true, then the parameters will be visited
   * in order, followed by a call to endParams.  Otherwise, the
   * visitor will not visit the parameters.
   *
   * \brief Begin visiting a function pointer's parameters.
   * \param ty The type being visited.
   * \param parent The context for the parent.
   * \return Whether or not to visit the parameters.
   */
  virtual bool beginParams(const FuncPtrGenType* ty,
                           T& parent) {
    return true;
  }

  /*!
   * This is called after visiting the parameters of a function
   * pointer.  This will not be called if beginParams returned false.
   *
   * \brief End a function pointer type.
   * \param ty The type being visited.
   * \param parent The context for the parent.
   */
  virtual void endParams(const FuncPtrGenType* ty,
                         T& parent) {}
};

// Some stock visitors, like a print visitor

/*!
 * \brief A visitor that prints out generated types to a stream.
 */
class GenTypePrintVisitor : public GenTypeCtxVisitor<bool> {
private:
  /*!
   * Stream used to print out types.
   */
  llvm::raw_ostream& stream;
public:
  /*!
   * Initialize this GenTypePrintVisitor with a given stream.  This
   * will be used to print out generated types.
   *
   * \brief Initialize with a given stream.
   * \param stream Stream to which to print types.
   */
  GenTypePrintVisitor(llvm::raw_ostream& stream) : stream(stream) {}

  virtual bool begin(const StructGenType* ty, bool&, bool&);
  virtual bool begin(const FuncPtrGenType* ty, bool&, bool&);
  virtual bool begin(const ArrayGenType* ty, bool&, bool&);

  virtual void end(const StructGenType* ty, bool&, bool&);
  virtual void end(const FuncPtrGenType* ty, bool&, bool&);
  virtual void end(const ArrayGenType* ty, bool&, bool&);

  virtual void visit(const NativePtrGenType* ty, bool&);
  virtual void visit(const GCPtrGenType* ty, bool&);
  virtual void visit(const PrimGenType* ty, bool&);

  virtual bool beginParams(const FuncPtrGenType* ty, bool&);
  virtual void endParams(const FuncPtrGenType* ty, bool&);

  /*!
   * This function prints the given type by having this visitor visit
   * it.
   *
   * \brief Print the given type.
   * \param ty The type to print.
   */
  void print(const GenType* ty);

  /*!
   * This function prints the given type by having this visitor visit
   * it.
   *
   * \brief Print the given type.
   * \param ty The type to print.
   */
  void print(const GenType& ty);
};

#endif
