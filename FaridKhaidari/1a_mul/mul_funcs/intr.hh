#ifndef INTR_HH
#define INTR_HH

#include <immintrin.h>
#include "matrix_light.hh"


namespace MUL
{
  using namespace MXL;

  using MXf = Matrix<float>;

  MXf intrinsics( const MXf & lhs, const MXf & rhs )
  {
    std::cout << "INTRINSICS" << std::endl;

    size_t lhs_r = lhs.rows(),
           lhs_c = lhs.cols(),
           rhs_c = rhs.cols();

    MXf res{lhs_r, rhs_c};


    for (size_t i = 0; i < lhs_r; ++i)
    {
      for (size_t j = 0; j < lhs_c; ++j)
      {
        size_t k = 0;

        __m256 a = _mm256_set1_ps(lhs[i][j]);

        for (size_t end = rhs_c - 8; k < end; k += 8)
          _mm256_storeu_ps(
            res[i] + k, _mm256_fmadd_ps(
                          a,
                          _mm256_loadu_ps(rhs[j] + k),
                          _mm256_loadu_ps(res[i] + k)
                        )
          );
        while (k < rhs_c)
        {
          res[i][k] += lhs[i][j] * rhs[j][k];
          ++k;
        }
      }
    }

    return res;
  }

} // namespace MUL 

/*

void _mm256_storeu_ps (float * mem_addr, __m256 a);

Store 256-bits (composed of 8 packed single-precision (32-bit) floating-point elements) from a 
into memory. mem_addr does not need to be aligned on any particular boundary.

Записывает 8 float'ов из __m256 a в память по указателю
__________________________________________________________________________________________________

__m256 _mm256_setzero_ps (void);

Return vector of type __m256 with all elements set to zero.

Возвращает вектор __m256 заполненный нулями
__________________________________________________________________________________________________

__m256 _mm256_set1_ps (float a);

Broadcast single-precision (32-bit) floating-point value a to all elements of dst.

Возвращает вектор __m256 заполненный одинаковыми числами a
__________________________________________________________________________________________________

__m256 _mm256_fmadd_ps (__m256 a, __m256 b, __m256 c);

Multiply packed single-precision (32-bit) floating-point elements in a and b, add the intermediate 
result to packed elements in c, and store the results in dst.

Перемножает элементы из a и b, складывает результат перемножения с элементами C и возвращает сумму
__________________________________________________________________________________________________

__m256 _mm256_loadu_ps (float const * mem_addr);

Load 256-bits (composed of 8 packed single-precision (32-bit) floating-point elements) from memory 
into dst. mem_addr does not need to be aligned on any particular boundary.

Загружает 8 float'ов из памяти в __m256.
__________________________________________________________________________________________________

*/

#endif // INTR_HH