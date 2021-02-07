// matrix.hpp
// author: Antonio C. Domínguez Brito <antonio.dominguez@ulpgc.es>
// creation date: september 20th 2020
// Description: This is the header file of class matrix which is a 2D matrix

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <system_error>
#include <cmath>

using namespace std;

namespace mat_lib {

  template<typename T>
  class matrix {

    using element_t = T;

    static_assert(
      is_integral<T>::value ||
      is_floating_point<T>::value,
      "\n"
      "[ERROR] <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"
      "[ERROR] <<<< BAD TYPE <<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"
      "[ERROR] <<<< ACCEPTED TYPES <<<<<<<<<<<<<<<<<<<<<<\n"
      "[ERROR] <<<<<<< - All numeric primitive types <<<<\n"
      "[ERROR] <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"
    );

  private:
    element_t* elements__;
    size_t rows__;
    size_t columns__;

    size_t row_offset__(size_t i) const { return i*columns__; }
    size_t offset__(size_t i, size_t j) const { return row_offset__(i)+j; }

    void copy_elements__(const matrix& m)
    {
      rows__=m.rows__; columns__=m.columns__;
      for (size_t i = 0; i < rows__; i++) {
        for (size_t j = 0; j < columns__; j++) {
          elements__[offset__(i,j)] = m.elements__[offset__(i,j)];
        }
      }
    }

  public:
    matrix()
    : rows__{0},
      columns__{0},
      elements__{nullptr}
    {}

    matrix (size_t rows, size_t columns)
    : rows__{rows},
      columns__{columns},
      elements__{new element_t[rows*columns]}
    {}

    matrix(const matrix& m)
    : elements__{new element_t[m.size()]}
    {
      copy_elements__(m);
    }

    matrix(matrix&& m)
    : rows__{m.rows__},
      columns__{m.columns__},
      elements__{m.elements__}
    {
      m.elements__=nullptr;
      m.rows__=m.columns__=0;
    }

    matrix& operator=(const matrix& m) // copy assigment
    {
      if(size() != m.size()) {
        delete [] elements__;
        elements__=((m.size())? new element_t[m.size()]: nullptr);
      }
      copy_elements__(m);
      return *this;
    }

    matrix& operator=(matrix&& m) // move assigment
    {
      delete[] elements__;
      elements__=m.elements__;
      rows__ = m.rows__; columns__ = m.columns__;

      m.elements__=nullptr;
      m.rows__=m.columns__=0;

      return *this;
    }

    ~matrix () { delete [] elements__; }

    element_t at(size_t i, size_t j) const;
    size_t columns() const {return columns__;}
    size_t rows() const {return rows__;}
    size_t size() const {return rows__*columns__;}

    element_t* operator[](size_t i) { return &(elements__[row_offset__(i)]); }
    const element_t* operator[](size_t i) const { return &(elements__[row_offset__(i)]); }

    matrix& operator+=(const matrix& m);
    matrix& operator-=(const matrix& m);
    matrix& operator*=(const matrix& m);
    matrix& operator/=(const matrix& m);

    matrix& operator+=(const T v);
    matrix& operator-=(const T v);
    matrix& operator*=(const T v);
    matrix& operator/=(const T v);
    matrix& operator^=(const T v);

    matrix abs()
    {

      matrix<element_t> r{*this};

      for (size_t i = 0; i < rows__; i++)
      {
        for (size_t j = 0; j < columns__; j++)
        {
          r[i][j] = std::abs(r[i][j]);
        }
      }

      return r;

    }

    matrix min(matrix m)
    {
      if ((rows()!=m.rows()) || (columns()!=m.columns())) {
        ostringstream str_stream;
        str_stream << "size mismatch! cannot min matrices ("
          << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

        throw invalid_argument(str_stream.str());
      }

      matrix<element_t> r{*this};

      for (size_t i = 0; i < rows__; i++) {
        for (size_t j = 0; j < columns__; j++) {
          r[i][j] = std::min(r[i][j], m[i][j]);
        }
      }

      return r;
    }

  };

  template<typename T>
  matrix<T>& matrix<T>::operator+=(const matrix<T>& m)
  {
    if ((rows()!=m.rows()) || (columns()!=m.columns())) {
      ostringstream str_stream;
      str_stream << "size mismatch! cannot add matrices ("
        << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

      throw invalid_argument(str_stream.str());
    }

    for (size_t i = 0; i < m.rows(); i++) {
      for (size_t j = 0; j < m.columns(); j++) {
        elements__[offset__(i, j)] += m[i][j];
      }
    }
    return *this;
  }

  template<typename T>
  matrix<T> operator+(const matrix<T>& a, const matrix<T>& b)
  {
    if ((a.rows()!=b.rows()) || (a.columns()!=b.columns())) {
      ostringstream str_stream;
      str_stream << "size mismatch! cannot add matrices ("
        << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

      throw invalid_argument(str_stream.str());
    }

    matrix<T> c{a};
    return c+=b;
  }

  template<typename T>
  matrix<T>& matrix<T>::operator-=(const matrix<T>& m)
  {
    if ((rows()!=m.rows()) || (columns()!=m.columns())) {
      ostringstream str_stream;
      str_stream << "size mismatch! cannot add matrices ("
        << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

      throw invalid_argument(str_stream.str());
    }

    for (size_t i = 0; i < m.rows(); i++) {
      for (size_t j = 0; j < m.columns(); j++) {
        elements__[offset__(i, j)] -= m[i][j];
      }
    }
    return *this;
  }

  template<typename T>
  matrix<T> operator-(const matrix<T>& a, const matrix<T>& b)
  {
    if ((a.rows()!=b.rows()) || (a.columns()!=b.columns())) {
      ostringstream str_stream;
      str_stream << "size mismatch! cannot add matrices ("
        << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

      throw invalid_argument(str_stream.str());
    }

    matrix<T> c{a};
    return c-=b;
  }

  template<typename T>
  matrix<T>& matrix<T>::operator*=(const matrix<T>& m)
  {
    if ((rows()!=m.rows()) || (columns()!=m.columns())) {
      ostringstream str_stream;
      str_stream << "size mismatch! cannot add matrices ("
        << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

      throw invalid_argument(str_stream.str());
    }

    for (size_t i = 0; i < m.rows(); i++) {
      for (size_t j = 0; j < m.columns(); j++) {
        elements__[offset__(i, j)] *= m[i][j];
      }
    }
    return *this;
  }

  template<typename T>
  matrix<T> operator*(const matrix<T>& a, const matrix<T>& b)
  {
    if ((a.rows()!=b.rows()) || (a.columns()!=b.columns())) {
      ostringstream str_stream;
      str_stream << "size mismatch! cannot add matrices ("
        << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

      throw invalid_argument(str_stream.str());
    }

    matrix<T> c{a};
    return c*=b;
  }

  template<typename T>
  matrix<T>& matrix<T>::operator/=(const matrix<T>& m)
  {
    if ((rows()!=m.rows()) || (columns()!=m.columns())) {
      ostringstream str_stream;
      str_stream << "size mismatch! cannot add matrices ("
        << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

      throw invalid_argument(str_stream.str());
    }

    for (size_t i = 0; i < m.rows(); i++) {
      for (size_t j = 0; j < m.columns(); j++) {
        elements__[offset__(i, j)] /= m[i][j];
      }
    }
    return *this;
  }

  template<typename T>
  matrix<T> operator/(const matrix<T>& a, const matrix<T>& b)
  {
    if ((a.rows()!=b.rows()) || (a.columns()!=b.columns())) {
      ostringstream str_stream;
      str_stream << "size mismatch! cannot add matrices ("
        << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

      throw invalid_argument(str_stream.str());
    }

    matrix<T> c{a};
    return c/=b;
  }

  template<typename T>
  matrix<T> operator+(const matrix<T>& a, const T v)
  {
    matrix<T> b{a};
    return b+=v;
  }

  template<typename T>
  matrix<T>& matrix<T>::operator+=(const T v)
  {

    for (size_t i = 0; i < rows(); i++) {
      for (size_t j = 0; j < columns(); j++) {
        elements__[offset__(i, j)] += v;
      }
    }
    return *this;
  }

  template<typename T>
  matrix<T> operator-(const matrix<T>& a, const T v)
  {
    matrix<T> b{a};
    return b-=v;
  }

  template<typename T>
  matrix<T>& matrix<T>::operator-=(const T v)
  {

    for (size_t i = 0; i < rows(); i++) {
      for (size_t j = 0; j < columns(); j++) {
        elements__[offset__(i, j)] -= v;
      }
    }
    return *this;
  }

  template<typename T>
  matrix<T> operator*(const matrix<T>& a, const T v)
  {
    matrix<T> b{a};
    return b*=v;
  }

  template<typename T>
  matrix<T>& matrix<T>::operator*=(const T v)
  {

    for (size_t i = 0; i < rows(); i++) {
      for (size_t j = 0; j < columns(); j++) {
        elements__[offset__(i, j)] *= v;
      }
    }
    return *this;
  }

  template<typename T>
  matrix<T> operator/(const matrix<T>& a, const T v)
  {
    matrix<T> b{a};
    return b/=v;
  }

  template<typename T>
  matrix<T>& matrix<T>::operator/=(const T v)
  {

    for (size_t i = 0; i < rows(); i++) {
      for (size_t j = 0; j < columns(); j++) {
        elements__[offset__(i, j)] /= v;
      }
    }
    return *this;
  }

  template<typename T>
  matrix<T> operator^(const matrix<T>& a, const T v)
  {
    matrix<T> b{a};
    return b^=v;
  }

  template<typename T>
  matrix<T>& matrix<T>::operator^=(const T v)
  {

    for (size_t i = 0; i < rows(); i++) {
      for (size_t j = 0; j < columns(); j++) {
        elements__[offset__(i, j)] = pow(elements__[offset__(i, j)], v);
      }
    }
    return *this;
  }

  template<typename T>
  matrix<T> operator>(const matrix<T>& a, const T v)
  {
    matrix<T> b(a.rows(), a.columns());
    for (size_t i = 0; i < b.rows(); i++) {
      for (size_t j = 0; j < b.columns(); j++) {
        b[i][j] = a[i][j] > v;
      }
    }
    return b;
  }

  template<typename T>
  matrix<T> operator<(const matrix<T>& a, const T v)
  {
    matrix<T> b(a.rows(), a.columns());
    for (size_t i = 0; i < b.rows(); i++) {
      for (size_t j = 0; j < b.columns(); j++) {
        b[i][j] = a[i][j] < v;
      }
    }
    return b;
  }

  template<typename T>
  matrix<T> operator&(const matrix<T>& a, const matrix<T>& b)
  {
    if ((a.rows()!=b.rows()) || (a.columns()!=b.columns())) {
      ostringstream str_stream;
      str_stream << "size mismatch! cannot compare matrices ("
        << __func__ << "() in "<< __FILE__<<":"<<__LINE__<<")";

      throw invalid_argument(str_stream.str());
    }
    matrix<T> c(a.rows(), a.columns());

    for (size_t i = 0; i < a.rows(); i++) {
      for (size_t j = 0; j < a.columns(); j++) {
        c[i][j] = a[i][j] && a[i][j];
      }
    }

    return c;
  }
}

#endif
