#pragma once
#include "../Tensor4_h/Tensor4.h"

using namespace Tensor;

Tensor4 conv_layer_fast(Tensor4& for_conv , Tensor4& filter);
Tensor4 treatment_fast(Tensor4& f_map , Tensor4& filter);
void treat(Tensor4& ten_part , Tensor4& result , Tensor4& filter , int counter);
void make_chan_fast(Chanel& chanel , Chanel& kernel , matrix::Matrix& result);
std::vector<Tensor4> make_parts_for_tile(Tensor4& f_map , int filter_high);