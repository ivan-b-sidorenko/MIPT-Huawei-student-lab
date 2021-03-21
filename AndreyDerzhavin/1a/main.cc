#include <iostream>
#include <chrono>
#include <vector>
#include <string>

#include "mul.hh"

using std::string;
using std::vector;
using mul_fnc = Mul::Mat (*)(const Mul::Mat &lhs, const Mul::Mat &rhs);

struct func_n_name final
{
  mul_fnc func;
  string name;
};

Mul::Mat Measure( const Mul::Mat &lhs, const Mul::Mat &rhs, const func_n_name &fname )
{
  std::cout << fname.name << ":" << std::endl;
  auto begin = std::chrono::high_resolution_clock::now();

  auto answ = fname.func(lhs, rhs);

  auto end = std::chrono::high_resolution_clock::now();

  auto res = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
  std::cout << res << " ms" << std::endl;

  return answ;
}

int main( void )
{
  Mul::Mat mat1, mat2, answ;

  std::cin >> mat1 >> mat2 >> answ;

  if (mat1.getCols() != mat2.getRows())
  {
    std::cout << "Incompatible matrix sizes" << std::endl;
    return 1;
  }


  vector<func_n_name> funcs = 
  {
    {Mul::Mul_Naive, "Naive"},
    {Mul::Mul_nai_Transpose, "Naive Transpose"},
    {Mul::Mul_prom2x, "Prom 2x"},
    {Mul::Mul_prom2x_t, "Prom 2x transpose"},
    {Mul::Mul_prom4x, "Prom 4x"},
    {Mul::Mul_prom4x_t, "Prom 4x transpose"}
  };

  for (auto &&f : funcs)
  {
    auto calc = Measure(mat1, mat2, f);

    if (!answ.empty())
    {
      if (calc == answ)
        std::cout << "Test successfully passed" << std::endl;
      else
        std::cout << "Test failed" << std::endl;
      std::cout << std::endl;
    }
  }


  return 0;
}