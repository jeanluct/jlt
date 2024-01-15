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
#include <string>
#include <jlt/matlab.hpp>
#include <jlt/stlio.hpp>
#include <jlt/exceptions.hpp>


namespace jlt {

template<class T>
class vector : public std::vector<T>
{
public:
  typedef typename std::vector<T>::size_type		size_type;
  using reference = typename std::vector<T>::reference;
  using const_reference = typename std::vector<T>::const_reference;

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

  [[nodiscard]] const_reference at(size_type i) const
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
				     const std::string name = "",
				     const std::string description = "") const
    {
      if (this->empty()) return strm;

      // Print description as comment if specified.
      if (!description.empty())
	strm << "(* " << description << " *)" << std::endl;

      // Only print = if variable name is specified.
      if (!name.empty()) strm << name << " = ";

      strm << "{";
      for (auto i = this->cbegin(); i != this->cend()-1; ++i)
	{
	  strm << *i << ",";
	}
      // Don't append newline, since in Mathematica it is common to
      // write on same line.
      strm << this->back() << "}";

      return strm;
    }

  std::ostream& printMatlabForm(std::ostream& strm,
				const std::string name = "",
				const std::string description = "") const
    {
      return jlt::printMatlabForm(strm,*this,name,description);
    }

#ifdef JLT_MATLAB_LIB_SUPPORT
  void printMatlabForm(MATFile *pmat,
		       const std::string name,
		       const std::string description = "",
		       const std::string orientation = "") const
    {
      jlt::printMatlabForm(pmat,*this,name,description,orientation);
    }
#endif // JLT_MATLAB_LIB_SUPPORT

};

} // namespace jlt

#endif // JLT_VECTOR_HPP
