//#include "matr_product.hpp"
#include "../optimized_algo/matr_product.hpp"
#include "my_time.hpp"

using namespace Linear_space;
using namespace Mul_optim;

#define OUT std::cout


template <typename Data>
void Buffer_init(Data& buffer, uint& num_of_rws, uint& num_of_clmns)
{
    #if  __INPUT__
    std::cout << "Input size of you matrix.\nRows X Cols\n";
    #endif

    std::cin >> num_of_rws >> num_of_clmns;

    buffer.reserve(num_of_rws * num_of_clmns);

    #if __INPUT__
    std::cout << "Input data for matrix in buffer:\n";
    #endif

    for (size_t i = 0; i < num_of_rws*num_of_clmns; ++i)
        std::cin >> buffer[i];
}


void Mul_testing(func_info& func_struct, Mul_optim::Matr_int& naive_mul, Mul_optim::Matr_int& lhs, 
                                                                         Mul_optim::Matr_int& rhs, 
                                                                         Mul_optim::Matr_int result)
{
    #if __TIMER__
    Time::Timer mul_time;
    #endif

    func_struct.func(lhs, rhs, result);
    
    #if __TIMER__
    std::cout << std::endl << func_struct.func_name << " time:" << mul_time.elapsed() << " millisecs\n";
    #endif

    #if __MATR_OUT__

    std::cout << "res matrix:\n";
    OUT << result << "              <================= result\n";

    #endif

    #if __CORRECT__
    if (naive_mul == result)
        std::cout << "Mul is right!\n";

    else
        std::cout << "Something went wrong!\n";
    #endif
}


int main()
{
    uint num_of_rws1 = 0, num_of_clmns1 = 0, num_of_rws2 = 0, num_of_clmns2 = 0;
    std::vector<int> buffer_lhs, buffer_rhs;

    Buffer_init(buffer_lhs, num_of_rws1, num_of_clmns1);
    Buffer_init(buffer_rhs, num_of_rws2, num_of_clmns2);

    Mul_optim::Matr_int lhs{num_of_rws1, num_of_clmns1, buffer_lhs};
    Mul_optim::Matr_int rhs{num_of_rws2, num_of_clmns2, buffer_rhs};

    Mul_optim::Matr_int result{num_of_rws1, num_of_clmns2, 0};

    Mul_optim::Matr_int naive_mul_{num_of_rws1, num_of_clmns2, 0};


    std::cout << "Program have started! Please, wait!\n";
    
    #if __TIMER__
    Time::Timer naive_mul_time;
    #endif

    naive_mul(lhs, rhs
    , naive_mul_);

    #if __TIMER__
    std::cout << std::endl << "naive_mul time:" << naive_mul_time.elapsed() << " millisecs\n";
    #endif

    #if __MATR_OUT__

    std::cout << "res matrix:\n";
    OUT << naive_mul_ << "              <================= result\n";

    #endif


    for (auto&& func: Mul_optim::functions)
        Mul_testing(func, naive_mul_, lhs, rhs, result);
    

    return 0;

}
