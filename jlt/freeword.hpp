//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_FREEWORD_HPP
#define JLT_FREEWORD_HPP

//
// freeword.hpp
//

// Reduced word in a free group.

#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <list>
#include <stdexcept>

#include "jlt/exceptions.hpp"


namespace jlt {

// Remove adjacent elements satisfying pred, like unique with a predicate.
template<class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_remove_if(ForwardIterator first,
                                   ForwardIterator last,
                                   BinaryPredicate pred)
{
  if (first == last) return last;

  ForwardIterator result = first;
  while (++first != last)
    {
      if (!pred(*result,*first))
        {
          *(++result) = std::move(*first);
        }
      else
        {
          if (++first == last) return result;
          *result = std::move(*first);
        }
    }
  return ++result;
}


// Default inverse map, appropriate for signed numeric generators.
template<class T>
inline T inverse(const T& i) { return -i; }


template<class T> class freeword;
template<class T>
freeword<T> operator*(const freeword<T>&, const T&);
template<class T>
freeword<T> operator*(const T&, const freeword<T>&);
template<class T>
freeword<T> operator*(const freeword<T>&, const freeword<T>&);


template<class T>
class freeword : public std::list<T>
{
  int ngen;

public:
  // Create an empty word with known generator count.
  freeword(int ngen_ = 1) : ngen(ngen_) {}

  // Build from a generator list and infer/check the generator bound.
  freeword(std::initializer_list<T> l, int ngen_ = 0) : std::list<T>(l)
  {
    int ngen_found = 0;
    for (auto i : *this)
      {
        ngen_found = std::max(std::abs(i),ngen_found);
      }
    if (ngen_ && ngen_found > ngen_)
      {
        JLT_THROW(std::invalid_argument("freeword: ngen is too small"));
      }
    ngen = std::max(ngen_found,ngen_);
  }

  // Cancel adjacent inverse pairs until no more cancellations are possible.
  freeword<T>& reduce();

  // Return the group inverse (reverse order and invert each generator).
  freeword<T> inverse() const
  {
    freeword<T> iw(ngen);
    for (auto i = this->rbegin(); i != this->rend(); ++i) iw.push_back(-(*i));
    return iw;
  }

  friend freeword<T> operator*<>(const freeword<T>&, const T&);
  friend freeword<T> operator*<>(const T&, const freeword<T>&);
  friend freeword<T> operator*<>(const freeword<T>&, const freeword<T>&);
};


template<class T>
inline freeword<T>& freeword<T>::reduce()
{
  struct is_inverse {
    bool operator()(const T& a, const T& b) { return (a == jlt::inverse(b)); }
  } isinv;

  while (true)
    {
      auto i = adjacent_remove_if(this->begin(),this->end(),isinv);
      if (i == this->end()) break;
      this->erase(i,this->end());
    }

  return *this;
}


template<class T>
inline freeword<T> operator*(const freeword<T>& ww, const T& ee)
{
  freeword<T> ww2 = ww;
  ww2.push_back(ee);
  return ww2;
}

template<class T>
inline freeword<T> operator*(const T& ee, const freeword<T>& ww)
{
  freeword<T> ww2 = ww;
  ww2.push_front(ee);
  return ww2;
}

template<class T>
inline freeword<T> operator*(const freeword<T>& w1, const freeword<T>& w2)
{
  freeword<T> w12 = w1;
  w12.insert(w12.end(),w2.begin(),w2.end());
  return w12;
}

} // namespace jlt

#endif // JLT_FREEWORD_HPP
