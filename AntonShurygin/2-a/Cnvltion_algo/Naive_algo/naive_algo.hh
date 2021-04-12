#ifndef __NAIVE_ALGO__
#define __NAIVE_ALGO__

#include "../../Tensor_4D/tensor.hh"

namespace Linear_space
{

void convolution_2d(Channel& channel,Channel& kernel, Matrix& result);


void convolution_3d(Channel& channel,Tensor_4d& kernel, Channel& result);


Tensor_4d naive_convolution( Tensor_4d& tensor, Tensor_4d& kernel);

float sum_and_mul( Matrix& layer,  Matrix& kernel);

}


#endif /* __NAIVE_ALGO__ */