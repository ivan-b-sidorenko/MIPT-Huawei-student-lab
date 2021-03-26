#include "matrix_mult.h"
#include "Matrix_products.h"

#define RES_STR 1001
#define RES_COL 1002
#define MID 1003

using namespace matrix;

int main()
{
	uint64_t duration_normal = 0;
	uint64_t duration_fast = 0;
	uint64_t duration_GPU = 0;

	Matrix res_normal(RES_STR , RES_COL);
	Matrix res_fast(RES_STR , RES_COL);
	Matrix res_GPU(RES_STR , RES_COL);

	Matrix test_1(RES_STR , MID , 0);
	Matrix test_2(MID , RES_COL , 0);

	auto t1 = std::chrono::high_resolution_clock::now();
	Matrix_product(test_1 , test_2 , res_normal);
	auto t2 = std::chrono::high_resolution_clock::now();
	duration_normal += std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1).count();

	auto t3 = std::chrono::high_resolution_clock::now();
	Matrix_product_fast(test_1 , test_2 , res_fast);
	auto t4 = std::chrono::high_resolution_clock::now();
	duration_fast += std::chrono::duration_cast<std::chrono::milliseconds> (t4 - t3).count();

	matrix_GPU test;
	auto t5 = std::chrono::high_resolution_clock::now();
	test.matrix_GPU_product(test_1 , test_2 , res_GPU);
	auto t6 = std::chrono::high_resolution_clock::now();
	duration_GPU += std::chrono::duration_cast<std::chrono::milliseconds> (t6 - t5).count();

	assert(res_normal == res_fast);
	assert(res_normal == res_GPU);

	std::cout << "Normal product - " << duration_normal << " - milliseconds" << std::endl;
	std::cout << "Fast product   - " << duration_fast << " - milliseconds" << std::endl;
	std::cout << "GPU product   - " << duration_GPU << " - milliseconds" << std::endl;
	//std::cout << std::thread::hardware_concurrency() << std::endl;

	return 0;
}

