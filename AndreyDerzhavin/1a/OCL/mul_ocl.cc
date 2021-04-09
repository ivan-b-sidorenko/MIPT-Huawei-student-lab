#include "mul_ocl.hh"

namespace Mul
{
  Mat oclNaive( const Driver &driver, const Mat &lhs, const Mat &rhs, cl_ulong &elapsed_ns )
  {
    return driver.MatMul(lhs, rhs, "Naive", elapsed_ns);
  }
}