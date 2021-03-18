#include "matrix.h" 

matrix::matrix(int str , int col)
{
        num_str = str;
        num_col = col;

        this->ptr = new int*[num_str]();

        for (int i = 0; i < num_str; ++i){
                this->ptr[i] = new int[col]();
	}

}


matrix::matrix(int str , int col, bool fl)
{
	num_str = str;
	num_col = col;

	ptr = new int*[num_str];

	for (int i = 0 ; i < num_str ; ++i)
		ptr[i] = new int[col];

	for (int i = 0 ; i < num_str ; ++i)
		for (int j = 0 ; j < num_col ; ++j)
			ptr[i][j] = rand()%10;
}

matrix::matrix(const matrix& rhs)
{

	ptr = new int*[num_str];

	for (int i = 0 ; i < rhs.num_str ; ++i)
		ptr[i] = new int[num_str];

	for (int i = 0 ; i < rhs.num_str ; i++)
		for (int j = 0 ; j <rhs.num_col ; j++)
			ptr[i][j] = rhs.ptr[i][j];
}

matrix::~matrix()
{
	for (int i = 0; i < num_str; i++)
                delete[] ptr[i];

        delete[] ptr;

}

bool matrix::operator==(const matrix& rhs)
{
	if ((this->num_col != rhs.num_col) || (this->num_str != rhs.num_str))
	{
		std::cout << "dimensions of objects are not equal";
		return false;
	}
	else
	{
		for (int i = 0; i < num_str; i++)
			for (int j = 0; j < num_col; j++)
				if (ptr[i][j] != rhs.ptr[i][j])
					return false;
	}
	return true;
}

std::ostream& operator<<(std::ostream& out, const matrix& rhs)
{
	for (int i = 0; i < rhs.num_str; i++)
	{
		for (int j = 0; j < rhs.num_col; j++)
			out << rhs[i][j] << " ";
		out << "\n";
	}

	return out;
}

