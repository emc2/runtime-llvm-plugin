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

#include "llvm/Pass.h"
#include "llvm/Module.h"
#include "llvm/Constants.h"
#include "GCType.h"
#include "GCTypeVisitors.h"
#include "ParseMetadataPass.h"

bool parseGCTypes(llvm::Module& M,
		  llvm::StringMap<const GCType*>& map) {
  const llvm::NamedMDNode* const md = M.getNamedMetadata("core.gc.types");
  GCTypePrintVisitor print(llvm::outs());

  for(unsigned int i = 0; i < md->getNumOperands(); i++) {
    const llvm::MDNode* const node = md->getOperand(i);
    const llvm::MDString* const tyname =
      llvm::cast<llvm::MDString>(node->getOperand(0));
    /*
    const unsigned mutability =
      llvm::cast<llvm::ConstantInt>(node->getOperand(1))->getZExtValue();
    */
    const llvm::MDNode* const desc =
      llvm::cast<llvm::MDNode>(node->getOperand(2));
    const GCType* const newty = GCType::get(M, desc);

    map[tyname->getString()] = newty;
  }

  return false;
}

bool ParseMetadataPass::runOnModule(llvm::Module& M) {
  bool out = false;

  out |= parseGCTypes(M, GCTypes);

  return out;
}

char ParseMetadataPass::ID = 0;
static llvm::RegisterPass<ParseMetadataPass> X("core-parse-metadata",
					       "Parse CORE Metadata",
					       false, true);
