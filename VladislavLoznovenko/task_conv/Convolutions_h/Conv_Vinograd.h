#pragma once
#include "../Tensor4_h/Tensor4.h"

using namespace Tensor;
using namespace matrix;

//BLOCK_SIZE 2
//KER_SIZE 3

enum{ A , AT , B , BT , G , GT };

std::vector<Matrix> Win_mrx();
Tensor4 convolution_layer_Win(Tensor4& for_conv , Tensor4& filter);
void treatment_Win(Chanel& chanel , Tensor4& kernels , Chanel& result);
void make_chan_Win(Chanel& chanel , Chanel& kernel , Matrix& result);
void BtdB(Matrix& block , Matrix& res);
void GgGt(Matrix& kernel , Matrix& res);
void AresAt(Matrix& res , Matrix& result);