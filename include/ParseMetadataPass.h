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

#ifndef _PARSE_METADATA_PASS_H_
#define _PARSE_METADATA_PASS_H_

#include "llvm/Pass.h"
#include "llvm/Module.h"
#include "llvm/Constants.h"
#include "llvm/ADT/StringMap.h"

/*!
 * This pass parses all metadata needed for GC realization.  The GC
 * type metadata will be parsed into GCType objects, and used to
 * populate the StringMap GCTypes accordingly.
 *
 * \brief A pass to parse all the metadata.
 */
struct ParseMetadataPass : public llvm::ModulePass {
  static char ID;

  llvm::StringMap<const GCType*> GCTypes;

  ParseMetadataPass() : llvm::ModulePass(ID) {}

  virtual bool runOnModule(llvm::Module& M);
};

#endif
