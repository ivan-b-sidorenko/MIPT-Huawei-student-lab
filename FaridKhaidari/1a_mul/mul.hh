#include <vector>

#include <mutex>
#include <thread>

#include <typeinfo>
#include <stdexcept>

#include "matrix_light.hh"

namespace MUL
{
  using namespace MXL;

  template <typename DataT>
  Matrix<DataT> trivial( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

  template <typename DataT>
  Matrix<DataT> trivial_cycle2x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

  template <typename DataT>
  Matrix<DataT> trivial_cycle4x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

  template <typename DataT>
  Matrix<DataT> trivial_cycle8x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

  template <typename DataT>
  Matrix<DataT> trivial_threads( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

  template <typename DataT>
  Matrix<DataT> trivial_threads2x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );


  template <typename DataT>
  Matrix<DataT> transpose( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

  template <typename DataT>
  Matrix<DataT> transpose_cycle2x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

  template <typename DataT>
  Matrix<DataT> transpose_cycle4x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

  template <typename DataT>
  Matrix<DataT> transpose_cycle8x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );

  template <typename DataT>
  Matrix<DataT> transpose_threads( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs );




  /**
   * 
   * REALISATIONS
   *
   */




  template <typename DataT>
  Matrix<DataT> trivial( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
  std::cout << "TRIVIAL" << std::endl;

  size_t lhs_r = lhs.rows(),
      lhs_c = lhs.cols(),
      rhs_c = rhs.cols();
  
  Matrix<DataT> tmp{lhs_r, rhs_c};

  for (size_t i = 0; i < lhs_r; ++i)
    for (size_t j = 0; j < rhs_c; ++j)
      for (size_t k = 0; k < lhs_c; ++k)
        tmp[i][j] += lhs[i][k] * rhs[k][j];

  return tmp;
  }

  template <typename DataT>
  Matrix<DataT> trivial_cycle2x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
  std::cout << "TRIVIAL 2X" << std::endl;

  size_t lhs_r = lhs.rows(),
         lhs_c = lhs.cols(),
         rhs_c = rhs.cols();
  
  Matrix<DataT> tmp{lhs_r, rhs_c};

  for (size_t i = 0; i < lhs_r; ++i)
    for (size_t j = 0; j < rhs_c; ++j)
    {
      size_t k = 0;
      for (size_t end = lhs_c - 2; k < end; k += 2)
        tmp[i][j] += lhs[i][k] * rhs[k][j]
                   + lhs[i][k+1] * rhs[k+1][j];
      while (k < lhs_c)
      {
        tmp[i][j] += lhs[i][k] * rhs[k][j];
        ++k;
      }   
    }

  //std::cout << tmp << std::endl;
  return tmp;
  }

  template <typename DataT>
  Matrix<DataT> trivial_cycle4x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRIVIAL 4X" << std::endl;

    size_t lhs_r = lhs.rows(),
       lhs_c = lhs.cols(),
       rhs_c = rhs.cols();
    
    Matrix<DataT> tmp{lhs_r, rhs_c};

    for (size_t i = 0; i < lhs_r; ++i)
    for (size_t j = 0; j < rhs_c; ++j)
    {
      size_t k = 0;
      for (size_t end = lhs_c - 4; k < end; k += 4)
        tmp[i][j] += lhs[i][k] * rhs[k][j]
                   + lhs[i][k+1] * rhs[k+1][j]
                   + lhs[i][k+2] * rhs[k+2][j]
                   + lhs[i][k+3] * rhs[k+3][j];
      while (k < lhs_c)
      {
        tmp[i][j] += lhs[i][k] * rhs[k][j];
        ++k;
      }   
    }

    return tmp;
  }

  template <typename DataT>
  Matrix<DataT> trivial_cycle8x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRIVIAL 8X" << std::endl;

    size_t lhs_r = lhs.rows(),
        lhs_c = lhs.cols(),
        rhs_c = rhs.cols();
    
    Matrix<DataT> tmp{lhs_r, rhs_c};

    for (size_t i = 0; i < lhs_r; ++i)
    for (size_t j = 0; j < rhs_c; ++j)
    {
      size_t k = 0;
      for (size_t end = lhs_c - 8; k < end; k += 8)
      tmp[i][j] += lhs[i][k] * rhs[k][j] +
                   lhs[i][k+1] * rhs[k+1][j] +
                   lhs[i][k+2] * rhs[k+2][j] +
                   lhs[i][k+3] * rhs[k+3][j] +
                   lhs[i][k+4] * rhs[k+4][j] +
                   lhs[i][k+5] * rhs[k+5][j] +
                   lhs[i][k+6] * rhs[k+6][j] +
                   lhs[i][k+7] * rhs[k+7][j];
      while (k < lhs_c)
      {
        tmp[i][j] += lhs[i][k] * rhs[k][j];
        ++k;
      }   
    }

    return tmp;
  }

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
  Matrix<DataT> transpose( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRANSPOSE" << std::endl;

    size_t lhs_r = lhs.rows(),
        lhs_c = lhs.cols(),
        rhs_c = rhs.cols();

    Matrix<DataT> tmp1{lhs_r, rhs_c};
    Matrix<DataT> tmp2{transpose(rhs)};

    for (size_t i = 0; i < lhs_r; ++i)
      for (size_t j = 0; j < rhs_c; ++j)
        for (size_t k = 0; k < lhs_c; ++k)
          tmp1[i][j] += lhs[i][k] * tmp2[j][k];

    return tmp1;
  }

  template <typename DataT>
  Matrix<DataT> transpose_cycle2x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRANSPOSE 2X" << std::endl;

    size_t lhs_r = lhs.rows(),
        lhs_c = lhs.cols(),
        rhs_c = rhs.cols();

    Matrix<DataT> tmp1{lhs_r, rhs_c};
    Matrix<DataT> tmp2{transpose(rhs)};

    for (size_t i = 0; i < lhs_r; ++i)
      for (size_t j = 0; j < rhs_c; ++j)
      {
        size_t k = 0;
        for (size_t end = lhs_c - 2; k < end; k += 2)
          tmp1[i][j] += lhs[i][k] * tmp2[j][k] + lhs[i][k+1] * tmp2[j][k+1];
        while (k < lhs_c)
        {
          tmp1[i][j] += lhs[i][k] * tmp2[j][k];
          ++k;
        }   
      }

    return tmp1;
  }

  template <typename DataT>
  Matrix<DataT> transpose_cycle4x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRANSPOSE 4X" << std::endl;

    size_t lhs_r = lhs.rows(),
        lhs_c = lhs.cols(),
        rhs_c = rhs.cols();

    Matrix<DataT> tmp1{lhs_r, rhs_c};
    Matrix<DataT> tmp2{transpose(rhs)};

    for (size_t i = 0; i < lhs_r; ++i)
      for (size_t j = 0; j < rhs_c; ++j)
      {
        size_t k = 0;
        for (size_t end = lhs_c - 4; k < end; k += 4)
          tmp1[i][j] += lhs[i][k] * tmp2[j][k] + 
                        lhs[i][k+1] * tmp2[j][k+1] + 
                        lhs[i][k+2] * tmp2[j][k+2] + 
                        lhs[i][k+3] * tmp2[j][k+3];
        while (k < lhs_c)
        {
          tmp1[i][j] += lhs[i][k] * tmp2[j][k];
          ++k;
        }   
      }

    return tmp1;
  }

  template <typename DataT>
  Matrix<DataT> transpose_cycle8x( const Matrix<DataT> & lhs, const Matrix<DataT> & rhs )
  {
    std::cout << "TRANSPOSE 8X" << std::endl;

    size_t lhs_r = lhs.rows(),
        lhs_c = lhs.cols(),
        rhs_c = rhs.cols();

    Matrix<DataT> tmp1{lhs_r, rhs_c};
    Matrix<DataT> tmp2{transpose(rhs)};

    for (size_t i = 0; i < lhs_r; ++i)
      for (size_t j = 0; j < rhs_c; ++j)
      {
        size_t k = 0;
        for (size_t end = lhs_c - 8; k < end; k += 8)
          tmp1[i][j] += lhs[i][k] * tmp2[j][k] + 
                        lhs[i][k+1] * tmp2[j][k+1] + 
                        lhs[i][k+2] * tmp2[j][k+2] + 
                        lhs[i][k+3] * tmp2[j][k+3] +
                        lhs[i][k+4] * tmp2[j][k+4] + 
                        lhs[i][k+5] * tmp2[j][k+5] + 
                        lhs[i][k+6] * tmp2[j][k+6] + 
                        lhs[i][k+7] * tmp2[j][k+7];
        while (k < lhs_c)
        {
          tmp1[i][j] += lhs[i][k] * tmp2[j][k];
          ++k;
        }   
      }

    return tmp1;
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