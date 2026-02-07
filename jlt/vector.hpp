//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_VECTOR_HPP
#define JLT_VECTOR_HPP

//
// vector.hpp
//

// Bounds-checked version of std::vector.

#if defined(JLT_VECTOR_CHECK_BOUNDS)
#  include <cassert>
#  define JLT_VECTOR_ASSERT(x) assert(x)
#else
#  define JLT_VECTOR_ASSERT(x)
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

// Use C++11-style argument forwarding.
template<typename... Args>
vector(Args&&... _args) : std::vector<T>(std::forward<Args>(_args)...) {}

// Forward initializer list as well.
vector(std::initializer_list<T> _l) : std::vector<T>(_l) {}

  //
  // Element access.
  //

  reference operator[](size_type i)
    {
#     ifdef JLT_VECTOR_CHECK_BOUNDS
        // Use std::vector::at() for bounds checking (throws std::out_of_range)
        return std::vector<T>::at(i);
#     else
        return std::vector<T>::operator[](i);
#     endif
    }

  const_reference operator[](size_type i) const
    {
#     ifdef JLT_VECTOR_CHECK_BOUNDS
        // Use std::vector::at() for bounds checking (throws std::out_of_range)
        return std::vector<T>::at(i);
#     else
        return std::vector<T>::operator[](i);
#     endif
    }

  // Note: at() is inherited from std::vector<T> and provides bounds checking

  std::vector<T>& operator=(const std::vector<T>& v)
    {
      return std::vector<T>::operator=(v);
    }

};

} // namespace jlt

#endif // JLT_VECTOR_HPP
