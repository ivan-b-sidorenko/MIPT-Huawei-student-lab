using namespace matrix;

void Matrix_product(const Matrix& lhs , const Matrix& rhs , Matrix& result)
{
	result.set_zero();
	int lhs_col = lhs.get_num_col();
	int lhs_str = lhs.get_num_str();
	int rhs_col = rhs.get_num_col();
	int rhs_str = rhs.get_num_str();

	assert(lhs_col == rhs_str);

	int sum = 0;

	for (int i = 0 ; i < lhs_str ; ++i)
	{
		for(int j = 0 ; j < rhs_col ; ++j)
		{
			for (int k = 0 ; k < lhs_col ; ++k)
				sum = sum + lhs[i][k] * rhs[k][j];
			result[i][j] = sum;
			sum = 0;
		};
	};
}

void Matrix_product_fast(const Matrix& lhs , const Matrix& rhs , Matrix& result)
{
	result.set_zero();
	int lhs_col = lhs.get_num_col();
	int lhs_str = lhs.get_num_str();
	int rhs_col = rhs.get_num_col();
	int rhs_str = rhs.get_num_str();

	assert(lhs_col == rhs_str);

	for (int i = 0 ; i < lhs_str ; ++i)
	{
		__m256i* res = (__m256i*)result[i];

		for (int j = 0 ; j < lhs_col ; ++j)
		{
			const __m256i* row_rhs = (__m256i*)rhs[j];
			int per_lhs = lhs[i][j];
			__m256i a = _mm256_set1_epi32(lhs[i][j]);

			for (int k = 0 ; k < rhs_col / 32 ; k += 1)
			{
				_mm256_storeu_si256(res + k * 4 + 0 , _mm256_add_epi32(_mm256_mullo_epi32(a , _mm256_loadu_si256(row_rhs + k * 4 + 0)) , _mm256_loadu_si256(res + k * 4 + 0)));
				_mm256_storeu_si256(res + k * 4 + 1 , _mm256_add_epi32(_mm256_mullo_epi32(a , _mm256_loadu_si256(row_rhs + k * 4 + 1)) , _mm256_loadu_si256(res + k * 4 + 1)));
				_mm256_storeu_si256(res + k * 4 + 2 , _mm256_add_epi32(_mm256_mullo_epi32(a , _mm256_loadu_si256(row_rhs + k * 4 + 2)) , _mm256_loadu_si256(res + k * 4 + 2)));
				_mm256_storeu_si256(res + k * 4 + 3 , _mm256_add_epi32(_mm256_mullo_epi32(a , _mm256_loadu_si256(row_rhs + k * 4 + 3)) , _mm256_loadu_si256(res + k * 4 + 3)));

			}

			for (int k = (rhs_col - rhs_col % 32) ; k < rhs_col ; ++k)
				result[i][k] += rhs[j][k] * per_lhs;
		}
	}
}