enable_testing()

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# TODO: Do we need to use add_test?
file(GLOB_RECURSE TEST_SOURCES *.cpp *.hpp)
add_executable(test_keepasslib ${SOURCES} ${TEST_SOURCES})
target_link_libraries(test_keepasslib keepasslib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
