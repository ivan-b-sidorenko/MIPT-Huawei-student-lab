#ifndef __CONV_H__
#define __CONV_H__

#include "Tensor.hh"

namespace Conv
{
  using linal::Tensor;
  using linal::Kernel;

  Tensor Conv( const Tensor &ten, const std::vector<Kernel> &kerns )
  {
    size_t conv_szs[3] = {ten.get_ch_size() - kerns[0].get_ch_amount() - 1, 
                          ten.get_width() - kerns[0].get_rows() - 1,
                          ten.get_height() - kerns[0].get_cols() - 1};

    linal::Tensor answ = {ten.get_bat_size(), conv_szs[0], conv_szs[1], conv_szs[2]};

    auto walker = [&ten, &kerns]( size_t num_ten, size_t i, size_t j, size_t k )
    {
      int res{};
      auto &rten = ten[num_ten];
      for (auto &&kern : kerns)
        for (size_t ii = 0, endii = kern.get_ch_amount(); ii < endii; ++ii)
          for (size_t jj = 0, endjj = kern.get_rows(); jj < endjj; ++jj)
            for (size_t kk = 0, endkk = kern.get_cols(); kk < endkk; ++kk)
              res += kern[ii][jj][kk] * rten[i + ii][j + jj][k + kk];

      return res;
    };

    answ.Walker(walker);

    return answ;
  }

}
#endif // __CONV_H__