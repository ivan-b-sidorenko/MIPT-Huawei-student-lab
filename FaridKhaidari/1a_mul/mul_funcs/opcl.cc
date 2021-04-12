#include "opcl.hh"

namespace MUL
{
  using namespace MXL;

  Driver MX_DRIVER{};

  Driver::Driver( )
  {
    if (!dev_choice())
      throw std::runtime_error{"No devices found"};

    context_ = cl::Context({device_});
    queue_ = cl::CommandQueue(context_, device_, CL_QUEUE_PROFILING_ENABLE);
  }

  void Driver::build( )
  {
    std::string src_code = {
      #include "opcl.cl"
    };

    cl::Program::Sources src{{src_code}};
    program_ = {context_, src};

    try
    {
      program_.build();
    }
    catch ( cl::Error & e )
    {
      std::cerr << e.what() << std::endl;
      std::cerr << program_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device_) << std::endl;
      throw;
    }
    ready_ = true;
  }

  MXf Driver::MXMul( const MXf & lhs, const MXf & rhs, const std::string &kern_name, const cl::NDRange &loc_ranges /* = cl::NullRange */ ) const
  {
    cl::Kernel kernel{};

    std::vector<float> lhs_buf, rhs_buf, C_buf;

    lhs_buf.resize(lhs.rows() * lhs.cols());
    rhs_buf.resize(rhs.cols() * rhs.rows());
    C_buf.resize(lhs.rows() * rhs.cols());
    
    vectorize(lhs_buf.begin(), lhs_buf.end(), lhs);
    vectorize(rhs_buf.begin(), rhs_buf.end(), rhs);

    cl::Buffer cl_lhs_buf{context_, lhs_buf.begin(), lhs_buf.end(), true, true},
               cl_rhs_buf{context_, rhs_buf.begin(), rhs_buf.end(), true, true};
    
    cl::Buffer clC_buf{context_, CL_MEM_READ_WRITE, C_buf.size() * sizeof(C_buf[0])};

    cl::NDRange global_ranges{lhs.rows(), rhs.cols()};

    cl_ulong elapsed_nano{};
    try
    {
      kernel = {program_, kern_name.c_str()};

      // Send buffers to kernel
      kernel.setArg(0, cl_lhs_buf);
      kernel.setArg(1, cl_rhs_buf);
      kernel.setArg(2, clC_buf);

      kernel.setArg(3, static_cast<unsigned>(lhs.rows()));
      kernel.setArg(4, static_cast<unsigned>(rhs.rows()));
      kernel.setArg(5, static_cast<unsigned>(rhs.cols()));
      
      elapsed_nano = kernel_exec(kernel, global_ranges, loc_ranges);
    }
    catch ( cl::Error & e )
    {
      std::cerr << e.what() << std::endl;
      std::cerr << decode_err(e.err()) << std::endl;
      return {};
    }

    queue_.enqueueReadBuffer(clC_buf, CL_TRUE, 0, sizeof(C_buf[0]) * C_buf.size(), C_buf.data());

    std::cout << "    GPU: " << static_cast<double>(elapsed_nano) / 1e6 << " msc, common: ";

    return {lhs.rows(), rhs.cols(), C_buf.begin(), C_buf.end()};
  }

  bool Driver::dev_choice( )
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

  cl_ulong Driver::kernel_exec( cl::Kernel &kern, const cl::NDRange &glob_ranges, const cl::NDRange &loc_ranges /* = cl::NullRange */ ) const
  {
    cl::Event event;

    cl_int err = queue_.enqueueNDRangeKernel(kern, cl::NullRange, glob_ranges, loc_ranges, nullptr, &event);
    if (err != CL_SUCCESS)
      throw std::runtime_error{"Error executing kernel"};

    event.wait();

    auto start = event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    auto end = event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
    return end - start;
  }


  const char * Driver::decode_err( cl_int err_code )
  {
    switch (err_code)
    {
    case 0:   return "CL_SUCCESS";
    case -1:  return "CL_DEVICE_NOT_FOUND";
    case -2:  return "CL_DEVICE_NOT_AVAILABLE";
    case -3:  return "CL_COMPILER_NOT_AVAILABLE";
    case -4:  return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5:  return "CL_OUT_OF_RESOURCES";
    case -6:  return "CL_OUT_OF_HOST_MEMORY";
    case -7:  return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8:  return "CL_MEM_COPY_OVERLAP";
    case -9:  return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUrhs_bufFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";
    case -69: return "CL_INVALID_PIPE_SIZE";
    case -70: return "CL_INVALID_DEVICE_QUEUE";
    case -71: return "CL_INVALID_SPEC_ID";
    case -72: return "CL_MAX_SIZE_RESTRICTION_EXCEEDED";
    default:  return "UNCKNOWN ERROR";
    }
  }

  MXf ocl( const MXf &lhs, const MXf &rhs )
  {
    std::cout << "OpenCL" << std::endl;
    if (!MX_DRIVER.is_ready())
      throw std::runtime_error{"Driver is not ready"};

    return MX_DRIVER.MXMul(lhs, rhs, "trivial");
  }
} // namespace MUL