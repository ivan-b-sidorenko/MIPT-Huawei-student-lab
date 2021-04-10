#pragma once
#include "../Tensor4_h/Tensor4.h"

using namespace Tensor;

matrix::Matrix GEMM_ker(Tensor4& filter);
matrix::Matrix GEMM_ten(Tensor4& f_map , Tensor4& filter);
void reverse_GEMM(matrix::Matrix& OFmap , Tensor4& result);
matrix::Matrix make_GEMM(Tensor4& f_map , Tensor4& filter);
Tensor4 conv_layer_GEMM(Tensor4& f_map , Tensor4& filter);