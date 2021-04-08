#ifndef __MUL_OCL_H__
#define __MUL_OCL_H__

#include <CL/cl.hpp>
#include <immintrin.h>
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
    bool ready_ = true;
  public:
    Driver( void )
    {
      if (!select_device())
      {
        ready_ = false;
        std::cerr << "No devices found. Stopped." << std::endl;
        return;
      }

      work_group_size_ = device_.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

      context_ = cl::Context({device_});
      queue_ = cl::CommandQueue(context_, device_);
      
    }

    Driver( const Driver & ) = delete;
    Driver &operator =( const Driver & ) = delete;

    bool build( void )
    {
      std::string src_code = {
        #include "mul.cl"
      };

      cl::Program::Sources src{1, std::make_pair(src_code.c_str(), src_code.length() + 1)};
      prog_ = {context_, src};

      try
      {
        prog_.build();
      }
      catch ( cl::Error &err )
      {
        std::cerr << err.what() << std::endl;
        std::cerr << prog_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device_) << std::endl;
        return false;
      }

      return true;
    }

    linal::Matrix<float> MatMul( const linal::Matrix<float> &A, const linal::Matrix<float> &B, const std::string &kern_name )
    {
      cl::Kernel kernel{};

      auto 
      try
      {
        kernel = {prog_, kern_name.c_str()};

        kernel.setArg(0, )

      }
      catch ( cl::Error &err )
      {
        std::cerr << err.what() << std::endl;
        throw;
      }




    }

    ~Driver( void ) = default;
  private:
    bool select_device( void )
    {
      std::vector<cl::Platform> pls;
      cl::Platform::get(&pls);

      for (auto &&pl_devs : pls)
      {
        std::vector<cl::Device> devs;
        pl_devs.getDevices(CL_DEVICE_TYPE_ALL, &devs);

        for (auto &&dev : devs)
          if (dev.getInfo<CL_DEVICE_AVAILABLE>() && dev.getInfo<CL_DEVICE_COMPILER_AVAILABLE>())
          {
            device_ = dev;
            return true;
          }
      }

      return false;
    }
  };
} // namespace Mul



#endif // __MUL_OCL_H__