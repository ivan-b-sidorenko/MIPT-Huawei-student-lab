#include <iostream>
#include <vector>
#include <assert.h>
#include <immintrin.h>

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
	Matrix(const Matrix& rhs , int col , int str , int start_col , int start_str);
	Matrix(const Matrix& rhs);
	~Matrix();

	int get_num_col() const {return num_col;};
	int get_num_str() const {return num_str;};
	int find_max();
	void set_zero();
	void cleanup();

	Matrix& operator=(const Matrix& rhs);
	bool operator==(const Matrix& rhs);
	float* operator[](const int rhs) const {return mrx[rhs];};
	friend std::ostream& operator<<(std::ostream& out , const Matrix& rhs);
	friend std::istream& operator>>(std::istream& in , Matrix& rhs);
};

void sum(Matrix& A , Matrix& B , Matrix& res);
void Matrix_product(const Matrix& lhs , const Matrix& rhs , Matrix& result);
void Matrix_product_fast(const Matrix& lhs , const Matrix& rhs , Matrix& result);

};