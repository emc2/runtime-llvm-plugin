cmake_minimum_required(VERSION 2.6)
project(runtime-llvm-plugin)

set(CMAKE_CXX_FLAGS "-g -fno-rtti -fno-exceptions -Wall")
set(SRCS src/GCType.cpp src/GCTypeVisitors.cpp src/ParseMetadataPass.cpp src/GCTypePrintVisitor.cpp src/TypeBuilder.cpp src/GCTypeRealizer.cpp src/GCTypeTraceGen.cpp)

include_directories("/usr/local/include/" "include/")

add_library(CORE ${SRCS})
