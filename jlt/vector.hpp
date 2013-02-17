#ifndef JLT_VECTOR_HPP
#define JLT_VECTOR_HPP

//
// vector.hpp
//

// Bounds-checked version of std::vector.

#if defined(VECTOR_CHECK_BOUNDS)
#  if defined(__PGI)
#    include <assert.h>
#  else
#    include <cassert>
#  endif
#  define VECTOR_ASSERT(x) assert(x)
#else
#  define VECTOR_ASSERT(x)
#endif

#include <vector>
#include <iostream>
#include <jlt/stlio.hpp>
#include <jlt/exceptions.hpp>

#ifdef JLT_MATLAB_SUPPORT
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

  // Empty vector of size 0.
  vector() : std::vector<T>() {}

  // mathvector of size _n filled with _x.
  explicit vector(size_type _n, const_reference _x = T())
    : std::vector<T>(_n,_x) {}

  // Copy constructor.
  vector(const std::vector<T>& _v) : std::vector<T>(_v) {}

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
	_THROW(std::out_of_range("Out of range exception in vector.\n"));

      return std::vector<T>::operator[](i);
    }

  const_reference at(size_type i) const
    {
      if (i >= size())
	_THROW(std::out_of_range("Out of range exception in vector.\n"));

      return std::vector<T>::operator[](i);
    }

  std::vector<T>& operator=(const std::vector<T>& v)
    {
      return std::vector<T>::operator=(v);
    }

  std::ostream& printMathematicaForm(std::ostream& strm) const
    {
      if (this->empty()) return strm;

      strm << "{";
      for (typename std::vector<T>::const_iterator i = this->begin();
	   i != this->end()-1; ++i)
	{
	  strm << *i << ",";
	}
      strm << this->back() << "}";

      return strm;
    }

  std::ostream& printMatlabForm(std::ostream& strm,
				const char name[] = 0,
				const char comment[] = 0) const
    {
      // Print comment if specified.
      if (comment) strm << "% " << comment << std::endl;

      // Only print = if filename is specified.
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

#ifdef JLT_MATLAB_SUPPORT
  void printMatlabForm(MATFile *pmat, const char name[],
		       const char orientation[] = 0)
  {
    mxArray *A;
    if (this->empty())
      {
	A = mxCreateDoubleMatrix(0,0,mxREAL);
      }
    else
      {
	if (!orientation) orientation = "column";
	if (!strcmp(orientation,"column"))
	  A = mxCreateDoubleMatrix(size(),1,mxREAL);
	else if (!strcmp(orientation,"row"))
	  A = mxCreateDoubleMatrix(1,size(),mxREAL);
	else
	  {
	    std::cerr << "Bad orientation argument in ";
	    std::cerr << "vector<>::printMatlabForm.\n";
	    exit(-1);
	  }
	double *Ap = mxGetPr(A);
	for (int i = 0; i < (int)size(); ++i) Ap[i] = (*this)[i];
      }
    matPutVariable(pmat, name, A);

    mxDestroyArray(A);
}
#endif // JLT_MATLAB_SUPPORT

};

} // namespace jlt

#endif // JLT_VECTOR_HPP
