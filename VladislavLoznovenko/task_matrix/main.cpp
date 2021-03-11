#include "Matrix.h"
#include "Matrix_products.h"

#define SIZE 400

using namespace matrix;

int main()
{
	uint64_t duration_normal = 0;
	uint64_t duration_fast = 0;
	Matrix<int> res_normal(SIZE , SIZE);
	Matrix<int> res_fast(SIZE , SIZE);

	for (int i = 0 ; i < 5 ; ++i)
	{
		Matrix<int> test_1(SIZE , SIZE , 0);
		Matrix<int> test_2(SIZE , SIZE , 0);

		auto t1 = std::chrono::high_resolution_clock::now();
		Matrix_product(test_1 , test_2 , res_normal);
		auto t2 = std::chrono::high_resolution_clock::now();
		duration_normal += std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1).count();

		auto t3 = std::chrono::high_resolution_clock::now();
		Matrix_product_fast(test_1 , test_2 , res_fast);
		auto t4 = std::chrono::high_resolution_clock::now();
		duration_fast += std::chrono::duration_cast<std::chrono::milliseconds> (t4 - t3).count();

		assert(res_normal == res_fast);
	}

	std::cout << "Normal product - " << duration_normal / 5 << " - milliseconds" << std::endl;
	std::cout << "Fast product   - " << duration_fast / 5 << " - milliseconds" << std::endl;

	return 0;
}

