# User configuration

## Libraries
##
## TASK: Set up your libraries here

### Define all the sources for the library

set(LIB_SRCS
    GenType.cpp
    GenTypeVisitors.cpp
    ParseMetadataPass.cpp
    GenTypePrintVisitor.cpp
    TypeBuilder.cpp
    TypeRealizer.cpp
    TraceGenerator.cpp)

### Create a static library, against which we'll link all the tests

add_library(${PROJECT_NAME}_static STATIC ${LIB_SRCS})
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

### Create a shared library as well

add_library(${PROJECT_NAME}_shared SHARED ${LIB_SRCS})
set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

## Set up install targets

install(TARGETS ${PROJECT_NAME}_shared LIBRARY DESTINATION lib COMPONENT libraries)
install(TARGETS ${PROJECT_NAME}_static ARCHIVE DESTINATION lib COMPONENT archives)

## Set up sources to feed into splint

set(SPLINT_SRCS ${EXEC_SRCS} ${LIB_SRCS})
set(SPLINT_EXPECTED 1)


# Additional configuration

## Add coverage information to all source files in this directory
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")

# Splint
add_custom_target(splint ${SPLINT} -strict -I${CMAKE_SOURCE_DIR}/include ${SPLINT_SRCS} +partial -expect ${SPLINT_EXPECTED}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMENT "Running splint" VERBATIM)
