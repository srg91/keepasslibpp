enable_testing()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# TODO: Do we need to use add_test?
file(GLOB_RECURSE TEST_SOURCE_FILES *.cpp *.hpp)
add_executable(test_keepasslib ${TEST_SOURCE_FILES})
target_link_libraries(test_keepasslib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
