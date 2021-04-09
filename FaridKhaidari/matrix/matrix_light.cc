#include "matrix_light.hh"

namespace MXL
{
  template<>
  bool is_zero(const double & val)
  {
    return std::abs(val) < 1e-12;
  }

  template<>
  bool is_zero(const float & val)
  {
    return std::abs(val) < 1e-6;
  }
}