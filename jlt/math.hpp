//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_MATH_HPP
#define JLT_MATH_HPP

//
// Overloaded definitions of C math funstions.
//
// The default GNU C math functions are double precision, but the
// library always provide a single precision and a long double version
// by appending f and l to the name, respectively.
//

#include <cmath>
#include <iostream>
#include <complex>
#include <cstdlib>

namespace jlt {

//
// The Mod function differs from the fmod function from <cmath>: it
// behaves like a true mod function, where the returned value always
// has the same sign as b.  Hence, Mod(-1/3,1) = 2/3, and not -1/3.
//

template<class T, class S>
inline T Mod(const T a, const S b)
{
  std::cerr << "Mod: You cannot perform this math operation.\n";
  exit(1);
}

template<class S>
inline float Mod(const float a, const S b)
{
  return ::fmodf(::fmodf(a,b) + b,b);
}

template<class S>
inline double Mod(const double a, const S b)
{
  return ::fmod(::fmod(a,b) + b,b);
}

template<class S>
inline long double Mod(const long double a, const S b)
{
  return ::fmodl(::fmodl(a,b) + b,b);
}


template<class T>
inline int Sign(const T a)
{
  // This function never returns 0.
  return (a >= 0 ? 1 : -1);
}

} // namespace jlt

#endif // JLT_MATH_HPP
