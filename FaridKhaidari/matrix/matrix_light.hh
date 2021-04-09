#ifndef MX_L_HH
#define MX_L_HH

#include <cmath>
#include <cassert>
#include <iomanip>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <initializer_list>

namespace MXL
{
  template<typename T>
  bool is_zero(const T & val)
  {return (val == 0);}

  template<>
  bool is_zero(const double & val)
  {
    return val < 1e-12;
  }

  template<>
  bool is_zero(const float & val)
  {
    return val < 1e-12;
  }

  template<typename T>
  class Matrix
  {
  private:
    
    size_t rows_;
    size_t cols_;

    T ** arr_;

  public:

    /* ctors */

    Matrix(size_t rows = 0, size_t cols = 0);

    Matrix(const Matrix &orig);

    template <typename It> 
    Matrix(size_t rows, size_t cols, It beg, It end);

    template <typename Func>
    Matrix(size_t rows, size_t cols, Func action);

    Matrix &operator=(const Matrix &orig);

    /* swap */
    void swap(Matrix & orig) noexcept;

    /* getters and setters for element */
    const T * operator[](size_t row) const;
    T * operator[](size_t row);

    const T &get(size_t row, size_t col) const;
    void set(size_t row, size_t col, T val);

    /* getters and setters for size */
    size_t cols( ) const;
    size_t rows( ) const;
    T ** arr( ) const;

    /* tr */
    Matrix &transpose() &;

    /* comparison */
    bool operator==(const Matrix &matr) const;
    bool operator!=(const Matrix &matr) const;
  };

  template <typename T>
  Matrix<T> transpose(const Matrix<T> matr);

  template <typename T>
  Matrix<T> operator*( const Matrix<T> & lhs, const Matrix<T> & rhs );












  template <typename T>
  Matrix<T>::Matrix(size_t rows /* = 0*/, size_t cols /* = 0 */) : rows_(rows), cols_(cols)
  {
    arr_ = new T* [rows_] {};

    for (size_t i = 0; i < rows_; ++i)
      arr_[i] = new T [cols_] {};
  }

  template <typename T>
  Matrix<T>::Matrix(const Matrix &orig) : Matrix(orig.rows_, orig.cols_)
  {
    for (size_t i = 0; i < rows_; ++i)
      std::copy(orig.arr_[i], orig.arr_[i] + cols_, arr_[i]);
  }

  template <typename T>
  template <typename It> 
  Matrix<T>::Matrix(size_t rows, size_t cols, It beg, It end) : Matrix(rows, cols)
  {
    for (size_t i = 0; i < rows_, beg != end; ++i)
      for (size_t j = 0; j < cols_, beg != end; ++j, ++beg)
        arr_[i][j] = *beg;
  }

  template <typename T>
  template <typename Func> 
  Matrix<T>::Matrix(size_t rows, size_t cols, Func action) : Matrix(rows, cols)
  {
    for (size_t i = 0; i < rows_; ++i)
      for (size_t j = 0; j < cols_; ++j)
        arr_[i][j] = action(i, j);
  }

  template <typename T>
  void Matrix<T>::swap(Matrix<T> & orig) noexcept
  {
    std::swap(arr_, orig.arr_);
    std::swap(rows_, orig.rows_);
    std::swap(cols_, orig.cols_);
  }

  template <typename T>
  const T & Matrix<T>::get(size_t row, size_t col) const
  {
    if (row >= rows_)
      throw std::runtime_error{"Row too big in method get"};
    else if (col >= cols_)
      throw std::runtime_error{"Col too big in method get"};
    
    return arr_[row][col];
  }

  template <typename T>
  void Matrix<T>::set(size_t row, size_t col, T val)
  {
    if (row >= rows_)
      throw std::runtime_error{"Row too big in method set"};
    else if (col >= cols_)
      throw std::runtime_error{"Col too big in method set"};
    
    arr_[row][col] = val;
  }

  template <typename T>
  const T * Matrix<T>::operator[](size_t row) const
  {
    if (row >= rows_)
      throw std::runtime_error{"Row too big in const operator[]"};
    return arr_[row];
  }

  template <typename T>
  T * Matrix<T>::operator[](size_t row)
  {
    if (row >= rows_)
      throw std::runtime_error{"Row too big in operator[]"};
    return arr_[row];
  }

  template <typename T>
  size_t Matrix<T>::cols( ) const
  {return cols_;}

  template <typename T>
  size_t Matrix<T>::rows( ) const
  {return rows_;}

  template <typename T>
  Matrix<T> & Matrix<T>::transpose() &
  {
    Matrix<T> tmp{cols_, rows_};

    for (size_t i = 0; i < rows_; ++i)
      for (size_t j = 0; j < cols_; ++j)
        tmp[j][i] = arr_[i][j];

    swap(tmp);
    return *this;
  }

  template <typename T>
  Matrix<T> & Matrix<T>::operator=(const Matrix &orig)
  {
    Matrix<T> tmp{orig};
    swap(tmp);
    return *this;
  }

  template <typename T>
  bool Matrix<T>::operator==(const Matrix &matr) const
  {
    if ((matr.rows_ != rows_) || (matr.cols_ != cols_))
      return false;

    for (size_t i = 0; i < rows_; ++i)
      for (size_t j = 0; j < cols_; ++j)
        if (!is_zero(arr_[i][j] - matr[i][j]))
        {
          std::cerr << std::endl << "(i, j) = (" << i << ", " << j << ")" << std::endl;
          return false;
        }
    
    return true;
  }

  template <typename T>
  bool Matrix<T>::operator!=(const Matrix &matr) const
  {
    return !(operator==(matr));
  }

  template <typename T>
  T ** Matrix<T>::arr( ) const
  {
    return arr_;
  }


    template <typename T>
  Matrix<T> transpose(const Matrix<T> matr)
  {
    Matrix<T> tmp{matr};
    return tmp.transpose();
  }
  
  template <typename T>
  Matrix<T> operator*( const Matrix<T> & lhs, const Matrix<T> & rhs )
  {
    size_t lhs_r = lhs.rows(),
           lhs_c = lhs.cols(),
           rhs_c = rhs.cols();
    
    Matrix<T> tmp{lhs_r, rhs_c};

    for (size_t i = 0; i < lhs_r; ++i)
      for (size_t j = 0; j < rhs_c; ++j)
        for (size_t k = 0; k < lhs_c; ++k)
          tmp[i][j] += lhs[i][k] * rhs[k][j];

    return tmp;
  }

  template <typename T> std::ostream &operator<<(std::ostream &ost, const Matrix<T> &matr)
  {
    ost << "   | ";
    for (size_t i = 0, cols = matr.cols(); i < cols; ++i)
      ost << std::setw(5) << i;

    ost << "|" << std::endl;

    ost << "   +-";
    for (size_t i = 0, cols = matr.cols(); i < cols; ++i)
      ost << "-----";

    ost << "+" << std::endl;

    for (size_t i = 0, cols = matr.cols(), rows = matr.rows(); i < rows; ++i)
    {
      ost << std::setw(3) << i << "| ";

      for (size_t j = 0; j < cols; ++j)
        ost << std::setw(4) << matr[i][j] << ";";

      ost << "|" << std::endl;
    }

    return ost;
  }
}

#endif // MX_L_HH