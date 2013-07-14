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
#ifndef _GC_TYPE_REALIZER_H_
#define _GC_TYPE_REALIZER_H_

#include "GCType.h"
#include "GCTypeVisitors.h"
#include "GCParams.h"
#include "TypeBuilder.h"
#include "llvm/IR/Module.h"

/*!
 * This is a subclass of GCTypeContextVisitor which works in
 * conjunction with TypeBuilders to create concrete representations
 * using llvm types..
 *
 * \brief A visitor which creates realizations of GC types.
 */
class GCTypeRealizer : public GCTypeContextVisitor<TypeBuilder*> {
private:
  llvm::Module& M;
  const GCParams& params;
public:
  GCTypeRealizer(llvm::Module& M, const GCParams& params) :
    M(M), params(params) {}

  virtual bool begin(const StructGCType*, TypeBuilder*&, TypeBuilder*&);
  virtual bool begin(const FuncPtrGCType*, TypeBuilder*&, TypeBuilder*&);
  virtual bool begin(const ArrayGCType*, TypeBuilder*&, TypeBuilder*&);

  virtual void end(const StructGCType*, TypeBuilder*&, TypeBuilder*&);
  virtual void end(const FuncPtrGCType*, TypeBuilder*&, TypeBuilder*&);
  virtual void end(const ArrayGCType*, TypeBuilder*&, TypeBuilder*&);

  virtual void visit(const NativePtrGCType*, TypeBuilder*&);
  virtual void visit(const GCPtrGCType*, TypeBuilder*&);
  virtual void visit(const PrimGCType*, TypeBuilder*&);

  /*!
   * This function realizes a GC type as an LLVM type by having the
   * visitor visit the type.
   *
   * \brief Convert a GC type into an LLVM type.
   * \param ty Type to convert.
   * \param name The name of the type.
   * \return The equivalent LLVM type.
   */
  virtual const llvm::Type* realize(const GCType* const ty,
				    const llvm::StringRef name);
};

#endif
