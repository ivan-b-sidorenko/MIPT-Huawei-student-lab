#include "matrix.h"
#include "log_duration.h"
#include "mat_mult.h"

int main()
{
	matrix mat_1(300, 150, true);
	matrix mat_2(150, 500, true);

	matrix simple(300, 500);
	matrix modified(300, 500);
	

	{
		// Simple Multiplication part
		LOG_DURATION("Simple Multiplication");
		Naive_mult(mat_1, mat_2, simple);
	}

	{	
		// Modified Multiplication part
		LOG_DURATION("Modified Multiplication");
		Modified_mult(mat_1, mat_2, modified);
	}

	assert(simple == modified);

	return 0;
}
