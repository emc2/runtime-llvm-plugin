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

#include "llvm/Pass.h"
#include "llvm/Module.h"
#include "llvm/Constants.h"
#include "llvm/ADT/StringMap.h"
#include "GCType.h"
#include "GCTypeVisitors.h"
#include <llvm/Support/raw_ostream.h>

bool parseGCTypes(llvm::Module& M,
		  llvm::StringMap<const GCType*>& map) {
  const llvm::NamedMDNode* const md = M.getNamedMetadata("core.gc.types");
  GCTypePrintVisitor print(llvm::outs());
  bool first = true;

  for(unsigned int i = 0; i < md->getNumOperands(); i++) {
    const llvm::MDNode* const node = md->getOperand(i);
    const llvm::MDString* const tyname =
      llvm::cast<llvm::MDString>(node->getOperand(0));
    const unsigned mutability =
      llvm::cast<llvm::ConstantInt>(node->getOperand(1))->getZExtValue();
    const llvm::MDNode* const desc =
      llvm::cast<llvm::MDNode>(node->getOperand(2));
    const GCType* const newty = GCType::get(M, desc);

    newty->accept(print, first);
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
