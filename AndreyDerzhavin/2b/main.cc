#include "conv.hh"

void input_args( linal::Tensor &ten, std::vector<linal::Kernel> &kerns, linal::Tensor &answ );

int main( void )
{
  linal::Tensor tensor, answ;
  std::vector<linal::Kernel> kerns;

  input_args(tensor, kerns, answ);



  return 0;
}


void input_args( linal::Tensor &ten, std::vector<linal::Kernel> &kerns, linal::Tensor &answ )
{
  std::cin >> ten;
  size_t kerns_size;

  std::cin >> kerns_size;
  kerns.resize(kerns_size);

  for (auto &&kern : kerns)
    std::cin >> kern;
  std::cin >> answ;
}