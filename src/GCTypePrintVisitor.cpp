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
  return true;
}

bool GCTypePrintVisitor::begin(const ArrayGCType* const ty,
			       bool& ctx, bool& first) {
  ctx = true;

  if(!first)
    stream << ", ";

  stream << ty->mutabilityName() << " " << "[ ";
  return true;
}

bool GCTypePrintVisitor::begin(const FuncPtrGCType* const ty,
			       bool& ctx, bool& first) {
  ctx = true;

  if(!first)
    stream << ", ";

  stream << ty->mutabilityName() << " ";
  return true;
}

void GCTypePrintVisitor::end(const StructGCType*, bool& first, bool&) {
  first = false;
  stream << " }";
}

void GCTypePrintVisitor::end(const ArrayGCType* const ty, bool& first, bool&) {
  first = false;
  if(ty->isSized())
    stream << " x " << ty->getNumElems();

  stream << " ]";
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
    ty->getLLVMType()->dump();
  }
}

void GCTypePrintVisitor::visit(const NativePtrGCType* const ty,
			       bool& first) {
  if(!first)
    stream << ", ";

  first = false;
  stream << ty->mutabilityName() << " ";
  ty->getElemTy()->dump();
  stream << "*";
}

void GCTypePrintVisitor::visit(const GCPtrGCType* const ty,
			       bool& first) {
  if(!first)
    stream << ", ";

  first = false;
  stream  << ty->mutabilityName() << " ";
  ty->getElemTy()->dump();
  stream << " " << ty->getMobilityName() << " gc " <<
    ty->getPtrClassName() << "*";
}

bool GCTypePrintVisitor::beginParams(const FuncPtrGCType*, bool&) {
  stream << "(";
  return true;
}

void GCTypePrintVisitor::endParams(const FuncPtrGCType*, bool&) {
  stream << ")";
}
