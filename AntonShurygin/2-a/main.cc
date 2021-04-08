#include "4D_tensor/tensor.hh"

using namespace Linear_space;

void Channel_test_func()
{
    
    //Channel test{matr_vec, height, widht}; 

}

int main()
{
    //Channel_test_func();

    Matr_flt mtr1 = Matr_flt{5, 5, 1};
    Matr_flt mtr2 = Matr_flt{5, 5, 2};
    Matr_flt mtr3 = Matr_flt{5, 5, 3};

    unsigned int height = 5, widht = 5;

    // matr_vec; matr_vec.reserve(3);
    std::vector<Matr_flt> matr_vec = {mtr1, mtr2, mtr3};

    //Channel test{5, 5, 5};
    Channel test1{matr_vec, 5, 5};
    std::cout << test1;

    //std::cout << test;
}