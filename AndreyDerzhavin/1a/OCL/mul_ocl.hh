#ifndef __MUL_OCL_H__
#define __MUL_OCL_H__

#include <CL/cl.hpp>
#include "matrix.hh"

namespace Mul
{
  class Driver final
  {
  private:
    cl::Device device_{};
    cl::Context context_{};
    cl::CommandQueue queue_{};
    cl::Program prog_{};

    std::size_t work_group_size_ = 0;
  public:
    Driver( void );

    Driver( const Driver & ) = delete;
    Driver &operator =( const Driver & ) = delete;

    void build( void );

    linal::Matrix<float> MatMul( const linal::Matrix<float> &A, const linal::Matrix<float> &B, const std::string &kern_name );

    ~Driver( void ) = default;
  private:
    bool select_device( void );

    bool kernel_exec( void );

    static const char *err_what(cl_int err_code);
  };

  Mat CL_Naive( const Mat &A, const Mat &B );
} // namespace Mul



#endif // __MUL_OCL_H__