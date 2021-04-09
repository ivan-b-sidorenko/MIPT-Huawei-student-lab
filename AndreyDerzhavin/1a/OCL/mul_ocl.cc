#include "mul_ocl.hh"

namespace Mul
{
  Mat CL_Naive( const Mat &A, const Mat &B )
  {
    Driver driver;
    driver.build();

    return driver.MatMul(A, B, "Naive");
  }
} // namespace Mul
