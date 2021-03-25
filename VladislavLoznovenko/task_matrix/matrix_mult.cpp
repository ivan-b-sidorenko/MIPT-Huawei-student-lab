#include "matrix_mult.h"

void matrix_GPU::set_device()
{
	cl_platform_id platform;
	clGetPlatformIDs(1 , &platform , NULL);
	clGetDeviceIDs(platform , CL_DEVICE_TYPE_GPU , 1 , &device , NULL);
	context = clCreateContext(NULL , 1 , &device , NULL , NULL , NULL);
}

void matrix_GPU::build_program()
{
	FILE *program_handel;
	if ((program_handel = fopen(PROGRAM_FILE, "r")) == NULL)
		std::cout << "fopen error" << std::endl;

    fseek(program_handel, 0, SEEK_END);
	size_t program_size = ftell(program_handel);
	rewind(program_handel);
	char* program_buffer = (char*)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer , sizeof(char) , program_size , program_handel);
	fclose(program_handel);
	program = clCreateProgramWithSource(context , 1 , (const char**)&program_buffer , &program_size , NULL);
	free(program_buffer);
	clBuildProgram(program , 0 , NULL , NULL , NULL , NULL);
}

void matrix_GPU::set_queue(int* lhs , int* rhs , int* result , int lhs_str , int lhs_col , int rhs_col)
{
	kernel = clCreateKernel(program, KERNEL_FUNC, NULL);
    queue = clCreateCommandQueue(context, device, 0, NULL);
    cl_mem mat_lhs_buff , mat_rhs_buff , res_buff , lhs_col_buff , rhs_col_buff;
    mat_lhs_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * lhs_str * lhs_col , lhs, NULL);
    mat_rhs_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * lhs_col * rhs_col , rhs, NULL);
    lhs_col_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) , &lhs_col, NULL);
    rhs_col_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) , &rhs_col, NULL);
	res_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * lhs_str * rhs_col, NULL, NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &mat_lhs_buff);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &mat_rhs_buff);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &res_buff);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &(lhs_col_buff));
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &(rhs_col_buff));

    size_t work_units_per_kernel = lhs_str;
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &work_units_per_kernel, NULL, 0, NULL, NULL);
    clEnqueueReadBuffer(queue, res_buff, CL_TRUE, 0, sizeof(int) * lhs_str * rhs_col, result, 0, NULL, NULL);

    clReleaseMemObject(mat_lhs_buff);
    clReleaseMemObject(mat_rhs_buff);
    clReleaseMemObject(lhs_col_buff);
    clReleaseMemObject(rhs_col_buff);
    clReleaseMemObject(res_buff);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);
}

void matrix_GPU::matrix_GPU_product(matrix::Matrix& lhs , matrix::Matrix& rhs , matrix::Matrix& result)
{
	set_device();
	build_program();

	int lhs_col = lhs.get_num_col();
	int lhs_str = lhs.get_num_str();
	int rhs_col = rhs.get_num_col();

	int* res = new int[rhs_col * lhs_col];

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

