#ifndef OPCL_HH
#define OPCL_HH

#include <CL/cl2.hpp>
#include "matrix_light.hh"

namespace MUL
{
  using namespace MXL;
  using MXf = Matrix<float>;

  class Driver final
  {

  private:
  
    cl::Device device_{};
    cl::Context context_{};
    cl::CommandQueue queue_{};
    cl::Program program_{};

    bool ready_{false};

  public:
  
    Driver( );

    Driver( const Driver & ) = delete;
    Driver &operator =( const Driver & ) = delete;

    ~Driver( ) = default;

    void build( );

    bool is_ready( )
    { return ready_; }

    MXf MXMul( const MXf & lhs, const MXf & rhs, 
               const std::string &kern_name, 
               const cl::NDRange &loc_ranges = cl::NullRange ) const;
  
  private:
  
    bool dev_choice( );

    cl_ulong kernel_exec( cl::Kernel & kern, const cl::NDRange &glob_ranges, const cl::NDRange &loc_ranges = cl::NullRange ) const;

    static const char * decode_err(cl_int err_code);
  };

  extern Driver MX_DRIVER;

  MXf ocl( const MXf &lhs, const MXf &rhs );

} // namespace MUL


#endif // OPCL_HH