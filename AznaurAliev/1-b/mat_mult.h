#pragma once

#include "matrix.h"

void Naive_mult(const matrix& lhs , const matrix& rhs, matrix& result)
{

	assert(lhs.num_col == rhs.num_str);

	for (int i = 0; i < lhs.num_str; i++)
	{
		for(int j = 0; j < rhs.num_col; j++)
		{
			for (int k = 0; k < lhs.num_col; k++)
				result[i][j] += lhs[i][k] * rhs[k][j];

		};
	};

}


void Modified_mult(const matrix& lhs , const matrix& rhs, matrix& result1)
{

        assert(lhs.num_col == rhs.num_str);

	int ost = lhs.num_col % 6;
	int block = 6;
	if (lhs.num_col < 5){
		int res[lhs.num_col];
		for (int i = 0; i < rhs.num_col; i++){
			int res[lhs.num_col];

			for (int t = 0; t < lhs.num_col; t++){
	
				res[t] = rhs[t][i];	
			}

    			for (int j = 0; j < lhs.num_str; j++){

				int cache = 0;
				const int* temp = lhs[j];

				for(int k1 = 0; k1 < lhs.num_col; k1++){
					cache += temp[k1]*res[k1];
				}
				result1[j][i] = cache;

			}
		}

	}

	else{
        	for (int i = 0; i < rhs.num_col; i++){
			
			int res[lhs.num_col];

			for (int t = 0; t < lhs.num_col; t++){
	
				res[t] = rhs[t][i];	
			}


                	for (int j = 0; j < lhs.num_str; j++){

				int cache = 0;
				const int* temp = lhs[j];
				int k = block - 1;

				for (; k < lhs.num_col; k += block){
					cache += temp[k] * res[k];
					cache += temp[k - 1] * res[k - 1];
					cache += temp[k - 2] * res[k - 2];
					cache += temp[k - 3] * res[k - 3];
					cache += temp[k - 4] * res[k - 4];
					cache += temp[k - 5] * res[k - 5];
				}		

				for(int t = k - block + 1; t < k + ost - block + 1; t++){
					cache += temp[t] * res[t]; // aligning
				}

				result1[j][i] = cache;
			}

                }
	 }

}
