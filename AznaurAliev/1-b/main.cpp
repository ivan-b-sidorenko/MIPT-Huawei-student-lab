#include "matrix.h"
#include "log_duration.h"
#include "mat_mult.h"

void TEST();


int main()
{

	TEST();

	return 0;
}


void TEST(){

	{
		std::cout << "TEST for 50X30 and 30x50:\n\n";

		matrix mat_1(50, 30, true);
		matrix mat_2(30, 50, true);

		matrix simple(50, 50);
		matrix modified(50, 50);
	

		{
			// Simple Multiplication part
			LOG_DURATION("\tSimple Multiplication");
			Naive_mult(mat_1, mat_2, simple);
		}

		{		
			// Modified Multiplication part
			LOG_DURATION("\tModified Multiplication");
			Modified_mult(mat_1, mat_2, modified);
		}	

		assert(simple == modified);
		cout << "\n";
	}

	{
		std::cout << "TEST for 150X300 and 300x500:\n\n";

		matrix mat_1(150, 300, true);
		matrix mat_2(300, 500, true);

		matrix simple(150, 500);
		matrix modified(150, 500);
	

		{
			// Simple Multiplication part
			LOG_DURATION("\tSimple Multiplication");
			Naive_mult(mat_1, mat_2, simple);
		}

		{		
			// Modified Multiplication part
			LOG_DURATION("\tModified Multiplication");
			Modified_mult(mat_1, mat_2, modified);
		}	

		assert(simple == modified);
		cout << "\n";
	}

	{
		std::cout << "TEST for 1000X500 and 500x1500:\n\n";

		matrix mat_1(1000, 500, true);
		matrix mat_2(500, 1000, true);

		matrix simple(1000, 1500);
		matrix modified(1000, 1500);
	

		{
			// Simple Multiplication part
			LOG_DURATION("\tSimple Multiplication");
			Naive_mult(mat_1, mat_2, simple);
		}

		{		
			// Modified Multiplication part
			LOG_DURATION("\tModified Multiplication");
			Modified_mult(mat_1, mat_2, modified);
		}	

		assert(simple == modified);
		cout << "\n";	
	}


	{
		std::cout << "TEST for 2000X750 and 750x980:\n\n";

		matrix mat_1(2000, 750, true);
		matrix mat_2(750, 980, true);

		matrix simple(2000, 980);
		matrix modified(2000, 980);
	

		{
			// Simple Multiplication part
			LOG_DURATION("\tSimple Multiplication");
			Naive_mult(mat_1, mat_2, simple);
		}

		{		
			// Modified Multiplication part
			LOG_DURATION("\tModified Multiplication");
			Modified_mult(mat_1, mat_2, modified);
		}	

		assert(simple == modified);
		cout << "\n";	
	}

}
