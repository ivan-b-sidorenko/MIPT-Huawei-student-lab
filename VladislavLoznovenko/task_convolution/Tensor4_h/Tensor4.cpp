#include "Tensor4.h"

Tensor4::Tensor4(int num_b , int num_m , int num_h , int num_w)
{
	num_butch = num_b;
	num_mrx = num_m;
	high = num_h;
	width = num_w;

	for (int i = 0 ; i < num_butch ; ++i)
	{
		butches.push_back(Chanel(num_mrx , high , width));
	}
}

Tensor4::Tensor4(int num_b , int num_m , int num_h , int num_w , int mod)
{
	num_butch = num_b;
	num_mrx = num_m;
	high = num_h;
	width = num_w;

	for (int i = 0 ; i < num_butch ; ++i)
	{
		butches.push_back(Chanel(num_m , num_h , num_w , 0));
	}
}

Tensor4::Tensor4(const Tensor4& rhs)
{
	high = rhs.get_high();
	width = rhs.get_width();
	num_mrx = rhs.get_num_mrx();
	num_butch = rhs.get_num_butch();

	if (butches.capacity() != 0)
	{
		butches.clear();
		butches.shrink_to_fit();
	}

	for (int i = 0 ; i < num_butch ; ++i)
		butches.push_back(rhs.butches[i]);
}

void Tensor4::set_zero()
{
	for (int i = 0 ; i < num_butch ; ++i)
		butches[i].set_zero();
}

bool Tensor4::operator==(const Tensor4& rhs)
{
	if ((high != rhs.get_high()) || (width != rhs.get_width()) || (num_mrx != rhs.get_num_mrx()) || (num_butch != rhs.get_num_butch()))
		return false;
	else
	{
		for (int i = 0 ; i < num_butch ; ++i)
		{
			if (butches[i] == rhs.butches[i])
				continue;
			else
				return false;
		}
	}

	return true;
}

Tensor4& Tensor4::operator=(const Tensor4& rhs)
{	
	if (*this == rhs)
		return *this;

	high = rhs.get_high();
	width = rhs.get_width();
	num_mrx = rhs.get_num_mrx();
	num_butch = rhs.get_num_butch();

	if (butches.capacity() != 0)
	{
		butches.clear();
		butches.shrink_to_fit();
	}

	for (int i = 0 ; i < num_butch ; ++i)
	{
		butches.push_back(rhs.butches[i]);
	}

	return *this;
}

std::ostream& operator<<(std::ostream& out , Tensor4& rhs)
{
	int num = rhs.get_num_butch();
	for (int i = 0 ; i < num ; ++i)
	{
		std::cout << "butch - " << i + 1 << std::endl;
		out << rhs.butches[i];
	}

	return out;
}