#include "Conv_Vinograd.h"

//Make vector of Matrix for Winograd F(2x2 , 3x3)
std::vector<Matrix> Win_mrx()
{
	std::vector<float> AT_ = {1 , 1 , 1 , 0 , 0 , 1 , -1 , -1};
	std::vector<float> BT_ = {1 , 0 , -1 , 0 , 0 , 1 , 1 , 0 , 0 , -1 , 1 , 0 , 0 , 1 , 0 , -1};
	std::vector<float> G_ = {1 , 0 , 0 , 0.5 , 0.5 , 0.5 , 0.5 , -0.5 , 0.5 , 0 , 0 , 1};

	Matrix at(2 , 4 , AT_);
	Matrix bt(4 , 4 , BT_);
	Matrix g(4 , 3 , G_);
	Matrix a = at.transpose();
	Matrix b = bt.transpose();
	Matrix gt = g.transpose();

	std::vector<Matrix> res = {a , at , b , bt , g , gt};

	return res;
}

Tensor4 convolution_layer_Win(Tensor4& for_conv , Tensor4& filter , std::vector<Matrix>& convert)
{
	int new_h = for_conv.get_high() - filter.get_high() + 1;
	int new_w = for_conv.get_width() - filter.get_width() + 1;

	Tensor4 result(for_conv.get_num_butch() , filter.get_num_butch() , new_h , new_w);

	for (int butch = 0 ; butch < for_conv.get_num_butch() ; ++butch)
		treatment_Win(for_conv[butch] , filter , result[butch] , convert);
	
	return result;		
}

void treatment_Win(Chanel& chanel , Tensor4& kernels , Chanel& result , std::vector<Matrix>& convert)
{
	for (int ker_b = 0 ; ker_b < kernels.get_num_butch() ; ++ker_b)
		make_chan_Win(chanel , kernels[ker_b] , result[ker_b] , convert);
}

void make_chan_Win(Chanel& chanel , Chanel& kernel , Matrix& result , std::vector<Matrix>& convert)
{
	for (int chan = 0 ; chan < kernel.get_num_mrx() ; ++chan)
	{

		Matrix res_per(result.get_num_str() , result.get_num_col());
		for (int i = 0 ; i < (chanel.get_high() - 2) / 2 ; ++i)
			for (int j = 0 ; j < (chanel.get_width() - 2) / 2 ; ++j)
			{
				//Take matrix 4x4 and use formule for Winograd
				Matrix for_Win(chanel[chan] , 4 , 4 , i * 2 , j * 2);
				Win_formula(for_Win , kernel[chan] , convert);
				for (int i_ = 0 ; i_ < 2 ; ++i_)
					for (int j_ = 0 ; j_ < 2 ; ++j_)
						res_per[i * 2 + i_][j * 2 + j_] = for_Win[i_][j_];
			}

		//ifы for Tensor with odd parameters
		if ((chanel.get_high() % 2) != 0)
		{
			for (int i = res_per.get_num_str() - 1 ; i < res_per.get_num_str() ; ++i)
				for (int j = 0 ; j < res_per.get_num_col() ; ++j)
				{
					matrix::Matrix per(chanel[chan] , kernel[chan].get_num_str() , kernel[chan].get_num_col() , i , j);
					res_per[i][j] = matrix::elem_sum(per , kernel[chan]);
				}
		}

		if ((chanel.get_width() % 2) != 0)
		{
			for (int i = 0 ; i < res_per.get_num_str() ; ++i)
				for (int j = res_per.get_num_col() - 1; j < res_per.get_num_col() ; ++j)
				{
					matrix::Matrix per(chanel[chan] , kernel[chan].get_num_str() , kernel[chan].get_num_col() , i , j);
					res_per[i][j] = matrix::elem_sum(per , kernel[chan]);
				}
		}
		sum(res_per , result , result);
	}
}

void Win_formule(Matrix& block , Matrix kernel , std::vector<Matrix>& convert)
{
	kernel = Matrix_product_fast(Matrix_product_fast(convert[G] , kernel) , convert[GT]);
	
	block = Matrix_product_fast(Matrix_product_fast(convert[BT] , block) , convert[B]);

	logic_product(kernel , block , block);

	block = Matrix_product_fast(Matrix_product_fast(convert[AT] , block) , convert[A]);
}