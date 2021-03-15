#include <vector>
//#include <thread>
//#include <typeinfo>
//#include <stdexcept>

#include "matrix.hh"

namespace MUL
{
    using namespace MX;

    const size_t THREADS_NUM = 2;


    template <typename DataT>
    Matrix<DataT> trivial( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

    template <typename DataT>
    Matrix<DataT> trivial_cycle2x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

    template <typename DataT>
    Matrix<DataT> trivial_cycle4x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

    template <typename DataT>
    Matrix<DataT> trivial_cycle8x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

    /*template <typename DataT>
    Matrix<DataT> trivial_threads( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );*/




    template <typename DataT>
    Matrix<DataT> transpose( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

    template <typename DataT>
    Matrix<DataT> transpose_cycle2x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

    template <typename DataT>
    Matrix<DataT> transpose_cycle4x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

    template <typename DataT>
    Matrix<DataT> transpose_cycle8x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );





    /**
     * 
     * REALISATIONS
     *
     */




    template <typename DataT>
    Matrix<DataT> trivial( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
    {
        uint lhs_r = lhs.rows(),
             lhs_c = lhs.cols(),
             rhs_c = rhs.cols();
        
        Matrix<DataT> tmp{lhs_r, rhs_c};

        for (uint i = 0; i < lhs_r; ++i)
            for (uint j = 0; j < rhs_c; ++j)
                for (uint k = 0; k < lhs_c; ++k)
                    tmp.set(i, j, tmp[i][j] + lhs[i][k] * rhs[k][j]);

        return tmp;
    }

    template <typename DataT>
    Matrix<DataT> trivial_cycle2x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
    {
        uint lhs_r = lhs.rows(),
             lhs_c = lhs.cols(),
             rhs_c = rhs.cols();
        
        Matrix<DataT> tmp{lhs_r, rhs_c};

        for (uint i = 0; i < lhs_r; ++i)
            for (uint j = 0; j < rhs_c; ++j)
            {
                uint k = 0;
                for (uint end = lhs_c - 2; k < end; k += 2)
                    tmp.set(i, j, tmp[i][j] + lhs[i][k] * rhs[k][j]
                                            + lhs[i][k+1] * rhs[k+1][j]);
                while (k < lhs_c)
                {
                    tmp.set(i, j, tmp[i][j] + lhs[i][k] * rhs[k][j]);
                    ++k;
                }   
            }

        return tmp;
    }

    template <typename DataT>
    Matrix<DataT> trivial_cycle4x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
    {
        uint lhs_r = lhs.rows(),
             lhs_c = lhs.cols(),
             rhs_c = rhs.cols();
        
        Matrix<DataT> tmp{lhs_r, rhs_c};

        for (uint i = 0; i < lhs_r; ++i)
            for (uint j = 0; j < rhs_c; ++j)
            {
                uint k = 0;
                for (uint end = lhs_c - 4; k < end; k += 4)
                    tmp.set(i, j, tmp[i][j] + lhs[i][k] * rhs[k][j] +
                                              lhs[i][k+1] * rhs[k+1][j] +
                                              lhs[i][k+2] * rhs[k+2][j] +
                                              lhs[i][k+3] * rhs[k+3][j]);
                while (k < lhs_c)
                {
                    tmp.set(i, j, tmp[i][j] + lhs[i][k] * rhs[k][j]);
                    ++k;
                }   
            }

        return tmp;
    }

    template <typename DataT>
    Matrix<DataT> trivial_cycle8x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
    {
        uint lhs_r = lhs.rows(),
             lhs_c = lhs.cols(),
             rhs_c = rhs.cols();
        
        Matrix<DataT> tmp{lhs_r, rhs_c};

        for (uint i = 0; i < lhs_r; ++i)
            for (uint j = 0; j < rhs_c; ++j)
            {
                uint k = 0;
                for (uint end = lhs_c - 8; k < end; k += 8)
                    tmp.set(i, j, tmp[i][j] + lhs[i][k] * rhs[k][j] +
                                              lhs[i][k+1] * rhs[k+1][j] +
                                              lhs[i][k+2] * rhs[k+2][j] +
                                              lhs[i][k+3] * rhs[k+3][j] +
                                              lhs[i][k+4] * rhs[k+4][j] +
                                              lhs[i][k+5] * rhs[k+5][j] +
                                              lhs[i][k+6] * rhs[k+6][j] +
                                              lhs[i][k+7] * rhs[k+7][j]);
                while (k < lhs_c)
                {
                    tmp.set(i, j, tmp[i][j] + lhs[i][k] * rhs[k][j]);
                    ++k;
                }   
            }

        return tmp;
    }

    /*void triv_thread_sub( const Matrix<double> & lhs, 
                          const Matrix<double> & rhs,
                          Matrix<double> & tmp,
                          size_t lhs_beg, 
                          size_t lhs_end )
    {
        uint lhs_r = lhs.rows(),
             lhs_c = lhs.cols(),
             rhs_r = rhs.rows(),
             rhs_c = rhs.cols();

        for (uint i = lhs_beg; i < lhs_end && i < lhs_r; ++i)
            for (uint j = 0; j < rhs_c; ++j)
                for (uint k = 0; k < lhs_c; ++k)
                    tmp.set(i, j, tmp[i][j] + lhs[i][k] * rhs[k][j]);
    }


    Matrix<double> trivial_threads( const Matrix<double> & lhs, const Matrix<double> & rhs )
    {
        if (typeid(double) != typeid(double))
            throw std::runtime_error("this function only for double");

        std::vector<std::thread> threads;
        size_t NN = lhs.rows() / THREADS_NUM;

        Matrix<double> tmp{lhs.rows(), rhs.cols()};

        for (size_t i = 0; i < THREADS_NUM; ++i)
            threads.push_back(std::thread(triv_thread_sub, lhs, rhs, tmp, NN * i, NN * (i + 1)));

        for (std::thread & t : threads)
            t.join();

        return tmp;
    }*/











    template <typename DataT>
    Matrix<DataT> transpose( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
    {
        uint lhs_r = lhs.rows(),
             lhs_c = lhs.cols(),
             rhs_c = rhs.cols();

        Matrix<DataT> tmp1{lhs_r, rhs_c};
        Matrix<DataT> tmp2{transpose(rhs)};

        for (uint i = 0; i < lhs_r; ++i)
            for (uint j = 0; j < rhs_c; ++j)
                for (uint k = 0; k < lhs_c; ++k)
                    tmp1.set(i, j, tmp1[i][j] + lhs[i][k] * tmp2[j][k]);

        return tmp1;
    }

    template <typename DataT>
    Matrix<DataT> transpose_cycle2x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
    {
        uint lhs_r = lhs.rows(),
             lhs_c = lhs.cols(),
             rhs_c = rhs.cols();

        Matrix<DataT> tmp1{lhs_r, rhs_c};
        Matrix<DataT> tmp2{transpose(rhs)};

        for (uint i = 0; i < lhs_r; ++i)
            for (uint j = 0; j < rhs_c; ++j)
            {
                uint k = 0;
                for (uint end = lhs_c - 2; k < end; k += 2)
                    tmp1.set(i, j, tmp1[i][j] + lhs[i][k] * tmp2[j][k] + lhs[i][k+1] * tmp2[j][k+1]);
                while (k < lhs_c)
                {
                    tmp1.set(i, j, tmp1[i][j] + lhs[i][k] * tmp2[j][k]);
                    ++k;
                }   
            }

        return tmp1;
    }

    template <typename DataT>
    Matrix<DataT> transpose_cycle4x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
    {
        uint lhs_r = lhs.rows(),
             lhs_c = lhs.cols(),
             rhs_c = rhs.cols();

        Matrix<DataT> tmp1{lhs_r, rhs_c};
        Matrix<DataT> tmp2{transpose(rhs)};

        for (uint i = 0; i < lhs_r; ++i)
            for (uint j = 0; j < rhs_c; ++j)
            {
                uint k = 0;
                for (uint end = lhs_c - 4; k < end; k += 4)
                    tmp1.set(i, j, tmp1[i][j] + lhs[i][k] * tmp2[j][k] + 
                                                lhs[i][k+1] * tmp2[j][k+1] + 
                                                lhs[i][k+2] * tmp2[j][k+2] + 
                                                lhs[i][k+3] * tmp2[j][k+3]);
                while (k < lhs_c)
                {
                    tmp1.set(i, j, tmp1[i][j] + lhs[i][k] * tmp2[j][k]);
                    ++k;
                }   
            }

        return tmp1;
    }

    template <typename DataT>
    Matrix<DataT> transpose_cycle8x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
    {
        uint lhs_r = lhs.rows(),
             lhs_c = lhs.cols(),
             rhs_c = rhs.cols();

        Matrix<DataT> tmp1{lhs_r, rhs_c};
        Matrix<DataT> tmp2{transpose(rhs)};

        for (uint i = 0; i < lhs_r; ++i)
            for (uint j = 0; j < rhs_c; ++j)
            {
                uint k = 0;
                for (uint end = lhs_c - 8; k < end; k += 8)
                    tmp1.set(i, j, tmp1[i][j] + lhs[i][k] * tmp2[j][k] + 
                                                lhs[i][k+1] * tmp2[j][k+1] + 
                                                lhs[i][k+2] * tmp2[j][k+2] + 
                                                lhs[i][k+3] * tmp2[j][k+3] +
                                                lhs[i][k+4] * tmp2[j][k+4] + 
                                                lhs[i][k+5] * tmp2[j][k+5] + 
                                                lhs[i][k+6] * tmp2[j][k+6] + 
                                                lhs[i][k+7] * tmp2[j][k+7]);
                while (k < lhs_c)
                {
                    tmp1.set(i, j, tmp1[i][j] + lhs[i][k] * tmp2[j][k]);
                    ++k;
                }   
            }

        return tmp1;
    }



} // namespace MUL