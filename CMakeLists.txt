cmake_minimum_required(VERSION 3.7)
project(myserver)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES server.c test.c)
add_executable(myserver ${SOURCE_FILES})