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
#include "GCType.h"
#include "metadata.h"
#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Metadata.h"
#include "llvm/Module.h"
#include <cppunit/extensions/HelperMacros.h>

class GCTypeUnitTest : public CppUnit::TestFixture  {
public:

  llvm::LLVMContext ctx;
  llvm::Module mod;
  const llvm::StringRef tyname;
  llvm::MDString* const tynamemd;
  llvm::Type* const opaquetype;
  llvm::NamedMDNode* const testmd;
  const PrimGCType* const unittype;
  llvm::Value* const unittag;
  llvm::Value* const inttag;
  llvm::Value* const floattag;
  llvm::Value* const arrtag;
  llvm::Value* const nativeptrtag;
  llvm::Value* const gcptrtag;
  llvm::Value* const mobiletag;
  llvm::Value* const immobiletag;
  llvm::Value* const mutabletag;
  llvm::Value* const writeoncetag;
  llvm::Value* const immutabletag;
  llvm::Value* const strongtag;
  llvm::Value* const softtag;
  llvm::Value* const weaktag;
  llvm::Value* const finalizertag;
  llvm::Value* const phantomtag;
  llvm::Value* const const0;
  llvm::Value* const const1;
  llvm::Value* const const8;
  llvm::Value* const const16;
  llvm::Value* const const32;
  llvm::Value* const const64;
  llvm::Value* const const128;
  llvm::Value* const unitvals[1];
  llvm::Value* const int1vals[2];
  llvm::Value* const int8vals[2];
  llvm::Value* const int16vals[2];
  llvm::Value* const int32vals[2];
  llvm::Value* const int64vals[2];
  llvm::Value* const int128vals[2];
  llvm::Value* const inttoolongvals[3];
  llvm::Value* const inttooshortvals[1];
  llvm::Value* const float16vals[2];
  llvm::Value* const float32vals[2];
  llvm::Value* const float64vals[2];
  llvm::Value* const float128vals[2];
  llvm::Value* const floattoolongvals[3];
  llvm::Value* const floattooshortvals[1];
  llvm::MDNode* const unitmd;
  llvm::MDNode* const int1md;
  llvm::MDNode* const int8md;
  llvm::MDNode* const int16md;
  llvm::MDNode* const int32md;
  llvm::MDNode* const int64md;
  llvm::MDNode* const int128md;
  llvm::MDNode* const inttoolongmd;
  llvm::MDNode* const inttooshortmd;
  llvm::MDNode* const float16md;
  llvm::MDNode* const float32md;
  llvm::MDNode* const float64md;
  llvm::MDNode* const float128md;
  llvm::MDNode* const floattoolongmd;
  llvm::MDNode* const floattooshortmd;
  llvm::Value* const unsizedarrvals[2];
  llvm::Value* const sizedarrvals[3];
  llvm::Value* const arrtoolongvals[4];
  llvm::Value* const arrtooshortvals[1];
  llvm::MDNode* const unsizedarrmd;
  llvm::MDNode* const sizedarrmd;
  llvm::MDNode* const arrtoolongmd;
  llvm::MDNode* const arrtooshortmd;
  llvm::Value* const nativeptrvals[2];
  llvm::Value* const nativeptrtoolongvals[3];
  llvm::Value* const nativeptrtooshortvals[1];
  llvm::MDNode* const nativeptrmd;
  llvm::MDNode* const nativeptrtoolongmd;
  llvm::MDNode* const nativeptrtooshortmd;
  llvm::Value* const gcptrstrongvals[4];
  llvm::Value* const gcptrsoftvals[4];
  llvm::Value* const gcptrweakvals[4];
  llvm::Value* const gcptrfinalizervals[4];
  llvm::Value* const gcptrphantomvals[4];
  llvm::Value* const gcptrimmobilevals[4];
  llvm::Value* const gcptrtoolongvals[5];
  llvm::Value* const gcptrtooshortvals[3];
  llvm::MDNode* const gcptrstrongmd;
  llvm::MDNode* const gcptrsoftmd;
  llvm::MDNode* const gcptrweakmd;
  llvm::MDNode* const gcptrfinalizermd;
  llvm::MDNode* const gcptrphantommd;
  llvm::MDNode* const gcptrimmobilemd;
  llvm::MDNode* const gcptrtoolongmd;
  llvm::MDNode* const gcptrtooshortmd;

  GCTypeUnitTest() :
    ctx(), mod(llvm::StringRef("Test"), ctx),
    tyname(llvm::StringRef("test")),
    tynamemd(llvm::MDString::get(ctx, tyname)),
    opaquetype(llvm::StructType::create(ctx, tyname)),
    testmd(mod.getOrInsertNamedMetadata("test")),
    unittype(PrimGCType::getUnit()),
    unittag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                   GC_MD_TYPE_UNIT)),
    inttag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_INT)),
    floattag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                    GC_MD_TYPE_FLOAT)),
    arrtag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                  GC_MD_TYPE_ARRAY)),
    nativeptrtag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                        GC_MD_TYPE_NATIVEPTR)),
    gcptrtag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                    GC_MD_TYPE_GCPTR)),
    mobiletag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                     GC_MD_MOB_MOBILE)),
    immobiletag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                       GC_MD_MOB_IMMOBILE)),
    mutabletag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                      GC_MD_MUT_MUTABLE)),
    writeoncetag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                        GC_MD_MUT_WRITEONCE)),
    immutabletag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                        GC_MD_MUT_IMMUTABLE)),
    strongtag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                     GC_MD_PTRCLASS_STRONG)),
    softtag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                   GC_MD_PTRCLASS_SOFT)),
    weaktag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                   GC_MD_PTRCLASS_WEAK)),
    finalizertag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                        GC_MD_PTRCLASS_FINALIZER)),
    phantomtag(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx),
                                      GC_MD_PTRCLASS_PHANTOM)),
    const0(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 0)),
    const1(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 1)),
    const8(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 8)),
    const16(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 16)),
    const32(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 32)),
    const64(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 64)),
    const128(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 128)),
    unitvals({ unittag }),
    int1vals({ inttag, const1 }),
    int8vals({ inttag, const8 }),
    int16vals({ inttag, const16 }),
    int32vals({ inttag, const32 }),
    int64vals({ inttag, const64 }),
    int128vals({ inttag, const128 }),
    inttoolongvals({ inttag, const32, const0 }),
    inttooshortvals({ inttag }),
    float16vals({ floattag, const16 }),
    float32vals({ floattag, const32 }),
    float64vals({ floattag, const64 }),
    float128vals({ floattag, const128 }),
    floattoolongvals({ floattag, const32, const0 }),
    floattooshortvals({ floattag }),
    unitmd(llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(unitvals))),
    int1md(llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(int1vals))),
    int8md(llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(int8vals))),
    int16md(llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(int16vals))),
    int32md(llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(int32vals))),
    int64md(llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(int64vals))),
    int128md(llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(int128vals))),
    inttoolongmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(inttoolongvals))
    ),
    inttooshortmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(inttooshortvals))
    ),
    float16md(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(float16vals))
    ),
    float32md(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(float32vals))
    ),
    float64md(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(float64vals))
    ),
    float128md(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(float128vals))
    ),
    floattoolongmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(floattoolongvals))
    ),
    floattooshortmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(floattooshortvals))
    ),
    unsizedarrvals({ arrtag, int32md }),
    sizedarrvals({ arrtag, int32md, const16 }),
    arrtoolongvals({ arrtag, int32md, const0, const0 }),
    arrtooshortvals({ arrtag }),
    unsizedarrmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(unsizedarrvals))
    ),
    sizedarrmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(sizedarrvals))
    ),
    arrtoolongmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(arrtoolongvals))
    ),
    arrtooshortmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(arrtooshortvals))
    ),
    nativeptrvals({ nativeptrtag, tynamemd }),
    nativeptrtoolongvals({ nativeptrtag, tynamemd, const0 }),
    nativeptrtooshortvals({ nativeptrtag }),
    nativeptrmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(nativeptrvals))
    ),
    nativeptrtoolongmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(nativeptrtoolongvals))
    ),
    nativeptrtooshortmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(nativeptrtooshortvals))
    ),
    gcptrstrongvals({ gcptrtag, mobiletag, strongtag, tynamemd }),
    gcptrsoftvals({ gcptrtag, mobiletag, softtag, tynamemd }),
    gcptrweakvals({ gcptrtag, mobiletag, weaktag, tynamemd }),
    gcptrfinalizervals({ gcptrtag, mobiletag, finalizertag, tynamemd }),
    gcptrphantomvals({ gcptrtag, mobiletag, phantomtag, tynamemd }),
    gcptrimmobilevals({ gcptrtag, immobiletag, strongtag, tynamemd }),
    gcptrtoolongvals({ gcptrtag, mobiletag, strongtag, tynamemd, const0 }),
    gcptrtooshortvals({ gcptrtag, mobiletag, strongtag }),
    gcptrstrongmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(gcptrstrongvals))
    ),
    gcptrsoftmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(gcptrsoftvals))
    ),
    gcptrweakmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(gcptrweakvals))
    ),
    gcptrfinalizermd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(gcptrfinalizervals))
    ),
    gcptrphantommd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(gcptrphantomvals))
    ),
    gcptrimmobilemd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(gcptrimmobilevals))
    ),
    gcptrtoolongmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(gcptrtoolongvals))
    ),
    gcptrtooshortmd(
      llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(gcptrtooshortvals))
    )
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
  }

  CPPUNIT_TEST_SUITE(GCTypeUnitTest);
  CPPUNIT_TEST(test_PrimGCType_getUnit);
  CPPUNIT_TEST(test_GCType_get_unit);
  CPPUNIT_TEST(test_PrimGCType_getInt_1);
  CPPUNIT_TEST(test_PrimGCType_getInt_8);
  CPPUNIT_TEST(test_PrimGCType_getInt_16);
  CPPUNIT_TEST(test_PrimGCType_getInt_32);
  CPPUNIT_TEST(test_PrimGCType_getInt_64);
  CPPUNIT_TEST(test_PrimGCType_getInt_128);
  CPPUNIT_TEST(test_GCType_get_int_1);
  CPPUNIT_TEST(test_GCType_get_int_8);
  CPPUNIT_TEST(test_GCType_get_int_16);
  CPPUNIT_TEST(test_GCType_get_int_32);
  CPPUNIT_TEST(test_GCType_get_int_64);
  CPPUNIT_TEST(test_GCType_get_int_128);
  CPPUNIT_TEST(test_PrimGCType_accept);
  CPPUNIT_TEST(test_ArrayGCType_get);
  CPPUNIT_TEST(test_ArrayGCType_accept_unsized_descend);
  CPPUNIT_TEST(test_ArrayGCType_accept_unsized_nodescend);
  CPPUNIT_TEST(test_ArrayGCType_accept_sized_descend);
  CPPUNIT_TEST(test_ArrayGCType_accept_sized_nodescend);
  CPPUNIT_TEST(test_NativePtrGCType_get);
  CPPUNIT_TEST(test_GCType_get_NativePtr);
  CPPUNIT_TEST(test_NativePtrGCType_accept);
  CPPUNIT_TEST(test_GCPtrGCType_get_strong);
  CPPUNIT_TEST(test_GCPtrGCType_get_soft);
  CPPUNIT_TEST(test_GCPtrGCType_get_weak);
  CPPUNIT_TEST(test_GCPtrGCType_get_finalizer);
  CPPUNIT_TEST(test_GCPtrGCType_get_phantom);
  CPPUNIT_TEST(test_GCPtrGCType_get_mobile);
  CPPUNIT_TEST(test_GCPtrGCType_get_immobile);
  CPPUNIT_TEST(test_GCType_get_GCPtr_strong);
  CPPUNIT_TEST(test_GCType_get_GCPtr_weak);
  CPPUNIT_TEST(test_GCType_get_GCPtr_soft);
  CPPUNIT_TEST(test_GCType_get_GCPtr_finalizer);
  CPPUNIT_TEST(test_GCType_get_GCPtr_phantom);
  CPPUNIT_TEST(test_GCType_get_GCPtr_immobile);
  CPPUNIT_TEST(test_GCPtrGCType_accept);
  CPPUNIT_TEST_SUITE_END();

  void test_PrimGCType_getUnit();
  void test_GCType_get_unit();
  void test_PrimGCType_getInt_1();
  void test_PrimGCType_getInt_8();
  void test_PrimGCType_getInt_16();
  void test_PrimGCType_getInt_32();
  void test_PrimGCType_getInt_64();
  void test_PrimGCType_getInt_128();
  void test_GCType_get_int_1();
  void test_GCType_get_int_8();
  void test_GCType_get_int_16();
  void test_GCType_get_int_32();
  void test_GCType_get_int_64();
  void test_GCType_get_int_128();
  void test_PrimGCType_accept();
  void test_ArrayGCType_get();
  void test_ArrayGCType_accept_unsized_descend();
  void test_ArrayGCType_accept_unsized_nodescend();
  void test_ArrayGCType_accept_sized_descend();
  void test_ArrayGCType_accept_sized_nodescend();
  void test_NativePtrGCType_get();
  void test_GCType_get_NativePtr();
  void test_NativePtrGCType_accept();
  void test_GCPtrGCType_get_strong();
  void test_GCPtrGCType_get_soft();
  void test_GCPtrGCType_get_weak();
  void test_GCPtrGCType_get_finalizer();
  void test_GCPtrGCType_get_phantom();
  void test_GCPtrGCType_get_mobile();
  void test_GCPtrGCType_get_immobile();
  void test_GCType_get_GCPtr_strong();
  void test_GCType_get_GCPtr_weak();
  void test_GCType_get_GCPtr_soft();
  void test_GCType_get_GCPtr_finalizer();
  void test_GCType_get_GCPtr_phantom();
  void test_GCType_get_GCPtr_immobile();
  void test_GCPtrGCType_accept();

};

CPPUNIT_TEST_SUITE_REGISTRATION(GCTypeUnitTest);

// Visitor for unit testing the visitor pattern.  It takes a script
// describing the visitation pattern, and checks it along the way.
class UnitTestVisitor : public GCTypeVisitor {
public:

  enum { VISIT, BEGIN, END, BEGINPARAMS, ENDPARAMS };

  struct Action {
    const GCType* const ty;
    const unsigned action;
    const bool descend;

    Action(const GCType* const ty,
           const unsigned action = VISIT,
           const bool descend = false) :
      ty(ty), action(action), descend(descend) {}
  };

  const Action* const script;
  const unsigned len;
  unsigned index;

  UnitTestVisitor(const Action* const script,
                  const unsigned len) :
    GCTypeVisitor(), script(script), len(len), index(0) {}

  inline void finish() { CPPUNIT_ASSERT(index == len); }

  virtual bool begin(const StructGCType* const ty);
  virtual bool begin(const FuncPtrGCType* const ty);
  virtual bool begin(const ArrayGCType* const ty);
  virtual void end(const StructGCType* const ty);
  virtual void end(const FuncPtrGCType* const ty);
  virtual void end(const ArrayGCType* const ty);
  virtual void visit(const NativePtrGCType* ty);
  virtual void visit(const GCPtrGCType* ty);
  virtual void visit(const PrimGCType* ty);
  virtual bool beginParams(const FuncPtrGCType* const ty);
  virtual void endParams(const FuncPtrGCType* ty);
};

// Unit test visitor implementation
bool UnitTestVisitor::begin(const StructGCType* const ty) {
  if(index < len) {
    CPPUNIT_ASSERT(script[index].action == BEGIN);
    CPPUNIT_ASSERT(script[index].ty == ty);
    return script[index++].descend;
  } else {
    CPPUNIT_FAIL("Visitor saw more types than script describes");
    return false;
  }
}

bool UnitTestVisitor::begin(const FuncPtrGCType* const ty) {
  if(index < len) {
    CPPUNIT_ASSERT(script[index].action == BEGIN);
    CPPUNIT_ASSERT(script[index].ty == ty);
    return script[index++].descend;
  } else {
    CPPUNIT_FAIL("Visitor saw more types than script describes");
    return false;
  }
}

bool UnitTestVisitor::begin(const ArrayGCType* const ty) {
  if(index < len) {
    CPPUNIT_ASSERT(script[index].action == BEGIN);
    CPPUNIT_ASSERT(script[index].ty == ty);
    return script[index++].descend;
  } else {
    CPPUNIT_FAIL("Visitor saw more types than script describes");
    return false;
  }
}

void UnitTestVisitor::end(const StructGCType* const ty) {
  if(index < len) {
    CPPUNIT_ASSERT(script[index].action == END);
    CPPUNIT_ASSERT(script[index].ty == ty);
    index++;
  } else {
    CPPUNIT_FAIL("Visitor saw more types than script describes");
  }
}

void UnitTestVisitor::end(const FuncPtrGCType* const ty) {
  if(index < len) {
    CPPUNIT_ASSERT(script[index].action == END);
    CPPUNIT_ASSERT(script[index].ty == ty);
    index++;
  } else {
    CPPUNIT_FAIL("Visitor saw more types than script describes");
  }
}

void UnitTestVisitor::end(const ArrayGCType* const ty) {
  if(index < len) {
    CPPUNIT_ASSERT(script[index].action == END);
    CPPUNIT_ASSERT(script[index].ty == ty);
    index++;
  } else {
    CPPUNIT_FAIL("Visitor saw more types than script describes");
  }
}

void UnitTestVisitor::visit(const NativePtrGCType* const ty) {
  if(index < len) {
    CPPUNIT_ASSERT(script[index].action == VISIT);
    CPPUNIT_ASSERT(script[index].ty == ty);
    index++;
  } else {
    CPPUNIT_FAIL("Visitor saw more types than script describes");
  }
}

void UnitTestVisitor::visit(const GCPtrGCType* const ty) {
  if(index < len) {
    CPPUNIT_ASSERT(script[index].action == VISIT);
    CPPUNIT_ASSERT(script[index].ty == ty);
    index++;
  } else {
    CPPUNIT_FAIL("Visitor saw more types than script describes");
  }
}

void UnitTestVisitor::visit(const PrimGCType* const ty) {
  if(index < len) {
    CPPUNIT_ASSERT(script[index].action == VISIT);
    CPPUNIT_ASSERT(script[index].ty == ty);
    index++;
  } else {
    CPPUNIT_FAIL("Visitor saw more types than script describes");
  }
}

bool UnitTestVisitor::beginParams(const FuncPtrGCType* const ty) {
  if(index < len) {
    CPPUNIT_ASSERT(script[index].action == BEGINPARAMS);
    CPPUNIT_ASSERT(script[index].ty == ty);
    return script[index++].descend;
  } else {
    CPPUNIT_FAIL("Visitor saw more types than script describes");
    return false;
  }
}

void UnitTestVisitor::endParams(const FuncPtrGCType* const ty) {
  if(index < len) {
    CPPUNIT_ASSERT(script[index].action == END);
    CPPUNIT_ASSERT(script[index].ty == ty);
    index++;
  } else {
    CPPUNIT_FAIL("Visitor saw more types than script describes");
  }
}

void GCTypeUnitTest::test_PrimGCType_getUnit() {
  CPPUNIT_ASSERT(NULL == unittype->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == unittype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == unittype->getTypeID());
}

void GCTypeUnitTest::test_GCType_get_unit() {
  const GCType* const got = GCType::get(mod, unitmd, GCType::ImmutableID);

  if(GCType::PrimTypeID == got->getTypeID()) {
    CPPUNIT_ASSERT(NULL == PrimGCType::narrow(got)->getLLVMType());
    CPPUNIT_ASSERT(GCType::ImmutableID == got->mutability());
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == got->getTypeID()");

  // Test 3: test parsing the metadata, starting with Mutable
  // The resulting type should still read as immutable (it's a unit).
  const GCType* const mutgot = GCType::get(mod, unitmd, GCType::MutableID);

  if(GCType::PrimTypeID == mutgot->getTypeID()) {
    CPPUNIT_ASSERT(NULL == PrimGCType::narrow(mutgot)->getLLVMType());
    CPPUNIT_ASSERT(GCType::ImmutableID == mutgot->mutability());
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == mutgot->getTypeID()");

  // Test 3: test parsing the metadata, starting with WriteOnce
  // The resulting type should still read as immutable (it's a unit).
  const GCType* const wogot = GCType::get(mod, unitmd, GCType::WriteOnceID);

  if(GCType::PrimTypeID == wogot->getTypeID()) {
    CPPUNIT_ASSERT(NULL == PrimGCType::narrow(wogot)->getLLVMType());
    CPPUNIT_ASSERT(GCType::ImmutableID == wogot->mutability());
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == mutgot->getTypeID()");

  // No need to test parsing a bad metadata, that will be handled by
  // the test for the top-level get.
}

void GCTypeUnitTest::test_PrimGCType_getInt_1() {

  // setup metadata that's too long and too short

  // Test 1: Test getInt by itself on all good metadata
  const PrimGCType* const immint1type =
    PrimGCType::getInt(mod, int1md, GCType::ImmutableID);
  const PrimGCType* const mutint1type =
    PrimGCType::getInt(mod, int1md, GCType::MutableID);
  const PrimGCType* const woint1type =
    PrimGCType::getInt(mod, int1md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getInt1Ty(ctx) == immint1type->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint1type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint1type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt1Ty(ctx) == mutint1type->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint1type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint1type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt1Ty(ctx) == woint1type->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint1type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint1type->getTypeID());

}

void GCTypeUnitTest::test_PrimGCType_getInt_8() {
  const PrimGCType* const immint8type =
    PrimGCType::getInt(mod, int8md, GCType::ImmutableID);
  const PrimGCType* const mutint8type =
    PrimGCType::getInt(mod, int8md, GCType::MutableID);
  const PrimGCType* const woint8type =
    PrimGCType::getInt(mod, int8md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getInt8Ty(ctx) == immint8type->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint8type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint8type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt8Ty(ctx) == mutint8type->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint8type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint8type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt8Ty(ctx) == woint8type->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint8type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint8type->getTypeID());
}

void GCTypeUnitTest::test_PrimGCType_getInt_16() {
  const PrimGCType* const immint16type =
    PrimGCType::getInt(mod, int16md, GCType::ImmutableID);
  const PrimGCType* const mutint16type =
    PrimGCType::getInt(mod, int16md, GCType::MutableID);
  const PrimGCType* const woint16type =
    PrimGCType::getInt(mod, int16md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getInt16Ty(ctx) == immint16type->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint16type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint16type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt16Ty(ctx) == mutint16type->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint16type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint16type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt16Ty(ctx) == woint16type->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint16type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint16type->getTypeID());
}

void GCTypeUnitTest::test_PrimGCType_getInt_32() {
  const PrimGCType* const immint32type =
    PrimGCType::getInt(mod, int32md, GCType::ImmutableID);
  const PrimGCType* const mutint32type =
    PrimGCType::getInt(mod, int32md, GCType::MutableID);
  const PrimGCType* const woint32type =
    PrimGCType::getInt(mod, int32md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) == immint32type->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint32type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint32type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) == mutint32type->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint32type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint32type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) == woint32type->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint32type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint32type->getTypeID());
}

void GCTypeUnitTest::test_PrimGCType_getInt_64() {
  const PrimGCType* const immint64type =
    PrimGCType::getInt(mod, int64md, GCType::ImmutableID);
  const PrimGCType* const mutint64type =
    PrimGCType::getInt(mod, int64md, GCType::MutableID);
  const PrimGCType* const woint64type =
    PrimGCType::getInt(mod, int64md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getInt64Ty(ctx) == immint64type->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint64type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint64type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt64Ty(ctx) == mutint64type->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint64type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint64type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt64Ty(ctx) == woint64type->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint64type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint64type->getTypeID());
}

void GCTypeUnitTest::test_PrimGCType_getInt_128() {
  const PrimGCType* const immint128type =
    PrimGCType::getInt(mod, int128md, GCType::ImmutableID);
  const PrimGCType* const mutint128type =
    PrimGCType::getInt(mod, int128md, GCType::MutableID);
  const PrimGCType* const woint128type =
    PrimGCType::getInt(mod, int128md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getIntNTy(ctx, 128) ==
                 immint128type->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint128type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint128type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getIntNTy(ctx, 128) ==
                 immint128type->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint128type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint128type->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getIntNTy(ctx, 128) ==
                 immint128type->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint128type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint128type->getTypeID());
}

void GCTypeUnitTest::test_GCType_get_int_1() {
  const GCType* const immint1gctype =
    GCType::get(mod, int1md, GCType::ImmutableID);
  const GCType* const mutint1gctype =
    GCType::get(mod, int1md, GCType::MutableID);
  const GCType* const woint1gctype =
    GCType::get(mod, int1md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getInt1Ty(ctx) ==
                 PrimGCType::narrow(immint1gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint1gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint1gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt1Ty(ctx) ==
                 PrimGCType::narrow(mutint1gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint1gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint1gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt1Ty(ctx) ==
                 PrimGCType::narrow(woint1gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint1gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint1gctype->getTypeID());
}

void GCTypeUnitTest::test_GCType_get_int_8() {
  const GCType* const immint8gctype =
    GCType::get(mod, int8md, GCType::ImmutableID);
  const GCType* const mutint8gctype =
    GCType::get(mod, int8md, GCType::MutableID);
  const GCType* const woint8gctype =
    GCType::get(mod, int8md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getInt8Ty(ctx) ==
                 PrimGCType::narrow(immint8gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint8gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint8gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt8Ty(ctx) ==
                 PrimGCType::narrow(mutint8gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint8gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint8gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt8Ty(ctx) ==
                 PrimGCType::narrow(woint8gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint8gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint8gctype->getTypeID());
}

void GCTypeUnitTest::test_GCType_get_int_16() {
  const GCType* const immint16gctype =
    GCType::get(mod, int16md, GCType::ImmutableID);
  const GCType* const mutint16gctype =
    GCType::get(mod, int16md, GCType::MutableID);
  const GCType* const woint16gctype =
    GCType::get(mod, int16md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getInt16Ty(ctx) ==
                 PrimGCType::narrow(immint16gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint16gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint16gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt16Ty(ctx) ==
                 PrimGCType::narrow(mutint16gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint16gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint16gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt16Ty(ctx) ==
                 PrimGCType::narrow(woint16gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint16gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint16gctype->getTypeID());
}

void GCTypeUnitTest::test_GCType_get_int_32() {
  const GCType* const immint32gctype =
    GCType::get(mod, int32md, GCType::ImmutableID);
  const GCType* const mutint32gctype =
    GCType::get(mod, int32md, GCType::MutableID);
  const GCType* const woint32gctype =
    GCType::get(mod, int32md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                 PrimGCType::narrow(immint32gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint32gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint32gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                 PrimGCType::narrow(mutint32gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint32gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint32gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                 PrimGCType::narrow(woint32gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint32gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint32gctype->getTypeID());
}

void GCTypeUnitTest::test_GCType_get_int_64() {
  const GCType* const immint64gctype =
    GCType::get(mod, int64md, GCType::ImmutableID);
  const GCType* const mutint64gctype =
    GCType::get(mod, int64md, GCType::MutableID);
  const GCType* const woint64gctype =
    GCType::get(mod, int64md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getInt64Ty(ctx) ==
                 PrimGCType::narrow(immint64gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint64gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint64gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt64Ty(ctx) ==
                 PrimGCType::narrow(mutint64gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint64gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint64gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getInt64Ty(ctx) ==
                 PrimGCType::narrow(woint64gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint64gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint64gctype->getTypeID());
}

void GCTypeUnitTest::test_GCType_get_int_128() {
  const GCType* const immint128gctype =
    GCType::get(mod, int128md, GCType::ImmutableID);
  const GCType* const mutint128gctype =
    GCType::get(mod, int128md, GCType::MutableID);
  const GCType* const woint128gctype =
    GCType::get(mod, int128md, GCType::WriteOnceID);

  CPPUNIT_ASSERT(llvm::Type::getIntNTy(ctx, 128) ==
                 PrimGCType::narrow(immint128gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == immint128gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == immint128gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getIntNTy(ctx, 128) ==
                 PrimGCType::narrow(mutint128gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::MutableID == mutint128gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == mutint128gctype->getTypeID());
  CPPUNIT_ASSERT(llvm::Type::getIntNTy(ctx, 128) ==
                 PrimGCType::narrow(woint128gctype)->getLLVMType());
  CPPUNIT_ASSERT(GCType::WriteOnceID == woint128gctype->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == woint128gctype->getTypeID());
}

void GCTypeUnitTest::test_ArrayGCType_get() {

  // setup metadata that's too long and too short

  // Test 1: Test ArrayType::get by itself on all good metadata
  const ArrayGCType* const unsizedimmtype =
    ArrayGCType::get(mod, unsizedarrmd, GCType::ImmutableID);
  const ArrayGCType* const unsizedmuttype =
    ArrayGCType::get(mod, unsizedarrmd, GCType::MutableID);
  const ArrayGCType* const unsizedwotype =
    ArrayGCType::get(mod, unsizedarrmd, GCType::WriteOnceID);
  const GCType* const unsizedimmelem = unsizedimmtype->getElemTy();
  const GCType* const unsizedmutelem = unsizedmuttype->getElemTy();
  const GCType* const unsizedwoelem = unsizedwotype->getElemTy();

  CPPUNIT_ASSERT(GCType::ImmutableID == unsizedimmtype->mutability());
  CPPUNIT_ASSERT(GCType::ImmutableID == unsizedimmelem->mutability());
  CPPUNIT_ASSERT(GCType::ArrayTypeID == unsizedimmtype->getTypeID());
  CPPUNIT_ASSERT(!unsizedimmtype->isSized());

  if(GCType::PrimTypeID == unsizedimmelem->getTypeID())
    CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                   PrimGCType::narrow(unsizedimmelem)->getLLVMType());
  else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");

  CPPUNIT_ASSERT(GCType::MutableID == unsizedmuttype->mutability());
  CPPUNIT_ASSERT(GCType::MutableID == unsizedmutelem->mutability());
  CPPUNIT_ASSERT(GCType::ArrayTypeID == unsizedmuttype->getTypeID());
  CPPUNIT_ASSERT(!unsizedmuttype->isSized());

  if(GCType::PrimTypeID == unsizedmutelem->getTypeID())
    CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                   PrimGCType::narrow(unsizedmutelem)->getLLVMType());
  else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");

  CPPUNIT_ASSERT(GCType::WriteOnceID == unsizedwotype->mutability());
  CPPUNIT_ASSERT(GCType::WriteOnceID == unsizedwoelem->mutability());
  CPPUNIT_ASSERT(GCType::ArrayTypeID == unsizedwotype->getTypeID());
  CPPUNIT_ASSERT(!unsizedwotype->isSized());

  if(GCType::PrimTypeID == unsizedwoelem->getTypeID())
    CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                   PrimGCType::narrow(unsizedwoelem)->getLLVMType());
  else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");

  const ArrayGCType* const sizedimmtype =
    ArrayGCType::get(mod, sizedarrmd, GCType::ImmutableID);
  const ArrayGCType* const sizedmuttype =
    ArrayGCType::get(mod, sizedarrmd, GCType::MutableID);
  const ArrayGCType* const sizedwotype =
    ArrayGCType::get(mod, sizedarrmd, GCType::WriteOnceID);
  const GCType* const sizedimmelem = sizedimmtype->getElemTy();
  const GCType* const sizedmutelem = sizedmuttype->getElemTy();
  const GCType* const sizedwoelem = sizedwotype->getElemTy();

  CPPUNIT_ASSERT(GCType::ImmutableID == sizedimmtype->mutability());
  CPPUNIT_ASSERT(GCType::ImmutableID == sizedimmelem->mutability());
  CPPUNIT_ASSERT(GCType::ArrayTypeID == sizedimmtype->getTypeID());
  CPPUNIT_ASSERT(sizedimmtype->isSized());
  CPPUNIT_ASSERT(16 == sizedimmtype->getNumElems());

  if(GCType::PrimTypeID == sizedimmelem->getTypeID())
    CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                   PrimGCType::narrow(sizedimmelem)->getLLVMType());
  else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");

  CPPUNIT_ASSERT(GCType::MutableID == sizedmuttype->mutability());
  CPPUNIT_ASSERT(GCType::MutableID == sizedmutelem->mutability());
  CPPUNIT_ASSERT(GCType::ArrayTypeID == sizedmuttype->getTypeID());
  CPPUNIT_ASSERT(sizedmuttype->isSized());
  CPPUNIT_ASSERT(16 == sizedmuttype->getNumElems());

  if(GCType::PrimTypeID == sizedmutelem->getTypeID())
    CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                   PrimGCType::narrow(sizedmutelem)->getLLVMType());
  else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");

  CPPUNIT_ASSERT(GCType::WriteOnceID == sizedwotype->mutability());
  CPPUNIT_ASSERT(GCType::WriteOnceID == sizedwoelem->mutability());
  CPPUNIT_ASSERT(GCType::ArrayTypeID == sizedwotype->getTypeID());
  CPPUNIT_ASSERT(sizedwotype->isSized());
  CPPUNIT_ASSERT(16 == sizedwotype->getNumElems());

  if(GCType::PrimTypeID == sizedwoelem->getTypeID())
    CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                   PrimGCType::narrow(sizedwoelem)->getLLVMType());
  else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");

  // Test 1: Test GCType::get on all good metadata
  const GCType* const unsizedimmgot =
    GCType::get(mod, unsizedarrmd, GCType::ImmutableID);
  const GCType* const unsizedmutgot =
    GCType::get(mod, unsizedarrmd, GCType::MutableID);
  const GCType* const unsizedwogot =
    GCType::get(mod, unsizedarrmd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(GCType::ImmutableID == unsizedimmgot->mutability());

  if (GCType::ArrayTypeID == unsizedimmgot->getTypeID()) {

    const ArrayGCType* const narrowed = ArrayGCType::narrow(unsizedimmgot);
    const GCType* const elem = narrowed->getElemTy();

    CPPUNIT_ASSERT(GCType::ImmutableID == elem->mutability());
    CPPUNIT_ASSERT(!narrowed->isSized());

    if(GCType::PrimTypeID == elem->getTypeID())
      CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                     PrimGCType::narrow(elem)->getLLVMType());
    else
      CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::ArrayTypeID == getTypeID()");

  CPPUNIT_ASSERT(GCType::MutableID == unsizedmutgot->mutability());

  if (GCType::ArrayTypeID == unsizedimmgot->getTypeID()) {

    const ArrayGCType* const narrowed = ArrayGCType::narrow(unsizedmutgot);
    const GCType* const elem = narrowed->getElemTy();

    CPPUNIT_ASSERT(GCType::MutableID == elem->mutability());
    CPPUNIT_ASSERT(!narrowed->isSized());

    if(GCType::PrimTypeID == elem->getTypeID())
      CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                     PrimGCType::narrow(elem)->getLLVMType());
    else
      CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::ArrayTypeID == getTypeID()");

  CPPUNIT_ASSERT(GCType::WriteOnceID == unsizedwogot->mutability());

  if (GCType::ArrayTypeID == unsizedimmgot->getTypeID()) {

    const ArrayGCType* const narrowed = ArrayGCType::narrow(unsizedwogot);
    const GCType* const elem = narrowed->getElemTy();

    CPPUNIT_ASSERT(GCType::WriteOnceID == elem->mutability());
    CPPUNIT_ASSERT(!narrowed->isSized());

    if(GCType::PrimTypeID == elem->getTypeID())
      CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                     PrimGCType::narrow(elem)->getLLVMType());
    else
      CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::ArrayTypeID == getTypeID()");

  const GCType* const sizedimmgot =
    GCType::get(mod, sizedarrmd, GCType::ImmutableID);
  const GCType* const sizedmutgot =
    GCType::get(mod, sizedarrmd, GCType::MutableID);
  const GCType* const sizedwogot =
    GCType::get(mod, sizedarrmd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(GCType::ImmutableID == sizedimmgot->mutability());

  if (GCType::ArrayTypeID == sizedimmgot->getTypeID()) {

    const ArrayGCType* const narrowed = ArrayGCType::narrow(sizedimmgot);
    const GCType* const elem = narrowed->getElemTy();

    CPPUNIT_ASSERT(GCType::ImmutableID == elem->mutability());
    CPPUNIT_ASSERT(narrowed->isSized());
    CPPUNIT_ASSERT(16 == narrowed->getNumElems());

    if(GCType::PrimTypeID == elem->getTypeID())
      CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                     PrimGCType::narrow(elem)->getLLVMType());
    else
      CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::ArrayTypeID == getTypeID()");

  CPPUNIT_ASSERT(GCType::MutableID == sizedmutgot->mutability());

  if (GCType::ArrayTypeID == sizedimmgot->getTypeID()) {

    const ArrayGCType* const narrowed = ArrayGCType::narrow(sizedmutgot);
    const GCType* const elem = narrowed->getElemTy();

    CPPUNIT_ASSERT(GCType::MutableID == elem->mutability());
    CPPUNIT_ASSERT(narrowed->isSized());
    CPPUNIT_ASSERT(16 == narrowed->getNumElems());

    if(GCType::PrimTypeID == elem->getTypeID())
      CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                     PrimGCType::narrow(elem)->getLLVMType());
    else
      CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::ArrayTypeID == getTypeID()");

  CPPUNIT_ASSERT(GCType::WriteOnceID == sizedwogot->mutability());

  if (GCType::ArrayTypeID == sizedimmgot->getTypeID()) {

    const ArrayGCType* const narrowed = ArrayGCType::narrow(sizedwogot);
    const GCType* const elem = narrowed->getElemTy();

    CPPUNIT_ASSERT(GCType::WriteOnceID == elem->mutability());
    CPPUNIT_ASSERT(narrowed->isSized());
    CPPUNIT_ASSERT(16 == narrowed->getNumElems());

    if(GCType::PrimTypeID == elem->getTypeID())
      CPPUNIT_ASSERT(llvm::Type::getInt32Ty(ctx) ==
                     PrimGCType::narrow(elem)->getLLVMType());
    else
      CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == getTypeID()");
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::ArrayTypeID == getTypeID()");

}

void GCTypeUnitTest::test_NativePtrGCType_get() {
  const NativePtrGCType* const immtype =
    NativePtrGCType::get(mod, nativeptrmd, GCType::ImmutableID);
  const NativePtrGCType* const muttype =
    NativePtrGCType::get(mod, nativeptrmd, GCType::MutableID);
  const NativePtrGCType* const wotype =
    NativePtrGCType::get(mod, nativeptrmd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(opaquetype == immtype->getElemTy());
  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());
  CPPUNIT_ASSERT(GCType::NativePtrTypeID == immtype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == muttype->getElemTy());
  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());
  CPPUNIT_ASSERT(GCType::NativePtrTypeID == muttype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == wotype->getElemTy());
  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());
  CPPUNIT_ASSERT(GCType::NativePtrTypeID == wotype->getTypeID());
}

void GCTypeUnitTest::test_GCType_get_NativePtr() {
  const GCType* const immtype =
    GCType::get(mod, nativeptrmd, GCType::ImmutableID);
  const GCType* const muttype =
    GCType::get(mod, nativeptrmd, GCType::MutableID);
  const GCType* const wotype =
    GCType::get(mod, nativeptrmd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());

  if(GCType::NativePtrTypeID == immtype->getTypeID())
    CPPUNIT_ASSERT(opaquetype == NativePtrGCType::narrow(immtype)->getElemTy());
  else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::NativePtrTypeID");

  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());

  if(GCType::NativePtrTypeID == immtype->getTypeID())
    CPPUNIT_ASSERT(opaquetype == NativePtrGCType::narrow(muttype)->getElemTy());
  else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::NativePtrTypeID");

  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());

  if(GCType::NativePtrTypeID == immtype->getTypeID())
    CPPUNIT_ASSERT(opaquetype == NativePtrGCType::narrow(wotype)->getElemTy());
  else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::NativePtrTypeID");

}

void GCTypeUnitTest::test_GCPtrGCType_get_strong() {
  const GCPtrGCType* const immtype =
    GCPtrGCType::get(mod, gcptrstrongmd, GCType::ImmutableID);
  const GCPtrGCType* const muttype =
    GCPtrGCType::get(mod, gcptrstrongmd, GCType::MutableID);
  const GCPtrGCType* const wotype =
    GCPtrGCType::get(mod, gcptrstrongmd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(opaquetype == immtype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::StrongPtrID == immtype->getPtrClass());
  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == immtype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == muttype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::StrongPtrID == muttype->getPtrClass());
  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == muttype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == wotype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::StrongPtrID == wotype->getPtrClass());
  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == wotype->getTypeID());
}

void GCTypeUnitTest::test_GCPtrGCType_get_soft() {
  const GCPtrGCType* const immtype =
    GCPtrGCType::get(mod, gcptrsoftmd, GCType::ImmutableID);
  const GCPtrGCType* const muttype =
    GCPtrGCType::get(mod, gcptrsoftmd, GCType::MutableID);
  const GCPtrGCType* const wotype =
    GCPtrGCType::get(mod, gcptrsoftmd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(opaquetype == immtype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::SoftPtrID == immtype->getPtrClass());
  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == immtype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == muttype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::SoftPtrID == muttype->getPtrClass());
  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == muttype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == wotype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::SoftPtrID == wotype->getPtrClass());
  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == wotype->getTypeID());
}

void GCTypeUnitTest::test_GCPtrGCType_get_weak() {
  const GCPtrGCType* const immtype =
    GCPtrGCType::get(mod, gcptrweakmd, GCType::ImmutableID);
  const GCPtrGCType* const muttype =
    GCPtrGCType::get(mod, gcptrweakmd, GCType::MutableID);
  const GCPtrGCType* const wotype =
    GCPtrGCType::get(mod, gcptrweakmd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(opaquetype == immtype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::WeakPtrID == immtype->getPtrClass());
  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == immtype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == muttype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::WeakPtrID == muttype->getPtrClass());
  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == muttype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == wotype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::WeakPtrID == wotype->getPtrClass());
  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == wotype->getTypeID());
}

void GCTypeUnitTest::test_GCPtrGCType_get_finalizer() {
  const GCPtrGCType* const immtype =
    GCPtrGCType::get(mod, gcptrfinalizermd, GCType::ImmutableID);
  const GCPtrGCType* const muttype =
    GCPtrGCType::get(mod, gcptrfinalizermd, GCType::MutableID);
  const GCPtrGCType* const wotype =
    GCPtrGCType::get(mod, gcptrfinalizermd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(opaquetype == immtype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::FinalizerPtrID == immtype->getPtrClass());
  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == immtype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == muttype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::FinalizerPtrID == muttype->getPtrClass());
  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == muttype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == wotype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::FinalizerPtrID == wotype->getPtrClass());
  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == wotype->getTypeID());
}

void GCTypeUnitTest::test_GCPtrGCType_get_phantom() {
  const GCPtrGCType* const immtype =
    GCPtrGCType::get(mod, gcptrphantommd, GCType::ImmutableID);
  const GCPtrGCType* const muttype =
    GCPtrGCType::get(mod, gcptrphantommd, GCType::MutableID);
  const GCPtrGCType* const wotype =
    GCPtrGCType::get(mod, gcptrphantommd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(opaquetype == immtype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::PhantomPtrID == immtype->getPtrClass());
  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == immtype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == muttype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::PhantomPtrID == muttype->getPtrClass());
  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == muttype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == wotype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::PhantomPtrID == wotype->getPtrClass());
  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == wotype->getTypeID());
}

void GCTypeUnitTest::test_GCPtrGCType_get_mobile() {
  const GCPtrGCType* const immtype =
    GCPtrGCType::get(mod, gcptrstrongmd, GCType::ImmutableID);
  const GCPtrGCType* const muttype =
    GCPtrGCType::get(mod, gcptrstrongmd, GCType::MutableID);
  const GCPtrGCType* const wotype =
    GCPtrGCType::get(mod, gcptrstrongmd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(opaquetype == immtype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::MobileID == immtype->getMobility());
  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == immtype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == muttype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::MobileID == muttype->getMobility());
  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == muttype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == wotype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::MobileID == wotype->getMobility());
  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == wotype->getTypeID());
}

void GCTypeUnitTest::test_GCPtrGCType_get_immobile() {
  const GCPtrGCType* const immtype =
    GCPtrGCType::get(mod, gcptrimmobilemd, GCType::ImmutableID);
  const GCPtrGCType* const muttype =
    GCPtrGCType::get(mod, gcptrimmobilemd, GCType::MutableID);
  const GCPtrGCType* const wotype =
    GCPtrGCType::get(mod, gcptrimmobilemd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(opaquetype == immtype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::ImmobileID == immtype->getMobility());
  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == immtype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == muttype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::ImmobileID == muttype->getMobility());
  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == muttype->getTypeID());

  CPPUNIT_ASSERT(opaquetype == wotype->getElemTy());
  CPPUNIT_ASSERT(GCPtrGCType::ImmobileID == wotype->getMobility());
  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());
  CPPUNIT_ASSERT(GCType::GCPtrTypeID == wotype->getTypeID());
}

void GCTypeUnitTest::test_GCType_get_GCPtr_strong() {
  const GCType* const immtype =
    GCType::get(mod, gcptrstrongmd, GCType::ImmutableID);
  const GCType* const muttype =
    GCType::get(mod, gcptrstrongmd, GCType::MutableID);
  const GCType* const wotype =
    GCType::get(mod, gcptrstrongmd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(immtype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::StrongPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(muttype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::StrongPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(wotype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::StrongPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");
}

void GCTypeUnitTest::test_GCType_get_GCPtr_weak() {
  const GCType* const immtype =
    GCType::get(mod, gcptrweakmd, GCType::ImmutableID);
  const GCType* const muttype =
    GCType::get(mod, gcptrweakmd, GCType::MutableID);
  const GCType* const wotype =
    GCType::get(mod, gcptrweakmd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(immtype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::WeakPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(muttype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::WeakPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(wotype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::WeakPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");
}

void GCTypeUnitTest::test_GCType_get_GCPtr_soft() {
  const GCType* const immtype =
    GCType::get(mod, gcptrsoftmd, GCType::ImmutableID);
  const GCType* const muttype =
    GCType::get(mod, gcptrsoftmd, GCType::MutableID);
  const GCType* const wotype =
    GCType::get(mod, gcptrsoftmd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(immtype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::SoftPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(muttype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::SoftPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(wotype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::SoftPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");
}

void GCTypeUnitTest::test_GCType_get_GCPtr_finalizer() {
  const GCType* const immtype =
    GCType::get(mod, gcptrfinalizermd, GCType::ImmutableID);
  const GCType* const muttype =
    GCType::get(mod, gcptrfinalizermd, GCType::MutableID);
  const GCType* const wotype =
    GCType::get(mod, gcptrfinalizermd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(immtype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::FinalizerPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(muttype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::FinalizerPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(wotype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::FinalizerPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");
}

void GCTypeUnitTest::test_GCType_get_GCPtr_phantom() {
  const GCType* const immtype =
    GCType::get(mod, gcptrphantommd, GCType::ImmutableID);
  const GCType* const muttype =
    GCType::get(mod, gcptrphantommd, GCType::MutableID);
  const GCType* const wotype =
    GCType::get(mod, gcptrphantommd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(immtype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::PhantomPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(muttype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::PhantomPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(wotype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::PhantomPtrID == ty->getPtrClass());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");
}

void GCTypeUnitTest::test_GCType_get_GCPtr_immobile() {
  const GCType* const immtype =
    GCType::get(mod, gcptrimmobilemd, GCType::ImmutableID);
  const GCType* const muttype =
    GCType::get(mod, gcptrimmobilemd, GCType::MutableID);
  const GCType* const wotype =
    GCType::get(mod, gcptrimmobilemd, GCType::WriteOnceID);

  CPPUNIT_ASSERT(GCType::ImmutableID == immtype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(immtype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::ImmobileID == ty->getMobility());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::MutableID == muttype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(muttype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::ImmobileID == ty->getMobility());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");

  CPPUNIT_ASSERT(GCType::WriteOnceID == wotype->mutability());

  if(GCType::GCPtrTypeID == immtype->getTypeID()) {
    const GCPtrGCType* const ty = GCPtrGCType::narrow(wotype);
    CPPUNIT_ASSERT(opaquetype == ty->getElemTy());
    CPPUNIT_ASSERT(GCPtrGCType::ImmobileID == ty->getMobility());
  } else
    CPPUNIT_FAIL("Assertion failed: getTypeID() == GCType::GCPtrTypeID");
}

void GCTypeUnitTest::test_PrimGCType_accept() {
  const PrimGCType* const type = PrimGCType::getUnit();
  UnitTestVisitor::Action script[] = { UnitTestVisitor::Action(type) };
  UnitTestVisitor visitor(script, 1);

  type->accept(visitor);
  visitor.finish();
}

void GCTypeUnitTest::test_ArrayGCType_accept_unsized_descend() {
  const ArrayGCType* const unsizedtype =
    ArrayGCType::get(mod, unsizedarrmd, GCType::ImmutableID);
  UnitTestVisitor::Action unsizeddescendscript[] = {
    UnitTestVisitor::Action(unsizedtype, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(unsizedtype->getElemTy()),
    UnitTestVisitor::Action(unsizedtype, UnitTestVisitor::END)
  };
  UnitTestVisitor unsizeddescendvisitor(unsizeddescendscript, 3);

  unsizedtype->accept(unsizeddescendvisitor);
  unsizeddescendvisitor.finish();
}

void GCTypeUnitTest::test_ArrayGCType_accept_unsized_nodescend() {
  const ArrayGCType* const unsizedtype =
    ArrayGCType::get(mod, unsizedarrmd, GCType::ImmutableID);
  UnitTestVisitor::Action unsizednodescendscript[] = {
    UnitTestVisitor::Action(unsizedtype, UnitTestVisitor::BEGIN, false),
    UnitTestVisitor::Action(unsizedtype, UnitTestVisitor::END)
  };
  UnitTestVisitor unsizednodescendvisitor(unsizednodescendscript, 2);

  unsizedtype->accept(unsizednodescendvisitor);
  unsizednodescendvisitor.finish();
}

void GCTypeUnitTest::test_ArrayGCType_accept_sized_descend() {
  const ArrayGCType* const sizedtype =
    ArrayGCType::get(mod, sizedarrmd, GCType::ImmutableID);
  UnitTestVisitor::Action sizeddescendscript[] = {
    UnitTestVisitor::Action(sizedtype, UnitTestVisitor::BEGIN, true),
    UnitTestVisitor::Action(sizedtype->getElemTy()),
    UnitTestVisitor::Action(sizedtype, UnitTestVisitor::END)
  };
  UnitTestVisitor sizeddescendvisitor(sizeddescendscript, 3);

  sizedtype->accept(sizeddescendvisitor);
  sizeddescendvisitor.finish();
}

void GCTypeUnitTest::test_ArrayGCType_accept_sized_nodescend() {
  const ArrayGCType* const sizedtype =
    ArrayGCType::get(mod, sizedarrmd, GCType::ImmutableID);
  UnitTestVisitor::Action sizednodescendscript[] = {
    UnitTestVisitor::Action(sizedtype, UnitTestVisitor::BEGIN, false),
    UnitTestVisitor::Action(sizedtype, UnitTestVisitor::END)
  };
  UnitTestVisitor sizednodescendvisitor(sizednodescendscript, 2);

  sizedtype->accept(sizednodescendvisitor);
  sizednodescendvisitor.finish();
}

void GCTypeUnitTest::test_GCPtrGCType_accept() {
  const GCPtrGCType* const type =
    GCPtrGCType::get(mod, gcptrstrongmd, GCType::ImmutableID);
  UnitTestVisitor::Action script[] = {
    UnitTestVisitor::Action(type)
  };
  UnitTestVisitor visitor(script, 1);

  type->accept(visitor);
  visitor.finish();
}

void GCTypeUnitTest::test_NativePtrGCType_accept() {
  const NativePtrGCType* const type =
    NativePtrGCType::get(mod, nativeptrmd, GCType::ImmutableID);
  UnitTestVisitor::Action script[] = { UnitTestVisitor::Action(type) };
  UnitTestVisitor visitor(script, 1);

  type->accept(visitor);
  visitor.finish();

}
