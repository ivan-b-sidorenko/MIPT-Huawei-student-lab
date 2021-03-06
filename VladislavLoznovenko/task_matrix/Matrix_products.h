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

typedef int v4i __attribute__ ((vector_size (16)));

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
			//Что-то типо векторизации
			int per_lhs = lhs[i][j];
			//выравниваем адрес
			for (int k = 0 ; k < 2 ; ++k)
				res[k] += row_rhs[k] * per_lhs;

			//объединяем данные
			v4i* res_vec = (v4i*)(res);
			v4i* row_rhs_vec = (v4i*)(row_rhs);

			//складываем объединённые данные
			for (int k = 2 ; k < (rhs_col - rhs_col % 4) / 4 ; ++k)
			{
				res_vec[k] += row_rhs_vec[k] * per_lhs;
			}

			//складываем остатки
			for (int k = (rhs_col - rhs_col % 4) ; k < rhs_col ; ++k)
				res[k] += row_rhs[k] * per_lhs;
		}
	}
	return result;
}