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

#include "GCTypeVisitors.h"

bool GCTypeVisitor::begin(const GCType*) { return true; }
bool GCTypeVisitor::begin(const StructGCType*) { return true; }
bool GCTypeVisitor::begin(const FuncPtrGCType*) { return true; }
bool GCTypeVisitor::begin(const ArrayGCType*) { return true; }
bool GCTypeVisitor::begin(const NativePtrGCType*) { return true; }
void GCTypeVisitor::end(const GCType*) {}
void GCTypeVisitor::end(const StructGCType*) {}
void GCTypeVisitor::end(const FuncPtrGCType*) {}
void GCTypeVisitor::end(const ArrayGCType*) {}
void GCTypeVisitor::end(const NativePtrGCType*) {}
void GCTypeVisitor::visit(const GCType*) {}
void GCTypeVisitor::visit(const GCPtrGCType*) {}
void GCTypeVisitor::visit(const AtomicGCType*) {}

template <typename T>
bool GCTypeContextVisitor<T>::begin(const GCType*, T&, T&) { return true; }

template <typename T>
bool GCTypeContextVisitor<T>::begin(const StructGCType*, T&, T&) {
  return true;
}

template <typename T>
bool GCTypeContextVisitor<T>::begin(const FuncPtrGCType*, T&, T&) {
  return true;
}

template <typename T>
bool GCTypeContextVisitor<T>::begin(const ArrayGCType*, T&, T&) {
  return true;
}

template <typename T>
bool GCTypeContextVisitor<T>::begin(const NativePtrGCType*, T&, T&) {
  return true;
}

template <typename T>
void GCTypeContextVisitor<T>::end(const GCType*, T&, T&) {}
template <typename T>
void GCTypeContextVisitor<T>::end(const StructGCType*, T&, T&) {}
template <typename T>
void GCTypeContextVisitor<T>::end(const FuncPtrGCType*, T&, T&) {}
template <typename T>
void GCTypeContextVisitor<T>::end(const ArrayGCType*, T&, T&) {}
template <typename T>
void GCTypeContextVisitor<T>::end(const NativePtrGCType*, T&, T&) {}
template <typename T>
void GCTypeContextVisitor<T>::visit(const GCType*, T&) {}
template <typename T>
void GCTypeContextVisitor<T>::visit(const GCPtrGCType*, T&) {}
template <typename T>
void GCTypeContextVisitor<T>::visit(const AtomicGCType*, T&) {}
