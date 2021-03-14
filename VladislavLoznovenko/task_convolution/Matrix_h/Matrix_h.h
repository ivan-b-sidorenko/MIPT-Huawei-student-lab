#include <iostream>
#include <vector>

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
	void set_zero();
	void cleanup();

	Matrix& operator=(const Matrix& rhs);
	bool operator==(const Matrix& rhs);
	float* operator[](const int rhs) const {return mrx[rhs];};
	friend std::ostream& operator<<(std::ostream& out , const Matrix& rhs);
	friend std::istream& operator>>(std::istream& in , Matrix& rhs);
};

};