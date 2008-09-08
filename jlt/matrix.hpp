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

  (Some of these are by analogy with std::vector.  See my
  implementation, Vec)

  grow()
  empty()
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
#include <jlt/exceptions.hpp>

#ifdef MATRIX_BOUNDS_CHECK
#  define MATRIX_CHECK_BOUNDS
#endif

#if defined(MATRIX_CHECK_BOUNDS)
#  if defined(__PGI)
#    include <assert.h>
#  else
#    include <cassert>
#  endif
#  define MATRIX_ASSERT(x) assert(x)
#else
#  define MATRIX_ASSERT(x)
#endif


namespace jlt {

template<class T>
class matrix
{
public:
  typedef T			value_type;	
  typedef T*			pointer;
  typedef const T*		const_pointer;
  typedef pointer		iterator;
  typedef const_pointer		const_iterator;
  typedef T&			reference;
  typedef const T&		const_reference;
  typedef size_t		size_type;

private:
  pointer	start;
  pointer	finish;
  size_type	m, n;		// Number of rows, columns.

public:
  //
  // Constructors
  //

  matrix() : start(0), finish(0), m(0), n(0) {}

  // Matrix of size _m*_n filled with _x.
  explicit matrix(size_type _m, size_type _n, const_reference _x = T())
    : m(_m), n(_n)
    {
      size_type mn = m*n;

      start = new T[mn];
      finish = start + mn;

      for (iterator i = start; i != finish; ++i) *i = _x;
    }

  matrix(const matrix<T>& _M)	: m(_M.m), n(_M.n)	// Copy constructor.
    {
      size_type mn = _M.size();

      start = new T[mn];
      finish = start + mn;

      iterator j = start;
      const_iterator i = _M.start;
      while (j != finish) *j++ = *i++;
    }

  // Destructor
  ~matrix() { if (start != 0) delete[] start; }

  //
  // Element access.
  //

  reference operator()(size_type i, size_type j)
    {
#ifdef MATRIX_CHECK_BOUNDS
      return at(i,j);
#else
      return *(start + n*i + j);
#endif
    }

  const_reference operator()(size_type i, size_type j) const
    {
#ifdef MATRIX_CHECK_BOUNDS
      return at(i,j);
#else
      return *(start + n*i + j);
#endif
    }

  reference at(size_type i, size_type j)
    {
      if (i >= m || j >= n)
	_THROW(std::out_of_range("Out of range exception in matrix.\n"));

      return *(start + n*i + j);
    }

  const_reference at(size_type i, size_type j) const
    {
      if (i >= m || j >= n)
	_THROW(std::out_of_range("Out of range exception in matrix.\n"));

      return *(start + n*i + j);
    }

  // The followind methods return a pointer.  Could return a Vec, but
  // that would be slower and would force Mat to work with Vec.
  // However, with pointers no bounds checking can be done on the
  // second square bracket in A[i][j].

  pointer operator[](size_type i)
    {
#ifdef MATRIX_CHECK_BOUNDS
      static bool only_once = true;
      if (only_once) {
	std::cerr << "Warning: no bounds checking done on second";
	std::cerr << " argument of [][] in matrix.\n";
	only_once = false;
      }
      if (i >= m)
	_THROW(std::out_of_range("Out of range exception in matrix.\n"));
#endif
      return (start + n*i);
    }

  const_pointer operator[](size_type i) const
    {
#ifdef MATRIX_CHECK_BOUNDS
      static bool only_once = true;
      if (only_once) {
	std::cerr << "Warning: no bounds checking done on second";
	std::cerr << " argument of [][] in matrix.\n";
	only_once = false;
      }
      if (i >= m)
	_THROW(std::out_of_range("Out of range exception in matrix.\n"));
#endif
      return (start + n*i);
    }

  // data() returns a pointer to the beginning of the data.
  pointer data()
    {
      return start;
    }

  // data() const returns a const_pointer to the beginning of the data.
  const_pointer data() const
    {
      return start;
    }

  std::vector<T> row(size_type i) const
    {
#ifdef MATRIX_CHECK_BOUNDS
      if (i >= m)
	_THROW(std::out_of_range("Out of range exception in matrix.\n"));
#endif
      return std::vector<T>(start + n*i,start + n*i + m);
    }

  // size() returns the total number of elements.
  size_type size() const { return m*n; }

  // dim() returns the number of columns.
  // Meant to be used with square matrices.
  size_type dim() const { return n; }
  // size_type dim() const { MATRIX_ASSERT(m=n); return n; }

  size_type rows() const { return m; }		// Number of rows.
  size_type columns() const { return n; }	// Number of columns.

  //
  // Queries
  //

  bool isSquare() const { return (m == n); }

  //
  // Iterators
  //

  iterator begin() { return iterator(start); }
  const_iterator begin() const { return iterator(start); }
  iterator end() { return iterator(finish); }
  const_iterator end() const { return iterator(finish); }

  // row/column iterators?  Diagonal iterator?

  matrix<T>& operator=(const matrix<T>& M)
    {
      if (&M == this) return *this;

      m = M.rows();
      n = M.columns();
      size_type mn = size();

      // Free the matrix if not empty.
      if (start != 0) delete[] start;

      start = new T[mn];
      finish = start + mn;

      iterator j = start;
      const_iterator i = M.start;
      while (j != finish) *j++ = *i++;

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
      if (start == 0) return strm;

      for (const_iterator i = start; i != finish-1; ++i)
	{
	  strm << *i << "\t";
	}
      strm << *(finish-1); 	// To avoid dangling tab.

      return strm;
    }

  std::ostream& printMatrixForm(std::ostream& strm) const
    {
      if (start == 0) return strm;

      for (const_iterator i = start; i != finish; i += n) {
	for (const_iterator j = i; j != i+n-1; ++j)
	  {
	    strm << *j << "\t";
	  }
	strm << *(i+n-1) << std::endl; 	// To avoid dangling tab.
      }

      return strm;
    }

  std::ostream& printMathematicaForm(std::ostream& strm) const
    {
      if (start == 0) return strm;

      strm << "{";
      for (const_iterator i = start; i != finish; i += n) {
	strm << "{";
	for (const_iterator j = i; j != i+n-1; ++j)
	  {
	    strm << *j << ",";
	  }
	if (i != finish-n)
	  strm << *(i+n-1) << "},";
	else
	  strm << *(i+n-1) << "}";
      }
      strm << "}";

      return strm;
    }
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
  for (typename matrix<T>::iterator i = M.begin(); i != M.end(); ++i)
    {
      strm >> *i;
    }

  return strm;
}

} // namespace jlt

#endif // JLT_MATRIX_HPP
