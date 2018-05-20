cmake_minimum_required(VERSION 3.10)
project(KeePassLib)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/keepasslib/*.cpp src/keepasslib/*.hpp)
# if you wanna dll
#add_library(keepasslib SHARED ${SOURCES})
add_library(keepasslib ${SOURCES})
target_include_directories(keepasslib PRIVATE src)

file(GLOB_RECURSE TEST_SOURCES src/tests/*.cpp src/tests/*.hpp)
add_executable(test_keepasslib ${SOURCES} ${TEST_SOURCES})
target_include_directories(test_keepasslib PRIVATE src)
