#include <iostream>
#include <stdio.h>
#include <CL/cl.hpp>
#include <fstream>
#include "Matrix_lib/Matrix.h"

#define PROGRAM_FILE "matrix_mult_true.cl"
#define KERNEL_FUNC "matrix_product_GPU"

class matrix_GPU final
{
public:
	void matrix_GPU_product(matrix::Matrix& lhs , matrix::Matrix& rhs , matrix::Matrix& result);

private:
	cl::Device device_;
	cl::Context context_;
	cl::Program program_;
	cl::Kernel kernel_;

	void set_device();
	void build_program();
	void set_queue(int* lhs , int* rhs , int* result , int lhs_str , int lhs_col , int rhs_col);
};
