#include <iostream>
#include <stdio.h>
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#include "Matrix_lib/Matrix.h"

#define PROGRAM_FILE "matrix_mult_true.cl"
#define KERNEL_FUNC "matrix_product_GPU"

class matrix_GPU final
{
public:
	void matrix_GPU_product(matrix::Matrix& lhs , matrix::Matrix& rhs , matrix::Matrix& result);

private:
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;

	void set_device();
	void build_program();
	void set_queue(int* lhs , int* rhs , int* result , int lhs_str , int lhs_col , int rhs_col);
};
