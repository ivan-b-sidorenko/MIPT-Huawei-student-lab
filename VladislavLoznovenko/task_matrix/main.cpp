#include "Matrix_lib/Matrix.h"
#include "Matrix_products.h"

#define RES_STR 501
#define RES_COL 707
#define MID 401

using namespace matrix;

int main()
{
	uint64_t duration_normal = 0;
	uint64_t duration_fast = 0;

	Matrix res_normal(RES_STR , RES_COL);
	Matrix res_fast(RES_STR , RES_COL);

	Matrix test_1(RES_STR , MID , 0);
	Matrix test_2(MID , RES_COL , 0);

	for (int i = 0 ; i < 5 ; ++i)
	{

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

