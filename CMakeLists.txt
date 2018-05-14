cmake_minimum_required(VERSION 3.10)
project(KeePassLib)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 14)

add_executable(KeePassLib main.cpp KeePassLib/PwUuid.cpp KeePassLib/PwUuid.h)
