#pragma once
#include "../Tensor4_h/Tensor4.h"

using namespace Tensor;

Tensor4 convolution_layer_naive(Tensor4& for_conv , Tensor4& filter);
void treatment_naive(Chanel& chanel , Tensor4& kernels , Chanel& result);
void make_chan(Chanel& chanel , Chanel& kernel , matrix::Matrix& result);
int strange_sum(matrix::Matrix& for_conv , matrix::Matrix& ker);