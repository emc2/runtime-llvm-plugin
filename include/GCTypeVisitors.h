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
#ifndef _GC_TYPE_VISITOR_H_
#define _GC_TYPE_VISITOR_H_

class GCType;
class PrimGCType;
class ArrayGCType;
class NativePtrGCType;
class GCPtrGCType;
class FuncPtrGCType;
class StructGCType;

/*!
 * This implements the visitor pattern for GC types.  It serves as the
 * basis for implementing all the other visitors.
 *
 * \brief Basic visitor class for GC types.
 */
class GCTypeVisitor {
public:
  virtual bool begin(const GCType* ty);
  virtual bool begin(const StructGCType* ty);
  virtual bool begin(const FuncPtrGCType* ty);
  virtual bool begin(const ArrayGCType* ty);

  virtual void end(const GCType* ty);
  virtual void end(const StructGCType* ty);
  virtual void end(const FuncPtrGCType* ty);
  virtual void end(const ArrayGCType* ty);

  virtual void visit(const GCType* ty);
  virtual void visit(const NativePtrGCType* ty);
  virtual void visit(const GCPtrGCType* ty);
  virtual void visit(const PrimGCType* ty);
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
  virtual bool begin(const GCType* ty, T& ctx, T& parent) {
    return true;
  }
  virtual bool begin(const StructGCType* ty, T& ctx, T& parent) {
    return true;
  }
  virtual bool begin(const FuncPtrGCType* ty, T& ctx, T& parent) {
    return true;
  }
  virtual bool begin(const ArrayGCType* ty, T& ctx, T& parent) {
    return true;
  }

  virtual void end(const GCType* ty, T& ctx, T& parent) {}
  virtual void end(const StructGCType* ty, T& ctx, T& parent) {}
  virtual void end(const FuncPtrGCType* ty, T& ctx, T& parent) {}
  virtual void end(const ArrayGCType* ty, T& ctx, T& parent) {}

  virtual void visit(const GCType* ty, T& parent) {}
  virtual void visit(const NativePtrGCType* ty, T& parent) {}
  virtual void visit(const GCPtrGCType* ty, T& parent) {}
  virtual void visit(const PrimGCType* ty, T& parent) {}
};

#endif
