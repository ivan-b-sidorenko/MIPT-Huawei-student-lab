#ifndef __CONV_H__
#define __CONV_H__

#include "Tensor.hh"

namespace Conv
{
  using linal::Tensor;
  using linal::Kernel;

  Tensor Naive( const Tensor &ten, const std::vector<Kernel> &kerns )
  {
    size_t conv_szs[3] = {ten.get_ch_size() - kerns[0].get_ch_amount() + 1, 
                          ten.get_width() - kerns[0].get_rows() + 1,
                          ten.get_height() - kerns[0].get_cols() + 1};

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

  Tensor Prom2x( const Tensor &ten, const std::vector<Kernel> &kerns )
  {
    size_t conv_szs[3] = {ten.get_ch_size() - kerns[0].get_ch_amount() + 1, 
                          ten.get_width() - kerns[0].get_rows() + 1,
                          ten.get_height() - kerns[0].get_cols() + 1};

    linal::Tensor answ = {ten.get_bat_size(), conv_szs[0], conv_szs[1], conv_szs[2]};

    auto walker = [&ten, &kerns]( size_t num_ten, size_t i, size_t j, size_t k )
    {
      int res{};
      size_t endk = kerns[0].get_cols();
      size_t end_all_k = endk - endk % 2;
      auto &rten = ten[num_ten];

      for (auto &&kern : kerns)
        for (size_t ii = 0, endii = kern.get_ch_amount(); ii < endii; ++ii)
          for (size_t jj = 0, endjj = kern.get_rows(); jj < endjj; ++jj)
          {
            size_t kk = 0;
            for (; kk < endk; kk += 2)
              res += kern[ii][jj][kk] * rten[i + ii][j + jj][k + kk] + 
                     kern[ii][jj][kk + 1] * rten[i + ii][j + jj][k + kk + 1];

            for (; kk < end_all_k; ++kk)
              res += kern[ii][jj][kk] * rten[i + ii][j + jj][k + kk];
          }

      return res;
    };

    answ.Walker(walker);

    return answ;
  }

  Tensor Prom4x( const Tensor &ten, const std::vector<Kernel> &kerns )
  {
    size_t conv_szs[3] = {ten.get_ch_size() - kerns[0].get_ch_amount() + 1, 
                          ten.get_width() - kerns[0].get_rows() + 1,
                          ten.get_height() - kerns[0].get_cols() + 1};

    linal::Tensor answ = {ten.get_bat_size(), conv_szs[0], conv_szs[1], conv_szs[2]};

    auto walker = [&ten, &kerns]( size_t num_ten, size_t i, size_t j, size_t k )
    {
      int res{};
      size_t endk = kerns[0].get_cols();
      size_t end_all_k = endk - endk % 4;
      auto &rten = ten[num_ten];

      for (auto &&kern : kerns)
        for (size_t ii = 0, endii = kern.get_ch_amount(); ii < endii; ++ii)
          for (size_t jj = 0, endjj = kern.get_rows(); jj < endjj; ++jj)
          {
            size_t kk = 0;
            for (; kk < endk; kk += 4)
              res += kern[ii][jj][kk] * rten[i + ii][j + jj][k + kk] + 
                     kern[ii][jj][kk + 1] * rten[i + ii][j + jj][k + kk + 1] + 
                     kern[ii][jj][kk + 2] * rten[i + ii][j + jj][k + kk + 2] + 
                     kern[ii][jj][kk + 3] * rten[i + ii][j + jj][k + kk + 3];

            for (; kk < end_all_k; ++kk)
              res += kern[ii][jj][kk] * rten[i + ii][j + jj][k + kk];
          }

      return res;
    };

    answ.Walker(walker);

    return answ;
  }

  Tensor Prom4x_tmp_var( const Tensor &ten, const std::vector<Kernel> &kerns )
  {
    size_t conv_szs[3] = {ten.get_ch_size() - kerns[0].get_ch_amount() + 1, 
                          ten.get_width() - kerns[0].get_rows() + 1,
                          ten.get_height() - kerns[0].get_cols() + 1};

    linal::Tensor answ = {ten.get_bat_size(), conv_szs[0], conv_szs[1], conv_szs[2]};

    auto walker = [&ten, &kerns]( size_t num_ten, size_t i, size_t j, size_t k )
    {
      int res{};
      size_t endk = kerns[0].get_cols();
      size_t end_all_k = endk - endk % 4;
      auto &rten = ten[num_ten];

      for (auto &&kern : kerns)
        for (size_t ii = 0, endii = kern.get_ch_amount(); ii < endii; ++ii)
        {
          const auto &keri = kern[ii];
          const auto &rteni = rten[i + ii];
          for (size_t jj = 0, endjj = kern.get_rows(); jj < endjj; ++jj)
          {
            auto *kerj = keri[jj];
            auto *rtenj = rteni[j + jj];
            size_t kk = 0;
            for (; kk < endk; kk += 4)
            {
              auto *kerk = kerj + kk;
              auto *rtenk = rtenj + k + kk;
              res += kerk[0] * rtenk[0] + 
                     kerk[1] * rtenk[1] + 
                     kerk[2] * rtenk[2] + 
                     kerk[3] * rtenk[3];
            }

            for (; kk < end_all_k; ++kk)
              res += kern[ii][jj][kk] * rten[i + ii][j + jj][k + kk];
          }
        }

      return res;
    };

    answ.Walker(walker);

    return answ;
  }

}
#endif // __CONV_H__