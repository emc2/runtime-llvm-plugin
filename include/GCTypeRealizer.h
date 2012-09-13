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

#include "GCTypeVisitors.h"
#include "GCParams.h"
#include "TypeBuilder.h"
#include "llvm/Module.h"

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

  virtual TypeBuilder* initial(const GCType* ty);

  virtual bool begin(const StructGCType*, TypeBuilder*&, TypeBuilder*&);
  virtual bool begin(const FuncPtrGCType*, TypeBuilder*&, TypeBuilder*&);
  virtual bool begin(const ArrayGCType*, TypeBuilder*&, TypeBuilder*&);

  virtual void end(const StructGCType*, TypeBuilder*&, TypeBuilder*&);
  virtual void end(const FuncPtrGCType*, TypeBuilder*&, TypeBuilder*&);
  virtual void end(const ArrayGCType*, TypeBuilder*&, TypeBuilder*&);

  virtual void visit(const NativePtrGCType*, TypeBuilder*&);
  virtual void visit(const GCPtrGCType*, TypeBuilder*&);
  virtual void visit(const PrimGCType*, TypeBuilder*&);

  virtual bool beginParams(const FuncPtrGCType*, TypeBuilder*&);
  virtual void endParams(const FuncPtrGCType*, TypeBuilder*&);
};

#endif
