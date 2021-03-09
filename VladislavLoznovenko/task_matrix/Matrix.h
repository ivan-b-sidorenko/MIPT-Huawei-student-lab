#include <iostream>
#include <chrono>
#include <thread>
#include <ratio>
#include <immintrin.h>
#include <assert.h>

namespace matrix{


template <typename T>
class Matrix final
{
private:
	int num_col;
	int num_str;
	T** mrx;
public:
	Matrix<T>(int col , int str);
	Matrix<T>(int col , int str , int mod);
	Matrix<T>(const Matrix& rhs);
	template <typename U>
	Matrix<T>(const Matrix<U>& rhs);
	~Matrix();

	int get_num_col() const {return num_col;};
	int get_num_str() const {return num_str;};
	void cleanup();

	Matrix<T>& operator=(const Matrix<T>& rhs);
	bool operator==(const Matrix<T>& rhs);
	T* operator[](const int rhs) const {return mrx[rhs];};
};

template <typename T>
Matrix<T>::Matrix(int str , int col)
{
	num_col = col;
	num_str = str;

	mrx = new T*[str];

	for (int i = 0 ; i < str ; ++i)
		mrx[i] = new T[col];
}

template <typename T>
Matrix<T>::Matrix(int str , int col , int mod)
{
	num_str = str;
	num_col = col;

	mrx = new T*[num_str];

	for (int i = 0 ; i < num_str ; ++i)
		mrx[i] = new T[col];

	for (int i = 0 ; i < num_str ; ++i)
		for (int j = 0 ; j < num_col ; ++j)
			mrx[i][j] = rand() % 10;
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& rhs)
{
	num_str = rhs.get_num_str();
	num_col = rhs.get_num_col();

	mrx = new T*[num_str];

	for (int i = 0 ; i < num_str ; ++i)
		mrx[i] = new T[num_str];

	for (int i = 0 ; i < num_str ; i++)
		for (int j = 0 ; j < num_col ; j++)
			mrx[i][j] = rhs[i][j];
}

template <typename T>
template <typename U>
Matrix<T>::Matrix(const Matrix<U>& rhs)
{
	num_str = rhs.get_num_str();
	num_col = rhs.get_num_col();
	mrx = new T* [num_str];

	for (int i = 0 ; i < num_str ; ++i)
	{
		mrx[i] = new T [num_col];
		for (int j = 0 ; j < num_col ; ++j)
			mrx[i][j] = rhs[i][j];
	};
}

template <typename T>
Matrix<T>::~Matrix()
{
	cleanup();
}

template <typename T>
void Matrix<T>::cleanup()
{
	for (int i = 0 ; i < num_str ; ++i)
		delete[] mrx[i];
	delete[] mrx;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs)
{
	if (*this == rhs)
		return *this;

	if ((rhs.get_num_str() != num_str) || (rhs.get_num_col() != num_col))
	{
		cleanup();
		mrx = new T*[rhs.get_num_str()];

		for (int i = 0 ; i < rhs.get_num_col() ; ++i)
			mrx[i] = new T[rhs.get_num_str()];

		num_col = rhs.get_num_col();
		num_str = rhs.get_num_str();
	}

	for (int i = 0 ; i < rhs.get_num_str() ; i++)
		for (int j = 0 ; j < rhs.get_num_col() ; j++)
			mrx[i][j] = rhs.mrx[i][j];

	return *this;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& rhs)
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

template <typename T>
std::ostream& operator<<(std::ostream& out , const Matrix<T>& rhs)
{
	for (int i = 0 ; i < rhs.get_num_str() ; ++i)
	{
		for (int j = 0 ; j < rhs.get_num_col() ; ++j)
			out << rhs[i][j] << " ";
		out <<"\n";
	}

	return out;
}

template <typename T>
std::istream& operator>>(std::istream& in , Matrix<T>& rhs)
{
	for (int i = 0 ; i < rhs.get_num_str() ; ++i)
	{
		for (int j = 0 ; j < rhs.get_num_col() ; ++j)
			in >> rhs[i][j];
	}

	return in;
}

};