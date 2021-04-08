#ifndef THRD_HH
#define THRD_HH

#include <vector>
#include <thread>

#include "matrix_light.hh"

namespace MUL
{
  using namespace MXL;

  template <typename DataT>
  Matrix<DataT> trivial_threads( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRIVIAL THREADS" << std::endl;

    size_t lhs_r = lhs.rows(),
       lhs_c = lhs.cols(),
       rhs_c = rhs.cols();

    auto THREADS_NUM = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    threads.reserve(THREADS_NUM);

    size_t NN = lhs_r / THREADS_NUM + 1;

    Matrix<DataT> tmp{lhs_r, rhs_c};

    auto f = [&](size_t c)
    {
    for (size_t m = c * NN, end = std::min((c + 1)  * NN, lhs_r); m < end; ++m)
    {
      for (size_t j = 0; j < rhs_c; ++j)
        for (size_t k = 0; k < lhs_c; ++k)
          tmp[m][j] += lhs[m][k] * rhs[k][j];
    }
    };

    for (size_t i = 0; i < THREADS_NUM; ++i)
    threads.push_back(std::thread(f, i));

    for (std::thread & t : threads)
    t.join();

    return tmp;
  }

  template <typename DataT>
  Matrix<DataT> trivial_threads2x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRIVIAL THREADS 2X" << std::endl;

    size_t lhs_r = lhs.rows(),
       lhs_c = lhs.cols(),
       rhs_c = rhs.cols();

    auto THREADS_NUM = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    threads.reserve(THREADS_NUM);

    size_t NN = lhs_r / THREADS_NUM + 1;

    Matrix<DataT> tmp{lhs_r, rhs_c};

    auto f = [&](size_t c)
    {
    for (size_t m = c * NN, end = std::min((c + 1)  * NN, lhs_r); m < end; ++m)
    {
      for (size_t j = 0; j < rhs_c; ++j)
      {
        size_t k = 0;
        for (size_t end = lhs_c - 2; k < end; k += 2)
          tmp[m][j] += lhs[m][k] * rhs[k][j]
                     + lhs[m][k+1] * rhs[k+1][j];
        
        while (k < lhs_c)
        {
          tmp[m][j] += lhs[m][k] * rhs[k][j];
          ++k;
        }
      }
    }
    };

    for (size_t i = 0; i < THREADS_NUM; ++i)
    threads.push_back(std::thread(f, i));

    for (std::thread & t : threads)
    t.join();

    return tmp;
  }

  template <typename DataT>
  Matrix<DataT> trivial_threads4x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRIVIAL THREADS 4X" << std::endl;

    size_t lhs_r = lhs.rows(),
       lhs_c = lhs.cols(),
       rhs_c = rhs.cols();

    auto THREADS_NUM = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    threads.reserve(THREADS_NUM);

    size_t NN = lhs_r / THREADS_NUM + 1;

    Matrix<DataT> tmp{lhs_r, rhs_c};

    auto f = [&](size_t c)
    {
    for (size_t m = c * NN, end = std::min((c + 1)  * NN, lhs_r); m < end; ++m)
    {
      for (size_t j = 0; j < rhs_c; ++j)
      {
        size_t k = 0;
        for (size_t end = lhs_c - 4; k < end; k += 4)
          tmp[m][j] += lhs[m][k] * rhs[k][j]
                     + lhs[m][k+1] * rhs[k+1][j]
                     + lhs[m][k+2] * rhs[k+2][j]
                     + lhs[m][k+3] * rhs[k+3][j];
        
        while (k < lhs_c)
        {
          tmp[m][j] += lhs[m][k] * rhs[k][j];
          ++k;
        }
      }
    }
    };

    for (size_t i = 0; i < THREADS_NUM; ++i)
    threads.push_back(std::thread(f, i));

    for (std::thread & t : threads)
    t.join();

    return tmp;
  }

  template <typename DataT>
  Matrix<DataT> trivial_threads8x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRIVIAL THREADS 8X" << std::endl;

    size_t lhs_r = lhs.rows(),
       lhs_c = lhs.cols(),
       rhs_c = rhs.cols();

    auto THREADS_NUM = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    threads.reserve(THREADS_NUM);

    size_t NN = lhs_r / THREADS_NUM + 1;

    Matrix<DataT> tmp{lhs_r, rhs_c};

    auto f = [&](size_t c)
    {
    for (size_t m = c * NN, end = std::min((c + 1)  * NN, lhs_r); m < end; ++m)
    {
      for (size_t j = 0; j < rhs_c; ++j)
      {
        size_t k = 0;
        for (size_t end = lhs_c - 8; k < end; k += 8)
          tmp[m][j] += lhs[m][k] * rhs[k][j]
                     + lhs[m][k+1] * rhs[k+1][j]
                     + lhs[m][k+2] * rhs[k+2][j]
                     + lhs[m][k+3] * rhs[k+3][j]
                     + lhs[m][k+4] * rhs[k+4][j]
                     + lhs[m][k+5] * rhs[k+5][j]
                     + lhs[m][k+6] * rhs[k+6][j]
                     + lhs[m][k+7] * rhs[k+7][j];
        
        while (k < lhs_c)
        {
          tmp[m][j] += lhs[m][k] * rhs[k][j];
          ++k;
        }
      }
    }
    };

    for (size_t i = 0; i < THREADS_NUM; ++i)
    threads.push_back(std::thread(f, i));

    for (std::thread & t : threads)
    t.join();

    return tmp;
  }






  template <typename DataT>
  Matrix<DataT> transpose_threads( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRANSPOSE THREADS" << std::endl;

    size_t lhs_r = lhs.rows(),
        lhs_c = lhs.cols(),
        rhs_c = rhs.cols();

    auto THREADS_NUM = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    threads.reserve(THREADS_NUM);

    size_t NN = lhs_r / THREADS_NUM + 1;

    Matrix<DataT> tmp{lhs_r, rhs_c};

    auto rhs_t = MXL::transpose(rhs);

    auto f = [&](size_t c)
    {
      for (size_t m = c * NN, end = std::min((c + 1)  * NN, lhs_r); m < end; ++m)
        for (size_t j = 0; j < rhs_c; ++j)
          for (size_t k = 0; k < lhs_c; ++k)
            tmp[m][j] += lhs[m][k] * rhs_t[j][k];
    };

    for (size_t i = 0; i < THREADS_NUM; ++i)
      threads.push_back(std::thread(f, i));

    for (std::thread & t : threads)
      t.join();

    return tmp;
  }

  template <typename DataT>
  Matrix<DataT> transpose_threads2x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRANSPOSE THREADS 2X" << std::endl;

    size_t lhs_r = lhs.rows(),
        lhs_c = lhs.cols(),
        rhs_c = rhs.cols();

    auto THREADS_NUM = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    threads.reserve(THREADS_NUM);

    size_t NN = lhs_r / THREADS_NUM + 1;

    Matrix<DataT> tmp{lhs_r, rhs_c};

    auto rhs_t = MXL::transpose(rhs);

    auto f = [&](size_t c)
    {
      for (size_t m = c * NN, end = std::min((c + 1)  * NN, lhs_r); m < end; ++m)
        for (size_t j = 0; j < rhs_c; ++j)
        {
          size_t k = 0;
          for (; k < lhs_c; k += 2)
            tmp[m][j] += lhs[m][k] * rhs_t[j][k]
                       + lhs[m][k+1] * rhs_t[j][k+1];
          while (k < lhs_c)
          {
            tmp[m][j] += lhs[m][k] * rhs[j][k];
            ++k;
          }
        }
    };

    for (size_t i = 0; i < THREADS_NUM; ++i)
      threads.push_back(std::thread(f, i));

    for (std::thread & t : threads)
      t.join();

    return tmp;
  }


  template <typename DataT>
  Matrix<DataT> transpose_threads4x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRANSPOSE THREADS 4X" << std::endl;

    size_t lhs_r = lhs.rows(),
        lhs_c = lhs.cols(),
        rhs_c = rhs.cols();

    auto THREADS_NUM = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    threads.reserve(THREADS_NUM);

    size_t NN = lhs_r / THREADS_NUM + 1;

    Matrix<DataT> tmp{lhs_r, rhs_c};

    auto rhs_t = MXL::transpose(rhs);

    auto f = [&](size_t c)
    {
      for (size_t m = c * NN, end = std::min((c + 1)  * NN, lhs_r); m < end; ++m)
        for (size_t j = 0; j < rhs_c; ++j)
        {
          size_t k = 0;
          for (; k < lhs_c; k += 4)
            tmp[m][j] += lhs[m][k] * rhs_t[j][k]
                       + lhs[m][k+1] * rhs_t[j][k+1]
                       + lhs[m][k+2] * rhs_t[j][k+2]
                       + lhs[m][k+3] * rhs_t[j][k+3];
          while (k < lhs_c)
          {
            tmp[m][j] += lhs[m][k] * rhs[j][k];
            ++k;
          }
        }
    };

    for (size_t i = 0; i < THREADS_NUM; ++i)
      threads.push_back(std::thread(f, i));

    for (std::thread & t : threads)
      t.join();

    return tmp;
  }

  template <typename DataT>
  Matrix<DataT> transpose_threads8x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRANSPOSE THREADS 8X" << std::endl;

    size_t lhs_r = lhs.rows(),
        lhs_c = lhs.cols(),
        rhs_c = rhs.cols();

    auto THREADS_NUM = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    threads.reserve(THREADS_NUM);

    size_t NN = lhs_r / THREADS_NUM + 1;

    Matrix<DataT> tmp{lhs_r, rhs_c};

    auto rhs_t = MXL::transpose(rhs);

    auto f = [&](size_t c)
    {
      for (size_t m = c * NN, end = std::min((c + 1)  * NN, lhs_r); m < end; ++m)
        for (size_t j = 0; j < rhs_c; ++j)
        {
          size_t k = 0;
          for (; k < lhs_c; k += 8)
            tmp[m][j] += lhs[m][k] * rhs_t[j][k]
                       + lhs[m][k+1] * rhs_t[j][k+1]
                       + lhs[m][k+2] * rhs_t[j][k+2]
                       + lhs[m][k+3] * rhs_t[j][k+3]
                       + lhs[m][k+4] * rhs_t[j][k+4]
                       + lhs[m][k+5] * rhs_t[j][k+5]
                       + lhs[m][k+6] * rhs_t[j][k+6]
                       + lhs[m][k+7] * rhs_t[j][k+7];
          while (k < lhs_c)
          {
            tmp[m][j] += lhs[m][k] * rhs[j][k];
            ++k;
          }
        }
    };

    for (size_t i = 0; i < THREADS_NUM; ++i)
      threads.push_back(std::thread(f, i));

    for (std::thread & t : threads)
      t.join();

    return tmp;
  }

} // namespace MUL

#endif //THRD_HH
