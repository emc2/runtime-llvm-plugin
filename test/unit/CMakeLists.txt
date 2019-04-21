# User Configuration

## TASK: Add all your test sources here
set(UNIT_TEST_SRCS
    unit_test_main.cpp
    GCParamsUnitTest.cpp
    GenTypeUnitTest.cpp)

# Additional Configuration

## Make this the output directory

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

## Add the test suite

set(ALL_UNIT_TEST_SRCS ${UNIT_TEST_SRCS} unit_test_main.cpp)
googletest_suite(unit_test ${UNIT_TEST_SRCS})

## Make sure that the right libraries are added as link targets.
## Here, we use the single static library convention.
##
## TASK: Add your link libraries

target_link_libraries(unit_test ${PROJECT_NAME}_static)
