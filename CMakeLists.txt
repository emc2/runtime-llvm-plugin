cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# USER CONFIGURATION

project("runtime-llvm-plugin" VERSION 1.0.0)

## Set target architecture.  If you're always building for the host
## platform, this is fine; otherwise, you'll need to replace this.
##
## TASK: Set your architecture appropriately

set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -march=native -std=gnu99")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CPP_FLAGS_RELEASE} -march=native -std=c++11")

## We have to use one target's build directory as the coverage info
## directory.  The best way to do this is to build a static library
## consisting of everything that gets linked against any tests.
##
## TASK: Set your coverage target, if needed
set(COVERAGE_TARGET "${PROJECT_NAME}_static")


find_package(LLVM REQUIRED)
include_directories(${LLVM_INCLUDE_DIR} "include/")

# Base Configuration

# Configuration for all builds

## Make people behave!

add_definitions(-Wall -Wextra -Werror -Wno-unused-parameter)

## Add the local include directory.  Also make it the output directory
## for any synthesized includes

include_directories(BEFORE include)
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

## Explicitly include /usr/local in the include and library paths

include_directories(BEFORE SYSTEM /usr/local/include)
link_directories(/usr/local/lib)

## Add ./lib to the link path.  This is to enable sandbox testing in
## an environment like Jenkins where this project may depend on others,
## and we want to use the latest artifacts at all times without having to
## install them on the host OS.  Put libraries here to link against
## them during build and test.  This path will be stripped from RPATH
## by the installer.

set(LOCAL_LINK_DIR ${CMAKE_SOURCE_DIR}/lib)
file(MAKE_DIRECTORY ${LOCAL_LINK_DIR})
link_directories(${LOCAL_LINK_DIR})
link_directories(${LLVM_LIBRARY_DIR})

# Configuration for debug builds

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99 -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -O0")
set(CMAKE_DEBUG_POSTFIX "d")

## We only enable coverage on executables at the top level.  We only
## want coverage info for source files under the src directory, but not
## for the test directory.

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")


# Coverage reporting setup

find_program(GCOVR gcovr)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/coverage)

set(COVERAGE_OBJ_DIR "${CMAKE_SOURCE_DIR}/src/CMakeFiles/${COVERAGE_TARGET}.dir/")

## Define two targets to generate coverage information: one to
## generate an HTML report, and another to generate Cobertura XML.

if(GCOV_COMMAND)

### The gcov command can get fussy on platforms that use LLVM, so allow
###  a substitute

add_custom_target(coverage-html
                  COMMAND ${GCOVR} --root=${CMAKE_SOURCE_DIR}/src --gcov-executable=${GCOV_COMMAND} --object-directory=${COVERAGE_OBJ_DIR} --html --html-details --html-absolute-paths --keep --output=${CMAKE_SOURCE_DIR}/coverage/coverage.html
                  COMMENT "Producing HTML coverage report."
                  )

add_custom_target(coverage
                  COMMAND ${GCOVR} --root=${CMAKE_SOURCE_DIR}/src --gcov-executable=${GCOV_COMMAND} --object-directory=${COVERAGE_OBJ_DIR} --xml --keep --output=${CMAKE_SOURCE_DIR}/coverage/coverage.xml
                  COMMENT "Producing XML (Cobertura) coverage report."
                  )

else()

add_custom_target(coverage-html
                  COMMAND ${GCOVR} --root=${CMAKE_SOURCE_DIR}/src --object-directory=${COVERAGE_OBJ_DIR} --html --html-details --html-absolute-paths --keep --output=${CMAKE_SOURCE_DIR}/coverage/coverage.html
                  COMMENT "Producing HTML coverage report."
                  )

add_custom_target(coverage
                  COMMAND ${GCOVR} --root=${CMAKE_SOURCE_DIR}/src --object-directory=${COVERAGE_OBJ_DIR} --xml --keep --output=${CMAKE_SOURCE_DIR}/coverage/coverage.xml
                  COMMENT "Producing XML (Cobertura) coverage report."
                  )

endif()

# Doxygen Configuration

find_package(Doxygen)

if(DOXYGEN_FOUND)

## Set up a depedence to rebuild Doxyfile if Doxyfile.in changes

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
               ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

## Add a target to run Doxygen

add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMENT "Generating documentation" VERBATIM)

endif(DOXYGEN_FOUND)


# Test configuration

enable_testing()

## Define a function for adding a unit test suite written with the
## GoogleTest framework.

function(googletest_suite target)
    find_package(LLVM REQUIRED)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} gtest pthread ${LLVM_MODULE_LIBS})

    add_test(${target} ${target} --gtest_output=xml:${CMAKE_SOURCE_DIR}/${target}.xml)
    set_tests_properties(${target} PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${LOCAL_LINK_DIR}")
endfunction()


# Splint Configuration

find_program(SPLINT splint)

# Subdirectories

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(include)
