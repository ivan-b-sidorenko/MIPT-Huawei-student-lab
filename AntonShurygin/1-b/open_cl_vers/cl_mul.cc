#include "cl_mul.hh"

namespace Mul_optim
{

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



void Driver::cl_mul(const Matr_int& lhs, const Matr_int& rhs)
{
  uint lhs_rows = lhs.nrows(), lhs_cols = lhs.nclmns(),
       rhs_rows = rhs.nrows(), rhs_cols = rhs.nclmns();

  
  
/*
  cl::Buffer cl_lhs_buf(context_, CL_MEM_READ_WRITE, sizeof(int) * lhs_rows * lhs_cols);
  queue_.enqueueWriteBuffer(cl_lhs_buf, CL_TRUE, 0, sizeof(int) * lhs_rows * lhs_cols, vec.data());

  cl::Buffer cl_rhs_buf(context_, CL_MEM_READ_WRITE, sizeof(int) * rhs_rows * rhs_cols);
  */

  cl_ulong gpu_time = 0;
  std::vector<cl::Event> events;

  //! Setting args for execution fast_sort_
  try
  {
    naive_mul_.setArg(0, lhs);
    naive_mul_.setArg(1, rhs);

    //! fast_sort_ execution
    if (!kernel_exec(fast_sort_, glob_size, loc_size, events))
      throw std::runtime_error{"Execution of simple_sort wasn't sucsessful!\n"};

    // event.wait();
  }
  catch (cl::Error &err)
  {
    std::cerr << "Error occured in " << err.what() << std::endl;
    std::cerr << err_what(err.err()) << std::endl;
  }
}

/*
void Driver::sort_extended(std::vector<int> &vec, Dir dir)
{
  //! Getting old size for resizing in future out vec
  size_t old_vec_size = vec.size();

  //! Resizing our vec for working with a number that is a power of two
  Vec_preparing(vec, dir);
  size_t new_vec_size = vec.size();

  //! global_size <=> number of work-items that I wish to execute
  //! vec.size() / 2 cause work item shall compare two elems
  size_t glob_size = vec.size() / 2;

  //! local_size <=> number of work-items that I wish to group into a work-group
  //! size of loc_size should be less or equal work_group
  //! This is the reason of comparing elems on this distance
  size_t loc_size = std::min(glob_size, work_group_size);

  //! Creating special buffer for working with glod memory in kernel
  cl::Buffer buffer(context_, CL_MEM_READ_WRITE, sizeof(int) * vec.size());
  queue_.enqueueWriteBuffer(buffer, CL_TRUE, 0, sizeof(int) * vec.size(), vec.data());

  //! Getting number of pairs of our resized vector
  uint num_of_pairs = std::ceil(std::log2(new_vec_size));

  uint cur_stage = std::log2(loc_size);

  //! Allocation local memory for working in fast_sort_
  cl::LocalSpaceArg local = cl::Local(2 * loc_size * sizeof(int));

  cl_ulong gpu_time = 0;
  std::vector<cl::Event> events;

  //! Setting args for execution fast_sort_
  try
  {
    fast_sort_.setArg(0, buffer);
    fast_sort_.setArg(1, cur_stage);
    fast_sort_.setArg(2, local);
    fast_sort_.setArg(3, static_cast<unsigned>(dir));

    //! fast_sort_ execution
    if (!kernel_exec(fast_sort_, glob_size, loc_size, events))
      throw std::runtime_error{"Execution of simple_sort wasn't sucsessful!\n"};

    // event.wait();
  }
  catch (cl::Error &err)
  {
    std::cerr << "Error occured in " << err.what() << std::endl;
    std::cerr << err_what(err.err()) << std::endl;
  }

  events[0].wait();

  gpu_timing(events, &gpu_time);

  /*
     There is we process all stages, which was skipped in fast_sort_,
     because of work_grp > loc_ size
  */
/*
  events.clear();

  for (; cur_stage < num_of_pairs; ++cur_stage)
  {
    for (uint passed_stage = 0; passed_stage < cur_stage + 1; ++passed_stage)
    {
      // cl::Event event_simple;
      try
      {
        //! Setting args for execution simple_sort_
        simple_sort_.setArg(0, buffer);
        simple_sort_.setArg(1, cur_stage);
        simple_sort_.setArg(2, passed_stage);
        simple_sort_.setArg(3, static_cast<unsigned>(dir));

        //! Same
        if (!kernel_exec(simple_sort_, glob_size, loc_size, events))
          throw std::runtime_error{"Execution of simple_sort wasn't sucsessful!\n"};
      }

      catch (cl::Error &err)
      {
        std::cerr << "Error occured in " << err.what() << std::endl;
        std::cerr << err_what(err.err()) << std::endl;
      }
    }
  }

  for (auto &&evnt : events)
    evnt.wait();
  // Getting sorted buf with help mapping cl::Buffer

  gpu_timing(events, &gpu_time);

  cl::copy(queue_, buffer, vec.begin(), vec.end());

#if TIME
  std::cout << "Driver gpu_time: " << gpu_time << " microsecs\n";
#endif

  vec.resize(old_vec_size);
} *//* End of 'sort_extended' function */

/**
 * @brief Function for execution kernel
 *
 * @param kernel
 * @param global_size
 * @param local_size
 * @return true
 * @return false
 */
bool Driver::kernel_exec(cl::Kernel kernel, size_t global_size, size_t local_size, std::vector<cl::Event> &events)
{
  cl::Event event;

  int err_num = queue_.enqueueNDRangeKernel(kernel, cl::NullRange, global_size, local_size, nullptr, &event);

  if (err_num != CL_SUCCESS)
    return false;

  events.push_back(event);

  return true;
} /* End of 'kernel_exec' function*/



void Driver::gpu_timing(std::vector<cl::Event> &events, cl_ulong *time)
{
#if TIME

  for (auto &&evnt : events)
  {
    auto start = evnt.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    auto end = evnt.getProfilingInfo<CL_PROFILING_COMMAND_END>();
    auto microsecs = (end - start) / 1000;
    *time += microsecs;
  }
#endif
}

} // namespace BTS