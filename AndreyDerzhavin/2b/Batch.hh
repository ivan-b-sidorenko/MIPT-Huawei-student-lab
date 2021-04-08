#ifndef __BATCH_H__
#define __BATCH_H__

#include <vector>
#include "matrix.hh"

namespace linal
{
  class Batch final
  {
  private:
    std::size_t height_, width_;
    std::vector<Matrix<int>> channels_;
  public:
    Batch( std::size_t ch_size, std::size_t height, std::size_t width, int val = {} ) 
          : Batch(ch_size, height, width, [val]( std::size_t, std::size_t ){return val;})
    {}

    template <typename empl_func>
    Batch( std::size_t ch_size, std::size_t height, std::size_t width, empl_func fn )
          : height_(height), width_(width), channels_(ch_size, Matrix<int>{height, width, fn})
    {}

    Batch( const Batch &rhs ) = default;
    Batch &operator =( const Batch &rhs ) = default;

    Batch( Batch &&rhs ) : channels_(std::move(rhs.channels_))
    {
      rhs.channels_.clear();
    }

    Batch &operator =( Batch &&rhs )
    {
      Batch tmp = std::move(rhs);

      std::swap(tmp, *this);

      return *this;
    }

    int At( std::size_t i, std::size_t j, std::size_t k ) const
    {
      if (i >= channels_.size())
        throw std::out_of_range{"Channel index is too big"};
      
      return channels_[i].At(j, k);
    }

    const Matrix<int> &operator []( std::size_t i ) const
    {      
      return channels_[i];
    }

    Matrix<int> &operator []( std::size_t i )
    {      
      return channels_[i];
    }
    
    std::size_t get_ch_size( void ) const { return channels_.size(); } 
    std::size_t get_height ( void ) const { return height_;          } 
    std::size_t get_width  ( void ) const { return width_;           } 

    ~Batch( void ) = default;
  };
}


#endif // __BATCH_H__