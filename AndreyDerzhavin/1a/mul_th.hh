#ifndef __MUL_TH_HH_
#define __MUL_TH_HH_

#include <vector>
#include <thread>
#include <mutex>
#include "matrix.hh"

namespace Mul
{
  using std::vector;

  template <size_t threads_num>
  Mat Mul_th( const Mat &lhs, const Mat &rhs )
  {
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols();
    
    size_t th_block = nrows / threads_num + 1;

    vector<std::thread> ths;
    ths.resize(threads_num);

    auto th_fnc = [&]( size_t ti )
    {
      for (size_t i = ti * th_block; i < std::min((ti + 1) * th_block, nrows); ++i)
        for (size_t j = 0; j < ncols; ++j)
        {
          for (size_t k = 0; k < com_sz; ++k)
            res[i][j] += lhs[i][k] * rhs[k][j];
        }
    };

    for (size_t ti = 0; ti < threads_num; ++ti)
      ths[ti] = std::thread{th_fnc, ti};


    for (auto && th : ths)
      th.join();

    return res;
  }

  template <size_t threads_num>
  Mat Mul_th_prom2x( const Mat &lhs, const Mat &rhs )
  {
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 2,
           th_block = nrows / threads_num + 1;

    vector<std::thread> ths;
    ths.resize(threads_num);

    auto th_fnc = [&]( size_t ti )
    {
      for (size_t i = ti * th_block; i < std::min((ti + 1) * th_block, nrows); ++i)
        for (size_t j = 0; j < ncols; ++j)
        {
          size_t k = 0;
          for (; k < end_k; k += 2)
            res[i][j] += lhs[i][k] * rhs[k][j] + lhs[i][k + 1] * rhs[k + 1][j];

          for (; k < com_sz; ++k)
            res[i][j] += lhs[i][k] * rhs[k][j];
        }
    };

    for (size_t ti = 0; ti < threads_num; ++ti)
      ths[ti] = std::thread{th_fnc, ti};


    for (auto && th : ths)
      th.join();

    return res;
  }

  template <size_t threads_num>
  Mat Mul_th_prom4x( const Mat &lhs, const Mat &rhs )
  {
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 4,
           th_block = nrows / threads_num + 1;

    vector<std::thread> ths;
    ths.resize(threads_num);

    auto th_fnc = [&]( size_t ti )
    {
      for (size_t i = ti * th_block; i < std::min((ti + 1) * th_block, nrows); ++i)
        for (size_t j = 0; j < ncols; ++j)
        {
          size_t k = 0;
          for (; k < end_k; k += 4)
            res[i][j] += lhs[i][k] * rhs[k][j] + lhs[i][k + 1] * rhs[k + 1][j] + 
                         lhs[i][k + 2] * rhs[k + 2][j] + lhs[i][k + 3] * rhs[k + 3][j];

          for (; k < com_sz; ++k)
            res[i][j] += lhs[i][k] * rhs[k][j];
        }
    };

    for (size_t ti = 0; ti < threads_num; ++ti)
      ths[ti] = std::thread{th_fnc, ti};


    for (auto && th : ths)
      th.join();

    return res;
  }

  template <size_t threads_num>
  Mat Mul_th_t( const Mat &lhs, const Mat &rhs )
  {
    auto rhs_t = Mat{rhs.Transposing()};
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols();
    
    size_t th_block = nrows / threads_num + 1;

    vector<std::thread> ths;
    ths.resize(threads_num);

    auto th_fnc = [&]( size_t ti )
    {
      for (size_t i = ti * th_block; i < std::min((ti + 1) * th_block, nrows); ++i)
        for (size_t j = 0; j < ncols; ++j)
        {
          for (size_t k = 0; k < com_sz; ++k)
            res[i][j] += lhs[i][k] * rhs_t[j][k];
        }
    };

    for (size_t ti = 0; ti < threads_num; ++ti)
      ths[ti] = std::thread{th_fnc, ti};


    for (auto && th : ths)
      th.join();

    return res;
  }

  template <size_t threads_num>
  Mat Mul_th_t_prom2x( const Mat &lhs, const Mat &rhs )
  {
    auto rhs_t = Mat{rhs.Transposing()};
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 2,
           th_block = nrows / threads_num + 1;

    vector<std::thread> ths;
    ths.resize(threads_num);

    auto th_fnc = [&]( size_t ti )
    {
      for (size_t i = ti * th_block; i < std::min((ti + 1) * th_block, nrows); ++i)
        for (size_t j = 0; j < ncols; ++j)
        {
          size_t k = 0;
          for (; k < end_k; k += 2)
            res[i][j] += lhs[i][k] * rhs_t[j][k] + lhs[i][k + 1] * rhs_t[j][k + 1];

          for (; k < com_sz; ++k)
            res[i][j] += lhs[i][k] * rhs_t[j][k];
        }
    };

    for (size_t ti = 0; ti < threads_num; ++ti)
      ths[ti] = std::thread{th_fnc, ti};


    for (auto && th : ths)
      th.join();

    return res;
  }

  template <size_t threads_num>
  Mat Mul_th_t_prom4x( const Mat &lhs, const Mat &rhs )
  {
    auto rhs_t = Mat{rhs.Transposing()};
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 4,
           th_block = nrows / threads_num + 1;

    vector<std::thread> ths;
    ths.resize(threads_num);

    auto th_fnc = [&]( size_t ti )
    {
      for (size_t i = ti * th_block; i < std::min((ti + 1) * th_block, nrows); ++i)
        for (size_t j = 0; j < ncols; ++j)
        {
          size_t k = 0;
          for (; k < end_k; k += 4)
            res[i][j] += lhs[i][k] * rhs_t[j][k] + lhs[i][k + 1] * rhs_t[j][k + 1] + 
                         lhs[i][k + 2] * rhs_t[j][k + 2] + lhs[i][k + 3] * rhs_t[j][k + 3];

          for (; k < com_sz; ++k)
            res[i][j] += lhs[i][k] * rhs_t[j][k];
        }
    };

    for (size_t ti = 0; ti < threads_num; ++ti)
      ths[ti] = std::thread{th_fnc, ti};


    for (auto && th : ths)
      th.join();

    return res;
  }

  template <size_t threads_num>
  Mat Mul_th_t_ptmp( const Mat &lhs, const Mat &rhs )
  {
    auto rhs_t = Mat{rhs.Transposing()};
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols();
    
    size_t th_block = nrows / threads_num + 1;

    vector<std::thread> ths;
    ths.resize(threads_num);

    auto th_fnc = [&]( size_t ti )
    {
      for (size_t i = ti * th_block; i < std::min((ti + 1) * th_block, nrows); ++i)
        for (size_t j = 0; j < ncols; ++j)
        {
          auto lptr = lhs[i];
          auto rptr = rhs_t[j];
          for (size_t k = 0; k < com_sz; ++k)
            res[i][j] += lptr[k] * rptr[k];
        }
    };

    for (size_t ti = 0; ti < threads_num; ++ti)
      ths[ti] = std::thread{th_fnc, ti};


    for (auto && th : ths)
      th.join();

    return res;
  }

  template <size_t threads_num>
  Mat Mul_th_t_ptmp_prom2x( const Mat &lhs, const Mat &rhs )
  {
    auto rhs_t = Mat{rhs.Transposing()};
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 2,
           th_block = nrows / threads_num + 1;

    vector<std::thread> ths;
    ths.resize(threads_num);

    auto th_fnc = [&]( size_t ti )
    {
      for (size_t i = ti * th_block; i < std::min((ti + 1) * th_block, nrows); ++i)
        for (size_t j = 0; j < ncols; ++j)
        {
          auto lptr = lhs[i];
          auto rptr = rhs_t[j];
          size_t k = 0;
          for (; k < end_k; k += 2)
            res[i][j] += lptr[k] * rptr[k] + lptr[k + 1] * rptr[k + 1];

          for (; k < com_sz; ++k)
            res[i][j] += lptr[k] * rptr[k];
        }
    };

    for (size_t ti = 0; ti < threads_num; ++ti)
      ths[ti] = std::thread{th_fnc, ti};


    for (auto && th : ths)
      th.join();

    return res;
  }

  template <size_t threads_num>
  Mat Mul_th_t_ptmp_prom4x( const Mat &lhs, const Mat &rhs )
  {
    auto rhs_t = Mat{rhs.Transposing()};
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 4,
           th_block = nrows / threads_num + 1;

    vector<std::thread> ths;
    ths.resize(threads_num);

    auto th_fnc = [&]( size_t ti )
    {
      for (size_t i = ti * th_block; i < std::min((ti + 1) * th_block, nrows); ++i)
        for (size_t j = 0; j < ncols; ++j)
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
    };

    for (size_t ti = 0; ti < threads_num; ++ti)
      ths[ti] = std::thread{th_fnc, ti};


    for (auto && th : ths)
      th.join();

    return res;
  }

  template <size_t threads_num>
  Mat Mul_th_t_ptmp_prom8x( const Mat &lhs, const Mat &rhs )
  {
    auto rhs_t = Mat{rhs.Transposing()};
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 8,
           th_block = nrows / threads_num + 1;

    vector<std::thread> ths;
    ths.resize(threads_num);

    auto th_fnc = [&]( size_t ti )
    {
      for (size_t i = ti * th_block; i < std::min((ti + 1) * th_block, nrows); ++i)
        for (size_t j = 0; j < ncols; ++j)
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
    };

    for (size_t ti = 0; ti < threads_num; ++ti)
      ths[ti] = std::thread{th_fnc, ti};


    for (auto && th : ths)
      th.join();

    return res;
  }

  template <size_t threads_num>
  Mat Mul_th_t_ptmp_prom16x( const Mat &lhs, const Mat &rhs )
  {
    auto rhs_t = Mat{rhs.Transposing()};
    auto res = Mat{lhs.getRows(), rhs.getCols()};
    size_t nrows = res.getRows(),
           ncols = res.getCols(),
           com_sz = lhs.getCols(),
           end_k = com_sz - com_sz % 16,
           th_block = nrows / threads_num + 1;

    vector<std::thread> ths;
    ths.resize(threads_num);

    auto th_fnc = [&]( size_t ti )
    {
      for (size_t i = ti * th_block; i < std::min((ti + 1) * th_block, nrows); ++i)
        for (size_t j = 0; j < ncols; ++j)
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
    };

    for (size_t ti = 0; ti < threads_num; ++ti)
      ths[ti] = std::thread{th_fnc, ti};


    for (auto && th : ths)
      th.join();

    return res;
  }
}


#endif // __MUL_TH_HH_