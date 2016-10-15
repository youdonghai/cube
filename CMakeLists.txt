cmake_minimum_required(VERSION 3.6)
project(cube)

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++11")

set(SOURCE_FILES main.cpp CubeDefine.h CubieCube.cpp CubieCube.h CoordCube.cpp CoordCube.h FaceCube.cpp FaceCube.h Search.cpp Search.h)
add_executable(cube ${SOURCE_FILES})