#include <string>
#include "conv.hh"
#include "Timer.hh"

void input_args( std::istream &ist, linal::Tensor &ten, std::vector<linal::Kernel> &kerns, linal::Tensor &answ );

using func = linal::Tensor (*)( const linal::Tensor &, const std::vector<linal::Kernel> & );

struct func_n_name final
{
  func fun;
  std::string name;
};

linal::Tensor Test( const linal::Tensor &ten, const std::vector<linal::Kernel> &kern, const func_n_name &fname );

int main( void )
{
  linal::Tensor tensor, answ;
  std::vector<linal::Kernel> kerns;

  try 
  {
    input_args(std::cin, tensor, kerns, answ);
  }
  catch ( std::runtime_error &err )
  {
    std::cerr << err.what() << std::endl;
    return -1;
  }

  std::vector<func_n_name> fnames = 
  {
    {Conv::Naive, "Naive"},
    {Conv::Prom2x, "Prom 2x"},
    {Conv::Prom4x, "Prom 4x"},
    {Conv::Prom4x_tmp_var, "Prom 4x with temp vars"}
  };

  for (auto &&fname : fnames)
  {
    auto calc_answ = Test(tensor, kerns, fname);

    if (calc_answ == answ)
      std::cout << "Test passed successfully." << std::endl;
    else 
      std::cout << "Test failed." << std::endl;
    std::cout << std::endl;
  }
  return 0;
}

linal::Tensor Test( const linal::Tensor &ten, const std::vector<linal::Kernel> &kerns, const func_n_name &fname )
{
  std::cout << fname.name << ":" << std::endl;
  timer::Timer timer;

  auto res = fname.fun(ten, kerns);

  auto res_time = static_cast<linal::ldbl>(timer.elapsed_mcs()) / 1'000;

  std::cout << res_time << " ms" << std::endl;

  return res;
}


void input_args( std::istream &ist, linal::Tensor &ten, std::vector<linal::Kernel> &kerns, linal::Tensor &answ )
{
  ist >> ten;
  size_t kerns_size;

  ist >> kerns_size;
  kerns.resize(kerns_size);

  for (auto &&kern : kerns)
    ist >> kern;
  if (kerns_size > 0)
    if (kerns[0].get_ch_amount() > ten.get_ch_size() || kerns[0].get_cols() > ten.get_width() || kerns[0].get_rows() > ten.get_height())
      throw std::runtime_error{"Incompatible kernel size"};

  ist >> answ;
}