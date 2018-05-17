cmake_minimum_required(VERSION 3.10)
project(KeePassLib)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(KeePassLib)

file(GLOB_RECURSE SOURCES KeePassLib/*.cpp KeePassLib/*.hpp)
add_executable(KeePassLib main.cpp ${SOURCES})

include_directories(KeePassLib Tests)
file(GLOB_RECURSE TEST_SOURCES Tests/*.cpp Tests/*.hpp)
add_executable(Tests tests_main.cpp ${TEST_SOURCES})