#ifndef JLT_POLYNOMIAL_HPP
#define JLT_POLYNOMIAL_HPP

//
// polynomial.hpp
//

#include <iostream>
#include <string>
#include <jlt/math.hpp>
#include <jlt/vector.hpp>


namespace jlt {

//
// Declare class and function templates
//

template<typename T>
T operator+(const T&);

template<typename T>
T operator-(const T&);

template<typename T>
T operator+(const T&, const T&);

template<typename T>
T operator-(const T&, const T&);

template<typename T, typename S>
T operator*(const S&, const T&);

template<typename T, typename S>
T operator*(const T&, const S&);

template<typename T, typename S>
T operator/(const T&, const S&);

template<typename T>
T operator*(const T&, const T&);

//
// class polynomial
//

template<class T, class P = long int>
class polynomial
{
private:
  P pmin, pmax;
  size_t cap;
  static const size_t initial_cap = 50;

  vector<T> coeff;

public:
  typedef size_t	size_type;
  typedef P		power_type;
  typedef const P	const_power_type;

  typedef T		coeff_type;
  typedef const T	const_coeff_type;
  typedef T&		coeff_reference;
  typedef const T&	const_coeff_reference;

  //
  // Constructors
  //

  polynomial(const_coeff_type _coeff = 0, const_power_type _pow = 0) :
    pmin(_pow), pmax(_pow), cap(initial_cap)
  {
    coeff.resize(cap); /* Is this the best thing to do? */
			      /* I think it would be preferable to let the vector manage this. */
    coeff[0] = _coeff;
  }

  // Copy constructor.
  polynomial(const polynomial<T,P>& p) :
    pmin(p.pmin), pmax(p.pmax), coeff(p.coeff) {}

  //
  // Member Operators
  //

  // Equate polynomial to another.
  polynomial<T,P>& operator=(const polynomial<T,P>& p)
  {
    pmin = p.pmin;
    pmax = p.pmax;

    coeff = p.coeff;

    return *this;
  }

  // Equate polynomial to a scalar.
  polynomial<T,P>& operator=(const T& a)
  {
    for (P i = 0; i <= pmax-pmin; ++i) coeff[i] = 0;

    pmin = pmax = 0;

    coeff[0] = a;

    return *this;
  }

  // Add a polynomial.
  polynomial<T,P>& operator+=(const polynomial<T,P>& p)
  {
    return add_polynomial(p);
  }

  // Add a constant.
  polynomial<T,P>& operator+=(const T& a)
  {
    return add_to_coeff(0,a);
  }

  // Subtract polynomial.
  polynomial<T,P>& operator-=(const polynomial<T,P>& p)
  {
    return add_polynomial_X_monomial(p,0,-1);
  }

  // Subtract a constant.
  polynomial<T,P>& operator-=(const T& a)
  {
    return add_to_coeff(0,-a);
  }

  // Multiply by a polynomial.
  polynomial<T,P>& operator*=(const polynomial<T,P>& p)
  {
    return multiply_by_polynomial(p);
  }

  // Multiply by a constant.
  polynomial<T,P>& operator*=(const T& a)
  {
    return multiply_by_monomial(0,a);
  }

  // Divide by a constant.
  polynomial<T,P>& operator/=(const T& a)
  {
    return multiply_by_monomial(0,(T)1/a);
  }

  // Test for equality of two polynomials.
  bool operator==(const polynomial<T,P>& p) const
  {
    if (pmin != p.pmin || pmax != pmax) return false;

    for (P i = 0; i <= pmax-pmin; ++i) {
      if (coeff[i] != p.coeff[i]) return false;
    }

    return true;
  }

  // Test for inequality of two polynomials.
  bool operator!=(const polynomial<T,P>& p) const
  {
    return !(*this == p);
  }


  //
  // Elementary Polynomial Operations
  //
  
  polynomial<T,P>&
  add_to_coeff(const_power_type n, const_coeff_reference c)
  {
    if (n >= pmin && n <= pmax) {
      return set_coeff(n, coeff[n-pmin] + c);
    } else {
      return set_coeff(n, c);
    }
  }

  polynomial<T,P>&
  set_coeff(const_power_type n, const_coeff_reference c)
  {
    try {
      if (n >= pmin) {
	// Use the bound-checked access operator.
	coeff.at(n-pmin) = c;
	if (n > pmax) pmax = n;
      } else {
	// The new power is less than our minimum power.  Need to shift
	// the entries right by (pmin-n).
	// Move coefficients.
	for (P i = pmax; i >= pmin; --i) {
	  // Use the bound-checked element access operator.
	  coeff.at(i-n) = coeff[i-pmin];
	}
	// Zero the old coefficient positions.
	for (P i = 0; i < pmin-n; ++i) coeff[i] = 0;
	pmin = n;
	coeff[0] = c;
      }
    } catch (std::out_of_range oor) {
      // We're going to run out of capacity.
      // Grow the vector.
      cap = 2*(size_t)(std::max(pmax,n) - std::min(pmin,n) + 1);
      // Reserve more memory space.
      coeff.reserve(cap); /* This is redundant in light of the next call, non? */
      // Also tell the vector it can grow to its full size.
      coeff.resize(cap);
      // Try to set the coefficient again.
      set_coeff(n,c);
    }

    /* Leaving the next line in causes problems for the assignment
       operator [], which tries to grow the polynomial whilst
       returning zero. */
    // if (c == 0) compact();

    return *this;
  }

  polynomial<T,P>&
  multiply_by_monomial(const_power_type n, const_coeff_reference c)
  {
    if (c == 0) {
      for (P i = 0; i <= pmax-pmin; ++i) coeff[i] = 0;
      pmin = pmax = 0;
    } else {
      pmin += n;
      pmax += n;
      for (P i = 0; i <= pmax-pmin; ++i) coeff[i] *= c;
    }

    return *this;
  }

  polynomial<T,P>&
  multiply_by_polynomial(const polynomial<T,P>& p)
  {
    polynomial<T,P>(q);

    for (P i = 0; i <= p.pmax-p.pmin; ++i) {
      q.add_polynomial_X_monomial(*this, p.pmin+i, p.coeff[i]);
    }

    *this = q;

    return *this;
  }

  // Add a polynomial.
  polynomial<T,P>&
  add_polynomial(const polynomial<T,P>& p)
  {
    for (P i = 0; i <= p.pmax-p.pmin; ++i)
      add_to_coeff(p.pmin+i,p.coeff[i]);

    /*
    pmin = min(pmin,p.pmin);
    pmax = max(pmax,p.pmax);
    */

    return *this;
  }

  // Add a polynomial * monomial.
  polynomial<T,P>&
  add_polynomial_X_monomial(const polynomial<T,P>& p, 
			    const_power_type n,
			    const_coeff_reference c)
  {
    for (P i = 0; i <= p.pmax-p.pmin; ++i)
      add_to_coeff(p.pmin+i + n,c*p.coeff[i]);

    /*
    pmin = min(pmin,p.pmin + n);
    pmax = max(pmax,p.pmax + n);
    */

    return *this;
  }

  //
  // Input/Output
  //

  std::ostream& print(std::ostream& strm) const
  {
    strm << pmin << "\t" << pmax;
    for (P i = 0; i <= pmax-pmin; ++i) {
      strm << "\t" << coeff[i];
    }
    // Do not include an endline.

    return strm;
  }

  std::ostream& printFancy(std::ostream& strm, const std::string var = "x")
    const
  {
    bool first = true;

    if (pmin == pmax && coeff[0] == 0) {
      strm << "0" << std::endl;
      return strm;
    }

    for (P i = pmax-pmin; i >= 0; --i) {
      if (coeff[i] != 0) {
	if (first) {
	  first = false;
	  if (coeff[i] < 0) strm << "-";
	} else {
	  if (coeff[i] > 0) strm << " + ";
	  if (coeff[i] < 0) strm << " - ";
	}
	if (i+pmin != 0) {
	  if (Abs(coeff[i]) != 1) {
	    strm << Abs(coeff[i]) << " ";
	  }
	  if (i+pmin != 1) {
	    strm << var << "^" << pmin+i;
	  } else {
	    strm << var;
	  }
	} else {
	  strm << Abs(coeff[i]);
	}
      }
    }
    // Do not include an endline.

    return strm;
  }

  //
  // Info
  //

  P degree() const { return pmax; }
  P minpower() const { return pmin; }
  P maxpower() const { return pmax; }
  P spread() const { return (pmax-pmin); }

  // Return a coefficient of the polynomial.
  const T operator[](const P i) const
  {
    if (i >= pmin && i <= pmax) {
      return coeff[i-pmin];
    } else {
      return 0;
    }
  }

  // Return a coefficient of the polynomial, allow assignment.
  T& operator[](const P i)
  {
    if (i < pmin || i > pmax) set_coeff(i,0);

    return coeff[i-pmin];
  }

  // Evaluate polynomial at a given value of x.
  template<class S>
  S operator()(const S& x) const
  {
    S c = 0;
    S xp = Pow(x,(T)pmin);

    for (P i = 0; i <= pmax-pmin; ++i) {
      if (coeff[i] != 0) c += coeff[i] * xp;
      xp *= x;
    }

    return c;
  }

  //
  // Polynomial Operations
  //

  // Differentiate the polynomial in place.
  polynomial<T,P>&  differentiate()
  {
    for (P i = 0; i <= pmax-pmin; ++i) {
      coeff[i] *= pmin+i;
    }
    pmin--;
    pmax--;

    compact();

    return *this;
  }

  // Return the derivative of the polynomial.
  polynomial<T,P>  derivative() const
  {
    polynomial<T,P> q(*this);

    return q.differentiate();
  }

  //
  // Maintenance Functions
  //

  // Remove leading/trailing zeroes.
  polynomial<T,P>&  compact()
  {
    // Look for leading zero elements.  Shorten the polynomial
    // accordingly.
    P izmin = 0;
    while (coeff[izmin] == 0 && izmin < pmax-pmin) ++izmin;
    if (izmin > 0) {
      // Recopy to new range.
      for (P i = izmin; i <= pmax-pmin; ++i) {
	coeff[i-izmin] = coeff[i];
      }
      // Zero trailing elements.
      for (P i = pmax-pmin-izmin+1; i <= pmax-pmin; ++i) coeff[i] = 0;
      pmin += izmin;
    }

    // Look for trailing zero elements.  Shorten the polynomial
    // accordingly.  Easier since there is no need to recopy data.
    while (coeff[pmax-pmin] == 0 && pmin < pmax) --pmax;

    // Check if we're back to the zero polynomial.
    if (pmin == pmax && coeff[0] == 0) pmin = pmax = 0;

    return *this;
  }

  //
  // Friends
  //

  friend polynomial<T,P> operator+<>(const polynomial<T,P>& p);

  friend polynomial<T,P> operator-<>(const polynomial<T,P>& q);

  friend polynomial<T,P> operator+<>(const polynomial<T,P>& p,
				       const polynomial<T,P>& q);

  friend polynomial<T,P> operator-<>(const polynomial<T,P>& p,
				       const polynomial<T,P>& q);

  friend polynomial<T,P> operator*<>(const_coeff_reference a,
				       const polynomial<T,P>& p);

  friend polynomial<T,P> operator*<>(const polynomial<T,P>& p,
				       const_coeff_reference a);

  friend polynomial<T,P> operator/<>(const polynomial<T,P>& p,
				       const_coeff_reference a);

  friend polynomial<T,P> operator*<>(const polynomial<T,P>& p,
				       const polynomial<T,P>& q);

}; // class polynomial


//
// Function definitions
//

template<class T, class P>
inline polynomial<T,P> operator+(const polynomial<T,P>& p)
{
  return p;
}

template<class T, class P>
inline polynomial<T,P> operator-(const polynomial<T,P>& p)
{
  polynomial<T,P> r(p);

  r.multiply_by_monomial(0,-1);

  return r;
}

template<class T, class P>
inline polynomial<T,P> operator+(const polynomial<T,P>& p,
				   const polynomial<T,P>& q)
{
  polynomial<T,P> r(p);

  r.add_polynomial(q);

  return r;
}

template<class T, class P>
inline polynomial<T,P> operator-(const polynomial<T,P>& p,
				   const polynomial<T,P>& q)
{
  polynomial<T,P> r(p);

  r.add_polynomial_X_monomial(q,0,-1);

  return r;
}


template<class T, class P>
inline polynomial<T,P> operator*(const T& a, const polynomial<T,P>& p)
{
  polynomial<T,P> r(p);

  r.multiply_by_monomial(0,a);

  return r;
}

template<class T, class P>
inline polynomial<T,P> operator*(const polynomial<T,P>& p, const T& a)
{
  return a*p;
}

template<class T, class P>
inline polynomial<T,P> operator/(const polynomial<T,P>& p, const T& a)
{
  return ((T)1/a)*p;
}

template<class T, class P>
inline polynomial<T,P> operator*(const polynomial<T,P>& p,
				   const polynomial<T,P>& q)
{
  polynomial<T,P> r(p);

  r.multiply_by_polynomial(q);

  return r;
}

template<class T, class P>
std::ostream& operator<<(std::ostream& strm, const polynomial<T,P>& p)
{
  return (p.printFancy(strm));
}


} // namespace jlt

#endif // JLT_POLYNOMIAL_HPP
