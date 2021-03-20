#include "Convolutions_h/Conv_naiv.h"
#include <chrono>
#include <thread>
#include <ratio>

using namespace matrix;

int main()
{

	uint64_t duration_normal = 0;
	uint64_t duration_fast = 0;
	Tensor4 test(1 , 16 , 100 , 100 , 0);
	Chanel kernels(16 , 4 , 4 , 0);
	for (int i = 0 ; i < 5 ; ++i)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		Tensor4 res_1 = conv_main_algo(test , kernels , 4);
		auto t2 = std::chrono::high_resolution_clock::now();
		duration_normal += std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1).count();

		auto t3 = std::chrono::high_resolution_clock::now();
		Tensor4 res_2 = conv_main_algo_fast(test , kernels , 4);
		auto t4 = std::chrono::high_resolution_clock::now();
		duration_fast += std::chrono::duration_cast<std::chrono::milliseconds> (t4 - t3).count();

		assert(res_1 == res_2);
	}

	std::cout << "Normal conv - " << duration_normal << " - milliseconds" << std::endl;
	std::cout << "Fast conv   - " << duration_fast << " - milliseconds" << std::endl;

	/*std::cout << "test\n" << test << std::endl;
	std::cout << "kernels\n" << kernels << std::endl;*/
	/*std::cout << "res_1\n" << res_1 << std::endl;
	std::cout << "res_2\n" << res_2 << std::endl;*/
	return 0;
}