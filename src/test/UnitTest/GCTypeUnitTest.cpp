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
  llvm::NamedMDNode* const testmd;
  const PrimGCType* const type;
  llvm::Value* const vals[1];
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
  llvm::MDNode* const md;
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

  GCTypeUnitTest() :
    ctx(), mod(llvm::StringRef("Test"), ctx),
    testmd(mod.getOrInsertNamedMetadata("test")),
    type(PrimGCType::getUnit()),
    vals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_UNIT)
      }),
    int1vals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_INT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 1)
      }),
    int8vals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_INT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 8)
      }),
    int16vals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_INT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 16)
      }),
    int32vals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_INT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 32)
      }),
    int64vals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_INT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 64)
      }),
    int128vals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_INT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 128)
      }),
    inttoolongvals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_INT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 32),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 0),
      }),
    inttooshortvals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_INT),
      }),
    float16vals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_FLOAT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 16)
      }),
    float32vals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_FLOAT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 32)
      }),
    float64vals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_FLOAT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 64)
      }),
    float128vals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_FLOAT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 128)
      }),
    floattoolongvals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_FLOAT),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 32),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 0),
      }),
    floattooshortvals({
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), GC_MD_TYPE_FLOAT),
      }),
    md(llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value*>(vals))),
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
    )
  {
    testmd->addOperand(md);
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
  }

  CPPUNIT_TEST_SUITE(GCTypeUnitTest);
  CPPUNIT_TEST(test_PrimGCType_getUnit);
  CPPUNIT_TEST(test_PrimGCType_getInt);
  CPPUNIT_TEST(test_PrimGCType_accept);
  CPPUNIT_TEST_SUITE_END();

  void test_PrimGCType_getUnit();
  void test_PrimGCType_getInt();
  void test_PrimGCType_accept();

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

  // Test 1: Test getUnit by itself
  CPPUNIT_ASSERT(NULL == type->getLLVMType());
  CPPUNIT_ASSERT(GCType::ImmutableID == type->mutability());
  CPPUNIT_ASSERT(GCType::PrimTypeID == type->getTypeID());

  // Set up a metadata describing a unit type

  // Test 2: test parsing the metadata, starting with Immutable
  const GCType* const got = GCType::get(mod, md, GCType::ImmutableID);

  if(GCType::PrimTypeID == got->getTypeID()) {
    CPPUNIT_ASSERT(NULL == PrimGCType::narrow(got)->getLLVMType());
    CPPUNIT_ASSERT(GCType::ImmutableID == got->mutability());
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == got->getTypeID()");

  // Test 3: test parsing the metadata, starting with Mutable
  // The resulting type should still read as immutable (it's a unit).
  const GCType* const mutgot = GCType::get(mod, md, GCType::MutableID);

  if(GCType::PrimTypeID == mutgot->getTypeID()) {
    CPPUNIT_ASSERT(NULL == PrimGCType::narrow(mutgot)->getLLVMType());
    CPPUNIT_ASSERT(GCType::ImmutableID == mutgot->mutability());
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == mutgot->getTypeID()");

  // Test 3: test parsing the metadata, starting with WriteOnce
  // The resulting type should still read as immutable (it's a unit).
  const GCType* const wogot = GCType::get(mod, md, GCType::WriteOnceID);

  if(GCType::PrimTypeID == wogot->getTypeID()) {
    CPPUNIT_ASSERT(NULL == PrimGCType::narrow(wogot)->getLLVMType());
    CPPUNIT_ASSERT(GCType::ImmutableID == wogot->mutability());
  } else
    CPPUNIT_FAIL("Assertion failed: GCType::PrimTypeID == mutgot->getTypeID()");

  // No need to test parsing a bad metadata, that will be handled by
  // the test for the top-level get.
}

void GCTypeUnitTest::test_PrimGCType_getInt() {

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

  // Test 2: getInt by itself on bad metadata

  // Test 3: Parse metadata with GCType::get
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

  // Test 4: test parsing bad metadata
}

void GCTypeUnitTest::test_PrimGCType_accept() {
  const PrimGCType* const type = PrimGCType::getUnit();
  UnitTestVisitor::Action script[] = { UnitTestVisitor::Action(type) };
  UnitTestVisitor visitor(script, 1);

  type->accept(visitor);
  visitor.finish();
}
