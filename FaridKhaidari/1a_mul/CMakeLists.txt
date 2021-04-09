cmake_minimum_required(VERSION 3.15)
project(Matrix)

find_package(OpenCL REQUIRED)

include_directories(../matrix)
include_directories(mul_funcs)

add_executable(matr 
  main.cc 
  mul_funcs/opcl.cc 
  ../matrix/matrix_light.cc
)

target_compile_options(matr PRIVATE -Wall -Wextra)
target_compile_options(matr PRIVATE -mavx2 -mbmi2 -mpopcnt -mfma -mcx16 -mtune=znver1)
target_compile_options(matr PRIVATE -g)
target_compile_options(matr PRIVATE -O0)

target_compile_features(matr PRIVATE cxx_std_20)

target_link_libraries(matr pthread OpenCL)

target_compile_definitions(matr PRIVATE
  __CL_ENABLE_EXCEPTIONS
  CL_TARGET_OPENCL_VERSION=220
)
