//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_RECIPROCAL_POLYNOMIAL_HPP
#define JLT_RECIPROCAL_POLYNOMIAL_HPP

//
// reciprocal_polynomial.hpp
//

//
// Class to represent reciprocal polynomials that satisfy
//
//    P(x) == x^n P(1/x),
//
// where n is the degree.
//

//
// TODO
//
// - Addition, etc.
// - Print via polynomial.hpp.
//

#include <iostream>
#include <jlt/vector.hpp>
#include <jlt/polynomial.hpp>

namespace jlt {

template<class T, class P = long int>
class reciprocal_polynomial
{
private:
  //
  // Polynomial is of the form
  //
  //   1 + a[0] x + a[1] x^2 + a[g-1] x^g + a[g-2] x^(g+1) + ...
  //              ... + a[0] x^(n-1) + x^n
  //
  // for n even and g=n/2, and
  //
  //   1 + a[0] x + a[1] x^2 + ...
  //              ... + a[g-1] x^g + a[g-1] x^(g+1) + a[g-2] x^(g+2) + ...
  //              ... + a[0] x^(n-1) + x^n
  //
  // for n odd and g=(n-1)/2.
  //
  int n; 	// The degree of the polynomial.  The length of a by
                // itself is not enough to give the degree, since a
                // reciprocal polynomial of degree 2g or 2g+1 has g
                // independent coefficients.
  vector<T> a;

public:
  typedef size_t	size_type;
  typedef P		power_type;
  typedef const P	const_power_type;

  typedef T		coeff_type;
  typedef const T	const_coeff_type;
  typedef T&		coeff_reference;
  typedef const T&	const_coeff_reference;

  reciprocal_polynomial(const int _n = 0) : n(_n), a(_n/2) {}

  // Construct from a vector.
  reciprocal_polynomial(const vector<T>& _a) : n(_a.size()), a(_a) {}

  // Construct from regular jlt::polynomial object.
  reciprocal_polynomial(const polynomial<T,P>& _p) : n(_p.degree()), a(n)
  {
    const int g = n/2;

    // Require the polynomial _p to be reciprocal.
    for (P i = 0; i < g; ++i)
      {
	if (_p[i] != _p[n-i])
	  {
	    std::cerr << _p << " not reciprocal in ";
	    std::cerr << "jlt::reciprocal_polynomial(polynomial<>).\n";
	    exit(1);
	  }
	a[i] = _p[i+1];
      }
  }

  P degree() const { return n; }

  // Equate reciprocal polynomial to another.
  reciprocal_polynomial<T,P>& operator=(const reciprocal_polynomial<T,P>& p)
  {
    n = p.degree();
    a = p.a;

    return *this;
  }

  // Test for equality of two reciprocal polynomials.
  bool operator==(const reciprocal_polynomial<T,P>& p) const
  {
    return (a == p.a);
  }

  // Test for inequality of two reciprocal polynomials.
  bool operator!=(const reciprocal_polynomial<T,P>& p) const
  {
    return !(*this == p);
  }

  // Convert to regular jlt::polynomial object.
  polynomial<T,P> to_polynomial() const
  {
    const int g = n/2;

    // Term x^0
    polynomial<T,P> q(1);
    // Terms x^1 to x^g
    for (P i = 1; i <= g; ++i) q[i] = a[i-1];
    // Terms x^(g+1) to x^(n-1)
    for (P i = g+1; i < n; ++i) q[i] = a[n-1-i];
    // Term x^n
    q[n] = 1;

    return q;
  }

  // Evaluate polynomial at a given value of x.
  template<class S>
  S operator()(const S& x) const
  {
    const int g = n/2;
    S xp = x;

    // Term x^0
    S p = 1;
    // Terms x^1 to x^g
    for (P i = 1; i <= g; ++i) { p += a[i-1]*xp; xp *= x; }
    // Terms x^(g+1) to x^(n-1)
    for (P i = g+1; i < n; ++i) { p += a[n-1-i]*xp; xp *= x; }
    // Term x^n
    p += xp;

    return p;
  }

  // Return a coefficient of the polynomial.
  const T operator[](const P i) const
  {
    const int g = n/2;

    if (i == 0 || i == n) return 1;
    if (i > 0 && i <= g) return a[i-1];
    if (i > g && i < n) return a[n-1-i];

    std::cerr << "Out of range coefficient " << i << " in ";
    std::cerr << "reciprocal_polynomial::operator[] const.\n";
    exit(1);
  }

  // Return a coefficient of the polynomial, allow assignment.
  T& operator[](const P i)
  {
    const int g = n/2;

    if (i > 0 && i <= g) return a[i-1];
    if (i > g && i < n) return a[n-1-i];

    if (i == 0 || i == n)
      std::cerr << "Unassignable";
    else
      std::cerr << "Out of range";
    std::cerr << " coefficient " << i << " in ";
    std::cerr << "reciprocal_polynomial::operator[].\n";
    exit(1);
  }

  // Return the derivative of the polynomial as a jlt::polynomial object.
  polynomial<T,P> derivative() const
  {
    const int g = n/2;
    polynomial<T,P> q;

    // Terms x^0 to x^(g-1)
    for (P i = 1; i <= g; ++i) q[i-1] = i*a[i-1];
    // Terms x^g to x^(n-2)
    for (P i = g+1; i < n; ++i) q[i-1] = i*a[n-1-i];
    // Term x^(n-1)
    q[n-1] = n;

    return q;
  }

  // Return the derivative of the polynomial at a point.
  // Generalise with higher order as option?
  template<class S>
  S derivative_at(const S& x) const
  {
    const int g = n/2;
    S p = 0, xp = 1;

    // Terms x^0 to x^(g-1)
    for (P i = 1; i <= g; ++i) { p += i*a[i-1]*xp; xp *= x; }
    // Terms x^g to x^(n-2)
    for (P i = g+1; i < n; ++i) { p += i*a[n-1-i]*xp; xp *= x; }
    // Term x^(n-1)
    p += n*xp;

    return p;
  }

};

} // namespace jlt

#endif // JLT_RECIPROCAL_POLYNOMIAL_HPP
