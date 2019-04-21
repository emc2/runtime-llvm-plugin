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
#include <gtest/gtest.h>


TEST(GCParams, testGCParams) {
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
  EXPECT_TRUE(writeLogging.writeLogging);
  EXPECT_TRUE(readBarriers.readBarriers);
  EXPECT_TRUE(clusterize.clusterize);
  EXPECT_TRUE(generational.generational);
  EXPECT_TRUE(doublePtrs.doublePtrs);
  EXPECT_TRUE(copyFuncs.copyFuncs);
  EXPECT_TRUE(moveFuncs.moveFuncs);
  EXPECT_TRUE(traceFuncs.traceFuncs);
}
