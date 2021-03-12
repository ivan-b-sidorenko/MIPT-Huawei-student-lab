#include <iostream>
#include <chrono>
#include <thread>
#include <ratio>
#include <immintrin.h>
#include <assert.h>

namespace matrix{


class Matrix final
{
private:
	int num_col;
	int num_str;
	int** mrx;
public:
	Matrix(int col , int str);
	Matrix(int col , int str , int mod);
	Matrix(const Matrix& rhs);
	~Matrix();

	int get_num_col() const {return num_col;};
	int get_num_str() const {return num_str;};
	void set_zero();
	void cleanup();

	Matrix& operator=(const Matrix& rhs);
	bool operator==(const Matrix& rhs);
	int* operator[](const int rhs) const {return mrx[rhs];};
};

};