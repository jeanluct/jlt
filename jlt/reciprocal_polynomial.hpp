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
// - Odd degree.
// - Addition, etc.
// - Print via polynomial.hpp.
//

#include <iostream>
#include <jlt/vector.hpp>
#include <jlt/polynomial.hpp>

namespace jlt {

template<class T, int g, class S = T, class P = int>
class reciprocal_polynomial
{
private:
  //
  // Polynomial is of the form
  //
  // 1 + a[0]x + a[1]x^2 + ...
  //              + a[g-1] x^g + a[g-2] x^(g-1) + ... + a[0] x^(n-1) + x^n
  //
  // Currently only for n=2g even.
  //
  // The degree is hard-coded since such polynomials cannot in general
  // be multiplied by x^m without breaking reciprocity.
  //
  vector<T> a;

public:
  typedef P		power_type;
  typedef const P	const_power_type;

  typedef T		coeff_type;
  typedef const T	const_coeff_type;
  typedef T&		coeff_reference;
  typedef const T&	const_coeff_reference;

  typedef S		arg_type;
  typedef const S	const_arg_type;
  typedef S&		arg_reference;
  typedef const S&	const_arg_reference;

  reciprocal_polynomial() : a(g) {}

  // Construct from a vector.
  reciprocal_polynomial(vector<T>& _a) : a(_a)
  {
    // Require vector _a to have length g.
    if (_a.size() != g)
      {
	std::cerr << "Wrong vector length in ";
	std::cerr << "jlt::reciprocal_polynomial(vector<>).\n";
	exit(1);
      }
  }

  // Construct from regular jlt::polynomial object.
  reciprocal_polynomial(polynomial<T,P>& _p)
  {
    // Require the polynomial _p to have matching degree.
    if (_p.degree() != 2*g)
      {
	std::cerr << "Wrong degree in ";
	std::cerr << "jlt::reciprocal_polynomial(polynomial<>).\n";
	exit(1);
      }

    // Require the polynomial _p to be reciprocal.
    for (P i = 0; i < g; ++i)
      {
	if (_p[i] != _p[2*g-i])
	  {
	    std::cerr << "Not reciprocal in ";
	    std::cerr << "jlt::reciprocal_polynomial(polynomial<>).\n";
	    exit(1);
	  }
      }

    for (P i = 0; i < g; ++i) a[i] = _p[i+1];
  }

  // Convert to regular jlt::polynomial object.
  polynomial<T,P> to_polynomial() const
  {
    polynomial<T,P> q(1);
    q[2*g] = 1;

    for (P i = 1; i <= g; ++i) q[i] = a[i-1];
    for (P i = g+1; i < 2*g; ++i) q[i] = a[2*g-1-i];

    return q;
  }

  P degree() const { return 2*g; }

  // Evaluate polynomial at a given value of x.
  S operator()(const S& x) const
  {
    // Term x^0
    S p = 1;
    S xp = x;

    // Terms x^1 to x^g
    for (P i = 0; i < g; ++i) {
      p += a[i] * xp;
      xp *= x;
    }
    // Terms x^(g+1) to x^(2g-1)
    for (P i = 0; i <= g-2; ++i) {
      p += a[g-2-i] * xp;
      xp *= x;
    }
    // Term x^(2g)
    p += xp;

    return p;
  }

  // Return a coefficient of the polynomial.
  const T operator[](const P i) const
  {
    if (i == 0 || i == 2*g) return 1;
    if (i > 0 && i <= g) return a[i-1];
    if (i > g && i < 2*g) return a[2*g-1-i];

    exit(1);
  }

  // Return a coefficient of the polynomial, allow assignment.
  T& operator[](const P i)
  {
    if (i > 0 && i <= g) return a[i-1];
    if (i > g && i <= 2*g) return a[2*g-1-i];

    exit(1);
  }

  // Return the derivative of the polynomial as a jlt::polynomial object.
  polynomial<T,P>  derivative() const
  {
    polynomial<T,P> q;

    // Terms x^0 to x^g-1
    for (P i = 0; i < g; ++i) q[i] = (i+1)*a[i];
    // Terms x^g to x^(2g-2)
    for (P i = g; i <= 2*g-2; ++i) q[i] = (i+1)*a[2*g-2-i];
    // Term x^(2g-1)
    q[2*g-1] = 2*g;

    return q;
  }

  // Return the derivative of the polynomial at a point.
  // Generalise with higher order as option?
  S derivative_at(const S& x) const
  {
    S p = 0, xp = 1;

    // Terms x^0 to x^g-1
    for (P i = 0; i < g; ++i) { p += (i+1)*a[i] * xp; xp *= x; }
    // Terms x^g to x^(2g-2)
    for (P i = g; i <= 2*g-2; ++i) { p += (i+1)*a[2*g-2-i] * xp; xp *= x; }
    // Term x^(2g-1)
    p += (2*g) * xp;

    return p;
  }

};

} // namespace jlt

#endif // JLT_RECIPROCAL_POLYNOMIAL_HPP
