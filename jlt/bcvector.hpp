#ifndef JLT_BCVECTOR_HPP
#define JLT_BCVECTOR_HPP

//
// bcvector.hpp
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

namespace jlt {

template<class T>
class bcvector : public std::vector<T>
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
  bcvector() : std::vector<T>() {}

  // mathvector of size _n filled with _x.
  explicit bcvector(size_type _n, const_reference _x = T())
    : std::vector<T>(_n,_x) {}

  // Copy constructor.
  bcvector(const std::vector<T>& _v) : std::vector<T>(_v) {}

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
	_THROW(std::out_of_range("Out of range exception in bcvector.\n"));

      return std::vector<T>::operator[](i);
    }

  const_reference at(size_type i) const
    {
      if (i >= size())
	_THROW(std::out_of_range("Out of range exception in bcvector.\n"));

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
};

} // namespace jlt

#endif // JLT_BCVECTOR_HPP
