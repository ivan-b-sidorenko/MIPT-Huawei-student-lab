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

	const int block = sizeof(__m256i)/sizeof(int);

	for (int i = 0; i < lhs.num_str; i++){
		
		__m256i* _cache = (__m256i*)(result1[i]);

		for (int j = 0; j < lhs.num_col; j++){

			const __m256i* _rhs = (__m256i*)(rhs[j]);
			const __m256i temp = _mm256_set1_epi32(lhs[i][j]);

			for (int k = 0; k < rhs.num_col / block; k++){
			
				__m256i integer_res = _mm256_loadu_si256(&_cache[k]);
				__m256i integer_rhs = _mm256_loadu_si256(&_rhs[k]);				
				_mm256_storeu_si256(&_cache[k], _mm256_add_epi32(_mm256_mullo_epi32(integer_rhs, temp), integer_res));					
			}
		

			for (int t = rhs.num_col - rhs.num_col % block; t < rhs.num_col; t++){
				result1[i][t] += rhs[j][t] * lhs[i][j];
			}
		}
	}

}

