#pragma once
#include "../Chanel_h/Chanel_h.h"

namespace Tensor{

class Tensor4 final
{
private:
	int num_butch;
	int num_mrx;
	int high;
	int width;
	std::vector<Chanel> butches;
public:
	Tensor4(int num_b , int num_m , int num_h , int num_w);
	Tensor4(int num_b , int num_m , int num_h , int num_w , int mod);
	Tensor4(int start_h , int num_h , Tensor4& rhs);
	Tensor4(const Tensor4& rhs);
	~Tensor4() {};

	int get_num_mrx() const {return num_mrx;};
	int get_high() const {return high;};
	int get_width() const {return width;};
	int get_num_butch() const {return num_butch;};
	void set_zero();

	Tensor4& operator=(const Tensor4& rhs);
	bool operator==(const Tensor4& rhs);
	Chanel& operator[](const int rhs) {return butches[rhs];};
	friend std::ostream& operator<<(std::ostream& out , Tensor4& rhs);
};

Tensor4 merge_ten(std::vector<Tensor4> rhs);
void add_ten(Tensor4& for_add , Tensor4& result , int counter);

};