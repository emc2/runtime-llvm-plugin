/* Copyright (c) 2019 Eric McCorkle.
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
#ifndef _ACCESSOR_GENERATOR_H_
#define _ACCESSOR_GENERATOR_H_

#include "GenType.h"
#include "GenTypeVisitors.h"

class AccessorGenerator : public GenTypeCtxVisitor<TypeBuilder*> {
private:
  llvm::Module& M;
  const GCParams& params;

public:
  AccessorGenerator(llvm::Module& M, const GCParams& params) :
    M(M), params(params) {}

  virtual bool begin(const StructGenType*, TypeBuilder*&, TypeBuilder*&);
  virtual bool begin(const FuncPtrGenType*, TypeBuilder*&, TypeBuilder*&);
  virtual bool begin(const ArrayGenType*, TypeBuilder*&, TypeBuilder*&);

  virtual void end(const StructGenType*, TypeBuilder*&, TypeBuilder*&);
  virtual void end(const FuncPtrGenType*, TypeBuilder*&, TypeBuilder*&);
  virtual void end(const ArrayGenType*, TypeBuilder*&, TypeBuilder*&);

  virtual void visit(const NativePtrGenType*, TypeBuilder*&);
  virtual void visit(const GCPtrGenType*, TypeBuilder*&);
  virtual void visit(const PrimGenType*, TypeBuilder*&);

  /*!
   * This function realizes a GC type as an LLVM type by having the
   * visitor visit the type.
   *
   * \brief Convert a GC type into an LLVM type.
   * \param ty Type to convert.
   * \param name The name of the type.
   * \return The equivalent LLVM type.
   */
  virtual const llvm::Type* generate(const GenType* ty);
}

#endif
