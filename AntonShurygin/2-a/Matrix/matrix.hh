//
// Created by anon on 10.11.2020.
//
#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <fstream>
#include <ctime>
#include <immintrin.h>
#include <cstdlib>


namespace Linear_space
{
 
    enum Mode
    {
        RANDOM
    };

    class Matrix final
    {
        private:

            //! Num of rows and columns of my matrix
            uint rows = 0, clmns = 0;

            //! Perfomance of my matrix
            float** matrix = nullptr;

            //! using for special data type of my matrix
            using floatIt = typename std::vector<float>::iterator;

            //! Constant for copmaring
            const int EPSILON = 1e-6;

            struct Row_struct
            {
                uint cols = 0;
                float *matr_row = nullptr;

                Row_struct(uint cols, float *row) : cols(cols),
                                                  matr_row(row)
                {}

                const float& operator [](size_t i) const
                {
                    assert(i < cols);
                    return matr_row[i];
                }

                float& operator [](size_t i)
                {
                    assert(i < cols);
                    return matr_row[i];
                }

                Row_struct(const Row_struct &row_m) = default;

                Row_struct& operator =(const Row_struct &row_m) = default;
            };


        public:

            //! Constructors for class Matrix


            //! Constructor for matrix of zeros
            Matrix(uint rows_, uint clmns_) : rows(rows_),
                                            clmns(clmns_)
            {
                assert(rows_*clmns_ != 0);

                matrix = new float* [rows];
                for (size_t i = 0; i < rows; ++i)
                {
                    matrix[i] = new float [clmns];

                    for (size_t j = 0; j < clmns; ++j)
                        matrix[i][j] = 0;
                }
            }


            //! Constructor for matrix class of value
            Matrix(uint rows_, uint clmns_, float val) : rows(rows_),
                                                                 clmns(clmns_)
            {
                assert(rows_ * clmns_ != 0);
                matrix = new float* [rows];

                for (size_t i = 0; i < rows; ++i)
                {
                    matrix[i] = new float[clmns];

                    for (size_t j = 0; j < clmns; ++j)
                        matrix[i][j] = val;
                }

            }

            Matrix(uint rows_, uint clmns_, enum Mode mode) : rows(rows_),
                                                                    clmns(clmns_)
            {
                assert(rows_ * clmns_ != 0);
                matrix = new float* [rows];

                for (size_t i = 0; i < rows; ++i)
                {
                    matrix[i] = new float[clmns];

                    for (size_t j = 0; j < clmns; ++j)
                    {
                        if (mode == Mode::RANDOM)
                            matrix[i][j] = static_cast<float>((rand() % 5)) / 2;
                    }
                }

            }

            //! Constructor for matrix class from buffer
            Matrix(uint rows_, uint clmns_, const std::vector<float>& buffer) : rows(rows_),
                                                                              clmns(clmns_)
            {
                assert(rows_ * clmns_ != 0);


                size_t num_of_elems = rows_*clmns_, i = 0;

                matrix = new float* [rows];

                for (i = 0; i < rows; ++i)
                    matrix[i] = new float[clmns];

                for (i = 0; i < num_of_elems; ++i)
                    matrix[i / clmns][i % clmns] = buffer[i];

            }

            void set_zero()
            {
                for (int i = 0 ; i < rows ; i++)
                    for (int j = 0 ; j < clmns ; j++)
                        matrix[i][j] = 0;
            }

            //! Constructor class matrix of two iterators to the vector
            Matrix(uint rows_, uint clmns_, const floatIt& beg, const floatIt& end) : rows(rows_),
                                                                                    clmns(clmns_)
            {
                assert(rows_ * clmns_ != 0);

                floatIt current = beg;
                int num_of_elems = rows*clmns, cnter = 0, i = 0;

                matrix = new float* [rows];

                for (i = 0; i < rows; ++i)
                    matrix[i] = new float[clmns];

                i = 0;

                for (floatIt cur = beg; i < num_of_elems; ++cur, ++i)
                    matrix[i / clmns][i % clmns] = *cur;

            }


            //! Function returns a matrix of the upper triangular type
            static Matrix U_matr(uint num, float elem)
            {
                Matrix matr{num, num};

                for (uint i = 0; i < num*num; ++i)
                {
                    if ((i % num) == 0)
                        for (int k = 0; k < (i / num); ++k)
                            i++;

                    uint rows_it = i / num;
                    uint clmns_it = i % num;

                    matr[rows_it][clmns_it] = elem;

                }

                return matr;
            }


            //! The function returns the identity matrix
            static Matrix eye(uint num)
            {
                Matrix matr(num, num);

                for (int i = 0; i < num; ++i)
                    matr.matrix[i][i] = 1;

                return matr;
            }

            //! Copy constructor for class Matrix
            Matrix(const Matrix& rhs) :      rows(rhs.nrows()),
                                             clmns(rhs.nclmns())
            {
                uint rhs_rows = rhs.nrows(), rhs_cols = rhs.nclmns();

                matrix = new float* [rhs_rows];

                for (size_t i = 0; i < rhs_rows; ++i)
                {
                    matrix[i] = new float [rhs_cols];

                    for (size_t j = 0; j < rhs_cols; ++j)
                        matrix[i][j] = rhs[i][j];
                }
            }

            Matrix(Matrix &&mtr) : rows( 0 ), clmns( 0 ), matrix( nullptr )
            {
                std::swap( rows, mtr.rows );
                std::swap( clmns, mtr.clmns );
                std::swap(this->matrix, mtr.matrix);
            }

            //! Destructor for matrix class
            ~Matrix()
            {
                for (size_t i = 0; i < rows; ++i)
                    delete[] matrix[i];

                delete[] matrix;
                matrix = nullptr;
            }

            //Getters of class matrix
            uint nrows() const
            {
                return rows;
            }

            uint nclmns() const
            {
                return clmns;
            }


            //! Reloading of operators for class matrix

            Matrix& operator =(const Matrix& rhs)
            {
                if (this == &rhs)
                    return *this;

                if (rows == rhs.rows && clmns == rhs.clmns)
                    Copy(*this, rhs);
                else
                {
                    Matrix tmp(rhs);
                    Swap(*this, tmp);
                }

                return *this;
            }


            void Copy(Matrix& dst, const Matrix& src)
            {
                assert(dst.rows == src.rows);
                assert(dst.clmns == src.clmns);

                for (size_t i = 0; i < dst.rows; ++i)
                    for (size_t j = 0; j < dst.clmns; ++j)

                        dst.matrix[i][j] = src.matrix[i][j];
            }

            void Swap(Matrix &lhs, Matrix &rhs)
            {
                std::swap(lhs.matrix, rhs.matrix);
                std::swap(lhs.clmns, rhs.clmns);
                std::swap(lhs.rows, rhs.rows);
            }


        /*
            Matrix& operator =(const Matrix& rhs)
            {
                //if (rows != rhs.rows || clmns != rhs.clmns)
                  //  Resize(rhs.rows, rhs.clmns);

                rows = rhs.rows;
                clmns = rhs.clmns;

                for (size_t i = 0; i < rows; ++i)

                    for (size_t j = 0; j < clmns; ++j)
                        matrix[i][j] = rhs.matrix[i][j];

                return (*this);
            }*/

            Matrix operator -() const
            {
                Matrix res_mtr{(*this)};

                for (int i = 0; i < rows; ++i)

                    for (int j = 0; j < clmns; ++j)
                        res_mtr.matrix[i][j] *= -1;

                return res_mtr;
            }


            Matrix& operator +=(const Matrix& mtr)
            {
                assert((rows == mtr.rows) && (clmns == mtr.clmns));

                rows = mtr.rows;
                clmns = mtr.clmns;

                for (size_t i = 0; i < rows; ++i)

                    for (size_t j = 0; j < clmns; ++j)
                        matrix[i][j] += mtr.matrix[i][j];

                return (*this);
            }

            Matrix& operator -=(const Matrix& mtr)
            {
                assert((rows == mtr.rows) && (clmns == mtr.clmns));

                rows = mtr.rows;
                clmns = mtr.clmns;

                for (size_t i = 0; i < rows; ++i)

                    for (size_t j = 0; j < clmns; ++j)
                        matrix[i][j] -= mtr.matrix[i][j];

                return (*this);
            }

            Matrix& operator *=(const Matrix& mtr)
            {
                assert(clmns == mtr.rows);

                Matrix tmp_mtr{rows, mtr.clmns, 0};

                for (uint i = 0; i < rows; ++i)

                    for (uint j = 0; j < mtr.clmns; ++j)

                        for (uint k = 0; k < mtr.rows; ++k)
                            tmp_mtr[i][j] += matrix[i][k] * mtr.matrix[k][j];

                *this = tmp_mtr;
                return (*this);
            }

            Matrix& operator *=(float value)
            {
                assert((((*this).rows) * ((*this).clmns)) != 0);

                for (int i = 0; i < rows; ++i)

                    for (int j = 0; j < clmns; ++j)
                        matrix[i][j] *= value;

                return (*this);
            }

            Matrix& operator /=(float value)
            {
                for (int i = 0; i < rows; ++i)

                    for (int j = 0; j < clmns; ++j)
                        matrix[i][j] /= value;

                return (*this);
            }


            bool operator ==(const Matrix& mtr)
            {
                if (rows != mtr.rows && clmns != mtr.clmns)
                    return false;

                for (size_t i = 0; i < rows; ++i)
                    for (size_t j = 0; j < clmns; ++j)

                        if (std::abs(mtr.matrix[i][j] - matrix[i][j]) > EPSILON)
                            return false;

                return true;
            }


            float* operator [](size_t i) const
            {
                return matrix[i];
            }

            /*
            Row_struct operator [](size_t i) const
            {
                assert(i < rows);
                return Row_struct{clmns, matrix[i]};
            }

            Row_struct operator [](size_t i)
            {
                assert(i < rows);
                return Row_struct{clmns, matrix[i]};
            }
            */


    private:
            //! Function for resing matrix

            /*
            void Resize(uint rows_, uint clmns_)
            {
                assert(rows_ * clmns_ != 0);

                for (size_t i = 0; i < rows; ++i)
                    delete[] matrix[i];

                delete[] matrix;

                matrix = new float* [rows_];

                for (size_t i = 0; i < rows_; ++i)
                    matrix[i] = new float[clmns_];
            }
             */

    public:

            //! Function for mul diagonal elements
            float Diag_mul(const Matrix& mtr)
            {
                assert(mtr.rows == mtr.clmns);
                assert(mtr.matrix);
                float res = 1;

                for (size_t i = 0; i < mtr.rows; ++i)
                    res *= static_cast<double>(mtr.matrix[i][i]);

                return res;
            }


            //! Function - realisation Gauss algorithm
            void Gauss_algo(Matrix& mtr, int* swap_counter, bool* is_zero)
            {
                for (size_t i = 0; i < mtr.clmns; ++i)
                {
                    size_t max_ind = i;
                    *is_zero = true;

                    //! Null det processing
                    if (mtr.matrix[i][i] != 0)
                        *is_zero = false;
                    else
                    {
                        //! Finding row with the biggest first elem
                        for (size_t j = i + 1; j < mtr.rows; ++j)
                        {
                            if (mtr.matrix[j][i] != 0)
                            {
                                *is_zero = false;

                                if (abs(mtr.matrix[j][i]) > abs(mtr.matrix[max_ind][i]))
                                {
                                    max_ind = j;
                                    *swap_counter = -(*swap_counter);
                                    std::swap(mtr.matrix[i], mtr.matrix[max_ind]);
                                }
                            }
                        }
                    }

                    //! Null det processing
                    if ((*is_zero))
                        return;

                    //! Swap the lines [i] and [max_ind]

                    for (size_t j = i + 1; j < mtr.rows; ++j)
                    {
                        double coeff = ( static_cast<double>(mtr.matrix[j][i]) ) / ( static_cast<double>(mtr.matrix[i][i]) );

                        for (size_t k = 0; k < mtr.clmns; k++)
                            mtr[j][k] -= coeff * static_cast<double>(mtr[i][k]);
                    }
                }
            }


            //! Function for trannsposing matrix
            void transposition()
            {
                Matrix tmp_mtr{(*this)};
                //Resize(clmns, rows);

                for (int i = 0; i < clmns; ++i)

                    for(int j = 0; j < rows; ++j)
                        matrix[i][j] = tmp_mtr[j][i];
            }


            //! Function for calculating determinant
            double determ()
            {
                assert(clmns == rows);
                assert(matrix);

                int swap_counter = 1;
                bool is_zero = false;

                Matrix tmp_mtr{(*this)};

                Gauss_algo(tmp_mtr, &swap_counter, &is_zero);

                if (is_zero)
                    return 0;

                float res = swap_counter*Diag_mul(tmp_mtr);

                return res;
            }


        //! Dump for matrix
            void dump(std::ostream& os) const
            {
                std::cout << std::endl;

                for (size_t i = 0; i < rows; ++i)
                {
                    os << "|| ";

                    for (size_t j = 0; j < clmns; ++j)
                        os << matrix[i][j] << " ";

                    os << "||" << std::endl;
                }
                std::cout << "Num of rows = " << rows << std::endl;
                std::cout << "Num of clmns = " << clmns << std::endl;

                std::cout << std::endl;
            }

            //! Functions for working with matrix rows

            //! Function for swapping matrix rows
            void swap_rows(uint i, uint j)
            {
                assert(i >= 0 && j >= 0);
                assert(i < (*this).rows && j < (*this).rows);

                std::swap((*this).matrix[i], (*this).matrix[j]);
            }

            //! Function for add matrix rows
            void sum_rows(uint i, uint j)
            {
                assert(i >= 0 && j >= 0);
                assert(i < (*this).rows && j < (*this).rows);

                for (int k = 0; k < (*this).clmns; ++k)
                    (*this).matrix[k][i] += (*this).matrix[k][j];
            }

            //! Function for mul matrix rows
            void mul_rows_to_sclr(uint i, float value)
            {
                assert(i >= 0);
                assert(i < (*this).rows);

                for (int k = 0; k < (*this).clmns; ++k)
                    (*this).matrix[k][i] *= static_cast<double>(value);
            }
        
    };


    //! Also binary reloaded operators for working with matrix

    Matrix operator* (const Matrix& lhs, const Matrix& rhs);
    Matrix operator -(const Matrix& lhs, const Matrix& rhs);
    Matrix operator +(const Matrix& lhs, const Matrix& rhs);
    std::ostream& operator <<(std::ostream &os, Matrix& matr);
}




#endif //{MATRIX_MATRIX_H}
