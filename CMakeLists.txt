cmake_minimum_required(VERSION 3.10)
project(src/KeePassLib)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(src/KeePassLib)

file(GLOB_RECURSE SOURCES src/KeePassLib/*.cpp src/KeePassLib/*.hpp)
add_executable(src/KeePassLib main.cpp ${SOURCES})

include_directories(src/KeePassLib Tests)
file(GLOB_RECURSE TEST_SOURCES Tests/*.cpp Tests/*.hpp)
add_executable(Tests tests_main.cpp ${TEST_SOURCES})