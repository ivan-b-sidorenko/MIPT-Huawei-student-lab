#ifndef __MUL_HH_
#define __MUL_HH_

#include "matrix.hh"

namespace Mul
{
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

  Mat Mul_Naive_ptmp( const Mat &lhs, const Mat &rhs )
  {
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols();

    for (size_t i = 0; i < nrows; ++i)
      for (size_t j = 0; j < ncols; ++j)
      {
        auto lptr = lhs[i];
        for (size_t k = 0; k < com_sz; ++k)
          res[i][j] += lptr[k] * rhs[k][j];
      }
  

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

  Mat Mul_nai_Transpose_ptmp( const Mat &lhs, const Mat &rhs )
  {
    Mat rhs_t{rhs.Transposing()};

    Mat res{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols();

    for (size_t i = 0; i < nrows; ++i)
      for (size_t j = 0; j < ncols; ++j)
      {
        auto lptr = lhs[i];
        auto rptr = rhs_t[j];
        for (size_t k = 0; k < com_sz; ++k)
          res[i][j] += lptr[k] * rptr[k];
      }

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

  Mat Mul_prom2x_ptmp( const Mat &lhs, const Mat &rhs )
  {
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 2;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        auto lptr = lhs[i];
        size_t k = 0;
        for (; k < end_k; k += 2)
          res[i][j] += lptr[k] * rhs[k][j] + lptr[k + 1] * rhs[k + 1][j];

        for (; k < com_sz; ++k)
          res[i][j] += lptr[k] * rhs[k][j];
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
          res[i][j] += lhs[i][k] * rhs_t[j][k];
      }

    return res;
  }

  Mat Mul_prom2x_t_ptmp( const Mat &lhs, const Mat &rhs )
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
        auto lptr = lhs[i];
        auto rptr = rhs_t[j];
        size_t k = 0;
        for (; k < end_k; k += 2)
          res[i][j] += lptr[k] * rptr[k] + lptr[k + 1] * rptr[k + 1];

        for (; k < com_sz; ++k)
          res[i][j] += lptr[k] * rptr[k];
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

  Mat Mul_prom4x_ptmp( const Mat &lhs, const Mat &rhs )
  {
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 4;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        auto lptr = lhs[i];
        size_t k = 0;
        for (; k < end_k; k += 4)
          res[i][j] += lptr[k] * rhs[k][j] + lptr[k + 1] * rhs[k + 1][j] + 
                       lptr[k + 2] * rhs[k + 2][j] + lptr[k + 3] * rhs[k + 3][j];

        for (; k < com_sz; ++k)
          res[i][j] += lptr[k] * rhs[k][j];
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
          res[i][j] += lhs[i][k] * rhs_t[j][k];
      }

    return res;
  }

  Mat Mul_prom4x_t_ptmp( const Mat &lhs, const Mat &rhs )
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
        auto lptr = lhs[i];
        auto rptr = rhs_t[j];
        size_t k = 0;
        for (; k < end_k; k += 4)
          res[i][j] += lptr[k] * rptr[k] + lptr[k + 1] * rptr[k + 1] + 
                       lptr[k + 2] * rptr[k + 2] + lptr[k + 3] * rptr[k + 3];

        for (; k < com_sz; ++k)
          res[i][j] += lptr[k] * rptr[k];
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

  Mat Mul_prom8x_ptmp( const Mat &lhs, const Mat &rhs )
  {
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 8;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        auto lptr = lhs[i];
        size_t k = 0;
        for (; k < end_k; k += 8)
          res[i][j] += lptr[k] * rhs[k][j] + lptr[k + 1] * rhs[k + 1][j] + 
                       lptr[k + 2] * rhs[k + 2][j] + lptr[k + 3] * rhs[k + 3][j] + 
                       lptr[k + 4] * rhs[k + 4][j] + lptr[k + 5] * rhs[k + 5][j] + 
                       lptr[k + 6] * rhs[k + 6][j] + lptr[k + 7] * rhs[k + 7][j];

        for (; k < com_sz; ++k)
          res[i][j] += lptr[k] * rhs[k][j];
      }

    return res;
  }

  Mat Mul_prom8x_t_ptmp( const Mat &lhs, const Mat &rhs )
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
        auto lptr = lhs[i];
        auto rptr = rhs_t[j];
        size_t k = 0;
        for (; k < end_k; k += 8)
          res[i][j] += lptr[k] * rptr[k] + lptr[k + 1] * rptr[k + 1] + 
                       lptr[k + 2] * rptr[k + 2] + lptr[k + 3] * rptr[k + 3] + 
                       lptr[k + 4] * rptr[k + 4] + lptr[k + 5] * rptr[k + 5] + 
                       lptr[k + 6] * rptr[k + 6] + lptr[k + 7] * rptr[k + 7];

        for (; k < com_sz; ++k)
          res[i][j] += lptr[k] * rptr[k];
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
                       lhs[i][k + 2] * rhs_t[j][k + 2] + lhs[i][k + 3] * rhs_t[j][k + 3] + 
                       lhs[i][k + 4] * rhs_t[j][k + 4] + lhs[i][k + 5] * rhs_t[j][k + 5] + 
                       lhs[i][k + 6] * rhs_t[j][k + 6] + lhs[i][k + 7] * rhs_t[j][k + 7];

        for (; k < com_sz; ++k)
          res[i][j] += lhs[i][k] * rhs_t[j][k];
      }

    return res;
  }

  Mat Mul_prom16x( const Mat &lhs, const Mat &rhs )
  {
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 16;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        size_t k = 0;
        for (; k < end_k; k += 16)
          res[i][j] += lhs[i][k] * rhs[k][j] + lhs[i][k + 1] * rhs[k + 1][j] + 
                       lhs[i][k + 2] * rhs[k + 2][j] + lhs[i][k + 3] * rhs[k + 3][j] + 
                       lhs[i][k + 4] * rhs[k + 4][j] + lhs[i][k + 5] * rhs[k + 5][j] + 
                       lhs[i][k + 6] * rhs[k + 6][j] + lhs[i][k + 7] * rhs[k + 7][j] + 
                       lhs[i][k + 8] * rhs[k + 8][j] + lhs[i][k + 9] * rhs[k + 9][j] + 
                       lhs[i][k + 10] * rhs[k + 10][j] + lhs[i][k + 11] * rhs[k + 11][j] + 
                       lhs[i][k + 12] * rhs[k + 12][j] + lhs[i][k + 13] * rhs[k + 13][j] + 
                       lhs[i][k + 14] * rhs[k + 14][j] + lhs[i][k + 15] * rhs[k + 15][j];

        for (; k < com_sz; ++k)
          res[i][j] += lhs[i][k] * rhs[k][j];
      }

    return res;
  }

  Mat Mul_prom16x_ptmp( const Mat &lhs, const Mat &rhs )
  {
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 16;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        auto lptr = lhs[i];
        size_t k = 0;
        for (; k < end_k; k += 16)
          res[i][j] += lptr[k] * rhs[k][j] + lptr[k + 1] * rhs[k + 1][j] + 
                       lptr[k + 2] * rhs[k + 2][j] + lptr[k + 3] * rhs[k + 3][j] + 
                       lptr[k + 4] * rhs[k + 4][j] + lptr[k + 5] * rhs[k + 5][j] + 
                       lptr[k + 6] * rhs[k + 6][j] + lptr[k + 7] * rhs[k + 7][j] + 
                       lptr[k + 8] * rhs[k + 8][j] + lptr[k + 9] * rhs[k + 9][j] + 
                       lptr[k + 10] * rhs[k + 10][j] + lptr[k + 11] * rhs[k + 11][j] + 
                       lptr[k + 12] * rhs[k + 12][j] + lptr[k + 13] * rhs[k + 13][j] + 
                       lptr[k + 14] * rhs[k + 14][j] + lptr[k + 15] * rhs[k + 15][j];

        for (; k < com_sz; ++k)
          res[i][j] += lptr[k] * rhs[k][j];
      }

    return res;
  }

  Mat Mul_prom16x_t( const Mat &lhs, const Mat &rhs )
  {
    Mat rhs_t{rhs.Transposing()};
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 16;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        size_t k = 0;
        for (; k < end_k; k += 16)
          res[i][j] += lhs[i][k] * rhs_t[j][k] + lhs[i][k + 1] * rhs_t[j][k + 1] + 
                       lhs[i][k + 2] * rhs_t[j][k + 2] + lhs[i][k + 3] * rhs_t[j][k + 3] + 
                       lhs[i][k + 4] * rhs_t[j][k + 4] + lhs[i][k + 5] * rhs_t[j][k + 5] + 
                       lhs[i][k + 6] * rhs_t[j][k + 6] + lhs[i][k + 7] * rhs_t[j][k + 7] + 
                       lhs[i][k + 8] * rhs_t[j][k + 8] + lhs[i][k + 9] * rhs_t[j][k + 9] + 
                       lhs[i][k + 10] * rhs_t[j][k + 10] + lhs[i][k + 11] * rhs_t[j][k + 11] + 
                       lhs[i][k + 12] * rhs_t[j][k + 12] + lhs[i][k + 13] * rhs_t[j][k + 13] + 
                       lhs[i][k + 14] * rhs_t[j][k + 14] + lhs[i][k + 15] * rhs_t[j][k + 15];

        for (; k < com_sz; ++k)
          res[i][j] += lhs[i][k] * rhs_t[j][k];
      }

    return res;
  }

  Mat Mul_prom16x_t_ptmp( const Mat &lhs, const Mat &rhs )
  {
    Mat rhs_t{rhs.Transposing()};
    Mat res{lhs.getRows(), rhs.getCols()};

    size_t res_c = res.getCols(), 
           res_r = res.getRows(), 
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 16;

    
    for (size_t i = 0; i < res_r; ++i)
      for (size_t j = 0; j < res_c; ++j)
      {
        auto lptr = lhs[i];
        auto rptr = rhs_t[j];
        size_t k = 0;
        for (; k < end_k; k += 16)
          res[i][j] += lptr[k] * rptr[k] + lptr[k + 1] * rptr[k + 1] + 
                       lptr[k + 2] * rptr[k + 2] + lptr[k + 3] * rptr[k + 3] + 
                       lptr[k + 4] * rptr[k + 4] + lptr[k + 5] * rptr[k + 5] + 
                       lptr[k + 6] * rptr[k + 6] + lptr[k + 7] * rptr[k + 7] + 
                       lptr[k + 8] * rptr[k + 8] + lptr[k + 9] * rptr[k + 9] + 
                       lptr[k + 10] * rptr[k + 10] + lptr[k + 11] * rptr[k + 11] + 
                       lptr[k + 12] * rptr[k + 12] + lptr[k + 13] * rptr[k + 13] + 
                       lptr[k + 14] * rptr[k + 14] + lptr[k + 15] * rptr[k + 15];

        for (; k < com_sz; ++k)
          res[i][j] += lptr[k] * rptr[k];
      }

    return res;
  }

} // namespace Mul



#endif // __MUL_HH_
