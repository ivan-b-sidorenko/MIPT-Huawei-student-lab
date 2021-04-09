#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

#include "just.hh"
#include "thrd.hh"
#include "intr.hh"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

using namespace MXL;

using Func = Matrix<float> (*) (const Matrix<float> &, const Matrix<float> &);

auto speed_test( Matrix<float> & m1,
                 Matrix<float> & m2,
                 Matrix<float> & mul,
                 Func func )
{
  static uint counter = 0;

  cout << std::setw(2) << counter++ << ": ";
  auto start = std::chrono::steady_clock::now();

  Matrix<float> res = (*func)(m1, m2);
  auto end = std::chrono::steady_clock::now();

  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

#if 0
  if (mul != res)
    cout << "INCORRECT" << endl;
#endif

  cout << "    " << time << " msc" << endl;

  return res;
}

int main( )
{
  uint rows1{}, rows2{},
       cols1{}, cols2{};

  cin >> rows1 >> cols1 >> rows2 >> cols2;

  vector<float> raw1;
  vector<float> raw2;

  raw1.reserve(rows1 * cols1);
  
  for (int i = 0, end = rows1 * cols1; i < end; ++i)
  {
    float tmp;
    cin >> tmp;
    raw1.push_back(tmp);
  }

  raw2.reserve(rows2 * cols2);
  for (int i = 0, end = rows2 * cols2; i < end; ++i)
  {
    float tmp;
    cin >> tmp;
    raw2.push_back(tmp);
  }

  Matrix<float> m1{rows1, cols1, raw1.begin(), raw1.end()};
  Matrix<float> m2{rows2, cols2, raw2.begin(), raw2.end()};

  assert(cols1 == rows2);

  vector<Func> functions{ MUL::trivial, 
                          MUL::transpose,

                          MUL::trivial_cycle2x,
                          MUL::transpose_cycle2x,

                          MUL::trivial_cycle4x,
                          MUL::transpose_cycle4x,

                          MUL::trivial_cycle8x,
                          MUL::transpose_cycle8x,

                          MUL::intrinsics,
                          
                          MUL::trivial_threads,
                          MUL::transpose_threads,
                          
                          MUL::trivial_threads2x,
                          MUL::transpose_threads2x,

                          MUL::trivial_threads4x,
                          MUL::transpose_threads4x,

                          MUL::trivial_threads8x,
                          MUL::transpose_threads8x};

  Matrix<float> mul{};

#if 0
  mul = m1 * m2;
#endif

  for (auto && func : functions)
    speed_test(m1, m2, mul, func);

  return 0;
}