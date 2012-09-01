cmake_minimum_required(VERSION 2.6)
project(runtime-llvm-plugin)

set(CMAKE_CXX_FLAGS "-O2 -fno-rtti -fno-exceptions")
set(SRCS src/GCType.cpp src/GCTypeVisitors.cpp src/ParseMetadataPass.cpp src/GCTypePrintVisitor.cpp)

include_directories("/usr/local/include/" "include/")

add_library(CORE ${SRCS})
