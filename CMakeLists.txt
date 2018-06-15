cmake_minimum_required(VERSION 3.10)
project(keepasspp)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

# if we want some options for msvc
#if (MSVC)
#else()
#endif()

# Add additional libriaries

#find_package(Boost REQUIRED COMPONENTS unit_test_framework)
#find_package(Boost COMPONENTS unit_test_framework system)
#include_directories(${Boost_INCLUDE_DIR})

# TODO: Check is boost exists
# Configure project
add_subdirectory(src)

include_directories(src)
add_executable(main main.cpp ${SOURCES})
target_link_libraries(main keepasspp libgcrypt libargon2)

# Configure tests
if (${CMAKE_BUILD_TYPE} STREQUAL Test OR ${CMAKE_BUILD_TYPE} STREQUAL Debug)
    # TODO: Is there way to do this in more beautifull way?
    include_directories(src)
    add_subdirectory(test)
endif()

#file(GLOB_RECURSE SOURCES src/keepasspp/*.cpp src/keepasspp/*.hpp)
# if you wanna dll
#add_library(keepasspp SHARED ${SOURCES})
#add_library(keepasspp ${SOURCES})
#target_include_directories(keepasspp PRIVATE src)
#file(GLOB_RECURSE TEST_SOURCES src/tests/*.cpp src/tests/*.hpp)
#add_executable(test_keepasslib ${SOURCES} ${TEST_SOURCES})
#target_include_directories(test_keepasslib PRIVATE src)
#target_include_directories(test_keepasslib PRIVATE ${Boost_INCLUDE_DIRS})
#target_link_libraries(test_keepasslib ${Boost_LIBRARIES})
#
