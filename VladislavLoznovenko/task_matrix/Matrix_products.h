using namespace matrix;

template <typename T>
Matrix<T> Matrix_product(const Matrix<T>& lhs , const Matrix<T>& rhs)
{
	int lhs_col = lhs.get_num_col();
	int lhs_str = lhs.get_num_str();
	int rhs_col = rhs.get_num_col();
	int rhs_str = rhs.get_num_str();

	assert(lhs_col == rhs_str);

	T sum = 0;
	Matrix<T> result(lhs_str , rhs_col);

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

	return result;
}

template <typename T>
Matrix<T> Matrix_product_fast(const Matrix<T>& lhs , const Matrix<T>& rhs)
{
	int lhs_col = lhs.get_num_col();
	int lhs_str = lhs.get_num_str();
	int rhs_col = rhs.get_num_col();
	int rhs_str = rhs.get_num_str();

	assert(lhs_col == rhs_str);
	//Create float Matrixs beacuse all (immintrin.h)'s functions are working with float
	Matrix<float> lhs_per(lhs);
	Matrix<float> rhs_per(rhs);
	Matrix<float> result_per(lhs_str , rhs_col);

	for (int i = 0 ; i < lhs_str ; ++i)
	{
		float* res = result_per[i];
		for (int k = 0 ; k < rhs_col / 8 ; k += 1)
		{
			_mm256_storeu_ps(res + k * 8 + 0 , _mm256_setzero_ps());
		}

		for (int j = 0 ; j < lhs_col ; ++j)
		{
			const float* row_rhs = rhs_per[j];
			float per_lhs = lhs_per[i][j];
			__m256 a = _mm256_set1_ps(lhs_per[i][j]);

			for (int k = 0 ; k < rhs_col / 32 ; k += 1)
			{
				_mm256_storeu_ps(res + k * 32 + 0 , _mm256_add_ps(_mm256_mul_ps(a , _mm256_loadu_ps(row_rhs + k * 32 + 0)) , _mm256_loadu_ps(res + k * 32 + 0)));
			    _mm256_storeu_ps(res + k * 32 + 8 , _mm256_add_ps(_mm256_add_ps(a , _mm256_loadu_ps(row_rhs + k * 32 + 8)) , _mm256_loadu_ps(res + k * 32 + 8)));
			    _mm256_storeu_ps(res + k * 32 + 16 , _mm256_add_ps(_mm256_mul_ps(a , _mm256_loadu_ps(row_rhs + k * 32 + 16)) , _mm256_loadu_ps(res + k * 32 + 16)));
			    _mm256_storeu_ps(res + k * 32 + 24 , _mm256_add_ps(_mm256_mul_ps(a , _mm256_loadu_ps(row_rhs + k * 32 + 24)) , _mm256_loadu_ps(res + k * 32 + 24)));
			}

			for (int k = (rhs_col - rhs_col % 32) ; k < rhs_col ; ++k)
				res[k] += row_rhs[k] * per_lhs;
		}
	}

	Matrix<T> result(result_per);

	return result;
}