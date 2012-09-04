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
#ifndef _GC_TYPE_REALIZER_H_
#define _GC_TYPE_REALIZER_H_

#include "GCTypeVisitors.h"
#include "TypeBuilder.h"

/*!
 * This is a subclass of GCTypeContextVisitor which works in
 * conjunction with TypeBuilders to create concrete representations
 * using llvm types..
 *
 * \brief A visitor which creates realizations of GC types.
 */
class GCTypeRealizer : public GCTypeContextVisitor<TypeBuilder*> {
public:

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
