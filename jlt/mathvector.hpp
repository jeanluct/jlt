//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
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
// Declare class and function templates
//

template<class T, class S> class mathvector;

template<class T, class S>
inline mathvector<T,S> operator+(const mathvector<T,S>& v);

template<class T, class S>
inline mathvector<T,S> operator-(const mathvector<T,S>& v);

template<class T, class S>
inline mathvector<T,S> operator+(const mathvector<T,S>& v,
				 const mathvector<T,S>& w);

template<class T, class S>
inline mathvector<T,S> operator-(const mathvector<T,S>& v,
				 const mathvector<T,S>& w);

template<class T, class S>
inline mathvector<T,S> operator*(const S& a, const mathvector<T,S>& v);

template<class T, class S>
inline mathvector<T,S> operator*(const mathvector<T,S>& v, const S& a);

template<class T, class S>
inline mathvector<T,S> operator/(const mathvector<T,S>& v, const S& a);

template<class T, class S>
inline mathvector<T,S> operator/(const mathvector<T,S>& v,
				 const mathvector<T,S>& w);

template<class T, class S>
inline S operator*(const mathvector<T,S>& v, const mathvector<T,S>& w);

template<class T, class S>
inline S dot(const mathvector<T,S>& v, const mathvector<T,S>& w);

// Was called mag, but potentially confusing since it returns the
// squared magnitude.
template<class T, class S>
inline S mag2(const mathvector<T,S>& v);

template<class T, class S>
inline S abs(const mathvector<T,S>& v);

template<class T, class S>
inline mathvector<T,S> cross(const mathvector<T,S>& v,
			     const mathvector<T,S>& w);

//
// class mathvector
//

template<class T, class S = T>
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

#if __cplusplus > 199711L
  // Use C++11-style argument forwarding.
  template<typename... Args>
  mathvector(Args&&... _args) : vector<T>(std::forward<Args>(_args)...) {}

  // Forward initializer list as well.
  mathvector(std::initializer_list<T> _l) : vector<T>(_l) {}
#else
  // Empty vector of size 0.
  mathvector() : vector<T>() {}

  // mathvector of size _n filled with _x.
  explicit mathvector(size_type _n, const_reference _x = T())
    : vector<T>(_n,_x) {}

  // Copy constructor.
  mathvector(const vector<T>& _v) : vector<T>(_v) {}

  const vector<T>& operator=(const vector<T>& v)
    {
      return vector<T>::operator=(v);
    }
#endif

  //
  // Vector Operations
  //

  // These are really slow, because of temporaries and copying, and
  // lead to nested loops.

  mathvector<T,S>& operator+=(const mathvector<T,S>& v)
    {
      VECTOR_ASSERT(this->size() == v.size());

      auto k = begin();
      for (auto i = v.cbegin(); i != v.cend(); ++i, ++k)
	{
	  *k += *i;
	}

      return *this;
    }

  mathvector<T,S>& operator-=(const mathvector<T,S>& v)
    {
      VECTOR_ASSERT(this->size() == v.size());

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
      VECTOR_ASSERT(this->size() == v.size());

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

  [[nodiscard]] bool isZero() const
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
// Specializations of cross product
//
template<>
inline mathvector<long double> cross(const mathvector<long double>& v,
				     const mathvector<long double>& w)
{
  // Cross product only works on vectors of size 3.
  VECTOR_ASSERT(v.size() == 3 && w.size() == 3);

  mathvector<long double> res(3);

  res[0] = v[1] * w[2] - v[2] * w[1];
  res[1] = v[2] * w[0] - v[0] * w[2];
  res[2] = v[0] * w[1] - v[1] * w[0];

  return res;
}

template<>
inline mathvector<double> cross(const mathvector<double>& v,
			     const mathvector<double>& w)
{
  // Cross product only works on vectors of size 3.
  VECTOR_ASSERT(v.size() == 3 && w.size() == 3);

  mathvector<double> res(3);

  res[0] = v[1] * w[2] - v[2] * w[1];
  res[1] = v[2] * w[0] - v[0] * w[2];
  res[2] = v[0] * w[1] - v[1] * w[0];

  return res;
}

template<>
inline mathvector<float> cross(const mathvector<float>& v,
			     const mathvector<float>& w)
{
  // Cross product only works on vectors of size 3.
  VECTOR_ASSERT(v.size() == 3 && w.size() == 3);

  mathvector<float> res(3);

  res[0] = v[1] * w[2] - v[2] * w[1];
  res[1] = v[2] * w[0] - v[0] * w[2];
  res[2] = v[0] * w[1] - v[1] * w[0];

  return res;
}

template<class T, class S>
inline mathvector<T,S> operator+(const mathvector<T,S>& v)
{
  return v;
}

template<class T, class S>
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
inline mathvector<T,S> operator+(const mathvector<T,S>& v,
				 const mathvector<T,S>& w)
{
  VECTOR_ASSERT(v.size() == w.size());

  mathvector<T,S> res(v.size());

  auto k = res.begin();
  for (auto i = v.cbegin(), j = w.cbegin(); i != v.cend(); ++i,++j,++k)
    {
      *k = (*i) + (*j);
    }

  return res;
}

template<class T, class S>
inline mathvector<T,S> operator-(const mathvector<T,S>& v,
				 const mathvector<T,S>& w)
{
  VECTOR_ASSERT(v.size() == w.size());

  mathvector<T,S> res(v.size());

  auto k = res.begin();
  for (auto i = v.cbegin(), j = w.cbegin(); i != v.cend(); ++i,++j,++k)
    {
      *k = (*i) - (*j);
    }

  return res;
}

template<class T, class S>
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
inline mathvector<T,S> operator/(const mathvector<T,S>& v,
				 const mathvector<T,S>& w)
{
  VECTOR_ASSERT(v.size() == w.size());

  mathvector<T,S> res(v.size());

  auto k = res.begin();
  for (auto i = v.cbegin(), j = w.cbegin(); i != v.cend(); ++i,++j,++k)
    {
      *k = (*i) / (*j);
    }

  return res;
}

template<class T, class S>
inline S dot(const mathvector<T,S>& v, const mathvector<T,S>& w)
{
  VECTOR_ASSERT(v.size() == w.size());

  S dotp = S();

  for (auto i = v.cbegin(), j = w.cbegin(); i != v.cend(); ++i, ++j)
    {
      dotp += (*i)*(*j);
    }

  return dotp;
}

template<class T, class S>
inline S operator*(const mathvector<T,S>& v, const mathvector<T,S>& w)
{
  return dot(v,w);
}

template<class T, class S>
inline S mag2(const mathvector<T,S>& v)
{
  S magn = S();

  for (auto i = v.cbegin(); i != v.cend(); ++i)
    {
      magn += (*i)*(*i);
    }

  return magn;
}

// Specializations of mag2 for complex types
template<class T, class S>
inline S mag2(const mathvector<std::complex<T>,S>& v)
{
  S magn = S();

  for (auto i = v.cbegin(); i != v.cend(); ++i)
    {
      // Note that std::complex::norm returns the squared magnitude.
      magn += norm(*i);
    }

  return magn;
}

template<class T, class S>
inline S abs(const mathvector<T,S>& v)
{
  return std::sqrt(mag2((v)));
}

} // namespace jlt

#endif // JLT_MATHVECTOR_HPP
