using namespace matrix;

Matrix Matrix_product(const Matrix& lhs , const Matrix& rhs)
{
	int lhs_col = lhs.get_num_col();
	int lhs_str = lhs.get_num_str();
	int rhs_col = rhs.get_num_col();
	int rhs_str = rhs.get_num_str();

	assert(lhs_col == rhs_str);

	int sum = 0;
	Matrix result(lhs_str , rhs_col);

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

Matrix Matrix_product_fast(const Matrix& lhs , const Matrix& rhs)
{
	int lhs_col = lhs.get_num_col();
	int lhs_str = lhs.get_num_str();
	int rhs_col = rhs.get_num_col();
	int rhs_str = rhs.get_num_str();

	assert(lhs_col == rhs_str);

	int sum = 0;
	Matrix result(lhs_str , rhs_col);

	for (int i = 0 ; i < lhs_str ; ++i)
	{
		int* res = result[i];
		for (int j = 0 ; j < lhs_col ; ++j)
		{
			const int* row_rhs = rhs[j];
			int per_lhs = lhs[i][j];

			for (int k = 0 ; k < rhs_col ; ++k)
				res[k] += row_rhs[k] * per_lhs;
		}
	}
	return result;
}