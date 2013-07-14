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

#define __STDC_LIMIT_MACROS 1
#define __STDC_CONSTANT_MACROS 1
#include "GCType.h"
#include "GCTypeVisitors.h"

bool GCTypePrintVisitor::begin(const StructGCType* const ty,
			       bool& ctx, bool& first) {
  ctx = true;

  if(!first)
    stream << ", ";

  stream << ty->mutabilityName() << " " << "struct { ";
  stream.flush();
  return true;
}

bool GCTypePrintVisitor::begin(const ArrayGCType* const ty,
			       bool& ctx, bool& first) {
  ctx = true;

  if(!first)
    stream << ", ";

  stream << ty->mutabilityName() << " " << "[ ";
  stream.flush();
  return true;
}

bool GCTypePrintVisitor::begin(const FuncPtrGCType* const ty,
			       bool& ctx, bool& first) {
  ctx = true;

  if(!first)
    stream << ", ";

  stream << ty->mutabilityName() << " ";
  stream.flush();
  return true;
}

void GCTypePrintVisitor::end(const StructGCType*, bool& first, bool&) {
  first = false;
  stream << " }";
  stream.flush();
}

void GCTypePrintVisitor::end(const ArrayGCType* const ty, bool& first, bool&) {
  first = false;
  if(ty->isSized())
    stream << " x " << ty->getNumElems();

  stream << " ]";
  stream.flush();
}

void GCTypePrintVisitor::end(const FuncPtrGCType*, bool& first, bool&) {
  first = false;
}

void GCTypePrintVisitor::visit(const PrimGCType* const ty,
			       bool& first) {
  if(!first)
    stream << ", ";

  first = false;

  if(NULL == ty->getLLVMType())
    stream << "unit";
  else {
    stream << ty->mutabilityName() << " ";
    ty->getLLVMType()->print(stream);
  }
  stream.flush();
}

void GCTypePrintVisitor::visit(const NativePtrGCType* const ty,
			       bool& first) {
  if(!first)
    stream << ", ";

  first = false;
  stream << ty->mutabilityName() << " ";
  ty->getElemTy()->print(stream);
  stream << "*";
  stream.flush();
}

void GCTypePrintVisitor::visit(const GCPtrGCType* const ty,
			       bool& first) {
  if(!first)
    stream << ", ";

  first = false;
  stream  << ty->mutabilityName() << " ";
  ty->getElemTy()->print(stream);
  stream << " " << ty->getMobilityName() << " gc " <<
    ty->getPtrClassName() << "*";
  stream.flush();
}

bool GCTypePrintVisitor::beginParams(const FuncPtrGCType*, bool&) {
  stream << "(";
  stream.flush();
  return true;
}

void GCTypePrintVisitor::endParams(const FuncPtrGCType*, bool&) {
  stream << ")";
  stream.flush();
}

void GCTypePrintVisitor::print(const GCType* ty) {
  bool first = true;
  ty->accept(*this, first);
}

void GCTypePrintVisitor::print(const GCType& ty) {
  bool first = true;
  ty.accept(*this, first);
}
