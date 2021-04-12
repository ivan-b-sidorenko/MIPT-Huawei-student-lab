cmake_minimum_required(VERSION 3.16)


project(Convolution)


set(WARN_OPTIONS
  -Wall -Wextra)

set(CMAKE_CXX_STANDARD 14)

add_executable(Convolution main.cc Channel/channel.cc Tensor_4D/tensor.cc Matrix/matrix.cc Cnvltion_algo/Naive_algo/naive_algo.cc)

