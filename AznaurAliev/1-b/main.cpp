#include "matrix.h"
#include "log_duration.h"
#include "mat_mult.h"

int main()
{
	matrix mat_1(300, 12 , true);
	matrix mat_2(12, 500, true);
	
	assert(Naive_mult(mat_1, mat_2) == Modified_mult(mat_1, mat_2));

	{
		// Simple Multiplication part
		LOG_DURATION("Simple Multiplication");
		matrix res = Naive_mult(mat_1, mat_2);
	}

	{	
		// Modified Multiplication part
		LOG_DURATION("Modified Multiplication");
		matrix res1 = Modified_mult(mat_1, mat_2);
	}
	return 0;
}
