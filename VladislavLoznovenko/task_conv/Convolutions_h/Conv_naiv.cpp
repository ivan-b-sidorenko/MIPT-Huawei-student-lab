#include "Conv_naiv.h"

Tensor4 convolution_layer_naive(Tensor4& for_conv , Tensor4& filter)
{
	int new_h = for_conv.get_high() - filter.get_high() + 1;
	int new_w = for_conv.get_width() - filter.get_width() + 1;

	Tensor4 result(for_conv.get_num_butch() , filter.get_num_butch() , new_h , new_w);

	for (int butch = 0 ; butch < for_conv.get_num_butch() ; ++butch)
		treatment_naive(for_conv[butch] , filter , result[butch]);
	
	return result;		
}

void treatment_naive(Chanel& chanel , Tensor4& kernels , Chanel& result)
{
	for (int ker_b = 0 ; ker_b < kernels.get_num_butch() ; ++ker_b)
		make_chan(chanel , kernels[ker_b] , result[ker_b]);
}

void make_chan(Chanel& chanel , Chanel& kernel , matrix::Matrix& result)
{
	for (int chan = 0 ; chan < kernel.get_num_mrx() ; ++chan)
	{
		matrix::Matrix res_per(result.get_num_col() , result.get_num_str());
		for (int i = 0 ; i < res_per.get_num_str() ; ++i)
			for (int j = 0 ; j < res_per.get_num_col() ; ++j)
			{
				matrix::Matrix per(chanel[chan] , kernel[chan].get_num_col() , kernel[chan].get_num_str() , i , j);
				res_per[i][j] = elem_sum(per , kernel[chan]);
			}
		matrix::sum(result , res_per , result);
	}
}

int elem_sum(matrix::Matrix& for_conv , matrix::Matrix& ker)
{
	int sum = 0;
	for (int i = 0 ; i < ker.get_num_str() ; ++i)
		for (int j = 0 ; j < ker.get_num_col() ; ++j)
			sum += for_conv[i][j] * ker[i][j];

	return sum;
}