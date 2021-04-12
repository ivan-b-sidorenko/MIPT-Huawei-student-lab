#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <limits>

#include "mul.hh"
#include "mul_th.hh"
#include "mul_ocl.hh"
#include "Timer.hh"

using std::string;
using std::vector;
using mul_fnc = Mul::Mat (*)(const Mul::Mat &lhs, const Mul::Mat &rhs);
using cl_mul_fnc = Mul::Mat (*)(const Mul::Driver &, const Mul::Mat &, const Mul::Mat &, cl_ulong &);

struct func_n_name final
{
  mul_fnc func;
  string name;
};


struct clfunc_n_name final
{
  cl_mul_fnc func;
  string name;
};


linal::ldbl naive_ms, min_ms = std::numeric_limits<linal::ldbl>::max(), min_cl_ms = std::numeric_limits<linal::ldbl>::max();

Mul::Mat Measure( const Mul::Mat &lhs, const Mul::Mat &rhs, const func_n_name &fname )
{
  std::cout << fname.name << ":" << std::endl;
  timer::Timer timer;

  auto answ = fname.func(lhs, rhs);

  auto res = static_cast<linal::ldbl>(timer.elapsed_mcs()) / 1'000;
  std::cout << res << " ms" << std::endl;

  if (fname.func == Mul::Mul_Naive)
    naive_ms = res;

  min_ms = std::min(min_ms, res);
  return answ;
}

Mul::Mat Run( const Mul::Mat &lhs, const Mul::Mat &rhs, const clfunc_n_name &fname )
{
  std::cout << fname.name << ":" << std::endl;

  try 
  {
    Mul::Driver driver;
    driver.build();

    cl_ulong elapsed_ns;
    timer::Timer timer;

    auto answ = fname.func(driver, lhs, rhs, elapsed_ns);

    auto res = static_cast<linal::ldbl>(timer.elapsed_mcs()) / 1'000;

    auto cl_res = static_cast<linal::ldbl>(elapsed_ns) / 1'000'000;
    std::cout << std::endl;
    std::cout << "  OpenCL outer time:" << std::endl;
    std::cout << "  " << res << " ms" << std::endl << std::endl;
    std::cout << "  OpenCL inner time:" << std::endl;
    std::cout << "  " << cl_res << " ms" << std::endl << std::endl;

    min_ms = std::min(min_ms, res);
    min_cl_ms = std::min(min_ms, cl_res);
    return answ;
  }
  catch ( std::runtime_error &err )
  {
    std::cerr << "Error occured in " << fname.name << std::endl;
    std::cerr << err.what() << std::endl;
  }

  return {};
}

int main( void )
{
  Mul::Mat mat1, mat2, answ;

  std::cin >> mat1 >> mat2 >> answ;

  if (mat1.getCols() != mat2.getRows())
  {
    std::cout << "Incompatible matrix sizes" << std::endl;
    return -1;
  }


  vector<func_n_name> funcs = 
  {
    {Mul::Mul_Naive, "Naive"},
    {Mul::Mul_nai_Transpose, "Naive Transpose"},
    {Mul::Mul_prom2x, "Prom 2x"},
    {Mul::Mul_prom2x_t, "Prom 2x transpose"},
    {Mul::Mul_prom4x, "Prom 4x"},
    {Mul::Mul_prom4x_t, "Prom 4x transpose"},
    {Mul::Mul_prom8x, "Prom 8x"},
    {Mul::Mul_prom8x_t, "Prom 8x transpose"},
    {Mul::Mul_prom16x, "Prom 16x"},
    {Mul::Mul_prom16x_t, "Prom 16x transpose"},
    {Mul::Mul_Naive_ptmp, "Naive tmp common var"},
    {Mul::Mul_nai_Transpose_ptmp, "Naive Transpose tmp common var"},
    {Mul::Mul_prom2x_ptmp, "Prom 2x tmp common var"},
    {Mul::Mul_prom2x_t_ptmp, "Prom 2x transpose tmp common var"},
    {Mul::Mul_prom4x_ptmp, "Prom 4x tmp common var"},
    {Mul::Mul_prom4x_t_ptmp, "Prom 4x transpose tmp common var"},
    {Mul::Mul_prom8x_ptmp, "Prom 8x tmp common var"},
    {Mul::Mul_prom8x_t_ptmp, "Prom 8x transpose tmp common var"},
    {Mul::Mul_prom16x_ptmp, "Prom 16x tmp common var"},
    {Mul::Mul_prom16x_t_ptmp, "Prom 16x transpose tmp common var"},
    //************************* THREADS ***************************
    {Mul::Mul_th<2>, "2 Threads"},
    {Mul::Mul_th<4>, "4 Threads"},
    {Mul::Mul_th<8>, "8 Threads"},
    {Mul::Mul_th<16>, "16 Threads"},
    {Mul::Mul_th_prom2x<2>, "2 Threads prom 2"},
    {Mul::Mul_th_prom2x<4>, "4 Threads prom 2"},
    {Mul::Mul_th_prom2x<8>, "8 Threads prom 2"},
    {Mul::Mul_th_prom2x<16>, "16 Threads prom 2"},
    {Mul::Mul_th_prom4x<2>, "2 Threads prom 4"},
    {Mul::Mul_th_prom4x<4>, "4 Threads prom 4"},
    {Mul::Mul_th_prom4x<8>, "8 Threads prom 4"},
    {Mul::Mul_th_prom4x<16>, "16 Threads prom 4"},
    {Mul::Mul_th_t<2>, "2 Threads transopse"},
    {Mul::Mul_th_t<4>, "4 Threads transopse"},
    {Mul::Mul_th_t<8>, "8 Threads transopse"},
    {Mul::Mul_th_t<16>, "16 Threads transopse"},
    {Mul::Mul_th_t_prom2x<2>, "2 Threads transopse prom 2"},
    {Mul::Mul_th_t_prom2x<4>, "4 Threads transopse prom 2"},
    {Mul::Mul_th_t_prom2x<8>, "8 Threads transopse prom 2"},
    {Mul::Mul_th_t_prom2x<16>, "16 Threads transopse prom 2"},
    {Mul::Mul_th_t_prom4x<2>, "2 Threads transopse prom 4"},
    {Mul::Mul_th_t_prom4x<4>, "4 Threads transopse prom 4"},
    {Mul::Mul_th_t_prom4x<8>, "8 Threads transopse prom 4"},
    {Mul::Mul_th_t_prom4x<16>, "16 Threads transopse prom 4"},
    {Mul::Mul_th_t_ptmp<2>, "2 Threads transopse tmp common var"},
    {Mul::Mul_th_t_ptmp<4>, "4 Threads transopse tmp common var"},
    {Mul::Mul_th_t_ptmp<8>, "8 Threads transopse tmp common var"},
    {Mul::Mul_th_t_ptmp<16>, "16 Threads transopse tmp common var"},
    {Mul::Mul_th_t_ptmp_prom2x<2>, "2 Threads transopse tmp common var prom 2"},
    {Mul::Mul_th_t_ptmp_prom2x<4>, "4 Threads transopse tmp common var prom 2"},
    {Mul::Mul_th_t_ptmp_prom2x<8>, "8 Threads transopse tmp common var prom 2"},
    {Mul::Mul_th_t_ptmp_prom2x<16>, "16 Threads transopse tmp common var prom 2"},
    {Mul::Mul_th_t_ptmp_prom4x<2>, "2 Threads transopse tmp common var prom 4"},
    {Mul::Mul_th_t_ptmp_prom4x<4>, "4 Threads transopse tmp common var prom 4"},
    {Mul::Mul_th_t_ptmp_prom4x<8>, "8 Threads transopse tmp common var prom 4"},
    {Mul::Mul_th_t_ptmp_prom4x<16>, "16 Threads transopse tmp common var prom 4"},
    {Mul::Mul_th_t_ptmp_prom8x<2>, "2 Threads transopse tmp common var prom 8"},
    {Mul::Mul_th_t_ptmp_prom8x<4>, "4 Threads transopse tmp common var prom 8"},
    {Mul::Mul_th_t_ptmp_prom8x<8>, "8 Threads transopse tmp common var prom 8"},
    {Mul::Mul_th_t_ptmp_prom8x<16>, "16 Threads transopse tmp common var prom 8"},
    {Mul::Mul_th_t_ptmp_prom16x<2>, "2 Threads transopse tmp common var prom 16"},
    {Mul::Mul_th_t_ptmp_prom16x<4>, "4 Threads transopse tmp common var prom 16"},
    {Mul::Mul_th_t_ptmp_prom16x<8>, "8 Threads transopse tmp common var prom 16"},
    {Mul::Mul_th_t_ptmp_prom16x<16>, "16 Threads transopse tmp common var prom 16"}
  };


  //***********                  HERE GOES OPENCL     ****************************
  std::vector<clfunc_n_name> cl_funcs = 
  {
    {Mul::oclNaive, "OpenCL naive"}
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

  std::cout << "*************************** HERE GOES OpenCL ****************************" << std::endl << std::endl;

  for (auto &&f : cl_funcs)
  {
    auto calc = Run(mat1, mat2, f);

    if (!answ.empty())
    {
      if (calc == answ)
        std::cout << "Test successfully passed" << std::endl;
      else
        std::cout << "Test failed" << std::endl;
      std::cout << std::endl;
    }
  }

  std::cout << "***************************************************************************" << std::endl << std::endl;

  std::cout << "Win from naive in " << naive_ms << " / " << min_ms << " = " << naive_ms / min_ms << " times" << std::endl;
  std::cout << "Win from naive (includes inner OpenCL time) in " << naive_ms << " / " << min_cl_ms << " = " << naive_ms / min_cl_ms << " times" << std::endl;

  return 0;
}