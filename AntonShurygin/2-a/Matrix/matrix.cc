#include "matrix.hh"

namespace Linear_space
{

std::ostream& operator <<(std::ostream &os, Matrix& matr)
    {
        for (size_t i = 0; i < matr.nrows(); ++i)
        {
           // os << "|| ";

            for (size_t j = 0; j < matr.nclmns(); ++j)
                os << matr[i][j] << " ";

            os << std::endl;
        }

        return os;
    }

     
    Matrix operator +(const Matrix& lhs, const Matrix& rhs)
    {
        Matrix res{lhs};

        res += rhs;
        return res;
    }

     
    Matrix operator -(const Matrix& lhs, const Matrix& rhs)
    {
        Matrix res{lhs};

        res -= rhs;
        return res;
    }

     /*
    Matrix operator *(const Matrix& lhs, const Matrix& rhs)
    {
        Matrix res{lhs};

        res *= rhs;
        return res;
    }*/
}