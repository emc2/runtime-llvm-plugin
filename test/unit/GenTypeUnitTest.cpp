/* Copyright (c) 2013 Eric McCorkle.
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
#include "metadata.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"
#include <gtest/gtest.h>
#include <iostream>

static llvm::LLVMContext ctx;
static llvm::Module mod(llvm::StringRef("Test"), ctx);
static const llvm::StringRef tyname(llvm::StringRef("test"));
static llvm::MDString* const tynamemd = llvm::MDString::get(ctx, tyname);
static llvm::Type* const opaquetype = llvm::StructType::create(ctx, tyname);
static llvm::NamedMDNode* const testmd = mod.getOrInsertNamedMetadata("test");
static const PrimGenType* const unittype = PrimGenType::getUnit();
static llvm::Constant* const unittag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GEN_TYPE_UNIT);
static llvm::Constant* const inttag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GEN_TYPE_INT);
static llvm::Constant* const floattag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GEN_TYPE_FLOAT);
static llvm::Constant* const arrtag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GEN_TYPE_ARRAY);
static llvm::Constant* const nativeptrtag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GEN_TYPE_NATIVEPTR);
static llvm::Constant* const gcptrtag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GEN_TYPE_GCPTR);
static llvm::Constant* const structtag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GEN_TYPE_STRUCT);
static llvm::Constant* const functag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GEN_TYPE_FUNC);
static llvm::Constant* const mobiletag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), PTR_MOB_MOBILE);
static llvm::Constant* const immobiletag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), PTR_MOB_IMMOBILE);
static llvm::Constant* const mutabletag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), TYPE_MUT_MUTABLE);
static llvm::Constant* const writeoncetag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), TYPE_MUT_WRITEONCE);
static llvm::Constant* const immutabletag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), TYPE_MUT_IMMUTABLE);
static llvm::Constant* const strongtag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), PTRCLASS_STRONG);
static llvm::Constant* const softtag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), PTRCLASS_SOFT);
static llvm::Constant* const weaktag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), PTRCLASS_WEAK);
static llvm::Constant* const finalizertag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), PTRCLASS_FINALIZER);
static llvm::Constant* const phantomtag =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), PTRCLASS_PHANTOM);
static llvm::Constant* const consttrue =
  llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), 1);
static llvm::Constant* const constfalse =
  llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), 0);
static llvm::Constant* const const0 =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 0);
static llvm::Constant* const const1 =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 1);
static llvm::Constant* const const8 =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 8);
static llvm::Constant* const const16 =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 16);
static llvm::Constant* const const32 =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 32);
static llvm::Constant* const const64 =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 64);
static llvm::Constant* const const128 =
  llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 128);
static llvm::Metadata* const unitvals[1] = {
  llvm::ConstantAsMetadata::get(unittag)
};
static llvm::Metadata* const int1vals[2] = {
  llvm::ConstantAsMetadata::get(inttag),
  llvm::ConstantAsMetadata::get(const1)
};
static llvm::Metadata* const int8vals[2] = {
  llvm::ConstantAsMetadata::get(inttag),
  llvm::ConstantAsMetadata::get(const8)
};
static llvm::Metadata* const int16vals[2] = {
  llvm::ConstantAsMetadata::get(inttag),
  llvm::ConstantAsMetadata::get(const16)
};
static llvm::Metadata* const int32vals[2] = {
  llvm::ConstantAsMetadata::get(inttag),
  llvm::ConstantAsMetadata::get(const32)
};
static llvm::Metadata* const int64vals[2] = {
  llvm::ConstantAsMetadata::get(inttag),
  llvm::ConstantAsMetadata::get(const64)
};
static llvm::Metadata* const int128vals[2] = {
  llvm::ConstantAsMetadata::get(inttag),
  llvm::ConstantAsMetadata::get(const128)
};
static llvm::Metadata* const inttoolongvals[3] = {
  llvm::ConstantAsMetadata::get(inttag),
  llvm::ConstantAsMetadata::get(const32),
  llvm::ConstantAsMetadata::get(const0)
};
static llvm::Metadata* const inttooshortvals[1] = {
  llvm::ConstantAsMetadata::get(inttag)
};
static llvm::Metadata* const float16vals[2] = {
  llvm::ConstantAsMetadata::get(floattag),
  llvm::ConstantAsMetadata::get(const16)
};
static llvm::Metadata* const float32vals[2] = {
  llvm::ConstantAsMetadata::get(floattag),
  llvm::ConstantAsMetadata::get(const32)
};
static llvm::Metadata* const float64vals[2] = {
  llvm::ConstantAsMetadata::get(floattag),
  llvm::ConstantAsMetadata::get(const64)
};
static llvm::Metadata* const float128vals[2] = {
  llvm::ConstantAsMetadata::get(floattag),
  llvm::ConstantAsMetadata::get(const128)
};
static llvm::Metadata* const floattoolongvals[3] = {
  llvm::ConstantAsMetadata::get(floattag),
  llvm::ConstantAsMetadata::get(const32),
  llvm::ConstantAsMetadata::get(const0)
};
static llvm::Metadata* const floattooshortvals[1] = {
  llvm::ConstantAsMetadata::get(floattag)
};
static llvm::MDNode* const unitmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(unitvals));
static llvm::MDNode* const int1md =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(int1vals));
static llvm::MDNode* const int8md =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(int8vals));
static llvm::MDNode* const int16md =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(int16vals));
static llvm::MDNode* const int32md =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(int32vals));
static llvm::MDNode* const int64md =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(int64vals));
static llvm::MDNode* const int128md =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(int128vals));
static llvm::MDNode* const inttoolongmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(inttoolongvals));
static llvm::MDNode* const inttooshortmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(inttooshortvals));
static llvm::MDNode* const float16md =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(float16vals));
static llvm::MDNode* const float32md =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(float32vals));
static llvm::MDNode* const float64md =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(float64vals));
static llvm::MDNode* const float128md =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(float128vals));
static llvm::MDNode* const floattoolongmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(floattoolongvals));
static llvm::MDNode* const floattooshortmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(floattooshortvals));
static llvm::Metadata* const unsizedarrvals[2] = {
  llvm::ConstantAsMetadata::get(arrtag),
  int32md
};
static llvm::Metadata* const sizedarrvals[3] = {
  llvm::ConstantAsMetadata::get(arrtag),
  int32md,
  llvm::ConstantAsMetadata::get(const16)
};
static llvm::Metadata* const arrtoolongvals[4] = {
  llvm::ConstantAsMetadata::get(arrtag),
  int32md,
  llvm::ConstantAsMetadata::get(const0),
  llvm::ConstantAsMetadata::get(const0)
};
static llvm::Metadata* const arrtooshortvals[1] = {
  llvm::ConstantAsMetadata::get(arrtag)
};
static llvm::MDNode* const unsizedarrmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(unsizedarrvals));
static llvm::MDNode* const sizedarrmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(sizedarrvals));
static llvm::MDNode* const arrtoolongmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(arrtoolongvals));
static llvm::MDNode* const arrtooshortmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(arrtooshortvals));
static llvm::Metadata* const nativeptrvals[2] = {
  llvm::ConstantAsMetadata::get(nativeptrtag),
  tynamemd
};
static llvm::Metadata* const nativeptrtoolongvals[3] = {
  llvm::ConstantAsMetadata::get(nativeptrtag),
  tynamemd,
  llvm::ConstantAsMetadata::get(const0)
};
static llvm::Metadata* const nativeptrtooshortvals[1] = {
  llvm::ConstantAsMetadata::get(nativeptrtag)
};
static llvm::MDNode* const nativeptrmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(nativeptrvals));
static llvm::MDNode* const nativeptrtoolongmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(nativeptrtoolongvals));
static llvm::MDNode* const nativeptrtooshortmd =
  llvm::MDNode::get(ctx,
                    llvm::ArrayRef<llvm::Metadata*>(nativeptrtooshortvals));
static llvm::Metadata* const gcptrstrongvals[4] = {
  llvm::ConstantAsMetadata::get(gcptrtag),
  llvm::ConstantAsMetadata::get(mobiletag),
  llvm::ConstantAsMetadata::get(strongtag),
  tynamemd
};
static llvm::Metadata* const gcptrsoftvals[4] = {
  llvm::ConstantAsMetadata::get(gcptrtag),
  llvm::ConstantAsMetadata::get(mobiletag),
  llvm::ConstantAsMetadata::get(softtag),
  tynamemd
};
static llvm::Metadata* const gcptrweakvals[4] = {
  llvm::ConstantAsMetadata::get(gcptrtag),
  llvm::ConstantAsMetadata::get(mobiletag),
  llvm::ConstantAsMetadata::get(weaktag),
  tynamemd
};
static llvm::Metadata* const gcptrfinalizervals[4] = {
  llvm::ConstantAsMetadata::get(gcptrtag),
  llvm::ConstantAsMetadata::get(mobiletag),
  llvm::ConstantAsMetadata::get(finalizertag),
  tynamemd
};
static llvm::Metadata* const gcptrphantomvals[4] = {
  llvm::ConstantAsMetadata::get(gcptrtag),
  llvm::ConstantAsMetadata::get(mobiletag),
  llvm::ConstantAsMetadata::get(phantomtag),
  tynamemd
};
static llvm::Metadata* const gcptrimmobilevals[4] = {
  llvm::ConstantAsMetadata::get(gcptrtag),
  llvm::ConstantAsMetadata::get(immobiletag),
  llvm::ConstantAsMetadata::get(strongtag),
  tynamemd
};
static llvm::Metadata* const gcptrtoolongvals[5] = {
  llvm::ConstantAsMetadata::get(gcptrtag),
  llvm::ConstantAsMetadata::get(mobiletag),
  llvm::ConstantAsMetadata::get(strongtag),
  tynamemd,
  llvm::ConstantAsMetadata::get(const0)
};
static llvm::Metadata* const gcptrtooshortvals[3] = {
  llvm::ConstantAsMetadata::get(gcptrtag),
  llvm::ConstantAsMetadata::get(mobiletag),
  llvm::ConstantAsMetadata::get(strongtag)
};
static llvm::MDNode* const gcptrstrongmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(gcptrstrongvals));
static llvm::MDNode* const gcptrsoftmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(gcptrsoftvals));
static llvm::MDNode* const gcptrweakmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(gcptrweakvals));
static llvm::MDNode* const gcptrfinalizermd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(gcptrfinalizervals));
static llvm::MDNode* const gcptrphantommd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(gcptrphantomvals));
static llvm::MDNode* const gcptrimmobilemd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(gcptrimmobilevals));
static llvm::MDNode* const gcptrtoolongmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(gcptrtoolongvals));
static llvm::MDNode* const gcptrtooshortmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(gcptrtooshortvals));
static llvm::Metadata* const immint1fieldvals[2] = {
  llvm::ConstantAsMetadata::get(immutabletag),
  int1md
};
static llvm::Metadata* const mutint1fieldvals[2] = {
  llvm::ConstantAsMetadata::get(mutabletag),
  int1md
};
static llvm::Metadata* const woint1fieldvals[2] = {
  llvm::ConstantAsMetadata::get(writeoncetag),
  int1md
};
static llvm::Metadata* const immint8fieldvals[2] = {
  llvm::ConstantAsMetadata::get(immutabletag),
  int8md
};
static llvm::Metadata* const mutint8fieldvals[2] = {
  llvm::ConstantAsMetadata::get(mutabletag),
  int8md
};
static llvm::Metadata* const woint8fieldvals[2] = {
  llvm::ConstantAsMetadata::get(writeoncetag),
  int8md
};
static llvm::Metadata* const immint32fieldvals[2] = {
  llvm::ConstantAsMetadata::get(immutabletag),
  int32md
};
static llvm::Metadata* const mutint32fieldvals[2] = {
  llvm::ConstantAsMetadata::get(mutabletag),
  int32md
};
static llvm::Metadata* const woint32fieldvals[2] = {
  llvm::ConstantAsMetadata::get(writeoncetag),
  int32md
};
static llvm::Metadata* const immnativeptrfieldvals[2] = {
  llvm::ConstantAsMetadata::get(immutabletag),
  nativeptrmd
};
static llvm::Metadata* const mutnativeptrfieldvals[2] = {
  llvm::ConstantAsMetadata::get(mutabletag),
  nativeptrmd
};
static llvm::Metadata* const wonativeptrfieldvals[2] = {
  llvm::ConstantAsMetadata::get(writeoncetag),
  nativeptrmd
};
static llvm::Metadata* const immgcptrfieldvals[2] = {
  llvm::ConstantAsMetadata::get(immutabletag),
  gcptrstrongmd
};
static llvm::Metadata* const mutgcptrfieldvals[2] = {
  llvm::ConstantAsMetadata::get(mutabletag),
  gcptrstrongmd
};
static llvm::Metadata* const wogcptrfieldvals[2] = {
  llvm::ConstantAsMetadata::get(writeoncetag),
  gcptrstrongmd
};
static llvm::Metadata* const immarrayfieldvals[2] = {
  llvm::ConstantAsMetadata::get(immutabletag),
  sizedarrmd
};
static llvm::Metadata* const mutarrayfieldvals[2] = {
  llvm::ConstantAsMetadata::get(mutabletag),
  sizedarrmd
};
static llvm::Metadata* const woarrayfieldvals[2] = {
  llvm::ConstantAsMetadata::get(writeoncetag),
  sizedarrmd
};
static llvm::MDNode* const immint1fieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(immint1fieldvals));
static llvm::MDNode* const mutint1fieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(mutint1fieldvals));
static llvm::MDNode* const woint1fieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(woint1fieldvals));
static llvm::MDNode* const immint8fieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(immint8fieldvals));
static llvm::MDNode* const mutint8fieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(mutint8fieldvals));
static llvm::MDNode* const woint8fieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(woint8fieldvals));
static llvm::MDNode* const immint32fieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(immint32fieldvals));
static llvm::MDNode* const mutint32fieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(mutint32fieldvals));
static llvm::MDNode* const woint32fieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(woint32fieldvals));
static llvm::MDNode* const immnativeptrfieldmd =
  llvm::MDNode::get(ctx,
                    llvm::ArrayRef<llvm::Metadata*>(immnativeptrfieldvals));
static llvm::MDNode* const mutnativeptrfieldmd =
  llvm::MDNode::get(ctx,
                    llvm::ArrayRef<llvm::Metadata*>(mutnativeptrfieldvals));
static llvm::MDNode* const wonativeptrfieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(wonativeptrfieldvals));
static llvm::MDNode* const immgcptrfieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(immgcptrfieldvals));
static llvm::MDNode* const mutgcptrfieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(mutgcptrfieldvals));
static llvm::MDNode* const wogcptrfieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(wogcptrfieldvals));
static llvm::MDNode* const immarrayfieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(immarrayfieldvals));
static llvm::MDNode* const mutarrayfieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(mutarrayfieldvals));
static llvm::MDNode* const woarrayfieldmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(woarrayfieldvals));
static llvm::Metadata* const structnormvals[5] = {
  llvm::ConstantAsMetadata::get(structtag),
  llvm::ConstantAsMetadata::get(constfalse),
  immint32fieldmd,
  wonativeptrfieldmd,
  mutarrayfieldmd
};
static llvm::Metadata* const structstrictvals[5] = {
  llvm::ConstantAsMetadata::get(structtag),
  llvm::ConstantAsMetadata::get(consttrue),
  immint32fieldmd,
  wonativeptrfieldmd,
  mutarrayfieldmd
};
static llvm::Metadata* const structallwovals[5] = {
  llvm::ConstantAsMetadata::get(structtag),
  llvm::ConstantAsMetadata::get(constfalse),
  woint32fieldmd,
  wonativeptrfieldmd,
  woarrayfieldmd
};
static llvm::Metadata* const structallimmvals[5] = {
  llvm::ConstantAsMetadata::get(structtag),
  llvm::ConstantAsMetadata::get(constfalse),
  immint32fieldmd,
  immnativeptrfieldmd,
  immarrayfieldmd
};
static llvm::MDNode* const structnormmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(structnormvals));
static llvm::MDNode* const structstrictmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(structstrictvals));
static llvm::MDNode* const structallwomd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(structallwovals));
static llvm::MDNode* const structallimmmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(structallimmvals));
static llvm::Metadata* const immstructnormfieldvals[2] = {
  llvm::ConstantAsMetadata::get(immutabletag),
  structnormmd
};
static llvm::Metadata* const wostructnormfieldvals[2] = {
  llvm::ConstantAsMetadata::get(writeoncetag),
  structnormmd
};
static llvm::Metadata* const mutstructnormfieldvals[2] = {
  llvm::ConstantAsMetadata::get(mutabletag),
  structnormmd
};
static llvm::Metadata* const structallwofieldvals[2] = {
  llvm::ConstantAsMetadata::get(mutabletag),
  structallwomd
};
static llvm::Metadata* const structallimmfieldvals[2] = {
  llvm::ConstantAsMetadata::get(mutabletag),
  structallimmmd
};
static llvm::MDNode* const immstructnormfieldmd =
  llvm::MDNode::get(ctx,
                    llvm::ArrayRef<llvm::Metadata*>(immstructnormfieldvals));
static llvm::MDNode* const wostructnormfieldmd =
  llvm::MDNode::get(ctx,
                    llvm::ArrayRef<llvm::Metadata*>(wostructnormfieldvals));
static llvm::MDNode* const mutstructnormfieldmd =
  llvm::MDNode::get(ctx,
                    llvm::ArrayRef<llvm::Metadata*>(mutstructnormfieldvals));
static llvm::MDNode* const structallwofieldmd =
  llvm::MDNode::get(ctx,
                    llvm::ArrayRef<llvm::Metadata*>(structallwofieldvals));
static llvm::MDNode* const structallimmfieldmd =
  llvm::MDNode::get(ctx,
                    llvm::ArrayRef<llvm::Metadata*>(structallimmfieldvals));
static llvm::Metadata* const structnestedvals[4] = {
  llvm::ConstantAsMetadata::get(structtag),
  llvm::ConstantAsMetadata::get(constfalse),
  immint32fieldmd,
  mutstructnormfieldmd
};
static llvm::Metadata* const structimmnestedvals[4] = {
  llvm::ConstantAsMetadata::get(structtag),
  llvm::ConstantAsMetadata::get(constfalse),
  immint32fieldmd,
  immstructnormfieldmd
};
static llvm::Metadata* const structimmnestedvals2[4] = {
  llvm::ConstantAsMetadata::get(structtag),
  llvm::ConstantAsMetadata::get(constfalse),
  immint32fieldmd,
  structallimmfieldmd
};
static llvm::Metadata* const structwonestedvals[4] = {
  llvm::ConstantAsMetadata::get(structtag),
  llvm::ConstantAsMetadata::get(constfalse),
  immint32fieldmd,
  wostructnormfieldmd
};
static llvm::Metadata* const structwonestedvals2[4] = {
  llvm::ConstantAsMetadata::get(structtag),
  llvm::ConstantAsMetadata::get(constfalse),
  immint32fieldmd,
  structallwofieldmd
};
static llvm::MDNode* const structnestedmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(structnestedvals));
static llvm::MDNode* const structimmnestedmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(structimmnestedvals));
static llvm::MDNode* const structimmnestedmd2 =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(structimmnestedvals2));
static llvm::MDNode* const structwonestedmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(structwonestedvals));
static llvm::MDNode* const structwonestedmd2 =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(structwonestedvals2));
static llvm::Metadata* const funczeroargvals[3] = {
  llvm::ConstantAsMetadata::get(functag),
  llvm::ConstantAsMetadata::get(constfalse),
  unitmd
};
static llvm::Metadata* const functwoargvals[5] = {
  llvm::ConstantAsMetadata::get(functag),
  llvm::ConstantAsMetadata::get(constfalse),
  unitmd,
  int32md,
  nativeptrmd
};
static llvm::Metadata* const funcvarargvals[5] = {
  llvm::ConstantAsMetadata::get(functag),
  llvm::ConstantAsMetadata::get(consttrue),
  unitmd,
  int32md,
  nativeptrmd
};
static llvm::Metadata* const funcnestedvals[4] = {
  llvm::ConstantAsMetadata::get(functag),
  llvm::ConstantAsMetadata::get(constfalse),
  gcptrstrongmd,
  structnestedmd
};
static llvm::MDNode* const funczeroargmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(funczeroargvals));
static llvm::MDNode* const functwoargmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(functwoargvals));
static llvm::MDNode* const funcvarargmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(funcvarargvals));
static llvm::MDNode* const funcnestedmd =
  llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Metadata*>(funcnestedvals));
/*
class GenTypeUnitTest : public CppUnit::TestFixture  {
public:
  {
    testmd->addOperand(unitmd);
    testmd->addOperand(int1md);
    testmd->addOperand(int8md);
    testmd->addOperand(int16md);
    testmd->addOperand(int32md);
    testmd->addOperand(int64md);
    testmd->addOperand(int128md);
    testmd->addOperand(inttoolongmd);
    testmd->addOperand(inttooshortmd);
    testmd->addOperand(float16md);
    testmd->addOperand(float32md);
    testmd->addOperand(float64md);
    testmd->addOperand(float128md);
    testmd->addOperand(floattoolongmd);
    testmd->addOperand(floattooshortmd);
    testmd->addOperand(unsizedarrmd);
    testmd->addOperand(sizedarrmd);
    testmd->addOperand(arrtoolongmd);
    testmd->addOperand(arrtooshortmd);
    testmd->addOperand(nativeptrmd);
    testmd->addOperand(nativeptrtoolongmd);
    testmd->addOperand(nativeptrtooshortmd);
    testmd->addOperand(gcptrstrongmd);
    testmd->addOperand(gcptrsoftmd);
    testmd->addOperand(gcptrweakmd);
    testmd->addOperand(gcptrfinalizermd);
    testmd->addOperand(gcptrphantommd);
    testmd->addOperand(gcptrimmobilemd);
    testmd->addOperand(gcptrtoolongmd);
    testmd->addOperand(gcptrtooshortmd);
    testmd->addOperand(structnormmd);
    testmd->addOperand(structstrictmd);
    testmd->addOperand(structallwomd);
    testmd->addOperand(structallimmmd);
    testmd->addOperand(structnestedmd);
    testmd->addOperand(structimmnestedmd);
    testmd->addOperand(structimmnestedmd2);
    testmd->addOperand(structwonestedmd);
    testmd->addOperand(structwonestedmd2);
    testmd->addOperand(funczeroargmd);
    testmd->addOperand(functwoargmd);
    testmd->addOperand(funcvarargmd);
    testmd->addOperand(funcnestedmd);
  }
};
*/
// Visitor for unit testing the visitor pattern.  It takes a script
// describing the visitation pattern, and checks it along the way.
class UnitTestVisitor : public GenTypeVisitor {
public:
  enum { VISIT, BEGIN, END, BEGINPARAMS, ENDPARAMS };

  struct Action {
    const GenType* const ty;
    const unsigned action;
    const bool descend;

    Action(const GenType* const ty,
           const unsigned action = VISIT,
           const bool descend = false) :
      ty(ty), action(action), descend(descend) {}
  };

  const Action* const script;
  const unsigned len;
  unsigned index;

  UnitTestVisitor(const Action* const script,
                  const unsigned len) :
    GenTypeVisitor(), script(script), len(len), index(0) {}

  inline void finish() { EXPECT_EQ(index, len); }

  virtual bool begin(const StructGenType* const ty);
  virtual bool begin(const FuncPtrGenType* const ty);
  virtual bool begin(const ArrayGenType* const ty);
  virtual void end(const StructGenType* const ty);
  virtual void end(const FuncPtrGenType* const ty);
  virtual void end(const ArrayGenType* const ty);
  virtual void visit(const NativePtrGenType* ty);
  virtual void visit(const GCPtrGenType* ty);
  virtual void visit(const PrimGenType* ty);
  virtual bool beginParams(const FuncPtrGenType* const ty);
  virtual void endParams(const FuncPtrGenType* ty);
};

std::ostream& operator<<(std::ostream& stream,
                         const UnitTestVisitor::Action& act) {
  switch(act.action) {
  default: return stream;
  case UnitTestVisitor::BEGIN: return stream << "BEGIN " << *act.ty;
  case UnitTestVisitor::VISIT: return stream << "VISIT " << *act.ty;
  case UnitTestVisitor::END: return stream << "END " << *act.ty;
  case UnitTestVisitor::BEGINPARAMS: return stream << "BEGINPARAM " << *act.ty;
  case UnitTestVisitor::ENDPARAMS: return stream << "ENDPARAM " << *act.ty;
  }
}

// Unit test visitor implementation
bool UnitTestVisitor::begin(const StructGenType* const ty) {
  if (index < len) {
    EXPECT_EQ(script[index].action, BEGIN);
    EXPECT_EQ(script[index].ty, ty);
  } else {
    ADD_FAILURE();
  }

  return script[index++].descend;
}

bool UnitTestVisitor::begin(const FuncPtrGenType* const ty) {
  if (index < len) {
    EXPECT_EQ(script[index].action, BEGIN);
    EXPECT_EQ(script[index].ty, ty);
  } else {
    ADD_FAILURE();
  }

  return script[index++].descend;
}

bool UnitTestVisitor::begin(const ArrayGenType* const ty) {
  if (index < len) {
    EXPECT_EQ(script[index].action, BEGIN);
    EXPECT_EQ(script[index].ty, ty);
  } else {
    ADD_FAILURE();
  }

  return script[index++].descend;
}

void UnitTestVisitor::end(const StructGenType* const ty) {
  ASSERT_LE(index, len);
  EXPECT_EQ(script[index].action, END);
  EXPECT_EQ(script[index].ty, ty);
  index++;
}

void UnitTestVisitor::end(const FuncPtrGenType* const ty) {
  ASSERT_LE(index, len);
  EXPECT_EQ(script[index].action, END);
  EXPECT_EQ(script[index].ty, ty);
  index++;
}

void UnitTestVisitor::end(const ArrayGenType* const ty) {
  ASSERT_LE(index, len);
  EXPECT_EQ(script[index].action, END);
  EXPECT_EQ(script[index].ty, ty);
  index++;
}

void UnitTestVisitor::visit(const NativePtrGenType* const ty) {
  ASSERT_LE(index, len);
  EXPECT_EQ(script[index].action, VISIT);
  EXPECT_EQ(script[index].ty, ty);
  index++;
}

void UnitTestVisitor::visit(const GCPtrGenType* const ty) {
  ASSERT_LE(index, len);
  EXPECT_EQ(script[index].action, VISIT);
  EXPECT_EQ(script[index].ty, ty);
  index++;
}

void UnitTestVisitor::visit(const PrimGenType* const ty) {
  ASSERT_LE(index, len);
  EXPECT_EQ(script[index].action, VISIT);
  EXPECT_EQ(script[index].ty, ty);
  index++;
}

bool UnitTestVisitor::beginParams(const FuncPtrGenType* const ty) {
  if (index < len) {
    EXPECT_EQ(script[index].action, BEGINPARAMS);
    EXPECT_EQ(script[index].ty, ty);
  } else {
    ADD_FAILURE();
  }

  return script[index++].descend;
}

void UnitTestVisitor::endParams(const FuncPtrGenType* const ty) {
  ASSERT_LE(index, len);
  EXPECT_EQ(script[index].action, ENDPARAMS);
  EXPECT_EQ(script[index].ty, ty);
  index++;
}

TEST(GenType, test_PrimGenType_getUnit) {
  EXPECT_EQ(unittype->getLLVMType(), NULL);
  EXPECT_EQ(unittype->mutability(), GenType::Immutable);
  EXPECT_EQ(unittype->getTypeID(), GenType::PrimTypeID);
}

TEST(GenType, test_GenType_get_unit) {
  const GenType* const got = GenType::get(mod, unitmd, GenType::Immutable);

  ASSERT_EQ(got->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(got)->getLLVMType(), NULL);
  EXPECT_EQ(got->mutability(), GenType::Immutable);

  // Test 3: test parsing the metadata, starting with Mutable
  // The resulting type should still read as immutable (it's a unit).
  const GenType* const mutgot = GenType::get(mod, unitmd, GenType::Mutable);

  ASSERT_EQ(mutgot->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(mutgot)->getLLVMType(), NULL);
  EXPECT_EQ(mutgot->mutability(), GenType::Immutable);

  // Test 3: test parsing the metadata, starting with WriteOnce
  // The resulting type should still read as immutable (it's a unit).
  const GenType* const wogot = GenType::get(mod, unitmd, GenType::WriteOnce);

  ASSERT_EQ(wogot->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(wogot)->getLLVMType(), NULL);
  EXPECT_EQ(wogot->mutability(), GenType::Immutable);

  // No need to test parsing a bad metadata, that will be handled by
  // the test for the top-level get.
}

TEST(GenType, test_PrimGenType_getInt_1) {

  // setup metadata that's too long and too short

  // Test 1: Test getInt by itself on all good metadata
  const PrimGenType* const immint1type =
    PrimGenType::getInt(mod, int1md, GenType::Immutable);
  const PrimGenType* const mutint1type =
    PrimGenType::getInt(mod, int1md, GenType::Mutable);
  const PrimGenType* const woint1type =
    PrimGenType::getInt(mod, int1md, GenType::WriteOnce);

  EXPECT_EQ(immint1type->getLLVMType(), llvm::Type::getInt1Ty(ctx));
  EXPECT_EQ(immint1type->mutability(), GenType::Immutable);
  EXPECT_EQ(immint1type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(mutint1type->getLLVMType(), llvm::Type::getInt1Ty(ctx));
  EXPECT_EQ(mutint1type->mutability(), GenType::Mutable);
  EXPECT_EQ(mutint1type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(woint1type->getLLVMType(), llvm::Type::getInt1Ty(ctx));
  EXPECT_EQ(woint1type->mutability(), GenType::WriteOnce);
  EXPECT_EQ(woint1type->getTypeID(), GenType::PrimTypeID);

}

TEST(GenType, test_PrimGenType_getInt_8) {
  const PrimGenType* const immint8type =
    PrimGenType::getInt(mod, int8md, GenType::Immutable);
  const PrimGenType* const mutint8type =
    PrimGenType::getInt(mod, int8md, GenType::Mutable);
  const PrimGenType* const woint8type =
    PrimGenType::getInt(mod, int8md, GenType::WriteOnce);

  EXPECT_EQ(immint8type->getLLVMType(), llvm::Type::getInt8Ty(ctx));
  EXPECT_EQ(immint8type->mutability(), GenType::Immutable);
  EXPECT_EQ(immint8type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(mutint8type->getLLVMType(), llvm::Type::getInt8Ty(ctx));
  EXPECT_EQ(mutint8type->mutability(), GenType::Mutable);
  EXPECT_EQ(mutint8type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(woint8type->getLLVMType(), llvm::Type::getInt8Ty(ctx));
  EXPECT_EQ(woint8type->mutability(), GenType::WriteOnce);
  EXPECT_EQ(woint8type->getTypeID(), GenType::PrimTypeID);
}

TEST(GenType, test_PrimGenType_getInt_16) {
  const PrimGenType* const immint16type =
    PrimGenType::getInt(mod, int16md, GenType::Immutable);
  const PrimGenType* const mutint16type =
    PrimGenType::getInt(mod, int16md, GenType::Mutable);
  const PrimGenType* const woint16type =
    PrimGenType::getInt(mod, int16md, GenType::WriteOnce);

  EXPECT_EQ(immint16type->getLLVMType(), llvm::Type::getInt16Ty(ctx));
  EXPECT_EQ(immint16type->mutability(), GenType::Immutable);
  EXPECT_EQ(immint16type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(mutint16type->getLLVMType(), llvm::Type::getInt16Ty(ctx));
  EXPECT_EQ(mutint16type->mutability(), GenType::Mutable);
  EXPECT_EQ(mutint16type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(woint16type->getLLVMType(), llvm::Type::getInt16Ty(ctx));
  EXPECT_EQ(woint16type->mutability(), GenType::WriteOnce);
  EXPECT_EQ(woint16type->getTypeID(), GenType::PrimTypeID);
}

TEST(GenType, test_PrimGenType_getInt_32) {
  const PrimGenType* const immint32type =
    PrimGenType::getInt(mod, int32md, GenType::Immutable);
  const PrimGenType* const mutint32type =
    PrimGenType::getInt(mod, int32md, GenType::Mutable);
  const PrimGenType* const woint32type =
    PrimGenType::getInt(mod, int32md, GenType::WriteOnce);

  EXPECT_EQ(immint32type->getLLVMType(), llvm::Type::getInt32Ty(ctx));
  EXPECT_EQ(immint32type->mutability(), GenType::Immutable);
  EXPECT_EQ(immint32type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(mutint32type->getLLVMType(), llvm::Type::getInt32Ty(ctx));
  EXPECT_EQ(mutint32type->mutability(), GenType::Mutable);
  EXPECT_EQ(mutint32type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(woint32type->getLLVMType(), llvm::Type::getInt32Ty(ctx));
  EXPECT_EQ(woint32type->mutability(), GenType::WriteOnce);
  EXPECT_EQ(woint32type->getTypeID(), GenType::PrimTypeID);
}

TEST(GenType, test_PrimGenType_getInt_64) {
  const PrimGenType* const immint64type =
    PrimGenType::getInt(mod, int64md, GenType::Immutable);
  const PrimGenType* const mutint64type =
    PrimGenType::getInt(mod, int64md, GenType::Mutable);
  const PrimGenType* const woint64type =
    PrimGenType::getInt(mod, int64md, GenType::WriteOnce);

  EXPECT_EQ(immint64type->getLLVMType(), llvm::Type::getInt64Ty(ctx));
  EXPECT_EQ(immint64type->mutability(), GenType::Immutable);
  EXPECT_EQ(immint64type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(mutint64type->getLLVMType(), llvm::Type::getInt64Ty(ctx));
  EXPECT_EQ(mutint64type->mutability(), GenType::Mutable);
  EXPECT_EQ(mutint64type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(woint64type->getLLVMType(), llvm::Type::getInt64Ty(ctx));
  EXPECT_EQ(woint64type->mutability(), GenType::WriteOnce);
  EXPECT_EQ(woint64type->getTypeID(), GenType::PrimTypeID);
}

TEST(GenType, test_PrimGenType_getInt_128) {
  const PrimGenType* const immint128type =
    PrimGenType::getInt(mod, int128md, GenType::Immutable);
  const PrimGenType* const mutint128type =
    PrimGenType::getInt(mod, int128md, GenType::Mutable);
  const PrimGenType* const woint128type =
    PrimGenType::getInt(mod, int128md, GenType::WriteOnce);

  EXPECT_EQ(immint128type->getLLVMType(), llvm::Type::getIntNTy(ctx, 128));
  EXPECT_EQ(immint128type->mutability(), GenType::Immutable);
  EXPECT_EQ(immint128type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(immint128type->getLLVMType(), llvm::Type::getIntNTy(ctx, 128));
  EXPECT_EQ(mutint128type->mutability(), GenType::Mutable);
  EXPECT_EQ(mutint128type->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(immint128type->getLLVMType(), llvm::Type::getIntNTy(ctx, 128));
  EXPECT_EQ(woint128type->mutability(), GenType::WriteOnce);
  EXPECT_EQ(woint128type->getTypeID(), GenType::PrimTypeID);
}

TEST(GenType, test_GenType_get_int_1) {
  const GenType* const immint1gctype =
    GenType::get(mod, int1md, TYPE_MUT_IMMUTABLE);
  const GenType* const mutint1gctype =
    GenType::get(mod, int1md, TYPE_MUT_MUTABLE);
  const GenType* const woint1gctype =
    GenType::get(mod, int1md, TYPE_MUT_WRITEONCE);

  ASSERT_EQ(immint1gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(immint1gctype)->getLLVMType(),
            llvm::Type::getInt1Ty(ctx));
  EXPECT_EQ(immint1gctype->mutability(), GenType::Immutable);
  ASSERT_EQ(mutint1gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(mutint1gctype)->getLLVMType(),
            llvm::Type::getInt1Ty(ctx));
  EXPECT_EQ(mutint1gctype->mutability(), GenType::Mutable);
  ASSERT_EQ(woint1gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(woint1gctype)->getLLVMType(),
            llvm::Type::getInt1Ty(ctx));
  EXPECT_EQ(woint1gctype->mutability(), GenType::WriteOnce);
}

TEST(GenType, test_GenType_get_int_8) {
  const GenType* const immint8gctype =
    GenType::get(mod, int8md, TYPE_MUT_IMMUTABLE);
  const GenType* const mutint8gctype =
    GenType::get(mod, int8md, TYPE_MUT_MUTABLE);
  const GenType* const woint8gctype =
    GenType::get(mod, int8md, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(PrimGenType::narrow(immint8gctype)->getLLVMType(),
            llvm::Type::getInt8Ty(ctx));
  EXPECT_EQ(immint8gctype->mutability(), GenType::Immutable);
  EXPECT_EQ(immint8gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(mutint8gctype)->getLLVMType(),
            llvm::Type::getInt8Ty(ctx));
  EXPECT_EQ(mutint8gctype->mutability(), GenType::Mutable);
  EXPECT_EQ(mutint8gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(woint8gctype)->getLLVMType(),
            llvm::Type::getInt8Ty(ctx));
  EXPECT_EQ(woint8gctype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(woint8gctype->getTypeID(), GenType::PrimTypeID);
}

TEST(GenType, test_GenType_get_int_16) {
  const GenType* const immint16gctype =
    GenType::get(mod, int16md, TYPE_MUT_IMMUTABLE);
  const GenType* const mutint16gctype =
    GenType::get(mod, int16md, TYPE_MUT_MUTABLE);
  const GenType* const woint16gctype =
    GenType::get(mod, int16md, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(PrimGenType::narrow(immint16gctype)->getLLVMType(),
            llvm::Type::getInt16Ty(ctx));
  EXPECT_EQ(immint16gctype->mutability(), GenType::Immutable);
  EXPECT_EQ(immint16gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(mutint16gctype)->getLLVMType(),
            llvm::Type::getInt16Ty(ctx));
  EXPECT_EQ(mutint16gctype->mutability(), GenType::Mutable);
  EXPECT_EQ(mutint16gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(woint16gctype)->getLLVMType(),
            llvm::Type::getInt16Ty(ctx));
  EXPECT_EQ(woint16gctype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(woint16gctype->getTypeID(), GenType::PrimTypeID);
}

TEST(GenType, test_GenType_get_int_32) {
  const GenType* const immint32gctype =
    GenType::get(mod, int32md, TYPE_MUT_IMMUTABLE);
  const GenType* const mutint32gctype =
    GenType::get(mod, int32md, TYPE_MUT_MUTABLE);
  const GenType* const woint32gctype =
    GenType::get(mod, int32md, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(PrimGenType::narrow(immint32gctype)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));
  EXPECT_EQ(immint32gctype->mutability(), GenType::Immutable);
  EXPECT_EQ(immint32gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(mutint32gctype)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));
  EXPECT_EQ(mutint32gctype->mutability(), GenType::Mutable);
  EXPECT_EQ(mutint32gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(woint32gctype)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));
  EXPECT_EQ(woint32gctype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(woint32gctype->getTypeID(), GenType::PrimTypeID);
}

TEST(GenType, test_GenType_get_int_64) {
  const GenType* const immint64gctype =
    GenType::get(mod, int64md, TYPE_MUT_IMMUTABLE);
  const GenType* const mutint64gctype =
    GenType::get(mod, int64md, TYPE_MUT_MUTABLE);
  const GenType* const woint64gctype =
    GenType::get(mod, int64md, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(PrimGenType::narrow(immint64gctype)->getLLVMType(),
            llvm::Type::getInt64Ty(ctx));
  EXPECT_EQ(immint64gctype->mutability(), GenType::Immutable);
  EXPECT_EQ(immint64gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(mutint64gctype)->getLLVMType(),
            llvm::Type::getInt64Ty(ctx));
  EXPECT_EQ(mutint64gctype->mutability(), GenType::Mutable);
  EXPECT_EQ(mutint64gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(woint64gctype)->getLLVMType(),
            llvm::Type::getInt64Ty(ctx));
  EXPECT_EQ(woint64gctype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(woint64gctype->getTypeID(), GenType::PrimTypeID);
}

TEST(GenType, test_GenType_get_int_128) {
  const GenType* const immint128gctype =
    GenType::get(mod, int128md, TYPE_MUT_IMMUTABLE);
  const GenType* const mutint128gctype =
    GenType::get(mod, int128md, TYPE_MUT_MUTABLE);
  const GenType* const woint128gctype =
    GenType::get(mod, int128md, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(PrimGenType::narrow(immint128gctype)->getLLVMType(),
            llvm::Type::getIntNTy(ctx, 128));
  EXPECT_EQ(immint128gctype->mutability(), GenType::Immutable);
  EXPECT_EQ(immint128gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(mutint128gctype)->getLLVMType(),
            llvm::Type::getIntNTy(ctx, 128));
  EXPECT_EQ(mutint128gctype->mutability(), GenType::Mutable);
  EXPECT_EQ(mutint128gctype->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(woint128gctype)->getLLVMType(),
            llvm::Type::getIntNTy(ctx, 128));
  EXPECT_EQ(woint128gctype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(woint128gctype->getTypeID(), GenType::PrimTypeID);
}

TEST(GenType, test_ArrayGenType_get) {

  // setup metadata that's too long and too short

  // Test 1: Test ArrayType::get by itself on all good metadata
  const ArrayGenType* const unsizedimmtype =
    ArrayGenType::get(mod, unsizedarrmd, GenType::Immutable);
  const ArrayGenType* const unsizedmuttype =
    ArrayGenType::get(mod, unsizedarrmd, GenType::Mutable);
  const ArrayGenType* const unsizedwotype =
    ArrayGenType::get(mod, unsizedarrmd, GenType::WriteOnce);
  const GenType* const unsizedimmelem = unsizedimmtype->getElemTy();
  const GenType* const unsizedmutelem = unsizedmuttype->getElemTy();
  const GenType* const unsizedwoelem = unsizedwotype->getElemTy();

  EXPECT_EQ(unsizedimmelem->mutability(), GenType::Immutable);
  EXPECT_EQ(unsizedimmtype->getTypeID(), GenType::ArrayTypeID);
  EXPECT_FALSE(unsizedimmtype->isSized());

  ASSERT_EQ(unsizedimmelem->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(unsizedimmelem)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));

  EXPECT_EQ(unsizedmutelem->mutability(), GenType::Mutable);
  EXPECT_EQ(unsizedmuttype->getTypeID(), GenType::ArrayTypeID);
  EXPECT_FALSE(unsizedmuttype->isSized());

  ASSERT_EQ(unsizedmutelem->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(unsizedmutelem)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));

  EXPECT_EQ(unsizedwoelem->mutability(), GenType::WriteOnce);
  EXPECT_EQ(unsizedwotype->getTypeID(), GenType::ArrayTypeID);
  EXPECT_FALSE(unsizedwotype->isSized());

  ASSERT_EQ(unsizedwoelem->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(unsizedwoelem)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));

  const ArrayGenType* const sizedimmtype =
    ArrayGenType::get(mod, sizedarrmd, GenType::Immutable);
  const ArrayGenType* const sizedmuttype =
    ArrayGenType::get(mod, sizedarrmd, GenType::Mutable);
  const ArrayGenType* const sizedwotype =
    ArrayGenType::get(mod, sizedarrmd, GenType::WriteOnce);
  const GenType* const sizedimmelem = sizedimmtype->getElemTy();
  const GenType* const sizedmutelem = sizedmuttype->getElemTy();
  const GenType* const sizedwoelem = sizedwotype->getElemTy();

  EXPECT_EQ(sizedimmelem->mutability(), GenType::Immutable);
  EXPECT_EQ(sizedimmtype->getTypeID(), GenType::ArrayTypeID);
  EXPECT_TRUE(sizedimmtype->isSized());
  EXPECT_EQ(sizedimmtype->getNumElems(), 16);

  ASSERT_EQ(sizedimmelem->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(sizedimmelem)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));

  EXPECT_EQ(sizedmutelem->mutability(), GenType::Mutable);
  EXPECT_EQ(sizedmuttype->getTypeID(), GenType::ArrayTypeID);
  EXPECT_TRUE(sizedmuttype->isSized());
  EXPECT_EQ(sizedmuttype->getNumElems(), 16);

  ASSERT_EQ(sizedmutelem->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(sizedmutelem)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));

  ASSERT_EQ(sizedwoelem->mutability(), GenType::WriteOnce);
  EXPECT_EQ(sizedwotype->getTypeID(), GenType::ArrayTypeID);
  EXPECT_TRUE(sizedwotype->isSized());
  EXPECT_EQ(sizedwotype->getNumElems(), 16);

  ASSERT_EQ(sizedwoelem->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(sizedwoelem)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));

  // Test 1: Test GenType::get on all good metadata
  const GenType* const unsizedimmgot =
    GenType::get(mod, unsizedarrmd, GenType::Immutable);
  const GenType* const unsizedmutgot =
    GenType::get(mod, unsizedarrmd, GenType::Mutable);
  const GenType* const unsizedwogot =
    GenType::get(mod, unsizedarrmd, GenType::WriteOnce);

  EXPECT_EQ(unsizedimmgot->mutability(), GenType::Immutable);

  ASSERT_EQ(unsizedimmgot->getTypeID(), GenType::ArrayTypeID);

  const ArrayGenType* const narrowed1 = ArrayGenType::narrow(unsizedimmgot);
  const GenType* const elem1 = narrowed1->getElemTy();

  EXPECT_EQ(elem1->mutability(), GenType::Immutable);
  EXPECT_FALSE(narrowed1->isSized());

  ASSERT_EQ(elem1->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(elem1)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));

  EXPECT_EQ(unsizedmutgot->mutability(), GenType::Mutable);

  ASSERT_EQ(unsizedimmgot->getTypeID(), GenType::ArrayTypeID);

  const ArrayGenType* const narrowed2 = ArrayGenType::narrow(unsizedmutgot);
  const GenType* const elem2 = narrowed2->getElemTy();

  EXPECT_EQ(elem2->mutability(), GenType::Mutable);
  EXPECT_FALSE(narrowed2->isSized());

  ASSERT_EQ(elem2->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(elem2)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));

  EXPECT_EQ(unsizedwogot->mutability(), GenType::WriteOnce);

  ASSERT_EQ(unsizedimmgot->getTypeID(), GenType::ArrayTypeID);

  const ArrayGenType* const narrowed3 = ArrayGenType::narrow(unsizedwogot);
  const GenType* const elem3 = narrowed3->getElemTy();

  EXPECT_EQ(elem3->mutability(), GenType::WriteOnce);
  EXPECT_TRUE(!narrowed3->isSized());

  ASSERT_EQ(elem3->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(elem3)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));

  const GenType* const sizedimmgot =
    GenType::get(mod, sizedarrmd, GenType::Immutable);
  const GenType* const sizedmutgot =
    GenType::get(mod, sizedarrmd, GenType::Mutable);
  const GenType* const sizedwogot =
    GenType::get(mod, sizedarrmd, GenType::WriteOnce);

  EXPECT_EQ(sizedimmgot->mutability(), GenType::Immutable);

  ASSERT_EQ(sizedimmgot->getTypeID(), GenType::ArrayTypeID);

  const ArrayGenType* const narrowed4 = ArrayGenType::narrow(sizedimmgot);
  const GenType* const elem4 = narrowed4->getElemTy();

  EXPECT_EQ(elem4->mutability(), GenType::Immutable);
  EXPECT_TRUE(narrowed4->isSized());
  EXPECT_EQ(narrowed4->getNumElems(), 16);

  ASSERT_EQ(elem4->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(elem4)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));

  EXPECT_EQ(sizedmutgot->mutability(), GenType::Mutable);

  ASSERT_EQ(sizedimmgot->getTypeID(), GenType::ArrayTypeID);

  const ArrayGenType* const narrowed5 = ArrayGenType::narrow(sizedmutgot);
  const GenType* const elem5 = narrowed5->getElemTy();

  EXPECT_EQ(elem5->mutability(), GenType::Mutable);
  EXPECT_TRUE(narrowed5->isSized());
  EXPECT_EQ(narrowed5->getNumElems(), 16);

  ASSERT_EQ(elem5->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(elem5)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));

  EXPECT_EQ(sizedwogot->mutability(), GenType::WriteOnce);

  ASSERT_EQ(sizedimmgot->getTypeID(), GenType::ArrayTypeID);

  const ArrayGenType* const narrowed6 = ArrayGenType::narrow(sizedwogot);
  const GenType* const elem6 = narrowed6->getElemTy();

  EXPECT_EQ(elem6->mutability(), GenType::WriteOnce);
  EXPECT_TRUE(narrowed6->isSized());
  EXPECT_EQ(narrowed6->getNumElems(), 16);

  ASSERT_EQ(elem6->getTypeID(), GenType::PrimTypeID);
  EXPECT_EQ(PrimGenType::narrow(elem6)->getLLVMType(),
            llvm::Type::getInt32Ty(ctx));
}

TEST(GenType, test_NativePtrGenType_get) {
  const NativePtrGenType* const immtype =
    NativePtrGenType::get(mod, nativeptrmd, GenType::Immutable);
  const NativePtrGenType* const muttype =
    NativePtrGenType::get(mod, nativeptrmd, GenType::Mutable);
  const NativePtrGenType* const wotype =
    NativePtrGenType::get(mod, nativeptrmd, GenType::WriteOnce);

  EXPECT_EQ(immtype->getElemTy(), opaquetype);
  EXPECT_EQ(immtype->mutability(), GenType::Immutable);
  EXPECT_EQ(immtype->getTypeID(), GenType::NativePtrTypeID);

  EXPECT_EQ(muttype->getElemTy(), opaquetype);
  EXPECT_EQ(muttype->mutability(), GenType::Mutable);
  EXPECT_EQ(muttype->getTypeID(), GenType::NativePtrTypeID);

  EXPECT_EQ(wotype->getElemTy(), opaquetype);
  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(wotype->getTypeID(), GenType::NativePtrTypeID);
}

TEST(GenType, test_GenType_get_NativePtr) {
  const GenType* const immtype =
    GenType::get(mod, nativeptrmd, TYPE_MUT_IMMUTABLE);
  const GenType* const muttype =
    GenType::get(mod, nativeptrmd, TYPE_MUT_MUTABLE);
  const GenType* const wotype =
    GenType::get(mod, nativeptrmd, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(immtype->mutability(), GenType::Immutable);
  ASSERT_EQ(immtype->getTypeID(), GenType::NativePtrTypeID);
  EXPECT_EQ(NativePtrGenType::narrow(immtype)->getElemTy(), opaquetype);
  EXPECT_EQ(muttype->mutability(), GenType::Mutable);
  ASSERT_EQ(immtype->getTypeID(), GenType::NativePtrTypeID);
  EXPECT_EQ(NativePtrGenType::narrow(muttype)->getElemTy(), opaquetype);
  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);
  ASSERT_EQ(immtype->getTypeID(), GenType::NativePtrTypeID);
  EXPECT_EQ(NativePtrGenType::narrow(wotype)->getElemTy(), opaquetype);

}

TEST(GenType, test_GCPtrGenType_get_strong) {
  const GCPtrGenType* const immtype =
    GCPtrGenType::get(mod, gcptrstrongmd, GenType::Immutable);
  const GCPtrGenType* const muttype =
    GCPtrGenType::get(mod, gcptrstrongmd, GenType::Mutable);
  const GCPtrGenType* const wotype =
    GCPtrGenType::get(mod, gcptrstrongmd, GenType::WriteOnce);

  EXPECT_EQ(immtype->getElemTy(), opaquetype);
  EXPECT_EQ(immtype->getPtrClass(), GCPtrGenType::StrongPtr);
  EXPECT_EQ(immtype->mutability(), GenType::Immutable);
  EXPECT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(muttype->getElemTy(), opaquetype);
  EXPECT_EQ(muttype->getPtrClass(), GCPtrGenType::StrongPtr);
  EXPECT_EQ(muttype->mutability(), GenType::Mutable);
  EXPECT_EQ(muttype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(wotype->getElemTy(), opaquetype);
  EXPECT_EQ(wotype->getPtrClass(), GCPtrGenType::StrongPtr);
  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(wotype->getTypeID(), GenType::GCPtrTypeID);
}

TEST(GenType, test_GCPtrGenType_get_soft) {
  const GCPtrGenType* const immtype =
    GCPtrGenType::get(mod, gcptrsoftmd, GenType::Immutable);
  const GCPtrGenType* const muttype =
    GCPtrGenType::get(mod, gcptrsoftmd, GenType::Mutable);
  const GCPtrGenType* const wotype =
    GCPtrGenType::get(mod, gcptrsoftmd, GenType::WriteOnce);

  EXPECT_EQ(immtype->getElemTy(), opaquetype);
  EXPECT_EQ(immtype->getPtrClass(), GCPtrGenType::SoftPtr);
  EXPECT_EQ(immtype->mutability(), GenType::Immutable);
  EXPECT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(muttype->getElemTy(), opaquetype);
  EXPECT_EQ(muttype->getPtrClass(), GCPtrGenType::SoftPtr);
  EXPECT_EQ(muttype->mutability(), GenType::Mutable);
  EXPECT_EQ(muttype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(wotype->getElemTy(), opaquetype);
  EXPECT_EQ(wotype->getPtrClass(), GCPtrGenType::SoftPtr);
  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(wotype->getTypeID(), GenType::GCPtrTypeID);
}

TEST(GenType, test_GCPtrGenType_get_weak) {
  const GCPtrGenType* const immtype =
    GCPtrGenType::get(mod, gcptrweakmd, GenType::Immutable);
  const GCPtrGenType* const muttype =
    GCPtrGenType::get(mod, gcptrweakmd, GenType::Mutable);
  const GCPtrGenType* const wotype =
    GCPtrGenType::get(mod, gcptrweakmd, GenType::WriteOnce);

  EXPECT_EQ(immtype->getElemTy(), opaquetype);
  EXPECT_EQ(immtype->getPtrClass(), GCPtrGenType::WeakPtr);
  EXPECT_EQ(immtype->mutability(), GenType::Immutable);
  EXPECT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(muttype->getElemTy(), opaquetype);
  EXPECT_EQ(muttype->getPtrClass(), GCPtrGenType::WeakPtr);
  EXPECT_EQ(muttype->mutability(), GenType::Mutable);
  EXPECT_EQ(muttype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(wotype->getElemTy(), opaquetype);
  EXPECT_EQ(wotype->getPtrClass(), GCPtrGenType::WeakPtr);
  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(wotype->getTypeID(), GenType::GCPtrTypeID);
}

TEST(GenType, test_GCPtrGenType_get_finalizer) {
  const GCPtrGenType* const immtype =
    GCPtrGenType::get(mod, gcptrfinalizermd, GenType::Immutable);
  const GCPtrGenType* const muttype =
    GCPtrGenType::get(mod, gcptrfinalizermd, GenType::Mutable);
  const GCPtrGenType* const wotype =
    GCPtrGenType::get(mod, gcptrfinalizermd, GenType::WriteOnce);

  EXPECT_EQ(immtype->getElemTy(), opaquetype);
  EXPECT_EQ(immtype->getPtrClass(), GCPtrGenType::FinalizerPtr);
  EXPECT_EQ(immtype->mutability(), GenType::Immutable);
  EXPECT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(muttype->getElemTy(), opaquetype);
  EXPECT_EQ(muttype->getPtrClass(), GCPtrGenType::FinalizerPtr);
  EXPECT_EQ(muttype->mutability(), GenType::Mutable);
  EXPECT_EQ(muttype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(wotype->getElemTy(), opaquetype);
  EXPECT_EQ(wotype->getPtrClass(), GCPtrGenType::FinalizerPtr);
  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(wotype->getTypeID(), GenType::GCPtrTypeID);
}

TEST(GenType, test_GCPtrGenType_get_phantom) {
  const GCPtrGenType* const immtype =
    GCPtrGenType::get(mod, gcptrphantommd, GenType::Immutable);
  const GCPtrGenType* const muttype =
    GCPtrGenType::get(mod, gcptrphantommd, GenType::Mutable);
  const GCPtrGenType* const wotype =
    GCPtrGenType::get(mod, gcptrphantommd, GenType::WriteOnce);

  EXPECT_EQ(immtype->getElemTy(), opaquetype);
  EXPECT_EQ(immtype->getPtrClass(), GCPtrGenType::PhantomPtr);
  EXPECT_EQ(immtype->mutability(), GenType::Immutable);
  EXPECT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(muttype->getElemTy(), opaquetype);
  EXPECT_EQ(muttype->getPtrClass(), GCPtrGenType::PhantomPtr);
  EXPECT_EQ(muttype->mutability(), GenType::Mutable);
  EXPECT_EQ(muttype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(wotype->getElemTy(), opaquetype);
  EXPECT_EQ(wotype->getPtrClass(), GCPtrGenType::PhantomPtr);
  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(wotype->getTypeID(), GenType::GCPtrTypeID);
}

TEST(GenType, test_GCPtrGenType_get_mobile) {
  const GCPtrGenType* const immtype =
    GCPtrGenType::get(mod, gcptrstrongmd, GenType::Immutable);
  const GCPtrGenType* const muttype =
    GCPtrGenType::get(mod, gcptrstrongmd, GenType::Mutable);
  const GCPtrGenType* const wotype =
    GCPtrGenType::get(mod, gcptrstrongmd, GenType::WriteOnce);

  EXPECT_EQ(immtype->getElemTy(), opaquetype);
  EXPECT_EQ(immtype->getMobility(), GCPtrGenType::Mobile);
  EXPECT_EQ(immtype->mutability(), GenType::Immutable);
  EXPECT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(muttype->getElemTy(), opaquetype);
  EXPECT_EQ(muttype->getMobility(), GCPtrGenType::Mobile);
  EXPECT_EQ(muttype->mutability(), GenType::Mutable);
  EXPECT_EQ(muttype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(wotype->getElemTy(), opaquetype);
  EXPECT_EQ(wotype->getMobility(), GCPtrGenType::Mobile);
  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(wotype->getTypeID(), GenType::GCPtrTypeID);
}

TEST(GenType, test_GCPtrGenType_get_immobile) {
  const GCPtrGenType* const immtype =
    GCPtrGenType::get(mod, gcptrimmobilemd, GenType::Immutable);
  const GCPtrGenType* const muttype =
    GCPtrGenType::get(mod, gcptrimmobilemd, GenType::Mutable);
  const GCPtrGenType* const wotype =
    GCPtrGenType::get(mod, gcptrimmobilemd, GenType::WriteOnce);

  EXPECT_EQ(immtype->getElemTy(), opaquetype);
  EXPECT_EQ(immtype->getMobility(), GCPtrGenType::Immobile);
  EXPECT_EQ(immtype->mutability(), GenType::Immutable);
  EXPECT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(muttype->getElemTy(), opaquetype);
  EXPECT_EQ(muttype->getMobility(), GCPtrGenType::Immobile);
  EXPECT_EQ(muttype->mutability(), GenType::Mutable);
  EXPECT_EQ(muttype->getTypeID(), GenType::GCPtrTypeID);

  EXPECT_EQ(wotype->getElemTy(), opaquetype);
  EXPECT_EQ(wotype->getMobility(), GCPtrGenType::Immobile);
  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);
  EXPECT_EQ(wotype->getTypeID(), GenType::GCPtrTypeID);
}

TEST(GenType, test_GenType_get_GCPtr_strong) {
  const GenType* const immtype =
    GenType::get(mod, gcptrstrongmd, TYPE_MUT_IMMUTABLE);
  const GenType* const muttype =
    GenType::get(mod, gcptrstrongmd, TYPE_MUT_MUTABLE);
  const GenType* const wotype =
    GenType::get(mod, gcptrstrongmd, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(immtype->mutability(), GenType::Immutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty1 = GCPtrGenType::narrow(immtype);

  EXPECT_EQ(ty1->getElemTy(), opaquetype);
  EXPECT_EQ(ty1->getPtrClass(), GCPtrGenType::StrongPtr);

  EXPECT_EQ(muttype->mutability(), GenType::Mutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty2 = GCPtrGenType::narrow(muttype);

  EXPECT_EQ(ty2->getElemTy(), opaquetype);
  EXPECT_EQ(ty2->getPtrClass(), GCPtrGenType::StrongPtr);

  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty3 = GCPtrGenType::narrow(wotype);

  EXPECT_EQ(ty3->getElemTy(), opaquetype);
  EXPECT_EQ(ty3->getPtrClass(), GCPtrGenType::StrongPtr);
}

TEST(GenType, test_GenType_get_GCPtr_weak) {
  const GenType* const immtype =
    GenType::get(mod, gcptrweakmd, TYPE_MUT_IMMUTABLE);
  const GenType* const muttype =
    GenType::get(mod, gcptrweakmd, TYPE_MUT_MUTABLE);
  const GenType* const wotype =
    GenType::get(mod, gcptrweakmd, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(immtype->mutability(), GenType::Immutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty1 = GCPtrGenType::narrow(immtype);

  EXPECT_EQ(ty1->getElemTy(), opaquetype);
  EXPECT_EQ(ty1->getPtrClass(), GCPtrGenType::WeakPtr);

  EXPECT_EQ(muttype->mutability(), GenType::Mutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty2 = GCPtrGenType::narrow(muttype);

  EXPECT_EQ(ty2->getElemTy(), opaquetype);
  EXPECT_EQ(ty2->getPtrClass(), GCPtrGenType::WeakPtr);

  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty3 = GCPtrGenType::narrow(wotype);

  EXPECT_EQ(ty3->getElemTy(), opaquetype);
  EXPECT_EQ(ty3->getPtrClass(), GCPtrGenType::WeakPtr);
}

TEST(GenType, test_GenType_get_GCPtr_soft) {
  const GenType* const immtype =
    GenType::get(mod, gcptrsoftmd, TYPE_MUT_IMMUTABLE);
  const GenType* const muttype =
    GenType::get(mod, gcptrsoftmd, TYPE_MUT_MUTABLE);
  const GenType* const wotype =
    GenType::get(mod, gcptrsoftmd, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(immtype->mutability(), GenType::Immutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty1 = GCPtrGenType::narrow(immtype);

  EXPECT_EQ(ty1->getElemTy(), opaquetype);
  EXPECT_EQ(ty1->getPtrClass(), GCPtrGenType::SoftPtr);

  EXPECT_EQ(muttype->mutability(), GenType::Mutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty2 = GCPtrGenType::narrow(muttype);

  EXPECT_EQ(ty2->getElemTy(), opaquetype);
  EXPECT_EQ(ty2->getPtrClass(), GCPtrGenType::SoftPtr);

  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty3 = GCPtrGenType::narrow(wotype);

  EXPECT_EQ(ty3->getElemTy(), opaquetype);
  EXPECT_EQ(ty3->getPtrClass(), GCPtrGenType::SoftPtr);
}

TEST(GenType, test_GenType_get_GCPtr_finalizer) {
  const GenType* const immtype =
    GenType::get(mod, gcptrfinalizermd, TYPE_MUT_IMMUTABLE);
  const GenType* const muttype =
    GenType::get(mod, gcptrfinalizermd, TYPE_MUT_MUTABLE);
  const GenType* const wotype =
    GenType::get(mod, gcptrfinalizermd, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(immtype->mutability(), GenType::Immutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty1 = GCPtrGenType::narrow(immtype);

  EXPECT_EQ(ty1->getElemTy(), opaquetype);
  EXPECT_EQ(ty1->getPtrClass(), GCPtrGenType::FinalizerPtr);

  EXPECT_EQ(muttype->mutability(), GenType::Mutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty2 = GCPtrGenType::narrow(muttype);

  EXPECT_EQ(ty2->getElemTy(), opaquetype);
  EXPECT_EQ(ty2->getPtrClass(), GCPtrGenType::FinalizerPtr);

  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty3 = GCPtrGenType::narrow(wotype);

  EXPECT_EQ(ty3->getElemTy(), opaquetype);
  EXPECT_EQ(ty3->getPtrClass(), GCPtrGenType::FinalizerPtr);
}

TEST(GenType, test_GenType_get_GCPtr_phantom) {
  const GenType* const immtype =
    GenType::get(mod, gcptrphantommd, TYPE_MUT_IMMUTABLE);
  const GenType* const muttype =
    GenType::get(mod, gcptrphantommd, TYPE_MUT_MUTABLE);
  const GenType* const wotype =
    GenType::get(mod, gcptrphantommd, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(immtype->mutability(), GenType::Immutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty1 = GCPtrGenType::narrow(immtype);

  EXPECT_EQ(ty1->getElemTy(), opaquetype);
  EXPECT_EQ(ty1->getPtrClass(), GCPtrGenType::PhantomPtr);

  EXPECT_EQ(muttype->mutability(), GenType::Mutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty2 = GCPtrGenType::narrow(muttype);

  EXPECT_EQ(ty2->getElemTy(), opaquetype);
  EXPECT_EQ(ty2->getPtrClass(), GCPtrGenType::PhantomPtr);

  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty3 = GCPtrGenType::narrow(wotype);

  EXPECT_EQ(ty3->getElemTy(), opaquetype);
  EXPECT_EQ(ty3->getPtrClass(), GCPtrGenType::PhantomPtr);
}

TEST(GenType, test_GenType_get_GCPtr_immobile) {
  const GenType* const immtype =
    GenType::get(mod, gcptrimmobilemd, TYPE_MUT_IMMUTABLE);
  const GenType* const muttype =
    GenType::get(mod, gcptrimmobilemd, TYPE_MUT_MUTABLE);
  const GenType* const wotype =
    GenType::get(mod, gcptrimmobilemd, TYPE_MUT_WRITEONCE);

  EXPECT_EQ(immtype->mutability(), GenType::Immutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty1 = GCPtrGenType::narrow(immtype);

  EXPECT_EQ(ty1->getElemTy(), opaquetype);
  EXPECT_EQ(ty1->getMobility(), GCPtrGenType::Immobile);

  EXPECT_EQ(muttype->mutability(), GenType::Mutable);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty2 = GCPtrGenType::narrow(muttype);

  EXPECT_EQ(ty2->getElemTy(), opaquetype);
  EXPECT_EQ(ty2->getMobility(), GCPtrGenType::Immobile);

  EXPECT_EQ(wotype->mutability(), GenType::WriteOnce);

  ASSERT_EQ(immtype->getTypeID(), GenType::GCPtrTypeID);

  const GCPtrGenType* const ty3 = GCPtrGenType::narrow(wotype);

  EXPECT_EQ(ty3->getElemTy(), opaquetype);
  EXPECT_EQ(ty3->getMobility(), GCPtrGenType::Immobile);
}

TEST(GenType, test_PrimGenType_accept) {
  const PrimGenType* const type = PrimGenType::getUnit();
  UnitTestVisitor::Action script[] = { UnitTestVisitor::Action(type) };
  UnitTestVisitor visitor(script, 1);

  type->accept(visitor);
  visitor.finish();
}

TEST(GenType, test_ArrayGenType_accept_unsized_descend) {
  const ArrayGenType* const unsizedtype =
    ArrayGenType::get(mod, unsizedarrmd, GenType::Immutable);
  UnitTestVisitor::Action unsizeddescendscript[] = {
    UnitTestVisitor::Action(unsizedtype, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(unsizedtype->getElemTy()),
    UnitTestVisitor::Action(unsizedtype, UnitTestVisitor::END)
  };
  UnitTestVisitor unsizeddescendvisitor(unsizeddescendscript, 3);

  unsizedtype->accept(unsizeddescendvisitor);
  unsizeddescendvisitor.finish();
}

TEST(GenType, test_ArrayGenType_accept_unsized_nodescend) {
  const ArrayGenType* const unsizedtype =
    ArrayGenType::get(mod, unsizedarrmd, GenType::Immutable);
  UnitTestVisitor::Action unsizednodescendscript[] = {
    UnitTestVisitor::Action(unsizedtype, UnitTestVisitor::BEGIN, false),
    UnitTestVisitor::Action(unsizedtype, UnitTestVisitor::END)
  };
  UnitTestVisitor unsizednodescendvisitor(unsizednodescendscript, 2);

  unsizedtype->accept(unsizednodescendvisitor);
  unsizednodescendvisitor.finish();
}

TEST(GenType, test_ArrayGenType_accept_sized_descend) {
  const ArrayGenType* const sizedtype =
    ArrayGenType::get(mod, sizedarrmd, GenType::Immutable);
  UnitTestVisitor::Action sizeddescendscript[] = {
    UnitTestVisitor::Action(sizedtype, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(sizedtype->getElemTy()),
    UnitTestVisitor::Action(sizedtype, UnitTestVisitor::END)
  };
  UnitTestVisitor sizeddescendvisitor(sizeddescendscript, 3);

  sizedtype->accept(sizeddescendvisitor);
  sizeddescendvisitor.finish();
}

TEST(GenType, test_ArrayGenType_accept_sized_nodescend) {
  const ArrayGenType* const sizedtype =
    ArrayGenType::get(mod, sizedarrmd, GenType::Immutable);
  UnitTestVisitor::Action sizednodescendscript[] = {
    UnitTestVisitor::Action(sizedtype, UnitTestVisitor::BEGIN, false),
    UnitTestVisitor::Action(sizedtype, UnitTestVisitor::END)
  };
  UnitTestVisitor sizednodescendvisitor(sizednodescendscript, 2);

  sizedtype->accept(sizednodescendvisitor);
  sizednodescendvisitor.finish();
}

TEST(GenType, test_GCPtrGenType_accept) {
  const GCPtrGenType* const type =
    GCPtrGenType::get(mod, gcptrstrongmd, GenType::Immutable);
  UnitTestVisitor::Action script[] = {
    UnitTestVisitor::Action(type)
  };
  UnitTestVisitor visitor(script, 1);

  type->accept(visitor);
  visitor.finish();
}

TEST(GenType, test_NativePtrGenType_accept) {
  const NativePtrGenType* const type =
    NativePtrGenType::get(mod, nativeptrmd, GenType::Immutable);
  UnitTestVisitor::Action script[] = { UnitTestVisitor::Action(type) };
  UnitTestVisitor visitor(script, 1);

  type->accept(visitor);
  visitor.finish();
}

TEST(GenType, test_StructGenType_accept_flat_nodescend) {
  const StructGenType* const type =
    StructGenType::get(mod, structnormmd, GenType::Mutable);
  UnitTestVisitor::Action script[] = {
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGIN, false),
    UnitTestVisitor::Action(type, UnitTestVisitor::END)
  };
  UnitTestVisitor visitor(script, 2);

  type->accept(visitor);
  visitor.finish();
}

TEST(GenType, test_StructGenType_accept_flat_descend) {
  const StructGenType* const type =
    StructGenType::get(mod, structnormmd, GenType::Mutable);
  const GenType* const field0 = type->fieldTy(0);
  const GenType* const field1 = type->fieldTy(1);
  const GenType* const field2 = type->fieldTy(2);
  UnitTestVisitor::Action script[] = {
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(field0),
    UnitTestVisitor::Action(field1),
    UnitTestVisitor::Action(field2, UnitTestVisitor::BEGIN, false),
    UnitTestVisitor::Action(field2, UnitTestVisitor::END),
    UnitTestVisitor::Action(type, UnitTestVisitor::END)
  };
  UnitTestVisitor visitor(script, 6);

  type->accept(visitor);
  visitor.finish();
}

TEST(GenType, test_StructGenType_accept_nested) {
  const StructGenType* const type =
    StructGenType::get(mod, structnestedmd, GenType::Mutable);
  const GenType* const field0 = type->fieldTy(0);
  const StructGenType* const field1 = StructGenType::narrow(type->fieldTy(1));
  const GenType* const innerfield0 = field1->fieldTy(0);
  const GenType* const innerfield1 = field1->fieldTy(1);
  const ArrayGenType* const innerfield2 =
    ArrayGenType::narrow(field1->fieldTy(2));
  const GenType* const elem = innerfield2->getElemTy();
  UnitTestVisitor::Action script[] = {
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(field0),
    UnitTestVisitor::Action(field1, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(innerfield0),
    UnitTestVisitor::Action(innerfield1),
    UnitTestVisitor::Action(innerfield2, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(elem),
    UnitTestVisitor::Action(innerfield2, UnitTestVisitor::END),
    UnitTestVisitor::Action(field1, UnitTestVisitor::END),
    UnitTestVisitor::Action(type, UnitTestVisitor::END)
  };
  UnitTestVisitor visitor(script, 10);

  type->accept(visitor);
  visitor.finish();
}

TEST(GenType, test_FuncPtrGenType_accept_nodescend) {
  const FuncPtrGenType* const type =
    FuncPtrGenType::get(mod, funczeroargmd, GenType::Mutable);
  UnitTestVisitor::Action script[] = {
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGIN, false),
    UnitTestVisitor::Action(type, UnitTestVisitor::END)
  };
  UnitTestVisitor visitor(script, 2);

  type->accept(visitor);
  visitor.finish();
}

TEST(GenType, test_FuncPtrGenType_accept_descend_noparams) {
  const FuncPtrGenType* const type =
    FuncPtrGenType::get(mod, funczeroargmd, GenType::Mutable);
  const GenType* const retty = type->returnTy();
  UnitTestVisitor::Action script[] = {
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(retty),
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGINPARAMS, false),
    UnitTestVisitor::Action(type, UnitTestVisitor::ENDPARAMS),
    UnitTestVisitor::Action(type, UnitTestVisitor::END)
  };
  UnitTestVisitor visitor(script, 5);

  type->accept(visitor);
  visitor.finish();
}

TEST(GenType, test_FuncPtrGenType_accept_descend_params) {
  const FuncPtrGenType* const type =
    FuncPtrGenType::get(mod, functwoargmd, GenType::Mutable);
  const GenType* const retty = type->returnTy();
  const GenType* const param0 = type->paramTy(0);
  const GenType* const param1 = type->paramTy(1);
  UnitTestVisitor::Action script[] = {
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(retty),
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGINPARAMS, true),
    UnitTestVisitor::Action(param0),
    UnitTestVisitor::Action(param1),
    UnitTestVisitor::Action(type, UnitTestVisitor::ENDPARAMS),
    UnitTestVisitor::Action(type, UnitTestVisitor::END)
  };
  UnitTestVisitor visitor(script, 7);

  type->accept(visitor);
  visitor.finish();
}

TEST(GenType, test_FuncPtrGenType_accept_nested_nodescend) {
  const FuncPtrGenType* const type =
    FuncPtrGenType::get(mod, funcnestedmd, GenType::Mutable);
  const GenType* const retty = type->returnTy();
  const GenType* const param = type->paramTy(0);
  UnitTestVisitor::Action script[] = {
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(retty),
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGINPARAMS, true),
    UnitTestVisitor::Action(param, UnitTestVisitor::BEGIN, false),
    UnitTestVisitor::Action(param, UnitTestVisitor::END),
    UnitTestVisitor::Action(type, UnitTestVisitor::ENDPARAMS),
    UnitTestVisitor::Action(type, UnitTestVisitor::END)
  };
  UnitTestVisitor visitor(script, 7);

  type->accept(visitor);
  visitor.finish();
}

TEST(GenTypeUnitTest, test_FuncPtrGenType_accept_nested_descend) {
  const FuncPtrGenType* const type =
    FuncPtrGenType::get(mod, funcnestedmd, GenType::Mutable);
  const GenType* const retty = type->returnTy();
  const StructGenType* const param = StructGenType::narrow(type->paramTy(0));
  const GenType* const field0 = param->fieldTy(0);
  const StructGenType* const field1 = StructGenType::narrow(param->fieldTy(1));
  const GenType* const innerfield0 = field1->fieldTy(0);
  const GenType* const innerfield1 = field1->fieldTy(1);
  const ArrayGenType* const innerfield2 =
    ArrayGenType::narrow(field1->fieldTy(2));
  const GenType* const elem = innerfield2->getElemTy();
  UnitTestVisitor::Action script[] = {
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(retty),
    UnitTestVisitor::Action(type, UnitTestVisitor::BEGINPARAMS, true),
    UnitTestVisitor::Action(param, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(field0),
    UnitTestVisitor::Action(field1, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(innerfield0),
    UnitTestVisitor::Action(innerfield1),
    UnitTestVisitor::Action(innerfield2, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(elem),
    UnitTestVisitor::Action(innerfield2, UnitTestVisitor::END),
    UnitTestVisitor::Action(field1, UnitTestVisitor::END),
    UnitTestVisitor::Action(param, UnitTestVisitor::END),
    UnitTestVisitor::Action(type, UnitTestVisitor::ENDPARAMS),
    UnitTestVisitor::Action(type, UnitTestVisitor::END)
  };
  UnitTestVisitor visitor(script, 15);

  type->accept(visitor);
  visitor.finish();
}
