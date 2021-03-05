#include <iostream>
#include <assert.h>

namespace matrix{

class Matrix final
{
private:
	int num_col;
	int num_str;
	float** mrx;
public:
	Matrix(int col , int str);
	Matrix(int col , int str , int mod);
	Matrix(const Matrix& rhs);
	~Matrix();

	int get_num_col() const {return num_col;};
	int get_num_str() const {return num_str;};
	void cleanup();

	Matrix& operator=(const Matrix& rhs);
	bool operator==(const Matrix& rhs);
	float* operator[](const int rhs) const {return mrx[rhs];};
	friend std::ostream& operator<<(std::ostream& out , const Matrix& rhs);
	friend std::istream& operator>>(std::istream& in , Matrix& rhs);
};

Matrix::Matrix(int str , int col)
{
	num_col = col;
	num_str = str;

	mrx = new float*[str];

	for (int i = 0 ; i < str ; ++i)
		mrx[i] = new float[col];
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
			mrx[i][j] = (double)rand() / RAND_MAX * 10;
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
			mrx[i][j] = rhs.mrx[i][j];
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

};