#ifndef __TIMER_H__
#define __TIMER_H__



#include <chrono>

namespace timer
{
  class Timer final
  {
    private:
      std::chrono::high_resolution_clock::time_point start;
    public:
      Timer( void ) : start(std::chrono::high_resolution_clock::now())
      {}

      void reset( void )
      {
        start = std::chrono::high_resolution_clock::now();
      }

      auto elapsed_ms( void )
      {
        auto end = std::chrono::high_resolution_clock::now();

        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
      }

      auto elapsed_mcs( void )
      {
        auto end = std::chrono::high_resolution_clock::now();

        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
      }

  };
}

#endif // __TIMER_H__