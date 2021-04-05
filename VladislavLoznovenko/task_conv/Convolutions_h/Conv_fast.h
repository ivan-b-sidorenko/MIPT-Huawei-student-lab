#pragma once
#include "../Tensor4_h/Tensor4.h"

using namespace Tensor;

matrix::Matrix GEMM_ker(Tensor4& filter);
matrix::Matrix GEMM_ten(Tensor4& f_map);