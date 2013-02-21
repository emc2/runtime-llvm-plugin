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
#include "GCType.h"
#include <cppunit/extensions/HelperMacros.h>

class GCTypeUnitTest : public CppUnit::TestFixture  {

CPPUNIT_TEST_SUITE(GCTypeUnitTest);
CPPUNIT_TEST(testGetUnit);
CPPUNIT_TEST_SUITE_END();

  void testGetUnit() {

    const PrimGCType* const type = PrimGCType::getUnit();
    CPPUNIT_ASSERT(NULL == type->getLLVMType());
    CPPUNIT_ASSERT(GCType::ImmutableID == type->mutability());
    CPPUNIT_ASSERT(GCType::PrimTypeID == type->getTypeID());
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(GCTypeUnitTest);
