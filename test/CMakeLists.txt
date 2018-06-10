enable_testing()

#set(Boost_USE_STATIC_LIBS ON)

#find_package(Boost COMPONENTS unit_test_framework REQUIRED)
#include_directories(${Boost_INCLUDE_DIRS})
find_package(GTest REQUIRED)

file(GLOB TEST_SOURCES *.cpp *.hpp)
add_executable(test_keepasslib ${SOURCES} ${TEST_SOURCES})
target_include_directories(test_keepasslib ${GTEST_INCLUDE_DIRS})
target_link_libraries(test_keepasslib keepasslibpp ${GTEST_BOTH_LIBRARIES} libgcrypt libargon2)

# TODO: Do we need to use add_test?

#[[
If you would like each Google test to show up in CTest as a test you may use the following macro. NOTE: It will slow down your tests by running an executable for each test and test fixture. You will also have to rerun CMake after adding or removing tests or test fixtures.

GTEST_ADD_TESTS(executable extra_args ARGN)

executable = The path to the test executable
extra_args = Pass a list of extra arguments to be passed to
executable enclosed in quotes (or "" for none)
ARGN =       A list of source files to search for tests & test
fixtures.
Example:
set(FooTestArgs --foo 1 --bar 2)
add_executable(FooTest FooUnitTest.cc)
GTEST_ADD_TESTS(FooTest "${FooTestArgs}" FooUnitTest.cc)
]]