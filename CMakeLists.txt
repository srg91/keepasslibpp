cmake_minimum_required(VERSION 3.10)
project(keepasslib)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 14)

if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

# if we want some options for msvc
#if (MSVC)
#else()
#endif()

# Add additional libriaries
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# TODO: Can we lib to libgcrypt20?
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

#find_package(Boost REQUIRED COMPONENTS unit_test_framework)
#find_package(Boost COMPONENTS unit_test_framework system)
#include_directories(${Boost_INCLUDE_DIR})

# TODO: Check is boost exists
# Configure project
add_subdirectory(src)

include_directories(src)
add_executable(main main.cpp ${SOURCES})
target_link_libraries(main keepasslib ${OPENSSL_CRYPTO_LIBRARY} libargon2)

# Configure tests
if (${CMAKE_BUILD_TYPE} STREQUAL Test OR ${CMAKE_BUILD_TYPE} STREQUAL Debug)
    # TODO: Is there way to do this in more beautifull way?
    include_directories(src)
    add_subdirectory(test)
endif()

#file(GLOB_RECURSE SOURCES src/keepasslibpp/*.cpp src/keepasslibpp/*.hpp)
# if you wanna dll
#add_library(keepasslibpp SHARED ${SOURCES})
#add_library(keepasslibpp ${SOURCES})
#target_include_directories(keepasslibpp PRIVATE src)
#file(GLOB_RECURSE TEST_SOURCES src/tests/*.cpp src/tests/*.hpp)
#add_executable(test_keepasslib ${SOURCES} ${TEST_SOURCES})
#target_include_directories(test_keepasslib PRIVATE src)
#target_include_directories(test_keepasslib PRIVATE ${Boost_INCLUDE_DIRS})
#target_link_libraries(test_keepasslib ${Boost_LIBRARIES})
#
