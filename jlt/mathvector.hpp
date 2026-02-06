//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_MATHVECTOR_HPP
#define JLT_MATHVECTOR_HPP

//
// mathvector.hpp
//

#include <stdexcept>
#include <complex>
#include <jlt/vector.hpp>
#include <jlt/stlio.hpp>

namespace jlt {

//
// Implementation details - not part of public API
// These are internal helpers that may change without notice.
//
namespace detail {

// Trait to extract the real scalar type from a value type.
// For real types: scalar_type<T>::type = T
// For complex types: scalar_type<std::complex<T>>::type = T
template<class T>
struct scalar_type
{
  using type = T;
};

template<class T>
struct scalar_type<std::complex<T>>
{
  using type = T;
};

template<class T>
using scalar_type_t = typename scalar_type<T>::type;

// Trait to compute the squared magnitude of a single element.
// For real types: computes val * val
// For complex types: computes |val|² using std::norm
template<class T>
struct mag2_traits
{
  static T compute(const T& val)
  {
    return val * val;
  }
};

template<class T>
struct mag2_traits<std::complex<T>>
{
  static T compute(const std::complex<T>& val)
  {
    // std::norm returns |z|² = z * conj(z) for complex numbers
    return std::norm(val);
  }
};

} // namespace detail

//
// Declare class and function templates
//

template<class T, class S> class mathvector;

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator+(const mathvector<T,S>& v);

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator-(const mathvector<T,S>& v);

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator+(const mathvector<T,S>& v,
				 const mathvector<T,S>& w);

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator-(const mathvector<T,S>& v,
				 const mathvector<T,S>& w);

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator*(const S& a, const mathvector<T,S>& v);

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator*(const mathvector<T,S>& v, const S& a);

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator/(const mathvector<T,S>& v, const S& a);

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator/(const mathvector<T,S>& v,
				 const mathvector<T,S>& w);

template<class T, class S>
[[nodiscard]]
inline S operator*(const mathvector<T,S>& v, const mathvector<T,S>& w);

template<class T, class S>
[[nodiscard]] inline S dot(const mathvector<T,S>& v, const mathvector<T,S>& w);

// Was called mag, but potentially confusing since it returns the
// squared magnitude.
template<class T, class S>
[[nodiscard]] inline S mag2(const mathvector<T,S>& v);

template<class T, class S>
[[nodiscard]] inline S abs(const mathvector<T,S>& v);

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> cross(const mathvector<T,S>& v,
			     const mathvector<T,S>& w);

//
// class mathvector
//

template<class T, class S = detail::scalar_type_t<T>>
class mathvector : public vector<T> // note that this is jlt::vector
{
public:
  typedef typename std::vector<T>::size_type		size_type;
  using reference = typename std::vector<T>::reference;
  using const_reference = typename std::vector<T>::const_reference;
  using iterator = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;

  using scalar_type = S;
  using const_scalar_type = const S;
  using scalar_reference = S &;
  using const_scalar_reference = const S &;

  using vector<T>::begin;
  using vector<T>::end;

  //
  // Constructors
  //

  // Use C++11-style argument forwarding.
  template<typename... Args>
  mathvector(Args&&... _args) : vector<T>(std::forward<Args>(_args)...) {}

  // Forward initializer list as well.
  mathvector(std::initializer_list<T> _l) : vector<T>(_l) {}

  //
  // Vector Operations
  //

  // These are really slow, because of temporaries and copying, and
  // lead to nested loops.

  mathvector<T,S>& operator+=(const mathvector<T,S>& v)
    {
      JLT_VECTOR_ASSERT(this->size() == v.size());

      auto k = begin();
      for (auto i = v.cbegin(); i != v.cend(); ++i, ++k)
	{
	  *k += *i;
	}

      return *this;
    }

  mathvector<T,S>& operator-=(const mathvector<T,S>& v)
    {
      JLT_VECTOR_ASSERT(this->size() == v.size());

      auto k = begin();
      for (auto i = v.cbegin(); i != v.cend(); ++i, ++k)
	{
	  *k -= *i;
	}

      return *this;
    }

  mathvector<T,S>& operator*=(const_scalar_reference a)
    {
      for (auto k = begin(); k != end(); ++k)
	{
	  *k *= a;
	}

      return *this;
    }

  mathvector<T,S>& operator/=(const_scalar_reference a)
    {
      for (auto k = begin(); k != end(); ++k)
	{
	  *k /= a;
	}

      return *this;
    }

  // Component-wise division.
  mathvector<T,S>& operator/=(const mathvector<T,S>& v)
    {
      JLT_VECTOR_ASSERT(this->size() == v.size());

      auto k = begin();
      for (auto i = v.cbegin(); i != v.cend(); ++i, ++k)
	{
	  *k /= *i;
	}

      return *this;
    }

  [[nodiscard]] T sum() const
    {
      T _sum = T();

      for (auto i = this->cbegin(); i != this->cend(); ++i)
        {
	  _sum += *i;
        }

      return _sum;
    }

  //
  // Queries
  //

  [[nodiscard]] bool is_zero() const
    {
      for (auto i = this->cbegin(); i != this->cend(); ++i)
	{
	  if (*i != T()) return false;
	}

      return true;
    }

  //
  // Friends
  //

  friend mathvector<T,S> operator+<>(const mathvector<T,S>& v);

  friend mathvector<T,S> operator-<>(const mathvector<T,S>& v);

  friend mathvector<T,S> operator+<>(const mathvector<T,S>& v,
				     const mathvector<T,S>& w);

  friend mathvector<T,S> operator-<>(const mathvector<T,S>& v,
				     const mathvector<T,S>& w);

  friend mathvector<T,S> operator*<>(const_scalar_reference a,
				     const mathvector<T,S>& v);

  friend mathvector<T,S> operator*<>(const mathvector<T,S>& v,
				     const_scalar_reference a);

  friend mathvector<T,S> operator/<>(const mathvector<T,S>& v,
				     const_scalar_reference a);

  // Component-wise division.
  friend mathvector<T,S> operator/<>(const mathvector<T,S>& v,
				  const mathvector<T,S>& w);

  // Dot product (not component-wise multiplication).
  friend scalar_type jlt::operator*<>(const mathvector<T,S>& v,
				      const mathvector<T,S>& w);

  friend scalar_type jlt::dot<>(const mathvector<T,S>& v,
				const mathvector<T,S>& w);

  friend scalar_type jlt::mag2<>(const mathvector<T,S>& v);

  friend scalar_type jlt::abs<>(const mathvector<T,S>& v);


}; // class mathvector


//
// Function definitions
//

//
// Cross product for mathvector
// Single template implementation replaces three specializations (float, double, long double)
//
template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> cross(const mathvector<T,S>& v,
			     const mathvector<T,S>& w)
{
  // Cross product only works on vectors of size 3.
  JLT_VECTOR_ASSERT(v.size() == 3 && w.size() == 3);

  mathvector<T,S> res(3);

  res[0] = v[1] * w[2] - v[2] * w[1];
  res[1] = v[2] * w[0] - v[0] * w[2];
  res[2] = v[0] * w[1] - v[1] * w[0];

  return res;
}

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator+(const mathvector<T,S>& v)
{
  return v;
}

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator-(const mathvector<T,S>& v)
{
  mathvector<T,S> res(v.size());

  auto k = res.begin();
  for (auto i = v.cbegin(); i != v.cend(); ++i,++k)
    {
      *k = -(*i);
    }

  return res;
}

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator+(const mathvector<T,S>& v,
				 const mathvector<T,S>& w)
{
  JLT_VECTOR_ASSERT(v.size() == w.size());

  mathvector<T,S> res(v.size());

  auto k = res.begin();
  for (auto i = v.cbegin(), j = w.cbegin(); i != v.cend(); ++i,++j,++k)
    {
      *k = (*i) + (*j);
    }

  return res;
}

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator-(const mathvector<T,S>& v,
				 const mathvector<T,S>& w)
{
  JLT_VECTOR_ASSERT(v.size() == w.size());

  mathvector<T,S> res(v.size());

  auto k = res.begin();
  for (auto i = v.cbegin(), j = w.cbegin(); i != v.cend(); ++i,++j,++k)
    {
      *k = (*i) - (*j);
    }

  return res;
}

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator*(const S& a, const mathvector<T,S>& v)
{
  mathvector<T,S> res(v.size());

  auto k = res.begin();
  for (auto i = v.cbegin(); i != v.cend(); ++i, ++k)
    {
      *k = a * (*i);
    }

  return res;
}

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator*(const mathvector<T,S>& v, const S& a)
{
  mathvector<T,S> res(v.size());

  auto k = res.begin();
  for (auto i = v.cbegin(); i != v.cend(); ++i, ++k)
    {
      *k = a * (*i);
    }

  return res;
}

template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator/(const mathvector<T,S>& v, const S& a)
{
  mathvector<T,S> res(v.size());

  auto k = res.begin();
  for (auto i = v.cbegin(); i != v.cend(); ++i, ++k)
    {
      *k = (*i) / a;
    }

  return res;
}

// Component-wise division.
template<class T, class S>
[[nodiscard]]
inline mathvector<T,S> operator/(const mathvector<T,S>& v,
				 const mathvector<T,S>& w)
{
  JLT_VECTOR_ASSERT(v.size() == w.size());

  mathvector<T,S> res(v.size());

  auto k = res.begin();
  for (auto i = v.cbegin(), j = w.cbegin(); i != v.cend(); ++i,++j,++k)
    {
      *k = (*i) / (*j);
    }

  return res;
}

template<class T, class S>
[[nodiscard]] inline S dot(const mathvector<T,S>& v, const mathvector<T,S>& w)
{
  JLT_VECTOR_ASSERT(v.size() == w.size());

  S dotp = S();

  for (auto i = v.cbegin(), j = w.cbegin(); i != v.cend(); ++i, ++j)
    {
      dotp += (*i)*(*j);
    }

  return dotp;
}

template<class T, class S>
[[nodiscard]]
inline S operator*(const mathvector<T,S>& v, const mathvector<T,S>& w)
{
  return dot(v,w);
}

template<class T, class S>
[[nodiscard]] inline S mag2(const mathvector<T,S>& v)
{
  S magn = S();

  for (auto i = v.cbegin(); i != v.cend(); ++i)
    {
      magn += detail::mag2_traits<T>::compute(*i);
    }

  return magn;
}

template<class T, class S>
[[nodiscard]] inline S abs(const mathvector<T,S>& v)
{
  return std::sqrt(mag2((v)));
}

} // namespace jlt

#endif // JLT_MATHVECTOR_HPP
