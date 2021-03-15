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
	const int block = 6;
	const int reg = 8;

	if (lhs.num_col < 5){
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
        	for (int i = 0; i < lhs.num_str; i++){
			
			__m256i* _cache = (__m256i*)(result1[i]);

                	for (int j = 0; j < lhs.num_col; j++){

				const __m256i* _rhs = (__m256i*)(rhs[j]);
				const __m256i temp = _mm256_set1_epi32(lhs[i][j]);

				for (int k = 0; k < rhs.num_col / 8; k++){
								
					__m256i mull = _mm256_mullo_epi32(temp , _mm256_loadu_si256(&_rhs[k]));	
					_mm256_storeu_si256(&_cache[k], _mm256_add_epi32( mull, _mm256_loadu_si256(&_cache[k])));					
					/*cache[k] = cache[k] + res[k] * temp
					 *
					__m256i integer_vector = _mm256_loadu_si256(&cache[k]);
					__m256i integer_row = _mm256_loadu_si256(&_rhs[k]);
					__m256i mull = _mm256_mullo_epi32(temp, integer_row);
					__m256i sum = _mm256_add_epi32(integer_vector, mull);
					_mm256_storeu_si256(&cache[k], sum); 
					*/
				}
			

				for (int t = rhs.num_col - rhs.num_col % 8; t < rhs.num_col; t++){
					result1[i][t] += rhs[j][t] * lhs[i][j];
				}
			}
		}
	}


}

