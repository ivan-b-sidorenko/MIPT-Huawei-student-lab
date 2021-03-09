#include <iostream>
#include <chrono>

#include "matrix_imp.hh"

using int_mat = linal::Matrix<int>;

int main( int argc, char *argv[] )
{
  int_mat mat1, mat2;

  std::cin >> mat1 >> mat2;

  auto begin = std::chrono::steady_clock::now();

  int_mat res = mat1 * mat2;

  auto end = std::chrono::steady_clock::now();

  if (argc != 1 && argv[1][0] == '-' && argv[1][1] == 'v' )
    std::cout << "matrix 1 = \n" << mat1 << "matrix 2 = \n" << mat2 << "matrix1 * matrix2 = \n" << res;

  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

  return 0;
}