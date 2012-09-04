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
#ifndef _TYPE_BUILDER_H_
#define _TYPE_BUILDER_H_

#include "GCType.h"
#include "llvm/Type.h"
#include "llvm/Module.h"

/*!
 * This class implements a simple interface for building compound LLVM
 * types.
 *
 * \brief A basic builder for LLVM types.
 */
class TypeBuilder {
public:

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
   * information from a StructGCType.
   *
   * \brief Create a structure builder.
   * \param gcty The GC structure type from which to initialize.
   * \param tyname The name to give the struture.  If the empty string
   *               is given, the struture will be unnamed.
   */
  StructTypeBuilder(const StructGCType* gcty,
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
   * information from an ArrayGCType.
   *
   * \brief Initialize an array builder from an ArrayGCType.
   * \param gcty The GC array type from which to initialize.
   */
  ArrayTypeBuilder(const ArrayGCType* gcty);

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
   * copying information from a FuncPtrGCType.
   *
   * \brief Initialize a function pointer builder from a FuncPtrGCType.
   * \param gcty The GC function pointer type from which to initialize.
   */
  FuncPtrTypeBuilder(const FuncPtrGCType* gcty);

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
