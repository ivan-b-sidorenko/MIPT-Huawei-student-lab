#include "Matrix.h"
#include "Matrix_products.h"
#include <chrono>
#include <thread>

using namespace matrix;

int main()
{
	Matrix test_1(10 , 5 , 0);
	Matrix test_2(5 , 9 , 0);

	/*std::cout << test_1 << std::endl;
	std::cout << test_2 << std::endl;*/

	auto t1 = std::chrono::system_clock::now();
	Matrix res = Matrix_product(test_1 , test_2);
	auto t2 = std::chrono::system_clock::now();
	std::cout << "Normal product - " << (t2 - t1).count() << std::endl;

	t1 = std::chrono::system_clock::now();
	Matrix res_fast = Matrix_product_fast(test_1 , test_2);
	t2 = std::chrono::system_clock::now();
	std::cout << "Fast product - " << (t2 - t1).count() << std::endl;

	return 0;
}