//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_MATRIX_HPP
#define JLT_MATRIX_HPP

//
// matrix.hpp
//

// General dense matrix class.

// Is there any reason to have a special class for square matrices?
// Most of the indexing stuff doesn't even require the knowledge of
// the number of rows (the way it's set up), and the memory overhead
// of an extra int is ludicrous.

// This class is not easy to specialize to sparse matrices, symmetric
// matrices, etc.

/*

  Implement:

  (Some of these are by analogy with std::vector.  See also vector.hpp)

  grow()
  back()
  reserve()
  capacity()
  assign()
  push_back(T&)
  push_back(vector<T>&)
  pop_back()

  row_iterator
  col_iterator

*/

#include <iostream>
#include <vector>
#include <string>
#include <jlt/matlab.hpp>
#include <jlt/exceptions.hpp>

#ifdef MATRIX_BOUNDS_CHECK
#  define JLT_MATRIX_CHECK_BOUNDS
#endif

#if defined(JLT_MATRIX_CHECK_BOUNDS)
#  include <cassert>
#  define JLT_MATRIX_ASSERT(x) assert(x)
#else
#  define JLT_MATRIX_ASSERT(x)
#endif

#ifdef JLT_MATLAB_LIB_SUPPORT
#  include "mat.h"
#endif

namespace jlt {

// Forward declarations.
template<typename T> class matrix;

//
// Implementation details - not part of public API
// These are internal helpers that may change without notice.
//
namespace detail {

// Cannot use printMatlabForm from matlab.hpp, since the forward
// declarations below takes precendence (for some reason) and GCC
// complains about providing default arguments.  As a workaround,
// define nodefaults:: versions, which are just meant to be called
// internally by the methods in jlt::matrix.

template<typename T> std::ostream&
printMatlabForm_nodefaults(std::ostream&, const matrix<T>&,
			   const std::string, const std::string);

#ifdef JLT_MATLAB_LIB_SUPPORT
template<typename T> void
printMatlabForm_nodefaults(MATFile *, const matrix<T>&,
			   const std::string, const std::string);
#endif

} // namespace detail


template<class T>
class matrix
{
public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

private:
  size_type	m{0}, n{0};		// Number of rows, columns.
  std::vector<T> storage;		// Storage for matrix elements

public:
  //
  // Constructors
  //

  matrix() = default;

  // Matrix of size _m*_n filled with _x.
  explicit matrix(size_type _m, size_type _n, const_reference _x = T())
    : m(_m), n(_n), storage(_m * _n, _x) {}

  matrix(const matrix<T>& _M) = default;	// Copy constructor.

  // C++11-style list initialization.
  // example: matrix(3,2,{1,2,3,4,5,6})
  matrix(size_type _m, size_type _n, std::initializer_list<T> _l)
    : m(_m), n(_n), storage(_l)
    {
      if (storage.size() != _m * _n)
        {
	  JLT_THROW
	    (std::out_of_range("Out of range exception in jlt::matrix."));
        }
    }

  // Move constructor
  matrix(matrix<T>&& _M) noexcept
    : m(_M.m), n(_M.n), storage(std::move(_M.storage))
    {
      _M.m = 0;
      _M.n = 0;
    }

  // Destructor - not needed, vector handles it

  //
  // Element access.
  //

  reference operator()(size_type i, size_type j)
    {
#ifdef JLT_MATRIX_CHECK_BOUNDS
      return at(i,j);
#else
      return storage[n*i + j];
#endif
    }

  const_reference operator()(size_type i, size_type j) const
    {
#ifdef JLT_MATRIX_CHECK_BOUNDS
      return at(i,j);
#else
      return storage[n*i + j];
#endif
    }

  reference at(size_type i, size_type j)
    {
      if (i >= m || j >= n)
	JLT_THROW(std::out_of_range("Out of range exception in jlt::matrix."));

      return storage[n*i + j];
    }

  [[nodiscard]] const_reference at(size_type i, size_type j) const
    {
      if (i >= m || j >= n)
	JLT_THROW(std::out_of_range("Out of range exception in jlt::matrix."));

      return storage[n*i + j];
    }

  // The following methods return a pointer to the beginning of row i.
  // This allows efficient A[i][j] access but has an important limitation:
  // When JLT_MATRIX_CHECK_BOUNDS is defined, only the first index (i) is checked.
  // The second index (j) cannot be bounds-checked because operator[] returns
  // a raw pointer to the row, not an object with bounds checking.

  pointer operator[](size_type i)
    {
#ifdef JLT_MATRIX_CHECK_BOUNDS
      if (i >= m)
	JLT_THROW(std::out_of_range("Out of range exception in jlt::matrix."));
#endif
      return &storage[n*i];
    }

  const_pointer operator[](size_type i) const
    {
#ifdef JLT_MATRIX_CHECK_BOUNDS
      if (i >= m)
	JLT_THROW(std::out_of_range("Out of range exception in jlt::matrix."));
#endif
      return &storage[n*i];
    }

  // data() returns a pointer to the beginning of the data.
  pointer data()
    {
      return storage.data();
    }

  // data() const returns a const_pointer to the beginning of the data.
  [[nodiscard]] const_pointer data() const
    {
      return storage.data();
    }

  [[nodiscard]] std::vector<T> row(size_type i) const
    {
#ifdef JLT_MATRIX_CHECK_BOUNDS
      if (i >= m)
	JLT_THROW(std::out_of_range("Out of range exception in jlt::matrix."));
#endif
      return std::vector<T>(storage.begin() + n*i, storage.begin() + n*i + n);
    }

  // size() returns the total number of elements.
  [[nodiscard]] constexpr size_type size() const noexcept { return m*n; }

  // dim() returns the number of columns.
  // Meant to be used with square matrices.
  [[nodiscard]] constexpr size_type dim() const noexcept { return n; }

  [[nodiscard]] constexpr size_type rows() const noexcept { return m; }
  [[nodiscard]] constexpr size_type columns() const noexcept { return n; }

  //
  // Queries
  //

  [[nodiscard]] bool empty() const noexcept { return storage.empty(); }

  [[nodiscard]] constexpr bool is_square() const noexcept { return (m == n); }

  //
  // Iterators
  //

  iterator begin() { return storage.data(); }
  [[nodiscard]] const_iterator begin() const { return storage.data(); }
  [[nodiscard]] const_iterator cbegin() const { return storage.data(); }
  iterator end() { return storage.data() + storage.size(); }
  [[nodiscard]] const_iterator end() const { return storage.data() + storage.size(); }
  [[nodiscard]] const_iterator cend() const { return storage.data() + storage.size(); }

  // row/column iterators?  Diagonal iterator?

  // Copy-and-swap assignment operator (exception-safe)
  matrix<T>& operator=(const matrix<T>& M)
    {
      if (&M != this) {
        m = M.m;
        n = M.n;
        storage = M.storage;
      }
      return *this;
    }

// Move assignment operator
matrix<T>& operator=(matrix<T>&& M) noexcept
  {
    if (&M != this) {
      m = M.m;
      n = M.n;
      storage = std::move(M.storage);
      M.m = 0;
      M.n = 0;
    }
    return *this;
  }

  //
  // Transpose
  //
  const matrix<T>& transpose()
    {
      if (rows() == columns())
	{
	  // Square: do transpose "in place"
	  for (size_type i = 0; i < rows()-1; ++i)
	    {
	      for (size_type j = i+1; j < columns(); ++j)
		{
		  T temp = (*this)(j,i);
		  (*this)(j,i) = (*this)(i,j);
		  (*this)(i,j) = temp;
		}
	    }
	}
      else
	{
	  // Not square: use temporary matrix.

	  std::cerr << "transpose(): Not implemented for nonsquare matrices yet.\n";
	}

      return *this;
    }

  //
  // Output
  //

  // The default printing style is on one line.
  std::ostream& printOn(std::ostream& strm) const
    {
      if (storage.empty()) return strm;

      for (const_iterator i = storage.data(); i != storage.data() + storage.size() - 1; ++i)
	{
	  strm << *i << "\t";
	}
      strm << *(storage.data() + storage.size() - 1);	// To avoid dangling tab.

      return strm;
    }

  std::ostream& printMatrixForm(std::ostream& strm) const
    {
      if (storage.empty()) return strm;

      for (const_iterator i = storage.data(); i != storage.data() + storage.size(); i += n) {
	for (const_iterator j = i; j != i + n - 1; ++j)
	  {
	    strm << *j << "\t";
	  }
	strm << *(i + n - 1) << std::endl;	// To avoid dangling tab.
      }

      return strm;
    }

  std::ostream& printMathematicaForm(std::ostream& strm,
				     const std::string name = "",
				     const std::string comment = "") const
    {
      if (storage.empty()) return strm;

      // Print comment if specified.
      if (!comment.empty()) strm << "(* " << comment << " *)" << std::endl;

      // Only print = if variable name is specified.
      if (!name.empty()) strm << name << " = ";

      strm << "{";
      for (const_iterator i = storage.data(); i != storage.data() + storage.size(); i += n) {
	strm << "{";
	for (const_iterator j = i; j != i + n - 1; ++j)
	  {
	    strm << *j << ",";
	  }
	if (i != storage.data() + storage.size() - n)
	  strm << *(i + n - 1) << "},";
	else
	  strm << *(i + n - 1) << "}";
      }
      // Don't append newline, since in Mathematica it is common to
      // write on same line.
      strm << "}";

      return strm;
    }

  std::ostream& printMatlabForm(std::ostream& strm,
				const std::string name = "",
				const std::string description = "") const
    {
      return detail::printMatlabForm_nodefaults(strm,*this,name,description);
    }

#ifdef JLT_MATLAB_LIB_SUPPORT
  void printMatlabForm(MATFile *pmat,
		       const std::string name = "",
		       const std::string description = "") const
  {
    detail::printMatlabForm_nodefaults(pmat,*this,name,description);
  }
#endif // JLT_MATLAB_LIB_SUPPORT

};

template<class T>
std::ostream& operator<<(std::ostream& strm, const matrix<T>& M)
{
  return (M.printOn(strm));
}

// Read M.size() = m*n elements from strm, overwriting content of M.
// Works if matrix is in row/column or single row format.
template<class T>
std::istream& operator>>(std::istream& strm, matrix<T>& M)
{
  for (auto i = M.begin(); i != M.end(); ++i)
    {
      strm >> *i;
    }

  return strm;
}

} // namespace jlt

#endif // JLT_MATRIX_HPP
