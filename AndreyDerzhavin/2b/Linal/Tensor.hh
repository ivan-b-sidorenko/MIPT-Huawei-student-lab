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
    Tensor( void ) = default;

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

    Tensor( Tensor &&rhs ) : ch_size_(rhs.ch_size_), height_(rhs.height_), width_(rhs.width_), batches_(std::move(rhs.batches_))
    {
      rhs.batches_.clear();
    }

    Tensor &operator =( Tensor &&rhs )
    {
      if (&rhs == this)
        return *this;
      Tensor tmp = std::move(rhs);

      std::swap(tmp.batches_, batches_);
      std::swap(ch_size_, tmp.ch_size_);
      std::swap(height_, tmp.height_);
      std::swap(width_, tmp.width_);

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

    bool IsEq( const Tensor &ten ) const
    {
      if (batches_.size() != ten.batches_.size() || ch_size_ != ten.ch_size_ || height_ != ten.height_ || width_ != ten.width_)
        return false;

      for (size_t i = 0; i < batches_.size(); ++i)
        for (size_t j = 0; j < ch_size_; ++j)
          if (batches_[i][j] != ten.batches_[i][j])
            return false;

      return true;
    }

    std::size_t get_bat_size( void ) const { return batches_.size();  }
    std::size_t get_ch_size ( void ) const { return ch_size_;         }
    std::size_t get_height  ( void ) const { return height_;          }
    std::size_t get_width   ( void ) const { return width_;           }

    std::istream &Input( std::istream &ist )
    {
      size_t bhs;
      ist >> bhs >> ch_size_ >> height_ >> width_;

      Tensor tmp = {bhs, ch_size_, height_, width_, 0};
      tmp.Walker([&ist](size_t, size_t, size_t, size_t)
      {
        int val;
        ist >> val;

        return val;
      });

      std::swap(*this, tmp);

      return ist;
    }

    template <typename walker>
    void Walker( walker walk )
    {
      for (size_t i = 0, endi = batches_.size(); i < endi; ++i)
        for (size_t j = 0; j < ch_size_; ++j)
        {
          auto two_walk = [walk, i, j](size_t k, size_t w) { return walk(i, j, k, w); };
          batches_[i][j].Walker(two_walk);
        }
    }

    std::ostream &Dump( std::ostream &ost ) const
    {
      for (size_t i = 0, endi = batches_.size(); i < endi; ++i)
      {
        for (size_t j = 0; j < ch_size_; ++j)
          ost << batches_[i][j] << std::endl;
        ost << std::endl;
      }

      return ost;
    }

    ~Tensor( void ) = default;
  };

  bool operator ==( const Tensor &rhs, const Tensor &lhs )
  {
    return rhs.IsEq(lhs);
  }

  bool operator !=( const Tensor &rhs, const Tensor &lhs )
  {
    return !rhs.IsEq(lhs);
  }
}

std::istream &operator >>( std::istream &ist, linal::Tensor &tensor )
{
  return tensor.Input(ist);
}

std::ostream &operator <<( std::ostream &ost, const linal::Tensor &tensor )
{
  return tensor.Dump(ost);
}

#endif // __TENSOR_H__