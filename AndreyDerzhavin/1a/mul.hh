#ifndef __MUL_HH_
#define __MUL_HH_

#include "matrix.hh"

namespace Mul
{
  using type = int;
  using Mat = linal::Matrix<type>;
  

  Mat Mul_Naive( const Mat &lhs, const Mat &rhs )
  {
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols();

    for (size_t i = 0; i < nrows; ++i)
      for (size_t j = 0; j < ncols; ++j)
        for (size_t k = 0; k < com_sz; ++k)
          res[i][j] += lhs[i][k] * rhs[k][j];
  

    return res;
  }

  Mat Mul_nai_Transpose( const Mat &lhs, const Mat &rhs )
  {
    Mat rhs_t{rhs.Transposing()};

    Mat res{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols();

    for (size_t i = 0; i < nrows; ++i)
      for (size_t j = 0; j < ncols; ++j)
        for (size_t k = 0; k < com_sz; ++k)
          res[i][j] += lhs[i][k] * rhs_t[j][k];

    return res;
  }


  Mat Mul_prom2x( const Mat &lhs, const Mat &rhs )
  {
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 2;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        size_t k = 0;
        for (; k < end_k; k += 2)
          res[i][j] += lhs[i][k] * rhs[k][j] + lhs[i][k + 1] * rhs[k + 1][j];

        for (; k < com_sz; ++k)
          res[i][j] += lhs[i][k] * rhs[k][j];
      }

    return res;
  }

  Mat Mul_prom2x_t( const Mat &lhs, const Mat &rhs )
  {
    Mat rhs_t{rhs.Transposing()};
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 2;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        size_t k = 0;
        for (; k < end_k; k += 2)
          res[i][j] += lhs[i][k] * rhs_t[j][k] + lhs[i][k + 1] * rhs_t[j][k + 1];

        for (; k < com_sz; ++k)
          res[i][j] += lhs[i][k] * rhs[k][j];
      }

    return res;
  }

  Mat Mul_prom4x( const Mat &lhs, const Mat &rhs )
  {
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 4;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        size_t k = 0;
        for (; k < end_k; k += 4)
          res[i][j] += lhs[i][k] * rhs[k][j] + lhs[i][k + 1] * rhs[k + 1][j] + 
                       lhs[i][k + 2] * rhs[k + 2][j] + lhs[i][k + 3] * rhs[k + 3][j];

        for (; k < com_sz; ++k)
          res[i][j] += lhs[i][k] * rhs[k][j];
      }

    return res;
  }

  Mat Mul_prom4x_t( const Mat &lhs, const Mat &rhs )
  {
    Mat rhs_t{rhs.Transposing()};
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 4;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        size_t k = 0;
        for (; k < end_k; k += 4)
          res[i][j] += lhs[i][k] * rhs_t[j][k] + lhs[i][k + 1] * rhs_t[j][k + 1] + 
                       lhs[i][k + 2] * rhs_t[j][k + 2] + lhs[i][k + 3] * rhs_t[j][k + 3];

        for (; k < com_sz; ++k)
          res[i][j] += lhs[i][k] * rhs[k][j];
      }

    return res;
  }

  Mat Mul_prom8x( const Mat &lhs, const Mat &rhs )
  {
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 8;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        size_t k = 0;
        for (; k < end_k; k += 8)
          res[i][j] += lhs[i][k] * rhs[k][j] + lhs[i][k + 1] * rhs[k + 1][j] + 
                       lhs[i][k + 2] * rhs[k + 2][j] + lhs[i][k + 3] * rhs[k + 3][j] + 
                       lhs[i][k + 4] * rhs[k + 4][j] + lhs[i][k + 5] * rhs[k + 5][j] + 
                       lhs[i][k + 6] * rhs[k + 6][j] + lhs[i][k + 7] * rhs[k + 7][j];

        for (; k < com_sz; ++k)
          res[i][j] += lhs[i][k] * rhs[k][j];
      }

    return res;
  }

  Mat Mul_prom8x_t( const Mat &lhs, const Mat &rhs )
  {
    Mat rhs_t{rhs.Transposing()};
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 8;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        size_t k = 0;
        for (; k < end_k; k += 8)
          res[i][j] += lhs[i][k] * rhs_t[j][k] + lhs[i][k + 1] * rhs_t[j][k + 1] + 
                       lhs[i][k + 2] * rhs_t[j][k + 2] + lhs[i][k + 3] * rhs_t[j][k + 3];

        for (; k < com_sz; ++k)
          res[i][j] += lhs[i][k] * rhs[k][j];
      }

    return res;
  }

} // namespace Mul



#endif // __MUL_HH_
