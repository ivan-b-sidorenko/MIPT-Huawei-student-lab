#include "Chanel_h.h"

namespace Tensor{

Chanel::Chanel(int num , int num_h , int num_w)
{
	high = num_h;
	width = num_w;
	num_mrx = num;

	for (int i = 0 ; i < num ; ++i)
	{
		chanels.push_back(matrix::Matrix(high , width));
	}
}

Chanel::Chanel(int num , int num_h , int num_w , int mod)
{
	high = num_h;
	width = num_w;
	num_mrx = num;

	for (int i = 0 ; i < num ; ++i)
	{
		chanels.push_back(matrix::Matrix(num_h , num_w , 0));
	}
}

Chanel::Chanel(const Chanel& rhs)
{
	high = rhs.get_high();
	width = rhs.get_width();
	num_mrx = rhs.get_num_mrx();

	if (chanels.size() != 0)
		chanels.clear();

	for (int i = 0 ; i < num_mrx ; ++i)
		chanels.push_back(rhs.chanels[i]);
}

Chanel::Chanel(int start_h , int num_h , Chanel& rhs)
{
	high = num_h;
	width = rhs.get_width();
	num_mrx = rhs.get_num_mrx();

	for (int i = 0 ; i < num_mrx ; ++i)
		chanels.push_back(matrix::Matrix(rhs[i] , high , width , start_h , 0));
}

void Chanel::set_zero()
{
	for (auto it = chanels.begin() ; it != chanels.end() ; ++it)
	{
		it->set_zero();
	}
}

bool Chanel::operator==(const Chanel& rhs)
{
	if ((num_mrx != rhs.get_num_mrx()) || (high != rhs.get_high()) || (width != rhs.get_width()))
		return false;
	for (int i = 0 ; i < num_mrx ; ++i)
	{
		if (chanels[i] == rhs.chanels[i])
			continue;
		else
			return false;
	}

	return true;
}

Chanel& Chanel::operator=(const Chanel& rhs)
{
	if (*this == rhs)
		return *this;

	high = rhs.get_high();
	width = rhs.get_width();
	num_mrx = rhs.get_num_mrx();

	chanels.clear();

	for (int i = 0 ; i < num_mrx ; ++i)
		chanels.push_back(rhs.chanels[i]);

	return *this;
}

std::ostream& operator<<(std::ostream& out , const Chanel& rhs)
{
	int num = rhs.get_num_mrx();
	for (int i = 0 ; i < num ; ++i)
	{
		std::cout << "Layer - " << i + 1 << std::endl;
		out << rhs.chanels[i];
	}

	return out;
}

};