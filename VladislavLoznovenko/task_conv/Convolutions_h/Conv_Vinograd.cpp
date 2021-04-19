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

Tensor4 convolution_layer_Win(Tensor4& for_conv , Tensor4& filter)
{
	int new_h = for_conv.get_high() - filter.get_high() + 1;
	int new_w = for_conv.get_width() - filter.get_width() + 1;

	Tensor4 result(for_conv.get_num_butch() , filter.get_num_butch() , new_h , new_w);

	for (int butch = 0 ; butch < for_conv.get_num_butch() ; ++butch)
		treatment_Win(for_conv[butch] , filter , result[butch]);
	
	return result;		
}

void treatment_Win(Chanel& chanel , Tensor4& kernels , Chanel& result)
{
	for (int ker_b = 0 ; ker_b < kernels.get_num_butch() ; ++ker_b)
		make_chan_Win(chanel , kernels[ker_b] , result[ker_b]);
}

void make_chan_Win(Chanel& chanel , Chanel& kernel , Matrix& result)
{
	int kernels = kernel.get_num_mrx();
	int chanel_h_Win = (chanel.get_high() - 2) / 2;
	int chanel_w_Win = (chanel.get_width() - 2) / 2;

	for (int chan = 0 ; chan < kernels ; ++chan)
	{
		Matrix res_per(result.get_num_str() , result.get_num_col());
		Matrix res_per_Win(4 , 4);
		Matrix block_per(4 , 4);
		Matrix res_Win(2 , 2);

		GgGt(kernel[chan] , res_per_Win);
		
		for (int i = 0 ; i < chanel_h_Win ; ++i)
			for (int j = 0 ; j < chanel_w_Win ; ++j)
			{
				//Take matrix 4x4 and use formule for Winograd
				Matrix for_Win(chanel[chan] , 4 , 4 , i * 2 , j * 2);

				//Win_formule
				BtdB(for_Win , block_per);
				logic_product(block_per , res_per_Win , block_per);
				AresAt(block_per , res_Win);

				res_per[i * 2 + 0][j * 2 + 0] = res_Win[0][0];
				res_per[i * 2 + 0][j * 2 + 1] = res_Win[0][1];
				res_per[i * 2 + 1][j * 2 + 0] = res_Win[1][0];
				res_per[i * 2 + 1][j * 2 + 1] = res_Win[1][1];
			}

		//if for Tensor with odd parameters
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

void BtdB(Matrix& block , Matrix& res)
{
	res[0][0] = (block[0][0] - block[2][0]) - (block[0][2] - block[2][2]);
	res[0][1] = (block[0][1] - block[2][1]) + (block[0][2] - block[2][2]);
	res[0][2] = (block[0][2] - block[2][2]) - (block[0][1] - block[2][1]);
	res[0][3] = (block[0][1] - block[2][1]) - (block[0][3] - block[2][3]);

	res[1][0] = (block[1][0] + block[2][0]) - (block[1][2] + block[2][2]);
	res[1][1] = (block[1][1] + block[2][1]) + (block[1][2] + block[2][2]);
	res[1][2] = (block[1][2] + block[2][2]) - (block[1][1] + block[2][1]);
	res[1][3] = (block[1][1] + block[2][1]) - (block[1][3] + block[2][3]);

	res[2][0] = (block[2][0] - block[1][0]) - (block[2][2] - block[1][2]);
	res[2][1] = (block[2][1] - block[1][1]) + (block[2][2] - block[1][2]);
	res[2][2] = (block[2][2] - block[1][2]) - (block[2][1] - block[1][1]);
	res[2][3] = (block[2][1] - block[1][1]) - (block[2][3] - block[1][3]);

	res[3][0] = (block[1][0] - block[3][0]) - (block[1][2] - block[3][2]);
	res[3][1] = (block[1][1] - block[3][1]) + (block[1][2] - block[3][2]);
	res[3][2] = (block[1][2] - block[3][2]) - (block[1][1] - block[3][1]);
	res[3][3] = (block[1][1] - block[3][1]) - (block[1][3] - block[3][3]);
}

void GgGt(Matrix& kernel , Matrix& res)
{
	res[0][0] = kernel[0][0];
	res[0][1] = (kernel[0][0] + kernel[0][2] + kernel[0][1]) / 2;
	res[0][2] = (kernel[0][0] + kernel[0][2] - kernel[0][1]) / 2;
	res[0][3] = kernel[0][2];

	res[1][0] = (kernel[0][0] + kernel[2][0] + kernel[1][0]) / 2;
	res[1][1] = ((kernel[0][0] + kernel[2][0] + kernel[1][0]) + (kernel[0][2] + kernel[2][2] + kernel[1][2]) + (kernel[0][1] + kernel[2][1] + kernel[1][1])) / 4;
	res[1][2] = ((kernel[0][0] + kernel[2][0] + kernel[1][0]) + (kernel[0][2] + kernel[2][2] + kernel[1][2]) - (kernel[0][1] + kernel[2][1] + kernel[1][1])) / 4;
	res[1][3] = (kernel[0][2] + kernel[2][2] + kernel[1][2]) / 2;

	res[2][0] = (kernel[0][0] + kernel[2][0] - kernel[1][0]) / 2;
	res[2][1] = ((kernel[0][0] + kernel[2][0] - kernel[1][0]) + (kernel[0][2] + kernel[2][2] - kernel[1][2]) + (kernel[0][1] + kernel[2][1] - kernel[1][1])) / 4;
	res[2][2] = ((kernel[0][0] + kernel[2][0] - kernel[1][0]) + (kernel[0][2] + kernel[2][2] - kernel[1][2]) - (kernel[0][1] + kernel[2][1] - kernel[1][1])) / 4;;
	res[2][3] = (kernel[0][2] + kernel[2][2] - kernel[1][2]) / 2;

	res[3][0] = kernel[2][0];
	res[3][1] = (kernel[2][0] + kernel[2][2] + kernel[2][1]) / 2;
	res[3][2] = (kernel[2][0] + kernel[2][2] - kernel[2][1]) / 2;
	res[3][3] = kernel[2][2];
}

void AresAt(Matrix& res , Matrix& result)
{
	float* per = new float[8];

	per[0] = res[0][0] + res[0][1] + res[0][2];
	per[1] = res[0][1] - res[0][2] - res[0][3];
	per[2] = res[1][0] + res[1][1] + res[1][2];
	per[3] = res[1][1] - res[1][2] - res[1][3];
	per[4] = res[2][0] + res[2][1] + res[2][2];
	per[5] = res[2][1] - res[2][2] - res[2][3];
	per[6] = res[3][0] + res[3][1] + res[3][2];
	per[7] = res[3][1] - res[3][2] - res[3][3];

	result[0][0] = per[0] + per[2] + per[4];
	result[0][1] = per[1] + per[3] + per[5];
	result[1][0] = per[2] - per[4] - per[6];
	result[1][1] = per[3] - per[5] - per[7];

	delete[] per;
}