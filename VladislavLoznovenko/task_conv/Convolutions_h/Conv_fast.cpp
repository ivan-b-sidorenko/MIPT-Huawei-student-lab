#include "Conv_fast.h"

Tensor4 conv_layer_fast(Tensor4& for_conv , Tensor4& filter)
{
	int new_h = for_conv.get_high() - filter.get_high() + 1;
	int new_w = for_conv.get_width() - filter.get_width() + 1;

	Tensor4 result(for_conv.get_num_butch() , filter.get_num_butch() , new_h , new_w);

	std::vector<Tensor4> ten_parts = make_parts_for_tile(for_conv , filter.get_high());

	std::vector<std::thread> threads;

	for (int i = 0 ; i < ten_parts.size() / 8 ; ++i)
	{
		treat(ten_parts[i * 8 + 0] , result , filter , i * 8 + 0);
		treat(ten_parts[i * 8 + 1] , result , filter , i * 8 + 1);
		treat(ten_parts[i * 8 + 2] , result , filter , i * 8 + 2);
		treat(ten_parts[i * 8 + 3] , result , filter , i * 8 + 3);
		treat(ten_parts[i * 8 + 4] , result , filter , i * 8 + 4);
		treat(ten_parts[i * 8 + 5] , result , filter , i * 8 + 5);
		treat(ten_parts[i * 8 + 6] , result , filter , i * 8 + 6);
		treat(ten_parts[i * 8 + 7] , result , filter , i * 8 + 7);
	}

	for (int i = ten_parts.size() - ten_parts.size() % 8 ; i < ten_parts.size() ; ++i)
		treat(ten_parts[i] , result , filter , i);

	return result;		
}

void treat(Tensor4& ten_part , Tensor4& result , Tensor4& filter , int counter)
{
	Tensor4 per = treatment_fast(ten_part , filter);
	add_ten(per , result , counter);
}

Tensor4 treatment_fast(Tensor4& f_map , Tensor4& filter)
{
	int new_h = f_map.get_high() - filter.get_high() + 1;
	int new_w = f_map.get_width() - filter.get_width() + 1;
	Tensor4 result(f_map.get_num_butch() , filter.get_num_butch() , new_h , new_w);

	for (int butch = 0 ; butch < f_map.get_num_butch() ; ++butch)
	{
		for (int ker = 0 ; ker < filter.get_num_butch() ; ++ker)
		{
			make_chan_fast(f_map[butch] , filter[ker] , result[butch][ker]);
		}
	}
	return result;
}

void make_chan_fast(Chanel& chanel , Chanel& kernel , matrix::Matrix& result)
{
	for (int chan = 0 ; chan < kernel.get_num_mrx() ; ++chan)
	{
		matrix::Matrix res_per(result.get_num_str() , result.get_num_col());
		for (int i = 0 ; i < res_per.get_num_str() ; ++i)
			for (int j = 0 ; j < res_per.get_num_col() ; ++j)
			{
				matrix::Matrix per(chanel[chan] , kernel[chan].get_num_str() , kernel[chan].get_num_col() , i , j);
				res_per[i][j] = matrix::elem_sum(per , kernel[chan]);
			}
		matrix::sum(result , res_per , result);
	}
}

std::vector<Tensor4> make_parts_for_tile(Tensor4& f_map , int filter_high)
{
	std::vector<Tensor4> tile_parts;
	for (int i = 0 ; i < f_map.get_high() - filter_high + 1 ; ++i)
		tile_parts.emplace_back(i , filter_high , f_map);

	return tile_parts;
}


