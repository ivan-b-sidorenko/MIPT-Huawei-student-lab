__kernel void matrix_product_GPU(__global int* lhs , __global int* rhs , __global int* result , int lhs_col , int rhs_col)
{
	int i = get_global_id(0);
	for (int k = 0 ; k < rhs_col ; ++k)
		result[i * rhs_col + k] = 0;

	for (int k = 0 ; k < rhs_col ; ++k)
		for (int j = 0 ; j < lhs_col ; ++j)
			result[i * rhs_col + k] += lhs[i * lhs_col + j] * rhs[j * rhs_col + k];
}