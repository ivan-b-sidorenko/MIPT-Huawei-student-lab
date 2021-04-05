#include "Conv_fast.h"

matrix::Matrix GEMM_ker(Tensor4& filter)
{
	int data_h = filter.get_high();
	int data_w = filter.get_width();
	int data_c = filter.get_num_mrx();
	int data_b = filter.get_num_butch();

	int fil_high = data_h * data_w * data_c;
	int fil_width = data_b;

	matrix::Matrix filter_matrix(fil_high , fil_width);

	for (int butch = 0 ; butch < data_b ; ++butch)
		for (int chan = 0 ; chan < data_c ; ++chan)
			for (int high = 0 ; high < data_h ; ++high)
				for (int width = 0 ; width < data_w ; ++width)
					filter_matrix[chan * data_w * data_h + high * data_w + width][butch] = filter[butch][chan][high][width];

	return filter_matrix;
}

matrix::Matrix GEMM_ten(Tensor4& f_map)
{
	int data_h = f_map.get_high();
	int data_w = f_map.get_width();
	int data_c = f_map.get_num_mrx();
	int data_b = f_map.get_num_butch();

	int fil_high = data_h * data_w * data_c;
	int fil_width = data_b;

	matrix::Matrix filter_matrix(fil_high , fil_width);
}

