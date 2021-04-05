#pragma once
#include "../Tensor4_h/Tensor4.h"

using namespace Tensor;

matrix::Matrix GEMM_ker(Tensor4& filter);
matrix::Matrix GEMM_ten(Tensor4& f_map , Tensor4& filter);
void reverse_GEMM(matrix::Matrix& OFmap , Tensor4& result);
Tensor4 conv_layer_fast(Tensor4& f_map , Tensor4& filter);