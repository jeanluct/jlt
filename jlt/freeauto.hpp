//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_FREEAUTO_HPP
#define JLT_FREEAUTO_HPP

//
// freeauto.hpp
//

// Automorphism acting on a free group.

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "jlt/exceptions.hpp"
#include "jlt/freeword.hpp"


namespace jlt {

template<class T>
class freeauto : std::vector<freeword<T>>
{
public:
  // Create identity automorphism on ngen generators.
  explicit freeauto(int ngen_) : std::vector<freeword<T>>(ngen_ + 1)
  {
    for (std::size_t i = 1; i <= numgens(); ++i)
      std::vector<freeword<T>>::operator[](i).push_back(static_cast<T>(i));
  }

  // Construct from explicit images of positive generators.
  freeauto(const std::vector<freeword<T>>& a) : std::vector<freeword<T>>(a)
  {
  }

  // Number of positive generators represented by this automorphism.
  std::size_t numgens() const { return (this->size() - 1); }

  // Mutable image of a positive generator.
  freeword<T>& operator[](const T a)
  {
    if (a == 0)
      {
        JLT_THROW(std::invalid_argument(
            "freeauto: cannot specify action on identity"));
      }
    if (a < 0)
      {
        JLT_THROW(std::invalid_argument(
            "freeauto: assign to positive generators only"));
      }
    return std::vector<freeword<T>>::operator[](a);
  }

  // Read-only image of a positive generator.
  const freeword<T>& operator[](const T a) const
  {
    if (a == 0)
      {
        JLT_THROW(std::invalid_argument(
            "freeauto: cannot specify action on identity"));
      }
    if (a < 0)
      {
        JLT_THROW(std::invalid_argument(
            "freeauto: assign to positive generators only"));
      }
    return std::vector<freeword<T>>::operator[](a);
  }

  // Image of any nonzero generator, extending to negatives by inversion.
  freeword<T> get_action(const T a) const
  {
    if (a == 0)
      {
        JLT_THROW(std::invalid_argument(
            "freeauto: cannot specify action on identity"));
      }
    if (a > 0)
      return std::vector<freeword<T>>::operator[](a);

    return (std::vector<freeword<T>>::operator[](-a)).inverse();
  }

  // Compose automorphisms: this = this followed by a.
  freeauto<T>& operator*=(const freeauto<T>& a)
  {
    if (numgens() != a.numgens())
      {
        JLT_THROW(std::invalid_argument(
            "freeauto: can only compose with same number of generators"));
      }

    freeauto<T> res(std::vector<freeword<T>>(numgens() + 1));
    for (std::size_t i = 1; i <= numgens(); ++i)
      {
        for (auto j : this->operator[](static_cast<T>(i)))
          {
            res[static_cast<T>(i)] =
              res[static_cast<T>(i)] * a.get_action(j);
          }
      }
    return (*this = res);
  }
};


template<class T>
inline freeauto<T> operator*(const freeauto<T>& a, const freeauto<T>& b)
{
  return (freeauto<T>(a) *= b);
}

template<class T>
std::ostream& operator<<(std::ostream& strm, const freeauto<T>& a)
{
  constexpr int wid = 5;

  for (std::size_t i = 1; i <= a.numgens(); ++i)
    {
      strm << std::setw(wid);
      strm << i << " -> ";
      for (auto j : a[static_cast<T>(i)]) strm << std::setw(wid) << j;
      strm << std::endl;
    }
  return strm;
}

} // namespace jlt

#endif // JLT_FREEAUTO_HPP
