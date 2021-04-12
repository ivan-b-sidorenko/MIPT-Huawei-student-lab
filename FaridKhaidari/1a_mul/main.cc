#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

#include "just.hh"
#include "thrd.hh"
#include "intr.hh"
#include "opcl.hh"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

using namespace MXL;
using MXf = Matrix<float>;

using Func = MXf (*) (const MXf &, const MXf &);

auto speed_test( MXf & m1,
                 MXf & m2,
                 MXf & mul,
                 Func func )
{
  static uint counter = 0;

  cout << std::setw(2) << counter++ << ": ";

  MXf res{};

  auto start = std::chrono::steady_clock::now();

  try
  {
    res = (*func)(m1, m2);
  }
  catch ( std::runtime_error &err )
  {
    std::cerr << err.what() << std::endl;
  }

  auto end = std::chrono::steady_clock::now();

  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

#if 0
  if (mul != res)
    cout << "INCORRECT: " << endl;
#endif

  cout << "    " << time << " msc" << endl;

  return res;
}

int main( )
{
  uint rows1{}, rows2{},
       cols1{}, cols2{};

  cin >> rows1 >> cols1 >> rows2 >> cols2;

  assert(cols1 == rows2);

  MXf m1{rows1, cols1};
  MXf m2{rows2, cols2};

  for (size_t i = 0; i < rows1; ++i)
    for (size_t j = 0; j < cols1; ++j)
      cin >> m1[i][j];

  for (size_t i = 0; i < rows2; ++i)
    for (size_t j = 0; j < cols2; ++j)
      cin >> m2[i][j];

  vector<Func> functions{ MUL::trivial, 
                          MUL::transpose,

                          MUL::trivial_cycle2x,
                          MUL::transpose_cycle2x,

                          MUL::trivial_cycle4x,
                          MUL::transpose_cycle4x,

                          MUL::trivial_cycle8x,
                          MUL::transpose_cycle8x,
                          
                          MUL::trivial_threads,
                          MUL::transpose_threads,
                          
                          MUL::trivial_threads2x,
                          MUL::transpose_threads2x,

                          MUL::trivial_threads4x,
                          MUL::transpose_threads4x,

                          MUL::trivial_threads8x,
                          MUL::transpose_threads8x,
                          
                          MUL::intrinsics,

                          MUL::ocl};

  MXf mul{};

#if 0
  mul = m1 * m2;
#endif

  MUL::MX_DRIVER.build();

  for (auto && func : functions)
    speed_test(m1, m2, mul, func);

  return 0;
}