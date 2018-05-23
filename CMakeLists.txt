cmake_minimum_required(VERSION 3.10)
project(keepasslib)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

# if we want some options for msvc
#if (MSVC)
#else()
#endif()

# Add additional libriaries
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

#find_package(Boost REQUIRED COMPONENTS unit_test_framework)
#find_package(Boost COMPONENTS unit_test_framework system)
#include_directories(${Boost_INCLUDE_DIR})

# TODO: Check is boost exists
# Configure project
add_subdirectory(src)

include_directories(src)
add_executable(main main.cpp ${SOURCES})
target_link_libraries(main keepasslib)

# Configure tests
if (${CMAKE_BUILD_TYPE} STREQUAL Test)
    # TODO: Is there way to do this in more beautifull way?
    include_directories(src)
    add_subdirectory(test)
endif()

#file(GLOB_RECURSE SOURCES src/keepasslib/*.cpp src/keepasslib/*.hpp)
# if you wanna dll
#add_library(keepasslib SHARED ${SOURCES})
#add_library(keepasslib ${SOURCES})
#target_include_directories(keepasslib PRIVATE src)
#file(GLOB_RECURSE TEST_SOURCES src/tests/*.cpp src/tests/*.hpp)
#add_executable(test_keepasslib ${SOURCES} ${TEST_SOURCES})
#target_include_directories(test_keepasslib PRIVATE src)
#target_include_directories(test_keepasslib PRIVATE ${Boost_INCLUDE_DIRS})
#target_link_libraries(test_keepasslib ${Boost_LIBRARIES})
#
