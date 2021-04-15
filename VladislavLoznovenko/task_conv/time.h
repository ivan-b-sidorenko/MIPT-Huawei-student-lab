#include <chrono>
#include <thread>
#include <ratio>

namespace my_time
{

class Timer final
{
private:
	std::chrono::high_resolution_clock::time_point time;

public:
	Timer() : time(std::chrono::high_resolution_clock::now()) {};

	void restart() {time = std::chrono::high_resolution_clock::now();};

	auto get_mls()
	{
		auto end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds> (end - time).count();
	};
};

};