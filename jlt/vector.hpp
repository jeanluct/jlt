//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_VECTOR_HPP
#define JLT_VECTOR_HPP

//
// vector.hpp
//

// Bounds-checked version of std::vector.

#if defined(VECTOR_CHECK_BOUNDS)
#  include <cassert>
#  define VECTOR_ASSERT(x) assert(x)
#else
#  define VECTOR_ASSERT(x)
#endif

#include <vector>
#include <iostream>
#include <jlt/stlio.hpp>
#include <jlt/exceptions.hpp>

#ifdef JLT_MATLAB_LIB_SUPPORT
#  include <jlt/matlab.hpp>
#  include <cstring> // for strcmp
#endif


namespace jlt {

template<class T>
class vector : public std::vector<T>
{
public:
  typedef typename std::vector<T>::size_type		size_type;
  typedef typename std::vector<T>::reference		reference;
  typedef typename std::vector<T>::const_reference	const_reference;

  using std::vector<T>::size;

  //
  // Constructors
  //

#if __cplusplus > 199711L
  // Use C++11-style argument forwarding.
  template<typename... Args>
  vector(Args&&... _args) : std::vector<T>(std::forward<Args>(_args)...) {}

  // Forward initializer list as well.
  vector(std::initializer_list<T> _l) : std::vector<T>(_l) {}
#else
  // Empty vector of size 0.
  vector() : std::vector<T>() {}

  // mathvector of size _n filled with _x.
  explicit vector(size_type _n, const_reference _x = T())
    : std::vector<T>(_n,_x) {}

  // Copy constructor.
  vector(const std::vector<T>& _v) : std::vector<T>(_v) {}
#endif

  //
  // Element access.
  //

  reference operator[](size_type i)
    {
#     ifdef VECTOR_CHECK_BOUNDS
        return at(i);
#     else
        return std::vector<T>::operator[](i);
#     endif
    }

  const_reference operator[](size_type i) const
    {
#     ifdef VECTOR_CHECK_BOUNDS
        return at(i);
#     else
        return std::vector<T>::operator[](i);
#     endif
    }

  // Bound-checked
  reference at(size_type i) 
    {
      if (i >= size())
	JLT_THROW(std::out_of_range("Out of range exception in jlt::vector."));

      return std::vector<T>::operator[](i);
    }

  const_reference at(size_type i) const
    {
      if (i >= size())
	JLT_THROW(std::out_of_range("Out of range exception in jlt::vector."));

      return std::vector<T>::operator[](i);
    }

  std::vector<T>& operator=(const std::vector<T>& v)
    {
      return std::vector<T>::operator=(v);
    }

  std::ostream& printMathematicaForm(std::ostream& strm,
				     const char name[] = 0,
				     const char comment[] = 0) const
    {
      if (this->empty()) return strm;

      // Print comment if specified.
      if (comment) strm << "(* " << comment << " *)" << std::endl;

      // Only print = if variable name is specified.
      if (name) strm << name << " = ";

      strm << "{";
      for (typename std::vector<T>::const_iterator i = this->begin();
	   i != this->end()-1; ++i)
	{
	  strm << *i << ",";
	}
      // Don't append newline, since in Mathematica it is common to
      // write on same line.
      strm << this->back() << "}";

      return strm;
    }

  std::ostream& printMatlabForm(std::ostream& strm,
				const char name[] = 0,
				const char comment[] = 0) const
    {
      if (this->empty()) return strm;

      // Print comment if specified.
      if (comment) strm << "% " << comment << std::endl;

      // Only print = if variable name is specified.
      if (name) strm << name << " = ";

      // If the vector is empty, just print "[];"
      if (this->empty()) { strm << "[];\n"; return strm; }

      strm << "[\n";

      for (typename std::vector<T>::const_iterator i = this->begin();
	   i != this->end(); ++i)
	{
	  strm << " " << *i << std::endl;
	}
      strm << "];\n";

      return strm;
    }

#ifdef JLT_MATLAB_LIB_SUPPORT
  void printMatlabForm(MATFile *pmat, const char name[],
		       const char orientation[] = 0) const
  {
    // orientation is either "row" or "column" (default).
    //
    // If orientation is neither then eventually it will be used as a
    // description string.  Also introduce at final description[]
    // argument.
    //
    // The optional description string is currently ignored.  It is
    // included for compatibility with printMatlabForm(std::ostream).
    // In the future, could write to MAT file a name_descr string.
    mxArray *A;
    if (this->empty())
      {
	A = mxCreateDoubleMatrix(0,0,mxREAL);
      }
    else
      {
	if (!orientation)
	  {
	    // Default is a column vector.
	    A = mxCreateDoubleMatrix(size(),1,mxREAL);
	  }
	else
	  {
	    if (!strcmp(orientation,"column"))
	      A = mxCreateDoubleMatrix(size(),1,mxREAL);
	    else if (!strcmp(orientation,"row"))
	      A = mxCreateDoubleMatrix(1,size(),mxREAL);
	    else
	      {
		// The orientation string is neither "column" nor
		// "row", and is thus assumed to be a comment, which
		// are ignored currently.  The default is then a
		// column vector.
		A = mxCreateDoubleMatrix(size(),1,mxREAL);
	      }
	  }
	double *Ap = mxGetPr(A);
	for (int i = 0; i < (int)size(); ++i) Ap[i] = (*this)[i];
      }
    matPutVariable(pmat, name, A);

    mxDestroyArray(A);
}
#endif // JLT_MATLAB_LIB_SUPPORT

};

} // namespace jlt

#endif // JLT_VECTOR_HPP
