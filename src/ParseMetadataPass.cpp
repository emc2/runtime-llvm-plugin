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
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/StringMap.h"
#include "GCType.h"
#include "GCTypeVisitors.h"
#include <llvm/Support/raw_ostream.h>

#include "GCParams.h"
#include "GCTypeRealizer.h"

bool parseGCTypes(llvm::Module& M,
		  llvm::StringMap<const GCType*>& map) {
  const llvm::NamedMDNode* const md = M.getNamedMetadata("core.gc.types");
  GCParams params(true, false, true, true, true, true, true, true);
  GCTypePrintVisitor print(llvm::outs());
  GCTypeRealizer realizer(M, params);

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

    print.print(newty);
    llvm::outs() << "\n";
    llvm::outs() << tyname->getString() << "\n";
    realizer.realize(newty, tyname->getString())->dump();
    llvm::outs() << "\n";
    map[tyname->getString()] = newty;
  }

  return false;
}

struct ParseMetadataPass : public llvm::ModulePass {
  static char ID;

  llvm::StringMap<const GCType*> GCTypes;

  ParseMetadataPass() : llvm::ModulePass(ID) {}

  virtual bool runOnModule(llvm::Module& M) {

    bool out = false;

    out |= parseGCTypes(M, GCTypes);

    return out;
  }
};

char ParseMetadataPass::ID = 0;
static llvm::RegisterPass<ParseMetadataPass> X("core-parse-metadata",
					       "Parse CORE Metadata",
					       false, true);
