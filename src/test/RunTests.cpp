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
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <iostream>

int main(int argc, char **argv) {
  CppUnit::TestResult controller;
  CppUnit::TestResultCollector result;
  CppUnit::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = 
    CppUnit::TestFactoryRegistry::getRegistry();

  controller.addListener(&result);
  runner.addTest(registry.makeTest());
  runner.run(controller);

  CppUnit::XmlOutputter out(&result, std::cout);
  out.write();

  return 0;
} 
