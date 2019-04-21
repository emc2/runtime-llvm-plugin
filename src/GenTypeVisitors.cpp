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
#include "GenType.h"
#include "GenTypeVisitors.h"

bool GenTypeVisitor::begin(const StructGenType*) { return true; }
bool GenTypeVisitor::begin(const FuncPtrGenType*) { return true; }
bool GenTypeVisitor::begin(const ArrayGenType*) { return true; }
void GenTypeVisitor::end(const StructGenType*) {}
void GenTypeVisitor::end(const FuncPtrGenType*) {}
void GenTypeVisitor::end(const ArrayGenType*) {}
void GenTypeVisitor::visit(const GCPtrGenType*) {}
void GenTypeVisitor::visit(const PrimGenType*) {}
void GenTypeVisitor::visit(const NativePtrGenType*) {}
bool GenTypeVisitor::beginParams(const FuncPtrGenType*) { return true; }
void GenTypeVisitor::endParams(const FuncPtrGenType*) {}
