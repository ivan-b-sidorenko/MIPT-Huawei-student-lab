cmake_minimum_required(VERSION 3.12)

project(matrix)

find_package(OpenCL REQUIRED)

set(EX_NAME ${PROJECT_NAME}.o)

# some useful warnings
set(WARN_OPTIONS 
  -Wall -Wextra -Wunused -Wpedantic # classic
  -Wno-old-style-cast
  -Weffc++
  -Wswitch-enum
  -Wunreachable-code)

add_executable(${EX_NAME} main.cc)

# Specify standard
target_compile_features(${EX_NAME} PRIVATE cxx_std_20)

# Specify warning options
target_compile_options(${EX_NAME} PRIVATE ${WARN_OPTIONS})
target_compile_options(${EX_NAME} PRIVATE -g -O0)

# Required libraries
target_link_libraries(${EX_NAME} pthread OpenCL)

target_include_directories(${EX_NAME} PRIVATE Linal Just)

target_compile_definitions(${EX_NAME} PRIVATE 
  CL_HPP_ENABLE_EXCEPTIONS
  CL_HPP_TARGET_OPENCL_VERSION=210
)

