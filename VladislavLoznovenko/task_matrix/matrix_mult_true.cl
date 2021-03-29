__kernel void matrix_product_GPU(__global int* lhs , __global int* rhs , __global int* result , int lhs_col , int rhs_col)
{
	int i = get_global_id(0);
	for (int k = 0 ; k < rhs_col ; ++k)
		result[i * rhs_col + k] = 0;

	for (int k = 0 ; k < rhs_col ; ++k)
	{
		for (int j = 0 ; j < lhs_col / 8 ; j++)
		{
			result[i * rhs_col + k] += lhs[i * lhs_col + j * 8] * rhs[j * 8 * rhs_col + k];
			result[i * rhs_col + k] += lhs[i * lhs_col + j * 8 + 1] * rhs[(j * 8 + 1) * rhs_col + k];
			result[i * rhs_col + k] += lhs[i * lhs_col + j * 8 + 2] * rhs[(j * 8 + 2) * rhs_col + k];
			result[i * rhs_col + k] += lhs[i * lhs_col + j * 8 + 3] * rhs[(j * 8 + 3) * rhs_col + k];
			result[i * rhs_col + k] += lhs[i * lhs_col + j * 8 + 4] * rhs[(j * 8 + 4) * rhs_col + k];
			result[i * rhs_col + k] += lhs[i * lhs_col + j * 8 + 5] * rhs[(j * 8 + 5) * rhs_col + k];
			result[i * rhs_col + k] += lhs[i * lhs_col + j * 8 + 6] * rhs[(j * 8 + 6) * rhs_col + k];
			result[i * rhs_col + k] += lhs[i * lhs_col + j * 8 + 7] * rhs[(j * 8 + 7) * rhs_col + k];
		}
		for (int j = (lhs_col - lhs_col % 8) ; j < lhs_col ; ++j)
			result[i * rhs_col + k] += lhs[i * lhs_col + j] * rhs[j * rhs_col + k];
	}
}