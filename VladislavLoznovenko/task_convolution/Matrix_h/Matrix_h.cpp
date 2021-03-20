#include "Matrix_h.h"

namespace matrix{

Matrix::Matrix(int str , int col)
{
	num_col = col;
	num_str = str;

	mrx = new float*[str];

	for (int i = 0 ; i < str ; ++i)
		mrx[i] = new float[col];

	for (int i = 0 ; i < str ; ++i)
		for (int j = 0 ; j < col ; ++j)
			mrx[i][j] = 0;
}

Matrix::Matrix(int str , int col , int mod)
{
	num_str = str;
	num_col = col;

	mrx = new float*[num_str];

	for (int i = 0 ; i < num_str ; ++i)
		mrx[i] = new float[col];

	for (int i = 0 ; i < num_str ; ++i)
		for (int j = 0 ; j < num_col ; ++j)
			mrx[i][j] = rand() % 3;
}

Matrix::Matrix(const Matrix& rhs)
{
	num_str = rhs.get_num_str();
	num_col = rhs.get_num_col();

	mrx = new float*[num_str];

	for (int i = 0 ; i < num_str ; ++i)
		mrx[i] = new float[num_str];

	for (int i = 0 ; i < num_str ; i++)
		for (int j = 0 ; j < num_col ; j++)
			mrx[i][j] = rhs[i][j];
}

Matrix::Matrix(const Matrix& rhs , int col , int str , int start_col , int start_str)
{
	num_col = col;
	num_str = str;

	mrx = new float*[str];

	for (int i = 0 ; i < str ; ++i)
		mrx[i] = new float[col];

	for (int i = 0 ; i < str ; ++i)
		for (int j = 0 ; j < col ; ++j)
			mrx[i][j] = rhs[start_str + i][start_col + j];
}

void Matrix::set_zero()
{
	for (int i = 0 ; i < num_str ; i++)
		for (int j = 0 ; j < num_col ; j++)
			mrx[i][j] = 0;
}

int Matrix::find_max()
{
	int max = mrx[0][0];
	for (int i = 0 ; i < num_str ; ++i)
		for (int j = 0 ; j < num_col ; ++j)
			if (max < mrx[i][j])
				max = mrx[i][j];

	return max;
}

void sum(Matrix& A , Matrix& B , Matrix& res)
{
	assert(A.get_num_str() == B.get_num_str());
	assert(A.get_num_col() == B.get_num_col());

	for (int i = 0 ; i < A.get_num_str() ; ++i)
		for (int j = 0 ; j < B.get_num_col() ; ++j)
			res[i][j] = A[i][j] + B[i][j];
}

Matrix::~Matrix()
{
	cleanup();
}

void Matrix::cleanup()
{
	for (int i = 0 ; i < num_str ; ++i)
		delete[] mrx[i];
	delete[] mrx;
}

Matrix& Matrix::operator=(const Matrix& rhs)
{
	if (*this == rhs)
		return *this;

	if ((rhs.get_num_str() != num_str) || (rhs.get_num_col() != num_col))
	{
		cleanup();
		mrx = new float*[rhs.get_num_str()];

		for (int i = 0 ; i < rhs.get_num_col() ; ++i)
			mrx[i] = new float[rhs.get_num_str()];

		num_col = rhs.get_num_col();
		num_str = rhs.get_num_str();
	}

	for (int i = 0 ; i < rhs.get_num_str() ; i++)
		for (int j = 0 ; j < rhs.get_num_col() ; j++)
			mrx[i][j] = rhs.mrx[i][j];

	return *this;
}

bool Matrix::operator==(const Matrix& rhs)
{
	if ((num_col != rhs.get_num_col()) || (num_str != rhs.get_num_str()))
	{
		return false;
	}
	else
	{
		for (int i = 0 ; i < num_str ; ++i)
			for (int j = 0 ; j < num_col ; ++j)
				if (mrx[i][j] != rhs.mrx[i][j])
					return false;
	}
	return true;
}

std::ostream& operator<<(std::ostream& out , const Matrix& rhs)
{
	for (int i = 0 ; i < rhs.get_num_str() ; ++i)
	{
		for (int j = 0 ; j < rhs.get_num_col() ; ++j)
			out << rhs[i][j] << " ";
		out <<"\n";
	}

	return out;
}

std::istream& operator>>(std::istream& in , Matrix& rhs)
{
	for (int i = 0 ; i < rhs.get_num_str() ; ++i)
	{
		for (int j = 0 ; j < rhs.get_num_col() ; ++j)
			in >> rhs[i][j];
	}

	return in;
}

void Matrix_product(const Matrix& lhs , const Matrix& rhs , Matrix& result)
{
	result.set_zero();
	int lhs_col = lhs.get_num_col();
	int lhs_str = lhs.get_num_str();
	int rhs_col = rhs.get_num_col();
	int rhs_str = rhs.get_num_str();

	assert(lhs_col == rhs_str);

	float sum = 0;

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
		float* res = result[i];
		for (int k = 0 ; k < rhs_col / 8 ; k += 1)
		{
			_mm256_storeu_ps(res + k * 8 + 0 , _mm256_setzero_ps());
		}

		for (int j = 0 ; j < lhs_col ; ++j)
		{
			const float* row_rhs = rhs[j];
			float per_lhs = lhs[i][j];
			__m256 a = _mm256_set1_ps(lhs[i][j]);

			for (int k = 0 ; k < rhs_col / 32 ; k += 1)
			{
				_mm256_storeu_ps(res + k * 32 + 0 , _mm256_fmadd_ps(a , _mm256_loadu_ps(row_rhs + k * 32 + 0) , _mm256_loadu_ps(res + k * 32 + 0)));
			    _mm256_storeu_ps(res + k * 32 + 8 , _mm256_fmadd_ps(a , _mm256_loadu_ps(row_rhs + k * 32 + 8) , _mm256_loadu_ps(res + k * 32 + 8)));
			    _mm256_storeu_ps(res + k * 32 + 16 , _mm256_fmadd_ps(a , _mm256_loadu_ps(row_rhs + k * 32 + 16) , _mm256_loadu_ps(res + k * 32 + 16)));
			    _mm256_storeu_ps(res + k * 32 + 24 , _mm256_fmadd_ps(a , _mm256_loadu_ps(row_rhs + k * 32 + 24) , _mm256_loadu_ps(res + k * 32 + 24)));
			}

			for (int k = (rhs_col - rhs_col % 32) ; k < rhs_col ; ++k)
				res[k] += row_rhs[k] * per_lhs;
		}
	}
}

};