cmake_minimum_required(VERSION 2.6)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "./")

project(runtime-llvm-plugin)

find_package(LLVM REQUIRED)

set(CMAKE_CXX_FLAGS "-O3 -fno-rtti -fno-exceptions -Wall -Werror")

set(SRCS
    src/GCType.cpp
    src/GCTypeVisitors.cpp
    src/ParseMetadataPass.cpp
    src/GCTypePrintVisitor.cpp
    src/TypeBuilder.cpp
    src/GCTypeRealizer.cpp
    src/GCTypeTraceGen.cpp
   )

include_directories(${LLVM_INCLUDE_DIR} "include/")

add_library(CORE ${SRCS})

project(tests)

include_directories(${LLVM_INCLUDE_DIR} "include/")

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++0x")

set(UNIT_TEST_SRCS
    src/test/UnitTest/GCParamsUnitTest.cpp
    src/test/UnitTest/GCTypeUnitTest.cpp
   )

set(RUN_TESTS_SRC src/test/RunTests.cpp)

link_directories( ${LLVM_LIBRARY_DIR} )
add_executable(UnitTests ${RUN_TESTS_SRC} ${UNIT_TEST_SRCS} ${SRCS})
target_link_libraries (UnitTests cppunit pthread ffi dl m LLVMCore LLVMSupport)
