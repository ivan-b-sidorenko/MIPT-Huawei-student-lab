#include "Matrix.h"
#include "Matrix_products.h"

using namespace matrix;

int main()
{
	Matrix<int> test_1(400 , 400 , 0);
	Matrix<int> test_2(400 , 400 , 0);

	auto t1 = std::chrono::high_resolution_clock::now();
	Matrix<int> res = Matrix_product(test_1 , test_2);
	auto t2 = std::chrono::high_resolution_clock::now();
	uint64_t duration = std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1).count();
	std::cout << "Normal product - " << duration << " - milliseconds" << std::endl;

	auto t3 = std::chrono::high_resolution_clock::now();
	Matrix<int> res_fast = Matrix_product_fast(test_1 , test_2);
	auto t4 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds> (t4 - t3).count();
	std::cout << "Fast product   - " << duration << " - milliseconds" << std::endl;

	return 0;
}

