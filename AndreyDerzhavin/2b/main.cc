#include "conv.hh"
#include "Timer.hh"

void input_args( std::istream &ist, linal::Tensor &ten, std::vector<linal::Kernel> &kerns, linal::Tensor &answ );

struct func_n_name final
{

};

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

  auto calc_answ = Conv::Conv(tensor, kerns);

  if (calc_answ == answ)
    std::cout << "Test passed succesfully\n";
  else 
    std::cout << "FUCK...\n";

  return 0;
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