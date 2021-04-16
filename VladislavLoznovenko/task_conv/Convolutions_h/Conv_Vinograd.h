#pragma once
#include "../Tensor4_h/Tensor4.h"

using namespace Tensor;
using namespace matrix;

#define BLOCK_SIZE 2
#define KER_SIZE 3

enum{ A , AT , B , BT , G , GT };

std::vector<Matrix> Win_mrx();
Tensor4 convolution_layer_Win(Tensor4& for_conv , Tensor4& filter , std::vector<Matrix>& convert);
void treatment_Win(Chanel& chanel , Tensor4& kernels , Chanel& result , std::vector<Matrix>& convert);
void make_chan_Win(Chanel& chanel , Chanel& kernel , Matrix& result , std::vector<Matrix>& convert);
void Win_formule(Matrix& block , Matrix kernel , std::vector<Matrix>& convert);