#include "Tensor4_h/Tensor4.h"


int main()
{
	Tensor4 test_1(1 , 5 , 5 , 5);
	Tensor4 test_2(1 , 4 , 3 , 3 , 0);
	test_2 = test_1;
	Tensor4 test_3 = test_2;
	std::cout << test_1;
	std::cout << test_2;
	return 0;
}