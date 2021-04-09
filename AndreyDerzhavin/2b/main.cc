#include "conv.hh"

void input_args( std::istream &ist, linal::Tensor &ten, std::vector<linal::Kernel> &kerns, linal::Tensor &answ );

int main( void )
{
  linal::Tensor tensor, answ;
  std::vector<linal::Kernel> kerns;

  input_args(std::cin, tensor, kerns, answ);

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

  ist >> answ;
}