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
}


#endif // __MUL_TH_HH_