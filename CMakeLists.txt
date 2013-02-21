cmake_minimum_required(VERSION 2.6)

project(runtime-llvm-plugin)

set(CMAKE_CXX_FLAGS "-O3 -fno-rtti -fno-exceptions -Wall")

set(SRCS
    src/GCParams.cpp
    src/GCType.cpp
    src/GCTypeVisitors.cpp
    src/ParseMetadataPass.cpp
    src/GCTypePrintVisitor.cpp
    src/TypeBuilder.cpp
    src/GCTypeRealizer.cpp
    src/GCTypeTraceGen.cpp
   )

include_directories("/usr/local/include/" "include/")

add_library(CORE ${SRCS})

project(tests)

set(CMAKE_CXX_FLAGS "-g -Wall -DUNIT_TEST")

set(RUN_TESTS_SRC src/test/RunTests.cpp)

link_directories(/usr/lib64/llvm)
add_executable(UnitTests ${RUN_TESTS_SRC} ${SRCS})
target_link_libraries (UnitTests cppunit pthread ffi dl m LLVMCore LLVMSupport)