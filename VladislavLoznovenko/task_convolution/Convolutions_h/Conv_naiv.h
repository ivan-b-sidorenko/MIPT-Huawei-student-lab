#include "../Tensor4_h/Tensor4.h"

Tensor4 conv_main_algo(Tensor4 for_conv , Chanel filter , int layers);
Tensor4 convolution_layer_naive(Tensor4& for_conv , Chanel& filter);
matrix::Matrix treatment_naive(matrix::Matrix& chanel , matrix::Matrix& kernel);

Tensor4 conv_main_algo_fast(Tensor4 for_conv , Chanel filter , int layers);
Tensor4 convolution_layer_fast(Tensor4& for_conv , Chanel& filter);
matrix::Matrix treatment_fast(matrix::Matrix& chanel , matrix::Matrix& kernel);