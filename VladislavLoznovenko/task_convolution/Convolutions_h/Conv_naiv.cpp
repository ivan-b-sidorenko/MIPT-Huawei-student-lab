#include "Conv_naiv.h"

Tensor4 conv_main_algo(Tensor4 for_conv , Chanel filter , int layers)
{
	Tensor4 per_res = for_conv;
	for (int i = 0 ; i < layers ; ++i)
		per_res = convolution_layer_naive(per_res , filter);

	/*Chanel result(per_res.get_num_butch() , per_res.get_high() , per_res.get_width());

	for (int i = 0 ; i < for_conv.get_num_butch() ; ++i)
	{
		matrix::Matrix res(result.get_width() , result.get_high());
		for (int j = 0 ; j < for_conv.get_num_mrx() ; ++j)
			sum(res , per_res[i][j] , res);

		result[i] = res;
	}*/

	return per_res;
}


Tensor4 convolution_layer_naive(Tensor4& for_conv , Chanel& filter)
{
	int new_high = for_conv.get_high() - filter.get_high() + 1;
	int new_width = for_conv.get_width() - filter.get_width() + 1;
	Tensor4 res(for_conv.get_num_butch() , for_conv.get_num_mrx() , new_high , new_width);

	for (int i = 0 ; i < for_conv.get_num_butch() ; ++i)
	{
		for (int j = 0 ; j < for_conv.get_num_mrx() ; ++j)
			res[i][j] = treatment_naive(for_conv[i][j] , filter[j]);
	}

	return res;
}

matrix::Matrix treatment_naive(matrix::Matrix& chanel , matrix::Matrix& kernel)
{
	int res_col = chanel.get_num_col() - kernel.get_num_col() + 1;
	int res_str = chanel.get_num_str() - kernel.get_num_str() + 1;
	matrix::Matrix result(res_col , res_str);

	for (int i = 0 ; i < res_str ; ++i)
	{
		for (int j = 0 ; j < res_col ; ++j)
		{
			matrix::Matrix per(chanel , kernel.get_num_col() , kernel.get_num_str() , i , j);
			matrix::Matrix res_per(per.get_num_str() , kernel.get_num_col());
			Matrix_product(per , kernel , res_per);
			result[i][j] = res_per.find_max();
		}
	}

	return result;
}

Tensor4 conv_main_algo_fast(Tensor4 for_conv , Chanel filter , int layers)
{
	Tensor4 per_res = for_conv;
	for (int i = 0 ; i < layers ; ++i)
		per_res = convolution_layer_naive(per_res , filter);

	/*Chanel result(per_res.get_num_butch() , per_res.get_high() , per_res.get_width());

	for (int i = 0 ; i < for_conv.get_num_butch() ; ++i)
	{
		matrix::Matrix res(result.get_width() , result.get_high());
		for (int j = 0 ; j < for_conv.get_num_mrx() ; ++j)
			sum(res , per_res[i][j] , res);

		result[i] = res;
	}*/

	return per_res;
}


Tensor4 convolution_layer_fast(Tensor4& for_conv , Chanel& filter)
{
	int new_high = for_conv.get_high() - filter.get_high() + 1;
	int new_width = for_conv.get_width() - filter.get_width() + 1;
	Tensor4 res(for_conv.get_num_butch() , for_conv.get_num_mrx() , new_high , new_width);

	for (int i = 0 ; i < for_conv.get_num_butch() ; ++i)
	{
		for (int j = 0 ; j < for_conv.get_num_mrx() ; ++j)
			res[i][j] = treatment_naive(for_conv[i][j] , filter[j]);
	}

	return res;
}

matrix::Matrix treatment_fast(matrix::Matrix& chanel , matrix::Matrix& kernel)
{
	int res_col = chanel.get_num_col() - kernel.get_num_col() + 1;
	int res_str = chanel.get_num_str() - kernel.get_num_str() + 1;
	matrix::Matrix result(res_col , res_str);

	for (int i = 0 ; i < res_str ; ++i)
	{
		for (int j = 0 ; j < res_col ; ++j)
		{
			matrix::Matrix per(chanel , kernel.get_num_col() , kernel.get_num_str() , i , j);
			matrix::Matrix res_per(per.get_num_str() , kernel.get_num_col());
			Matrix_product_fast(per , kernel , res_per);
			result[i][j] = res_per.find_max();
		}
	}

	return result;
}