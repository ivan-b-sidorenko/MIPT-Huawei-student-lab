#include "matrix_mult.h"

void matrix_GPU::set_device()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	for (const auto& platform : platforms)
	{
		std::vector<cl::Device> platform_devices;
		platform.getDevices(CL_DEVICE_TYPE_ALL , &platform_devices);
		for (const auto& device : platform_devices)
			if(device.getInfo<CL_DEVICE_COMPILER_AVAILABLE>())
	    	{
	    		device_ = device;
	    		return;
	    	}
	}
}

void matrix_GPU::build_program()
{
	std::ifstream program_file(PROGRAM_FILE);
	std::string program_String(std::istreambuf_iterator<char>(program_file),(std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, std::make_pair(program_String.c_str(), program_String.length()+1));
	cl::Program program(context_, source);
	program.build();

	kernel_ = cl::Kernel(program , KERNEL_FUNC);
}

void matrix_GPU::set_queue(int* lhs , int* rhs , int* result , int lhs_str , int lhs_col , int rhs_col)
{	
	cl::Buffer lhs_buff(context_ , CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR , sizeof(int) * lhs_str * lhs_col , lhs , NULL);
	cl::Buffer rhs_buff(context_ , CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR , sizeof(int) * rhs_col * lhs_col , rhs , NULL);
	cl::Buffer res_buff(context_ , CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR , sizeof(int) * lhs_str * rhs_col , result , NULL);

	kernel_.setArg(0 , lhs_buff);
	kernel_.setArg(1 , rhs_buff);
	kernel_.setArg(2 , res_buff);
	kernel_.setArg(3 , lhs_col);
	kernel_.setArg(4 , rhs_col);

	cl::NDRange offset(0);
	cl::NDRange global_size(lhs_str);
	cl::NDRange local_size(1);

	cl::CommandQueue queue_(context_ , device_ , CL_QUEUE_PROFILING_ENABLE);
	queue_.enqueueNDRangeKernel(kernel_ , offset , global_size , local_size , NULL , NULL);
	queue_.enqueueReadBuffer(res_buff , CL_TRUE , 0 , sizeof(int) * lhs_str * rhs_col , result , NULL , NULL);
}

void matrix_GPU::matrix_GPU_product(matrix::Matrix& lhs , matrix::Matrix& rhs , matrix::Matrix& result)
{
	set_device();
	context_ = cl::Context(device_);
	build_program();

	int lhs_col = lhs.get_num_col();
	int lhs_str = lhs.get_num_str();
	int rhs_col = rhs.get_num_col();

	int* res = new int[rhs_col * lhs_str];

	int* lhs_per = new int[lhs_col * lhs_str];
	for (int i = 0 ; i < lhs_str ; ++i)
	{
		for (int j = 0 ; j < lhs_col ; ++j)
		{
			lhs_per[i * lhs_col + j] = lhs[i][j];
		}
	}

	int* rhs_per = new int[lhs_col * rhs_col];
	for (int i = 0 ; i < lhs_col ; ++i)
	{
		for (int j = 0 ; j < rhs_col ; ++j)
		{
			rhs_per[i * rhs_col + j] = rhs[i][j];
		}
	}

	set_queue(lhs_per , rhs_per , res , lhs_str , lhs_col , rhs_col);

	for (int i = 0 ; i < lhs_str ; ++i)
	{
		for (int j = 0 ; j < rhs_col ; ++j)
			result[i][j] = res[i * rhs_col + j];
	}

	delete[] res;
	delete[] rhs_per;
	delete[] lhs_per;
}

