cmake_minimum_required(VERSION 3.15)
project(Matrix)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(CMAKE_CXX_FLAGS "-mavx2")
set(CMAKE_CXX_FLAGS "-mbmi2")
set(CMAKE_CXX_FLAGS "-mpopcnt")
set(CMAKE_CXX_FLAGS "-mfma")
set(CMAKE_CXX_FLAGS "-mcx16")
set(CMAKE_CXX_FLAGS "-mtune=znver1")

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O0")

set(CMAKE_CXX_FLAGS "-pthread")

include_directories(../matrix)
include_directories(mul_funcs)

add_executable(matr main.cc)
