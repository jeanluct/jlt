#ifndef JLT_MATHVECTOR_HPP
#define JLT_MATHVECTOR_HPP

//
// mathvector.hpp
//

#include <stdexcept>
#include <jlt/bcvector.hpp>
#include <jlt/math.hpp>
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

template<class T, class S>
inline S mag(const mathvector<T,S>& v);

template<class T, class S>
inline S abs(const mathvector<T,S>& v);

template<class T, class S>
inline mathvector<T,S> cross(const mathvector<T,S>& v,
			     const mathvector<T,S>& w);

//
// class mathvector
//

template<class T, class S = T>
class mathvector : public bcvector<T>
{
private:
  typedef typename std::vector<T>::size_type		size_type;
  typedef typename std::vector<T>::reference		reference;
  typedef typename std::vector<T>::const_reference	const_reference;
  typedef typename std::vector<T>::iterator		iterator;
  typedef typename std::vector<T>::const_iterator	const_iterator;

public:
  typedef S		scalar_type;
  typedef const S	const_scalar_type;
  typedef S&		scalar_reference;
  typedef const S&	const_scalar_reference;

  using bcvector<T>::begin;
  using bcvector<T>::end;

  //
  // Constructors
  //

  // Empty vector of size 0.
  mathvector() : bcvector<T>() {}

  // mathvector of size _n filled with _x.
  explicit mathvector(size_type _n, const_reference _x = T())
    : bcvector<T>(_n,_x) {}

  // Copy constructor.
  mathvector(const bcvector<T>& _v) : bcvector<T>(_v) {}

  const bcvector<T>& operator=(const bcvector<T>& v)
    {
      return bcvector<T>::operator=(v);
    }

  //
  // Vector Operations
  //

  // These are really slow, because of temporaries and copying, and
  // lead to nested loops.

  mathvector<T,S>& operator+=(const mathvector<T,S>& v)
    {
      VECTOR_ASSERT(size() == v.size());

      iterator k = begin();
      for (const_iterator i = v.begin(); i != v.end(); ++i, ++k)
	{
	  *k += *i;
	}

      return *this;
    }

  mathvector<T,S>& operator-=(const mathvector<T,S>& v)
    {
      VECTOR_ASSERT(size() == v.size());

      iterator k = begin();
      for (const_iterator i = v.begin(); i != v.end(); ++i, ++k)
	{
	  *k -= *i;
	}

      return *this;
    }

  mathvector<T,S>& operator*=(const_scalar_reference a)
    {
      for (iterator k = begin(); k != end(); ++k)
	{
	  *k *= a;
	}

      return *this;
    }

  mathvector<T,S>& operator/=(const_scalar_reference a)
    {
      for (iterator k = begin(); k != end(); ++k)
	{
	  *k /= a;
	}

      return *this;
    }

  // Component-wise division.
  mathvector<T,S>& operator/=(const mathvector<T,S>& v)
    {
      VECTOR_ASSERT(size() == v.size());

      iterator k = begin();
      for (const_iterator i = v.begin(); i != v.end(); ++i, ++k)
	{
	  *k /= *i;
	}

      return *this;
    }

  T sum() const
    {
      T _sum = T();

      for (const_iterator i = begin(); i != end(); ++i)
        {
	  _sum += *i;
        }

      return _sum;
    }

  //
  // Queries
  //

  bool isZero() const
    {
      for (const_iterator i = begin(); i != end(); ++i)
	{
	  if (*i != T()) return false;
	}

      return true;
    }

  //
  // Friends
  //

#ifdef __PGI
  friend mathvector<T,S> operator+(const mathvector<T,S>& v);

  friend mathvector<T,S> operator-(const mathvector<T,S>& v);

  friend mathvector<T,S> operator+(const mathvector<T,S>& v,
				   const mathvector<T,S>& w);

  friend mathvector<T,S> operator-(const mathvector<T,S>& v,
				   const mathvector<T,S>& w);

  friend mathvector<T,S> operator*(const_scalar_reference a,
				   const mathvector<T,S>& v);

  friend mathvector<T,S> operator*(const mathvector<T,S>& v,
				   const_scalar_reference a);

  friend mathvector<T,S> operator/(const mathvector<T,S>& v,
				   const_scalar_reference a);

  // Component-wise division.
  friend mathvector<T,S> operator/(const mathvector<T,S>& v,
				   const mathvector<T,S>& w);

  // Dot product (not component-wise multiplication).
  friend scalar_type operator*(const mathvector<T,S>& v,
			       const mathvector<T,S>& w);

  friend scalar_type dot(const mathvector<T,S>& v, const mathvector<T,S>& w);

  friend scalar_type mag(const mathvector<T,S>& v);

  friend scalar_type abs(const mathvector<T,S>& v);
#else
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

  friend scalar_type jlt::mag<>(const mathvector<T,S>& v);

  friend scalar_type jlt::abs<>(const mathvector<T,S>& v);
#endif


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

  typename mathvector<T,S>::iterator k = res.begin();
  for (typename mathvector<T,S>::const_iterator i = v.begin();
       i != v.end(); ++i,++k)
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

  typename mathvector<T,S>::iterator k = res.begin();
  for (typename mathvector<T,S>::const_iterator i = v.begin(), j = w.begin();
       i != v.end(); ++i,++j,++k)
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

  typename mathvector<T,S>::iterator k = res.begin();
  for (typename mathvector<T,S>::const_iterator i = v.begin(), j = w.begin();
       i != v.end(); ++i,++j,++k)
    {
      *k = (*i) - (*j);
    }

  return res;
}

template<class T, class S>
inline mathvector<T,S> operator*(const S& a, const mathvector<T,S>& v)
{
  mathvector<T,S> res(v.size());

  typename mathvector<T,S>::iterator k = res.begin();
  for (typename mathvector<T,S>::const_iterator i = v.begin();
       i != v.end(); ++i, ++k)
    {
      *k = a * (*i);
    }

  return res;
}

template<class T, class S>
inline mathvector<T,S> operator*(const mathvector<T,S>& v, const S& a)
{
  mathvector<T,S> res(v.size());

  typename mathvector<T,S>::iterator k = res.begin();
  for (typename mathvector<T,S>::const_iterator i = v.begin();
       i != v.end(); ++i, ++k)
    {
      *k = a * (*i);
    }

  return res;
}

template<class T, class S>
inline mathvector<T,S> operator/(const mathvector<T,S>& v, const S& a)
{
  mathvector<T,S> res(v.size());

  typename mathvector<T,S>::iterator k = res.begin();
  for (typename mathvector<T,S>::const_iterator i = v.begin();
       i != v.end(); ++i, ++k)
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

  typename mathvector<T,S>::iterator k = res.begin();
  for (typename mathvector<T,S>::const_iterator i = v.begin(), j = w.begin();
       i != v.end(); ++i,++j,++k)
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

  for (typename mathvector<T,S>::const_iterator i = v.begin(), j = w.begin();
       i != v.end(); ++i, ++j)
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
inline S mag(const mathvector<T,S>& v)
{
  S magn = S();

  for (typename mathvector<T,S>::const_iterator i = v.begin();
       i != v.end(); ++i)
    {
      magn += (*i)*(*i);
    }

  return magn;
}

template<class T, class S>
inline S abs(const mathvector<T,S>& v)
{
  return Sqrt(mag((v)));
}

// For compatibility with math.hpp
template<class T, class S>
inline S Abs(const mathvector<T,S>& v)
{
  return abs(v);
}

} // namespace jlt

#endif // JLT_MATHVECTOR_HPP
