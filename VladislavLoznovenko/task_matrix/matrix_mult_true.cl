__kernel void matrix_product_GPU(__global int* lhs , __global int* rhs , __global int* result , __global int* lhs_col , __global int* rhs_col)
{
	int i = get_global_id(0);
	for (int k = 0 ; k < rhs_col[0] ; ++k)
		result[i * rhs_col[0] + k] = 0;

	for (int k = 0 ; k < rhs_col[0] ; ++k)
		for (int j = 0 ; j < lhs_col[0] ; ++j)
			result[i * rhs_col[0] + k] += lhs[i * lhs_col[0] + j] * rhs[j * lhs_col[0] + k];
}