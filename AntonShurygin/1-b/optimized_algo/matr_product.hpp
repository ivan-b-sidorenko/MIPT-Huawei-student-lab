#ifndef __MATR_PRODUCT_H__
#define __MATR_PRODUCT_H__

#include "../naive_algo/Matrix.hpp"


namespace Mul_optim
{
    using mul_func = void (*)(const Matr_int& lhs, const Matr_int& rhs, Matr_int& result);


    struct func_info
    {
        mul_func func;
        std::string func_name;
    };

    
    void naive_mul(const Matr_int &lhs, const Matr_int &rhs, Matr_int& result)
    {
        uint lhs_rows = lhs.nrows(), rhs_cols = rhs.nclmns(), lhs_cols = lhs.nclmns(),
             rhs_rows = rhs.nrows();

        assert(lhs_cols == rhs_rows);

        for (uint i = 0; i < lhs_rows; ++i)

            for (uint j = 0; j < rhs_cols; ++j)

                for (uint k = 0; k < rhs_rows; ++k)
                    result[i][j] += lhs[i][k] * rhs[k][j];
    }


    void mul_with_intrinsics(const Matr_int &lhs, const Matr_int &rhs, Matr_int& result)
    {
        uint lhs_rows = lhs.nrows();
        uint lhs_cols = lhs.nclmns();

        uint rhs_rows = rhs.nrows();
        uint rhs_cols = rhs.nclmns();

        uint optim_size = (rhs_cols - rhs_cols % 32);
        uint optim_rhs_col = rhs_cols / 32;

        assert(lhs_cols == rhs_rows);

        for (uint i = 0 ; i < lhs_rows ; ++i)
        {
            __m256i* res = (__m256i*)result[i];

            for (uint j = 0 ; j < lhs_cols ; ++j)
            {
                const __m256i* rhs_row = (__m256i*)rhs[j];
                __m256i lhs_elem = _mm256_set1_epi32(lhs[i][j]);

                int lhs_elem_ = lhs[i][j];
                const int* rhs_row_ = rhs[j];

                for (uint k = 0 ; k < optim_rhs_col ; k += 1)
                {
                    _mm256_storeu_si256(res + k * 4 + 0 , _mm256_add_epi32(_mm256_mullo_epi32(lhs_elem , _mm256_loadu_si256(rhs_row + k * 4 + 0)) , _mm256_loadu_si256(res + k * 4 + 0)));

                    _mm256_storeu_si256(res + k * 4 + 1 , _mm256_add_epi32(_mm256_mullo_epi32(lhs_elem , _mm256_loadu_si256(rhs_row + k * 4 + 1)) , _mm256_loadu_si256(res + k * 4 + 1)));

                    _mm256_storeu_si256(res + k * 4 + 2 , _mm256_add_epi32(_mm256_mullo_epi32(lhs_elem , _mm256_loadu_si256(rhs_row + k * 4 + 2)) , _mm256_loadu_si256(res + k * 4 + 2)));

                    _mm256_storeu_si256(res + k * 4 + 3 , _mm256_add_epi32(_mm256_mullo_epi32(lhs_elem , _mm256_loadu_si256(rhs_row + k * 4 + 3)) , _mm256_loadu_si256(res + k * 4 + 3)));

                }

                for (uint k = optim_size ; k < rhs_cols ; ++k)
                    result[i][k] += rhs_row_[k] * lhs_elem_;
            }
        }
    }



    /* LOOP UNORLLING */

    void mul_unroll_2x(const Matr_int &lhs, const Matr_int &rhs, Matr_int& result)
    {
        uint lhs_rows = lhs.nrows(), rhs_cols = rhs.nclmns(), lhs_cols = lhs.nclmns();
        uint unroll_size = lhs_cols - lhs_cols % 2;


        for (size_t i = 0; i < lhs_rows; ++i)
        {
            for (size_t j = 0; j < rhs_cols; ++j)
            {
                int* lhs_row = lhs[i];
                int* res = result[i];

                //! Common variable for cycle of mult
                size_t k = 0;
                for (; k < unroll_size; k += 2)
                {
                    res[j] += lhs_row[k] * rhs[k][j] + lhs_row[k + 1] * rhs[k + 1][j];
                }

                for (; k < lhs_cols; ++k)
                    res[j] += lhs_row[k] * rhs[k][j];

            }
        }
    }

    void mul_unroll_4x(const Matr_int &lhs, const Matr_int &rhs, Matr_int& result)
    {
        uint lhs_rows = lhs.nrows(), rhs_cols = rhs.nclmns(), lhs_cols = lhs.nclmns();
        uint unroll_size = lhs_cols - lhs_cols % 4;

        for (size_t i = 0; i < lhs_rows; ++i)
        {
            for (size_t j = 0; j < rhs_cols; ++j)
            {
                int* lhs_row = lhs[i];
                int* res = result[i];

                //! Common variable for cycle of mult
                size_t k = 0;
                for (; k < unroll_size; k += 4)
                {
                    result[i][j] += lhs_row[k] * rhs[k][j]
                                  + lhs_row[k + 1] * rhs[k + 1][j]
                                  + lhs_row[k + 2] * rhs[k + 2][j]
                                  + lhs_row[k + 3] * rhs[k + 3][j];
                }

                for (; k < lhs_cols; ++k)
                    res[j] += lhs_row[k] * rhs[k][j];

            }
        }
    }

    void mul_unroll_8x(const Matr_int &lhs, const Matr_int &rhs, Matr_int& result)
    {
        uint lhs_rows = lhs.nrows(), rhs_cols = rhs.nclmns(), lhs_cols = lhs.nclmns();
        uint unroll_size = lhs_cols - lhs_cols % 8;

        for (size_t i = 0; i < lhs_rows; ++i)
        {
            for (size_t j = 0; j < rhs_cols; ++j)
            {
                int* lhs_row = lhs[i];
                int* res = result[i];

                //! Common variable for cycle of mult
                size_t k = 0;
                for (; k < unroll_size; k += 8)
                {
                    res[j] +=         lhs_row[k] * rhs[k][j]
                                    + lhs_row[k + 1] * rhs[k + 1][j]
                                    + lhs_row[k + 2] * rhs[k + 2][j]
                                    + lhs_row[k + 3] * rhs[k + 3][j]
                                    + lhs_row[k + 4] * rhs[k + 4][j]
                                    + lhs_row[k + 5] * rhs[k + 5][j]
                                    + lhs_row[k + 6] * rhs[k + 6][j]
                                    + lhs_row[k + 7] * rhs[k + 7][j];
                }

                for (; k < lhs_cols; ++k)
                    res[j] += lhs_row[k] * rhs[k][j];

            }
        }
    }



    std::vector<func_info> functions = 
    {
        {mul_unroll_2x, "unrolling_2x"},
        {mul_unroll_4x, "unrolling_4x"},
        {mul_unroll_8x, "unrolling_8x"},
        {mul_with_intrinsics, "mul_with_intrinsics"},
    };

}

#endif __MATR_PRODUCT_H__