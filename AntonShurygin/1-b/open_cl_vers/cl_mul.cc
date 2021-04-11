#include "cl_mul.hh"

namespace Mul_optim
{

  Matr_int cl_matr_mul(Matr_int& lhs, Matr_int& rhs)
  {
    Driver driver{};

    Matr_int mtr = driver.cl_mul(lhs, rhs);

    return mtr;
  }


/**
 * @brief
 *
 * @param vec
 * @param dir
 */

/*
void Driver(std::vector<int> &vec, Dir dir)
{
  Driver::driver().sort_extended(vec, dir);
} *//* End of 'Driver' function */

/**
 * @brief Construct a new Driver::Driver object function
 *
 */
Driver::Driver()
{
  Device_selection();

  //! Getting the size of the ND range space that can be handled by a single invocation of a kernel compute unit.

  context_ = cl::Context{device_};
  queue_ = cl::CommandQueue{context_, device_, CL_QUEUE_PROFILING_ENABLE};

  if (!build())
    throw std::runtime_error{"Building of program wasn't sucsessful!\n"};
} /* End of 'Driver' function */

/**
 * @brief Function for selecting gpu-device
 */
void Driver::Device_selection()
{
  std::vector<cl::Platform> pls;
  cl::Platform::get(&pls);

  for (auto &&pl_devs : pls)
  {
    std::vector<cl::Device> devs;
    pl_devs.getDevices(CL_DEVICE_TYPE_GPU, &devs);

    for (auto &&dev : devs)
      if (dev.getInfo<CL_DEVICE_AVAILABLE>() && dev.getInfo<CL_DEVICE_COMPILER_AVAILABLE>())
      {
        device_ = dev;
        return;
      }
  }

  throw std::runtime_error("Devices didn't find!\n");
} /* End of 'Device_selection' function */

/**
 * @brief build - helper function for constructor: creating any members of class
 *
 * @return true
 * @return false
 */
bool Driver::build()
{
  src_code_ = {
#include "cl_mul.cl"
  };

  sources_ = cl::Program::Sources{src_code_};

  prog_ = cl::Program(context_, sources_);

  try
  {
    prog_.build();
  }
  
  catch (cl::Error &error)
  {
    std::cerr << error.what() << std::endl;
    std::cerr << prog_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device_) << std::endl;
    return false;
  }


  naive_mul_ = cl::Kernel{prog_, "naive_mul"};


  return true;
} /* End of 'build' function */

/**
 * @brief sort_extended - sort, which called by user in main
 *        "extened" because it work not only with numbers is a power of two
 *
 * @param vec
 * @param dir
 */



Matr_int Driver::cl_mul(Matr_int& lhs, Matr_int& rhs)
{
  const uint lhs_rows = lhs.nrows(), /*lhs_cols = lhs.nclmns(),*/
             rhs_rows = rhs.nrows(), rhs_cols = rhs.nclmns();

  std::vector<int> lhs_buf, rhs_buf, res_buf;
  cl::NDRange glob_size = {lhs_rows, rhs_cols};
  cl::NDRange loc_size = cl::NullRange;

  res_buf.resize(lhs_rows * rhs_cols);

  lhs.vectorize(lhs_buf);
  rhs.vectorize(rhs_buf);

  cl::Buffer cl_lhs_buf{context_, lhs_buf.begin(), lhs_buf.end(), true, true};

  cl::Buffer cl_rhs_buf{context_, rhs_buf.begin(), rhs_buf.end(), true, true};

  cl::Buffer cl_res_buf(context_, CL_MEM_READ_WRITE, sizeof(int) * res_buf.size());
  
  //! Setting args for execution fast_sort_
  try
  {

    naive_mul_.setArg(0, cl_lhs_buf);
    naive_mul_.setArg(1, cl_rhs_buf);
    naive_mul_.setArg(2, cl_res_buf);

    naive_mul_.setArg(3, static_cast<unsigned>(lhs_rows));
    naive_mul_.setArg(4, static_cast<unsigned>(rhs_rows));
    naive_mul_.setArg(5, static_cast<unsigned>(rhs_cols));
  

    if (!kernel_exec(naive_mul_, glob_size, loc_size))
      throw std::runtime_error{"Execution of simple_sort wasn't sucsessful!\n"};

  }
  catch (cl::Error &err)
  {
    std::cerr << "Error occured in " << err.what() << std::endl;
    std::cerr << err_what(err.err()) << std::endl;
  }

  cl::copy(queue_, cl_res_buf, res_buf.begin(), res_buf.end());

  return Matr_int{lhs_rows, rhs_cols, res_buf};
}

/**
 * @brief Function for execution kernel
 *
 * @param kernel
 * @param global_size
 * @param local_size
 * @return true
 * @return false
 */
bool Driver::kernel_exec(cl::Kernel kernel, cl::NDRange global_size, cl::NDRange local_size)
{
  cl::Event event;

  int err_num = queue_.enqueueNDRangeKernel(kernel, cl::NullRange, global_size, local_size, nullptr, &event);

  if (err_num != CL_SUCCESS)
    return false;

  event.wait();

  gpu_timing(event);

  return true;
} /* End of 'kernel_exec' function*/



/**
 * @brief Function for timing gpu process
 * @param events - 
 */

void Driver::gpu_timing(cl::Event &event)
{
  #if TIMER

    cl_ulong time = 0;

    auto start = event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    auto end = event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
    time = (end - start) / 1000;

    std::cout << "\ngpu time naive_mul: " << time << " microsecs\n\n";
    #endif

}


} // namespace BTS