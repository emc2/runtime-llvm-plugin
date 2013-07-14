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
#ifndef _GC_TYPE_VISITOR_H_
#define _GC_TYPE_VISITOR_H_

#include <llvm/Support/raw_ostream.h>

class GCType;
class PrimGCType;
class ArrayGCType;
class NativePtrGCType;
class GCPtrGCType;
class FuncPtrGCType;
class StructGCType;

/*!
 * This implements a visitor-like pattern for GC types.  It serves as
 * the basis for implementing all the other visitors.
 *
 * \brief Basic visitor class for GC types.
 */
class GCTypeVisitor {
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
  virtual bool begin(const StructGCType* ty);

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
  virtual bool begin(const FuncPtrGCType* ty);

  /*!
   * This is called when beginning to visit an array.  If the function
   * returns true, it will continue to visit the element type.
   * Otherwise, it will stop.
   *
   * \brief Begin an array type.
   * \param ty The type being visited.
   * \return Whether or not to visit the return type and parameters.
   */
  virtual bool begin(const ArrayGCType* ty);

  /*!
   * This is called after visiting all the fields in a structure.
   * This will not be called if begin returned false.
   *
   * \brief End a structure type.
   * \param ty The type being visited.
   */
  virtual void end(const StructGCType* ty);

  /*!
   * This is called after visiting the return type and parameters of a
   * function pointer.  This will not be called if begin returned
   * false.
   *
   * \brief End a function pointer type.
   * \param ty The type being visited.
   */
  virtual void end(const FuncPtrGCType* ty);

  /*!
   * This is called after visiting the element type of an
   * function pointer.  This will not be called if begin returned
   * false.
   *
   * \brief End a function pointer type.
   * \param ty The type being visited.
   */
  virtual void end(const ArrayGCType* ty);

  /*!
   * This is called when visiting a native pointer.  This is a leaf
   * node in a type, as native pointer types contain an LLVM type as
   * their element type.
   *
   * \brief Visit a native pointer.
   * \param ty The type being visited.
   */
  virtual void visit(const NativePtrGCType* ty);

  /*!
   * This is called when visiting a GC pointer.  This is a leaf
   * node in a type, as GC pointer types contain an LLVM type as
   * their element type.
   *
   * \brief Visit a native pointer.
   * \param ty The type being visited.
   */
  virtual void visit(const GCPtrGCType* ty);

  /*!
   * This is called when visiting a native type.  This is a leaf node
   * in a type.
   *
   * \brief Visit a primitive type.
   * \param ty The type being visited.
   */
  virtual void visit(const PrimGCType* ty);

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
  virtual bool beginParams(const FuncPtrGCType* ty);

  /*!
   * This is called after visiting the parameters of a function
   * pointer.  This will not be called if beginParams returned false.
   *
   * \brief End a function pointer type.
   * \param ty The type being visited.
   */
  virtual void endParams(const FuncPtrGCType* ty);
};

/*!
 * This implements a visitor which processes types with a hierarchical
 * context argument.  This can be used in conjunction with a builder
 * to construct trees without having to heap-allocate builders.
 *
 * \brief Contextual visitor class for GC types.
 */
template<typename T> class GCTypeContextVisitor {
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
  virtual bool begin(const StructGCType* ty, T& ctx, T& parent) {
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
  virtual bool begin(const FuncPtrGCType* ty, T& ctx, T& parent) {
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
  virtual bool begin(const ArrayGCType* ty, T& ctx, T& parent) {
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
  virtual void end(const StructGCType* ty, T& ctx, T& parent) {}

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
  virtual void end(const FuncPtrGCType* ty, T& ctx, T& parent) {}

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
  virtual void end(const ArrayGCType* ty, T& ctx, T& parent) {}

  /*!
   * This is called when visiting a native pointer.  This is a leaf
   * node in a type, as native pointer types contain an LLVM type as
   * their element type.
   *
   * \brief Visit a native pointer.
   * \param ty The type being visited.
   * \param parent The context for the parent.
   */
  virtual void visit(const NativePtrGCType* ty, T& parent) {}

  /*!
   * This is called when visiting a GC pointer.  This is a leaf
   * node in a type, as GC pointer types contain an LLVM type as
   * their element type.
   *
   * \brief Visit a native pointer.
   * \param ty The type being visited.
   * \param parent The context for the parent.
   */
  virtual void visit(const GCPtrGCType* ty, T& parent) {}

  /*!
   * This is called when visiting a native type.  This is a leaf node
   * in a type.
   *
   * \brief Visit a primitive type.
   * \param ty The type being visited.
   * \param parent The context for the parent.
   */
  virtual void visit(const PrimGCType* ty, T& parent) {}

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
  virtual bool beginParams(const FuncPtrGCType* ty, T& parent) { return true; }

  /*!
   * This is called after visiting the parameters of a function
   * pointer.  This will not be called if beginParams returned false.
   *
   * \brief End a function pointer type.
   * \param ty The type being visited.
   * \param parent The context for the parent.
   */
  virtual void endParams(const FuncPtrGCType* ty, T& parent) {}
};

// Some stock visitors, like a print visitor

class GCTypePrintVisitor : public GCTypeContextVisitor<bool> {
private:
  llvm::raw_ostream& stream;
public:
  GCTypePrintVisitor(llvm::raw_ostream& stream) : stream(stream) {}

  virtual bool begin(const StructGCType* ty, bool&, bool&);
  virtual bool begin(const FuncPtrGCType* ty, bool&, bool&);
  virtual bool begin(const ArrayGCType* ty, bool&, bool&);

  virtual void end(const StructGCType* ty, bool&, bool&);
  virtual void end(const FuncPtrGCType* ty, bool&, bool&);
  virtual void end(const ArrayGCType* ty, bool&, bool&);

  virtual void visit(const NativePtrGCType* ty, bool&);
  virtual void visit(const GCPtrGCType* ty, bool&);
  virtual void visit(const PrimGCType* ty, bool&);

  virtual bool beginParams(const FuncPtrGCType* ty, bool&);
  virtual void endParams(const FuncPtrGCType* ty, bool&);

  /*!
   * This function prints the given type by having this visitor visit
   * it.
   *
   * \brief Print the given type.
   * \param ty The type to print.
   */
  void print(const GCType* ty);

  /*!
   * This function prints the given type by having this visitor visit
   * it.
   *
   * \brief Print the given type.
   * \param ty The type to print.
   */
  void print(const GCType& ty);
};

#endif
