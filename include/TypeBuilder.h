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
#ifndef _TYPE_BUILDER_H_
#define _TYPE_BUILDER_H_

#include "GenType.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Module.h"

/*!
 * This class implements a simple interface for building compound LLVM
 * types.
 *
 * \brief A basic builder for LLVM types.
 */
class TypeBuilder {
public:

  /*!
   * Virtual destructor, which does nothing on the base class.
   */
  virtual ~TypeBuilder() {}

  /*!
   * This function adds a component type to the type this builder is
   * constructing.  The exact meaning of this varies depending on the
   * exact type being built.
   *
   * \brief Add a type to the type being built.
   * \param ty The type to add.
   */
  virtual void add(llvm::Type* ty) = 0;

  /*!
   * This function completes the construction of the type.  This may
   * only be called when all the necessary components have been
   * supplied.
   *
   * \brief Build the type.
   * \param M The module in which to create the type.
   * \return The result type.
   */
  virtual llvm::Type* build(llvm::Module& M) = 0;
};

/*!
 * This class implements a builder for structure types.  This builder
 * is designed for building the bodies of named structures, though it
 * can also create unnamed structures as well.
 *
 * \brief A builder for structure types.
 */
class StructTypeBuilder : public TypeBuilder {
  unsigned field;
  const unsigned nfields;
  llvm::Type** const fields;
  const llvm::StringRef tyname;
  const bool packed;
public:

  /*!
   * This constructor initializes a structure builder from its
   * necessary parameters.
   *
   * \brief Create a structure builder.
   * \param nfields The number of fields.
   * \param tyname The name to give the struture.  If the empty string
   *               is given, the struture will be unnamed.
   * \param packed Whether or not the struture is packed.
   */
  StructTypeBuilder(unsigned nfields,
		    const llvm::StringRef tyname = "",
		    bool packed = false);

  /*!
   * This constructor initializes a structure builder by copying
   * information from a StructGenType.
   *
   * \brief Create a structure builder.
   * \param gcty The GC structure type from which to initialize.
   * \param tyname The name to give the struture.  If the empty string
   *               is given, the struture will be unnamed.
   */
  StructTypeBuilder(const StructGenType* gcty,
		    const llvm::StringRef tyname = "");

  ~StructTypeBuilder();

  /*!
   * This function adds a field to the structure being built.  Each
   * successive call to this function adds a new field.  The resulting
   * type will be a structure containing the fields added by this
   * function, in the order in which they were added.
   *
   * This function may only be called nfields times.
   *
   * \brief Add a field to the structure being built.
   * \param ty The type to add.
   */
  virtual void add(llvm::Type* ty);

  /*!
   * This function completes the construction of the structure type.
   * This may only be called when exactly nfields calls to add have
   * been made.
   *
   * \brief Build the type.
   * \param M The module in which to create the type.
   * \return The result type.
   */
  virtual llvm::Type* build(llvm::Module& M);
};

/*!
 * This class implements a builder for an array type.
 *
 * \brief Builder for an array type.
 */
class ArrayTypeBuilder : public TypeBuilder {
  llvm::Type* elemty;
  const unsigned length;
public:

  /*!
   * This constructor initializes an array builder from its necessary
   * parameters.
   *
   * \brief Initialize an array builder from its length.
   * \param length The length of the array.  Length zero indicates an
   *               array of indeterminant size.
   */
  ArrayTypeBuilder(unsigned length = 0);

  /*!
   * This constructor initializes an array builder by copying
   * information from an ArrayGenType.
   *
   * \brief Initialize an array builder from an ArrayGenType.
   * \param gcty The GC array type from which to initialize.
   */
  ArrayTypeBuilder(const ArrayGenType* gcty);

  /*!
   * This function adds the element type of the array.
   *
   * This function may only be called once.
   *
   * \brief Add the element type to the array.
   * \param ty The type to add.
   */
  virtual void add(llvm::Type* ty);

  /*!
   * This function completes the construction of the array type.  This
   * may only be called when exactly one call to add has been made.
   *
   * \brief Build the type.
   * \param M The module in which to create the type.
   * \return The result type.
   */
  virtual llvm::Type* build(llvm::Module& M);
};

/*!
 * This class implements a builder for function pointer types.
 *
 * \brief A builder for structure types.
 */
class FuncPtrTypeBuilder : public TypeBuilder {
  unsigned param;
  const unsigned nparams;
  llvm::Type** const params;
  llvm::Type* retty;
  const bool vararg;
public:

  /*!
   * This constructor initializes a function pointer builder from its
   * necessary parameters.
   *
   * \brief Initialize a function pointer builder.
   * \param gcty The GC function pointer type from which to initialize.
   */
  FuncPtrTypeBuilder(unsigned nparams,
		     bool varargs = false);

  /*!
   * This constructor initializes a function pointer builder by
   * copying information from a FuncPtrGenType.
   *
   * \brief Initialize a function pointer builder from a FuncPtrGenType.
   * \param gcty The GC function pointer type from which to initialize.
   */
  FuncPtrTypeBuilder(const FuncPtrGenType* gcty);

  ~FuncPtrTypeBuilder();

  /*!
   * This function adds the return and parameter types.  The first
   * call will set the return type.  Thereafter, subsequent calls will
   * set the parameter types.
   *
   * This function may only be called nparams + 1 times.
   *
   * \brief Add result and parameter types to the function pointer.
   * \param ty The type to add.
   */
  virtual void add(llvm::Type* ty);

  /*!
   * This function completes the construction of the function pointer
   * type.  This may only be called when nparams + 1 calls to add have
   * been made.
   *
   * \brief Build the type.
   * \param M The module in which to create the type.
   * \return The result type.
   */
  virtual llvm::Type* build(llvm::Module& M);
};

#endif
