#include "naive_algo.hh"

namespace Linear_space
{




Tensor_4d naive_convolution(Tensor_4d& tensor,Tensor_4d& kernel)
{
    //! Creating size of output tensor
    uint res_height = tensor.get_height() - kernel.get_height() + 1;
    uint res_width = tensor.get_widht() - kernel.get_widht() + 1;
    uint tens_butch = tensor.get_n_butch();

    Tensor_4d result{res_height, res_width, kernel.get_n_butch(), tens_butch};

    //! Moving over the tensor_4d by kernel and conv3d
    for (uint cur_batch = 0; cur_batch < tens_butch; ++cur_batch)
        convolution_3d(tensor[cur_batch], kernel, result[cur_batch]);
    
    return result;
}

float sum_and_mul(Matrix& layer, Matrix& kernel)
{
    float res = 0;
    uint kernel_rows = kernel.nrows(), kernel_cols = kernel.nclmns();

    for (uint i = 0; i < kernel_rows; ++i)
        for (uint j = 0; j < kernel_cols; ++j)
        
            res += layer[i][j] * kernel[i][j];
}



void convolution_3d(Channel& channel,Tensor_4d& kernel, Channel& result)
{
    uint kernel_butch = kernel.get_n_butch();


    for (uint cur_kernel_btch = 0; cur_kernel_btch <  kernel_butch; ++cur_kernel_btch)
        convolution_2d(channel, kernel[cur_kernel_btch], result[cur_kernel_btch]);
}

void convolution_2d(Channel& channel,Channel& kernel, Matrix& result)
{
    uint chnls_num = kernel.get_n_chnls();
    uint res_rows = result.nrows(), res_cols = result.nclmns();

    for (uint cur_channel = 0; cur_channel < chnls_num; ++cur_channel)
    {
        Matrix cur_matr_layer_kern = kernel[cur_channel];
        Matrix cur_matr_layer_tens = channel[cur_channel];
        Matrix conv_res{res_rows, res_cols, 0};

        for (uint i = 0; i < res_rows; ++i)
            for (uint j = 0; j < res_cols; ++j)
            {
                Matrix chan_layer{cur_matr_layer_tens};
                conv_res[i][j] = sum_and_mul(chan_layer, cur_matr_layer_kern);
            }
            
            result += conv_res;
    }
}

}