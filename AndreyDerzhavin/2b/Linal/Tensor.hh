#ifndef __TENSOR_H__
#define __TENSOR_H__

#include "Batch.hh"
#include "Kernel.hh"

namespace linal
{
  class Tensor final
  {
  private:
    std::size_t ch_size_, height_, width_;
    std::vector<Batch> batches_;
  public:
    Tensor( std::size_t bh_size, std::size_t ch_size, std::size_t height, 
            std::size_t width, int val = {} ) 
          : Tensor(bh_size, ch_size, height, width, [val]( std::size_t, std::size_t ){return val;})
    {}

    template <typename empl_func>
    Tensor( std::size_t bh_size, std::size_t ch_size, std::size_t height, 
            std::size_t width, empl_func fn )
          : ch_size_(ch_size), height_(height), width_(width), 
            batches_(bh_size, {ch_size, height, width, fn})
    {}

    Tensor( const Tensor &rhs ) = default;
    Tensor &operator =( const Tensor &rhs ) = default;

    Tensor( Tensor &&rhs ) : batches_(std::move(rhs.batches_))
    {
      rhs.batches_.clear();
    }

    Tensor &operator =( Tensor &&rhs )
    {
      Tensor tmp = std::move(rhs);

      std::swap(tmp, *this);

      return *this;
    }

    int At( std::size_t n, std::size_t c, std::size_t h, std::size_t w ) const
    {
      if (n >= batches_.size())
        throw std::out_of_range{"Batch index is too big"};
      
      return batches_[n].At(c, h, w);
    }

    const Batch &operator []( std::size_t i ) const
    {      
      return batches_[i];
    }

    Batch &operator []( std::size_t i )
    {      
      return batches_[i];
    }

    std::size_t get_bat_size( void ) const { return batches_.size();  }
    std::size_t get_ch_size ( void ) const { return ch_size_;         }
    std::size_t get_height  ( void ) const { return height_;          }
    std::size_t get_width   ( void ) const { return width_;           }

    ~Tensor( void ) = default;
  };

}

#endif // __TENSOR_H__