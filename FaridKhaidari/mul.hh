#include "matrix.hh"

namespace MUL
{
    using namespace MX;

    template <typename DataT>
    Matrix<DataT> trivial( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
    {
        uint lhs_r = lhs.rows(),
             rhs_r = rhs.rows(),
             lhs_c = lhs.cols(),
             rhs_c = rhs.cols();
        
        Matrix<DataT> tmp{lhs_r, rhs_c};

        for (uint i = 0; i < lhs_r; ++i)
            for (uint j = 0; j < rhs_c; ++j)
                for (uint k = 0; k < lhs_c; ++k)
                    tmp.set(i, j, lhs[i][k] * rhs[k][j]);

        return tmp;
    }

    template <typename DataT>
    Matrix<DataT> transpose( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
    {
        uint lhs_r = lhs.rows(),
             rhs_r = rhs.rows(),
             lhs_c = lhs.cols(),
             rhs_c = rhs.cols();

        Matrix<DataT> tmp1{lhs_r, rhs_c};
        Matrix<DataT> tmp2{transpose(rhs)};

        for (uint i = 0; i < lhs_r; ++i)
            for (uint j = 0; j < rhs_c; ++j)
                for (uint k = 0; k < lhs_c; ++k)
                    tmp1.set(i, j, lhs[i][k] * rhs[k][j]);

        return tmp1;
    }

} // namespace MUL