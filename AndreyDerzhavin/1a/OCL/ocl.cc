#include "mul_ocl.hh"

namespace Mul
{
  Driver::Driver( void )
  {
    if (!select_device())
      throw std::runtime_error{"No devices found"};
    //work_group_size_ = device_.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

    context_ = cl::Context({device_});
    queue_ = cl::CommandQueue(context_, device_, CL_QUEUE_PROFILING_ENABLE);
  }

  void Driver::build( void )
  {
    std::string src_code = {
      #include "mul.cl"
    };

    cl::Program::Sources src{{src_code}};
    prog_ = {context_, src};

    try
    {
      prog_.build();
    }
    catch ( cl::Error &err )
    {
      std::cerr << err.what() << std::endl;
      std::cerr << prog_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device_) << std::endl;
      throw;
    }
  }

  linal::Matrix<float> Driver::MatMul( const linal::Matrix<float> &A, const linal::Matrix<float> &B, const std::string &kern_name, 
                                       cl_ulong &nsec_elapsed )
  {
    cl::Kernel kernel{};

    std::vector<float> A_buf, B_buf, C_buf;

    A_buf.resize(A.getRows() * A.getCols());
    B_buf.resize(B.getCols() * B.getRows());
    C_buf.resize(A.getRows() * B.getCols());
    
    linal::MatToIt(A_buf.begin(), A_buf.end(), A);
    linal::MatToIt(B_buf.begin(), B_buf.end(), B);

    cl::Buffer clA_buf{context_, A_buf.begin(), A_buf.end(), true, true},
                clB_buf{context_, B_buf.begin(), B_buf.end(), true, true};
    
    cl::Buffer clC_buf{context_, CL_MEM_READ_WRITE, C_buf.size() * sizeof(C_buf[0])};

    cl::NDRange global_ranges{A.getRows(), B.getCols()};

    try
    {
      kernel = {prog_, kern_name.c_str()};

      // Send buffers to kernel
      kernel.setArg(0, clA_buf);
      kernel.setArg(1, clB_buf);
      kernel.setArg(2, clC_buf);

      kernel.setArg(3, static_cast<unsigned>(A.getRows()));
      kernel.setArg(4, static_cast<unsigned>(B.getRows()));
      kernel.setArg(5, static_cast<unsigned>(B.getCols()));
      
      nsec_elapsed = kernel_exec(kernel, global_ranges);
    }
    catch ( cl::Error &err )
    {
      std::cerr << err.what() << std::endl;
      std::cerr << err_what(err.err()) << std::endl;
      return {};
    }

    queue_.enqueueReadBuffer(clC_buf, CL_TRUE, 0, sizeof(C_buf[0]) * C_buf.size(), C_buf.data());

    return {A.getRows(), B.getCols(), C_buf.begin(), C_buf.end()};
  }

  bool Driver::select_device( void )
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

  cl_ulong Driver::kernel_exec( cl::Kernel &kern, const cl::NDRange &glob_ranges )
  {
    cl::Event event;

    cl_int err = queue_.enqueueNDRangeKernel(kern, cl::NullRange, glob_ranges, cl::NullRange, nullptr, &event);
    if (err != CL_SUCCESS)
      throw std::runtime_error{"Error executing kernel"};

    event.wait();

    auto start = event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    auto end = event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
    return end - start;
  }

  const char *Driver::err_what( cl_int err_code )
  {
    switch (err_code)
    {
    case 0:
      return "CL_SUCCESS";
    case -1:
      return "CL_DEVICE_NOT_FOUND";
    case -2:
      return "CL_DEVICE_NOT_AVAILABLE";
    case -3:
      return "CL_COMPILER_NOT_AVAILABLE";
    case -4:
      return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5:
      return "CL_OUT_OF_RESOURCES";
    case -6:
      return "CL_OUT_OF_HOST_MEMORY";
    case -7:
      return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8:
      return "CL_MEM_COPY_OVERLAP";
    case -9:
      return "CL_IMAGE_FORMAT_MISMATCH";
    case -10:
      return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11:
      return "CL_BUILD_PROGRAM_FAILURE";
    case -12:
      return "CL_MAP_FAILURE";
    case -13:
      return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14:
      return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15:
      return "CL_COMPILE_PROGRAM_FAILURE";
    case -16:
      return "CL_LINKER_NOT_AVAILABLE";
    case -17:
      return "CL_LINK_PROGRAM_FAILURE";
    case -18:
      return "CL_DEVICE_PARTITION_FAILED";
    case -19:
      return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
    case -30:
      return "CL_INVALID_VALUE";
    case -31:
      return "CL_INVALID_DEVICE_TYPE";
    case -32:
      return "CL_INVALID_PLATFORM";
    case -33:
      return "CL_INVALID_DEVICE";
    case -34:
      return "CL_INVALID_CONTEXT";
    case -35:
      return "CL_INVALID_QUEUE_PROPERTIES";
    case -36:
      return "CL_INVALID_COMMAND_QUEUE";
    case -37:
      return "CL_INVALID_HOST_PTR";
    case -38:
      return "CL_INVALID_MEM_OBJECT";
    case -39:
      return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40:
      return "CL_INVALID_IMAGE_SIZE";
    case -41:
      return "CL_INVALID_SAMPLER";
    case -42:
      return "CL_INVALID_BINARY";
    case -43:
      return "CL_INVALID_BUILD_OPTIONS";
    case -44:
      return "CL_INVALID_PROGRAM";
    case -45:
      return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46:
      return "CL_INVALID_KERNEL_NAME";
    case -47:
      return "CL_INVALID_KERNEL_DEFINITION";
    case -48:
      return "CL_INVALID_KERNEL";
    case -49:
      return "CL_INVALID_ARG_INDEX";
    case -50:
      return "CL_INVALID_ARG_VALUE";
    case -51:
      return "CL_INVALID_ARG_SIZE";
    case -52:
      return "CL_INVALID_KERNEL_ARGS";
    case -53:
      return "CL_INVALID_WORK_DIMENSION";
    case -54:
      return "CL_INVALID_WORK_GROUP_SIZE";
    case -55:
      return "CL_INVALID_WORK_ITEM_SIZE";
    case -56:
      return "CL_INVALID_GLOBAL_OFFSET";
    case -57:
      return "CL_INVALID_EVENT_WAIT_LIST";
    case -58:
      return "CL_INVALID_EVENT";
    case -59:
      return "CL_INVALID_OPERATION";
    case -60:
      return "CL_INVALID_GL_OBJECT";
    case -61:
      return "CL_INVALID_BUFFER_SIZE";
    case -62:
      return "CL_INVALID_MIP_LEVEL";
    case -63:
      return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64:
      return "CL_INVALID_PROPERTY";
    case -65:
      return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66:
      return "CL_INVALID_COMPILER_OPTIONS";
    case -67:
      return "CL_INVALID_LINKER_OPTIONS";
    case -68:
      return "CL_INVALID_DEVICE_PARTITION_COUNT";
    case -69:
      return "CL_INVALID_PIPE_SIZE";
    case -70:
      return "CL_INVALID_DEVICE_QUEUE";
    case -71:
      return "CL_INVALID_SPEC_ID";
    case -72:
      return "CL_MAX_SIZE_RESTRICTION_EXCEEDED";

    default:
      return "UNCKNOWN ERROR";
    }
  }
}