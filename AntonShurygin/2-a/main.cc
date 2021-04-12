#include "Cnvltion_algo/Naive_algo/naive_algo.hh"
#include "my_time.hpp"

using namespace Linear_space;

void Simple_test_2d()
{
    std::cout << "simple_testing_2d have started:\n";
    Tensor_4d tensor{4, 4, 1, 1, Mode::RANDOM};
    std::cout << tensor;

    Tensor_4d kernel{3, 3, 1, 1, Mode::RANDOM};
    std::cout << kernel;
    
    Tensor_4d result;

    result = naive_convolution(tensor, kernel);

    std::cout << result; 
}

void Simple_test_3d()
{
    std::cout << "simple_testing_3d have started:\n";
    Tensor_4d tensor{9, 9, 3, 1, 1.25};
    std::cout << tensor;

    Tensor_4d kernel{3, 3, 3, 1, 8};
    std::cout << kernel;
    
    Tensor_4d result;

    result = naive_convolution(tensor, kernel);

    std::cout << result; 
}


void Simple_test_4d()
{   
    std::cout << "simple_testing_4d have started:\n";

    Tensor_4d tensor{9, 9, 3, 3, 0.625};
    std::cout << tensor;

    Tensor_4d kernel{3, 3, 3, 1, 16};
    std::cout << kernel;
    
    Tensor_4d result;

    result = naive_convolution(tensor, kernel);

    std::cout << result; 
}


void Conv_test()
{
    uint tens_butch = 4, tens_channels = 8, tens_h = 16, tens_w = 32;
    uint kernel_butch = 16, kernel_channels = 4, kernel_h = 4, kernel_w = 4; 

    Tensor_4d tensor{tens_h, tens_w, tens_channels, tens_butch, Mode::RANDOM};

    #if 0
    std::cout << tensor;
    #endif

    Tensor_4d kernel{kernel_h, kernel_w, kernel_channels, kernel_butch, Mode::RANDOM};

    //#if 0
    std::cout << kernel;
    //#endif

    Time::Timer naive_time;

    Tensor_4d result = naive_convolution(tensor, kernel);

    std::cout << "naive_conv time: " << naive_time.elapsed() << " milliseconds\n";

    #if 0
    std::cout << result; 
    #endif
}



int main()
{
    Conv_test();
}