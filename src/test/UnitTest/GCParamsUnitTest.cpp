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

#include "GCParams.h"
#include <cppunit/extensions/HelperMacros.h>

class GCParamsUnitTest : public CppUnit::TestFixture  {

  CPPUNIT_TEST_SUITE(GCParamsUnitTest);
  CPPUNIT_TEST(testGCParams);
  CPPUNIT_TEST_SUITE_END(); 

  void testGCParams();
};

CPPUNIT_TEST_SUITE_REGISTRATION(GCParamsUnitTest);

void GCParamsUnitTest::testGCParams() {
  GCParams writeLogging(true, false, false, false,
                        false, false, false, false);
  GCParams readBarriers(false, true, false, false,
                        false, false, false, false);
  GCParams clusterize(false, false, true, false,
                      false, false, false, false);
  GCParams generational(false, false, false, true,
                        false, false, false, false);
  GCParams doublePtrs(false, false, false, false,
                      true, false, false, false);
  GCParams copyFuncs(false, false, false, false,
                     false, true, false, false);
  GCParams moveFuncs(false, false, false, false,
                     false, false, true, false);
  GCParams traceFuncs(false, false, false, false,
                      false, false, false, true);
  CPPUNIT_ASSERT(writeLogging.writeLogging);
  CPPUNIT_ASSERT(readBarriers.readBarriers);
  CPPUNIT_ASSERT(clusterize.clusterize);
  CPPUNIT_ASSERT(generational.generational);
  CPPUNIT_ASSERT(doublePtrs.doublePtrs);
  CPPUNIT_ASSERT(copyFuncs.copyFuncs);
  CPPUNIT_ASSERT(moveFuncs.moveFuncs);
  CPPUNIT_ASSERT(traceFuncs.traceFuncs);
}
