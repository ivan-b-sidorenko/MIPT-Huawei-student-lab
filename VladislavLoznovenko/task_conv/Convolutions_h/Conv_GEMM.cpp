#include "Conv_GEMM.h"

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

matrix::Matrix GEMM_ten(Tensor4& f_map , Tensor4& filter)
{
	int data_h = f_map.get_high();
	int data_w = f_map.get_width();
	int data_c = f_map.get_num_mrx();
	int data_b = f_map.get_num_butch();

	int fil_h = filter.get_high();
	int fil_w = filter.get_width();

	int fil_high = data_b * (data_h - fil_h + 1) * (data_w - fil_w + 1);
	int fil_width = fil_h * fil_w * data_c; 

	matrix::Matrix IFmap_matrix(fil_high , fil_width);

	for (int butch = 0 ; butch < data_b ; ++butch)
		for (int chan = 0 ; chan < data_c ; ++chan)

			for (int i = 0 ; i < data_h - fil_h + 1 ; ++i)
				for (int j = 0 ; j < data_w - fil_w + 1 ; ++j)

					for (int start_h = i ; start_h < i + fil_h ; ++start_h)
						for (int start_w = j ; start_w < j + fil_w ; ++start_w)
							IFmap_matrix[butch * (data_h - fil_h + 1) * (data_w - fil_w + 1) + i * (data_w - fil_w + 1) + j][chan * fil_h * fil_w  + (start_h - i) * (fil_w) + start_w - j] = f_map[butch][chan][start_h][start_w];

	return IFmap_matrix;
}

void reverse_GEMM(matrix::Matrix& OFmap , Tensor4& result)
{
	for (int butch = 0 ; butch < result.get_num_butch() ; ++butch)
		for (int chan = 0 ; chan < result.get_num_mrx() ; ++chan)
			for (int i = 0 ; i < result.get_high() ; ++i)
				for (int j = 0 ; j < result.get_width() ; ++j)
					result[butch][chan][i][j] = OFmap[butch * result.get_high() * result.get_width() + i * result.get_width() + j][chan];
}

Tensor4 conv_layer_GEMM(Tensor4& f_map , Tensor4& filter)
{

	int new_h = f_map.get_high() - filter.get_high() + 1;
	int new_w = f_map.get_width() - filter.get_width() + 1;

	Tensor4 result(f_map.get_num_butch() , filter.get_num_butch() , new_h , new_w);

	matrix::Matrix OFmap_ = make_GEMM(f_map , filter);
	//std::cout << OFmap;

	reverse_GEMM(OFmap_ , result);

	return result;
}

matrix::Matrix make_GEMM(Tensor4& f_map , Tensor4& filter)
{
	matrix::Matrix IFmap = GEMM_ten(f_map , filter);
	matrix::Matrix fil_matrix = GEMM_ker(filter);

	matrix::Matrix OFmap(IFmap.get_num_str() , fil_matrix.get_num_col());

	matrix::Matrix_product_fast(IFmap , fil_matrix , OFmap);

	return OFmap;
}