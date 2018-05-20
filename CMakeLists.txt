cmake_minimum_required(VERSION 3.10)
project(KeePassLib)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(src/keepasslib)

file(GLOB_RECURSE SOURCES src/keepasslib/*.cpp src/keepasslib/*.hpp)
# if you wanna dll
#add_library(keepasslib SHARED ${SOURCES})
add_library(keepasslib ${SOURCES})
