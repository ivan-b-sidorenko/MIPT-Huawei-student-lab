#pragma once
#include "../Matrix_h/Matrix_h.h"

class Chanel final
{
private:
	int num_mrx;
	int high;
	int width;
	std::vector<matrix::Matrix> chanels;
public:
	Chanel(int num , int num_h , int num_w);
	Chanel(int num , int num_h , int num_w , int mod);
	Chanel(const Chanel& rhs);
	~Chanel() {};

	int get_num_mrx() const {return num_mrx;};
	int get_high() const {return high;};
	int get_width() const {return width;};
	void set_zero();

	Chanel& operator=(const Chanel& rhs);
	bool operator==(const Chanel& rhs);
	matrix::Matrix& operator[](int num) {return chanels[num];};
	friend std::ostream& operator<<(std::ostream& out , const Chanel& rhs);
};