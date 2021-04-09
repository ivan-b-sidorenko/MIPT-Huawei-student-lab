cmake_minimum_required(VERSION 3.15)
project(Matrix)

set(CMAKE_CXX_STANDARD 20)

include_directories(../matrix)
include_directories(mul_funcs)

set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_FLAGS "-O0")

set(CMAKE_CXX_FLAGS "-pthread")

add_compile_options(-Wall -Wextra)
add_compile_options(-mavx2 -mbmi2 -mpopcnt -mfma -mcx16 -mtune=znver1)
add_compile_options(-g)
add_compile_options(-O0)
add_compile_options(-pthread)

add_executable(matr main.cc)
