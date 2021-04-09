#ifndef __MUL_OCL_H__
#define __MUL_OCL_H__

#include <CL/cl2.hpp>
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

    //std::size_t work_group_size_ = 0;
  public:
    Driver( void );

    Driver( const Driver & ) = delete;
    Driver &operator =( const Driver & ) = delete;

    void build( void );

    linal::Matrix<float> MatMul( const linal::Matrix<float> &A, const linal::Matrix<float> &B, const std::string &kern_name, cl_ulong &elapsed );

    ~Driver( void ) = default;
  private:
    bool select_device( void );

    cl_ulong kernel_exec( cl::Kernel &kern, const cl::NDRange &glob_ranges );

    static const char *err_what(cl_int err_code);
  };

} // namespace Mul



#endif // __MUL_OCL_H__