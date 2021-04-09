#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "matrix.hh"

namespace linal
{

  class Kernel final
  {
  private:
    size_t rows_, cols_;
    std::vector<Mat> layers_;
  public:

    Kernel( size_t size = 0, size_t rows = 0, size_t cols = 0, int val = {} ) : Kernel(size, rows, cols, [val](size_t, size_t){ return val; })
    {
    }

    template<typename empl_f>
    Kernel( size_t size, size_t rows, size_t cols, empl_f func ) 
          : rows_(rows), cols_(cols), layers_{size, {rows, cols, func}}
    {}

    Kernel( const Kernel & ) = default;
    Kernel &operator =( const Kernel & ) = default;

    Mat &operator []( size_t i )
    {
      return layers_[i];
    }

    const Mat &operator []( size_t i ) const
    {
      return layers_[i];
    }

    int At( size_t i, size_t j, size_t k ) const
    {
      return layers_.at(i).At(j, k);
    }

    std::size_t get_ch_amount ( void ) const { return layers_.size();  }
    std::size_t get_rows      ( void ) const { return rows_;           }
    std::size_t get_cols      ( void ) const { return cols_;           }

    std::istream &Input( std::istream &ist )
    {
      size_t chs;
      ist >> chs >> rows_ >> cols_;

      Kernel tmp = {chs, rows_, cols_, 
      [&ist](size_t, size_t)
      {
        int val;
        ist >> val;

        return val;
      }
      };

      std::swap(tmp, *this);

      return ist;
    }

    template <typename walker>
    void Walker( walker walk )
    {
      for (size_t i = 0, endi = layers_.size(); i < endi; ++i)
      {
        auto two_walk = [walk, i](size_t j, size_t k) { return walk(i, j, k); };
        layers_[i].Walker(two_walk);
      }
    }

    std::ostream &Dump( std::ostream &ost ) const
    {
      for (size_t i = 0, endi = layers_.size(); i < endi; ++i)
        ost << layers_[i] << std::endl;

      return ost;
    }

    ~Kernel( void ) = default;
  };

}

std::istream &operator >>( std::istream &ist, linal::Kernel &kern )
{
  return kern.Input(ist);
}

std::ostream &operator >>( std::ostream &ost, const linal::Kernel &kern )
{
  return kern.Dump(ost);
}

#endif // __KERNEL_H__