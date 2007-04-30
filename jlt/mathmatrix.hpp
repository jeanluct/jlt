#ifndef JLT_MATHMATRIX_HPP
#define JLT_MATHMATRIX_HPP

//
// mathmatrix.hpp
//

#include <jlt/mathvector.hpp>
#include <jlt/matrix.hpp>
#include <jlt/matrixutil.hpp>

namespace jlt {

//
// Declare class and function templates
//

template<class T, class S> class mathmatrix;

template<class T, class S>
inline mathmatrix<T,S> operator+(const mathmatrix<T,S>& A);

template<class T, class S>
inline mathmatrix<T,S> operator-(const mathmatrix<T,S>& B);

template<class T, class S>
inline mathmatrix<T,S> operator+(const mathmatrix<T,S>& A,
				 const mathmatrix<T,S>& B);

template<class T, class S>
inline mathmatrix<T,S> operator-(const mathmatrix<T,S>& A,
				 const mathmatrix<T,S>& B);

template<class T, class S>
inline mathmatrix<T,S> operator*(const S& a, const mathmatrix<T,S>& A);

template<class T, class S_T, class V, class S_V>
inline mathvector<V,S_V> operator*(const mathmatrix<T,S_T>& A,
				   const mathvector<V,S_V>& v);

template<class T, class S>
inline mathmatrix<T,S> operator*(const mathmatrix<T,S>& A, const S& a);

template<class T, class S>
inline mathmatrix<T,S> operator/(const mathmatrix<T,S>& A, const S& a);

template<class T, class S>
inline mathmatrix<T,S> operator*(const mathmatrix<T,S>& A,
				 const mathmatrix<T,S>& B);

//
// class mathmatrix
//

template<class T, class S = T>
class mathmatrix : public matrix<T>
{
public:
  typedef typename matrix<T>::size_type		size_type;
  typedef typename matrix<T>::reference		reference;
  typedef typename matrix<T>::const_reference	const_reference;
  typedef typename matrix<T>::iterator		iterator;
  typedef typename matrix<T>::const_iterator	const_iterator;

  typedef S		scalar_type;
  typedef const S	const_scalar_type;
  typedef S&		scalar_reference;
  typedef const S&	const_scalar_reference;

  using matrix<T>::begin;
  using matrix<T>::end;
  using matrix<T>::dim1;
  using matrix<T>::dim2;

  //
  // Constructors
  //

  mathmatrix() : matrix<T>() {}

  // Matrix of size _m*_n filled with _x.
  explicit mathmatrix(size_type _m, size_type _n, const_reference _x = T())
    : matrix<T>(_m,_n,_x) {}

  mathmatrix(const matrix<T>& _M) : matrix<T>(_M) {}	// Copy constructor.

  //
  // Elementary Matrix Operations
  //

  // These are all inefficient.
  // Only use when abstraction is more important than speed.

  mathmatrix<T,S>& operator+=(const mathmatrix<T,S>& A)
    {
      const_iterator j = A.begin();
      for (iterator i = begin(); i != end(); ++i, ++j)
	{
	  *i += *j;
	}

      return *this;
    }

  // Adds a*Identity to matrix.
  mathmatrix<T,S>& operator+=(const_scalar_reference a)
    {
      MATRIX_ASSERT(isSquare());

      for (size_type i = 0; i < dim1(); ++i)
	{
	  (*this)(i,i) += a;
	}

      return *this;
    }

  mathmatrix<T,S>& operator-=(const mathmatrix<T,S>& A)
    {
      const_iterator j = A.begin();
      for (iterator i = begin(); i != end(); ++i, ++j)
	{
	  *i -= *j;
	}

      return *this;
    }

  // Subtracts a*Identity from matrix.
  mathmatrix<T,S>& operator-=(const_scalar_reference a)
    {
      MATRIX_ASSERT(isSquare());

      for (size_type i = 0; i < dim1(); ++i)
	{
	  (*this)(i,i) -= a;
	}

      return *this;
    }

  // Equate to a*identity
  mathmatrix<T,S>& operator=(const_scalar_reference a)
    {
      MATRIX_ASSERT(isSquare());

      identity(a);

      return *this;
    }

  // Multiply matrix by a*Identity.
  mathmatrix<T,S>& operator*=(const_scalar_reference a)
    {
      for (size_type i = 0; i < dim1(); ++i)
	{
	  (*this)(i,i) *= a;
	}

      return *this;
    }

  // Divide matrix by a*Identity.
  mathmatrix<T,S>& operator/=(const_scalar_reference a)
    {
      for (size_type i = 0; i < dim1(); ++i)
	{
	  (*this)(i,i) /= a;
	}

      return *this;
    }

  //
  // Comparison Operators
  //

  bool operator==(const mathmatrix<T,S>& A) const
    {
      const_iterator j = A.begin();
      for (const_iterator i = begin(); i != end(); ++i, ++j)
	{
	  if (*i != *j) return false;
	}
      return true;
    }

  bool operator!=(const mathmatrix<T,S>& A) const
    {
      return !(operator==(A));
    }

#if 0
  /* Bad idea, since above I declare operator= differently. */
  // Equality to a single T: every element has to equal that T.
  bool operator==(const_reference a) const
    {
      for (const_iterator i = begin(); i != end(); ++i)
	{
	  if (*i != a) return false;
	}

      return true;
    }
#endif

  // Reducible matrix: a high-enough power still contains zeros.
  bool isReducible() const
    {
      int n = dim1();

      if (n == 0) return false;

      // See Ham and Song preprint (2006), p. 9.
      // Take log2 since we nest the multiplications.
      int pmax = (int)ceil(log2(n*n - 2*n + 2));

      jlt::mathmatrix<double> Mp(*this);

      for (int p = 1; p < pmax; ++p)
	{
	  Mp = Mp * Mp;
	}

      for (const_iterator i = Mp.begin(); i != Mp.end(); ++i)
	{
	  if (*i == T()) return true;
	}

      return false;
    }

  //
  // Friends
  //

#ifdef __PGI
  friend mathmatrix<T,S> operator+(const mathmatrix<T,S>& A);

  friend mathmatrix<T,S> operator-(const mathmatrix<T,S>& B);

  friend mathmatrix<T,S> operator+(const mathmatrix<T,S>& A,
				   const mathmatrix<T,S>& B);

  friend mathmatrix<T,S> operator-(const mathmatrix<T,S>& A,
				   const mathmatrix<T,S>& B);

  friend mathmatrix<T,S> operator*(const_scalar_reference a,
				   const mathmatrix<T,S>& A);

  friend mathmatrix<T,S> operator*(const mathmatrix<T,S>& A,
				   const_scalar_reference a);

  friend mathmatrix<T,S> operator/(const mathmatrix<T,S>& A,
				   const_scalar_reference a);
#else
  friend mathmatrix<T,S> operator+<>(const mathmatrix<T,S>& A);

  friend mathmatrix<T,S> operator-<>(const mathmatrix<T,S>& B);

  friend mathmatrix<T,S> operator+<>(const mathmatrix<T,S>& A,
				     const mathmatrix<T,S>& B);

  friend mathmatrix<T,S> operator-<>(const mathmatrix<T,S>& A,
				     const mathmatrix<T,S>& B);

  friend mathmatrix<T,S> operator*<>(const_scalar_reference a,
				     const mathmatrix<T,S>& A);

  friend mathmatrix<T,S> operator*<>(const mathmatrix<T,S>& A,
				     const_scalar_reference a);

  friend mathmatrix<T,S> operator/<>(const mathmatrix<T,S>& A,
				     const_scalar_reference a);
#endif

  // Component-wise division.
  // friend const mathmatrix<T,S>& operator/(const mathmatrix<T,S>&, const
  // mathmatrix<T,S>&);

  //
  // Matrix Inverse
  //

  // The functions named invert() destroy the object,
  // the functions named inverse() leave it untouched.

  // Replace matrix *this by its inverse.
  void invert()
    {
      MATRIX_ASSERT(isSquare());
      unsigned int n = dim1();

      int perm;
      int* row_index = new int[n];

      LUdecomp<T,mathmatrix<T,S> >(*this, row_index, &perm);

      T* col = new T[n];
      mathmatrix<T,S> Ainv(n,n);

      for (unsigned int j = 0; j < n; ++j)
	{
	  for (unsigned int i = 0; i < n; ++i) col[i] = 0.;
	  col[j] = 1.;
	  LUbacksub<T,mathmatrix<T,S> >(*this, row_index, col);
	  for (unsigned int i = 0; i < n; ++i) Ainv(i,j) = col[i];
	}

      // Copy Ainv to A, without reallocating.
      iterator j = begin();
      const_iterator i = Ainv.begin();
      while (j != end()) *j++ = *i++;

      delete[] col;
      delete[] row_index;
    }

  // Replaces matrix Ainv by inverse, detroying *this.
  // Ainv has to be the same size as *this.
  // This should be the fastest method, with the least temporaries.
  void invert(mathmatrix<T,S>& Ainv)
    {
      MATRIX_ASSERT(m == Ainv.m && m == Ainv.n && isSquare());
      unsigned int n = dim1();

      int perm;
      int* row_index = new int[n];

      LUdecomp<T,mathmatrix<T,S> >(*this, row_index, &perm);

      T* col = new T[n];

      for (unsigned int j = 0; j < n; ++j)
	{
	  for (unsigned int i = 0; i < n; ++i) col[i] = 0.;
	  col[j] = 1.;
	  LUbacksub<T,mathmatrix<T,S> >(*this, row_index, col);
	  for (unsigned int i = 0; i < n; ++i) Ainv(i,j) = col[i];
	}

      delete[] col;
      delete[] row_index;
    }

  // Does not alter matrix.
  mathmatrix<T,S> inverse() const
    {
      MATRIX_ASSERT(isSquare());
      unsigned int n = dim1();

      int perm;
      int* row_index = new int[n];

      mathmatrix<T,S> A_LU(*this);

      LUdecomp<T,mathmatrix<T,S> >(A_LU, row_index, &perm);

      T* col = new T[n];
      mathmatrix<T,S> Ainv(n,n);

      for (unsigned int j = 0; j < n; ++j)
	{
	  for (unsigned int i = 0; i < n; ++i) col[i] = 0.;
	  col[j] = 1.;
	  LUbacksub<T,mathmatrix<T,S> >(A_LU, row_index, col);
	  for (unsigned int i = 0; i < n; ++i) Ainv(i,j) = col[i];
	}

      delete[] col;
      delete[] row_index;

      return Ainv;
    }

  // Replaces matrix Ainv by inverse, without altering *this.
  // Ainv has to be the same size as *this.
  mathmatrix<T,S> inverse(mathmatrix<T,S>& Ainv) const
    {
      MATRIX_ASSERT(m == Ainv.m && m == Ainv.n && isSquare());
      unsigned int n = dim1();

      int perm;
      int* row_index = new int[n];

      mathmatrix<T,S> A_LU(*this);

      LUdecomp<T,mathmatrix<T,S> >(A_LU, row_index, &perm);

      T* col = new T[n];

      for (unsigned int j = 0; j < n; ++j)
	{
	  for (unsigned int i = 0; i < n; ++i) col[i] = 0.;
	  col[j] = 1.;
	  LUbacksub<T,mathmatrix<T,S> >(A_LU, row_index, col);
	  for (unsigned int i = 0; i < n; ++i) Ainv(i,j) = col[i];
	}

      delete[] col;
      delete[] row_index;

      return Ainv;
    }

  //
  // Determinant and trace
  //

  T det() const
    {
      MATRIX_ASSERT(isSquare());

      T det = 1;
      int perm;
      int* row_index = new int[dim2()];

      // The price to pay to leave the object intact is creating a temporary.
      mathmatrix<T,S> A_LU(*this);

      LUdecomp<T,mathmatrix<T,S> >(A_LU, row_index, &perm);

      for (size_type i = 0; i < dim2(); ++i) det *= A_LU(i,i);

      return (perm*det);
    }

  T trace() const
    {
      MATRIX_ASSERT(isSquare());

      T tr = 0;

      for (size_type i = 0; i < dim1(); ++i)
	{
	  tr += this->operator()(i,i);
	}

      return tr;
    }

  //
  // Transpose
  //
  const mathmatrix<T,S>& transpose()
    {
      matrix<T>::transpose();

      return *this;
    }

  //
  // Some common matrices
  //

  void identity(const S& a = 1)
    {
      MATRIX_ASSERT(isSquare());

      for (size_type i = 0; i < dim1(); ++i)
	{
	  for (size_type j = 0; j < dim2(); ++j)
	    {
	      this->operator()(i,j) = (i == j ? a : 0);
	    }
	}
    }

}; // class mathmatrix


//
// Function definitions
//

template<class T, class S>
inline mathmatrix<T,S> operator+(const mathmatrix<T,S>& A)
{
  return A;
}

template<class T, class S>
inline mathmatrix<T,S> operator-(const mathmatrix<T,S>& A)
{
  mathmatrix<T,S> res(A.dim1(),A.dim2());

  typename mathmatrix<T,S>::iterator k = res.begin();
  for (typename mathmatrix<T,S>::const_iterator i = A.begin();
       i != A.end(); ++i, ++k)
    {
      *k = -(*i);
    }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator+(const mathmatrix<T,S>& A,
				 const mathmatrix<T,S>& B)
{
  mathmatrix<T,S> res(A.dim1(),A.dim2());

  typename mathmatrix<T,S>::iterator k = res.begin();
  for (typename mathmatrix<T,S>::const_iterator i = A.begin(), j = B.begin();
       i != A.end(); ++i, ++j, ++k)
    {
      *k = *i + *j;
    }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator-(const mathmatrix<T,S>& A,
				 const mathmatrix<T,S>& B)
{
  mathmatrix<T,S> res(A.dim1(),A.dim2());

  typename mathmatrix<T,S>::iterator k = res.begin();
  for (typename mathmatrix<T,S>::const_iterator i = A.begin(), j = B.begin();
       i != A.end(); ++i, ++j, ++k)
    {
      *k = *i - *j;
    }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator*(const S& a, const mathmatrix<T,S>& A)
{
  mathmatrix<T,S> res(A.dim1(),A.dim2());

  typename mathmatrix<T,S>::iterator k = res.begin();
  for (typename mathmatrix<T,S>::const_iterator i = A.begin();
       i != A.end(); ++i, ++k)
    {
      *k = a * (*i);
    }

  return res;
}

template<class T, class S_T, class V, class S_V>
inline mathvector<V,S_V> operator*(const mathmatrix<T,S_T>& A,
				   const mathvector<V,S_V>& v)
{
  typename mathmatrix<T,S_T>::size_type m = A.dim1();
  typename mathmatrix<T,S_T>::size_type n = A.dim2();

  MATRIX_ASSERT(n == v.size());

  mathvector<V,S_V> res(m);

  for (typename mathmatrix<T,S_T>::size_type i = 0; i < m; ++i)
    {
      // Multiplication of a type T and type V must be defined.
      res[i] = A(i,0)*v[0];
      for (typename mathmatrix<T,S_T>::size_type k = 1; k < n; ++k)
	res[i] += A(i,k)*v[k];
    }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator*(const mathmatrix<T,S>& A, const S& a)
{
  mathmatrix<T,S> res(A.dim1(),A.dim2());

  typename mathmatrix<T,S>::iterator k = res.begin();
  for (typename mathmatrix<T,S>::const_iterator i = A.begin();
       i != A.end(); ++i, ++k)
    {
      *k = a * (*i);
    }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator/(const mathmatrix<T,S>& A, const S& a)
{
  mathmatrix<T,S> res(A.dim1(),A.dim2());

  typename mathmatrix<T,S>::iterator k = res.begin();
  for (typename mathmatrix<T,S>::const_iterator i = A.begin();
       i != A.end(); ++i, ++k)
    {
      *k = (*i) / a;
    }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator*(const mathmatrix<T,S>& A,
				 const mathmatrix<T,S>& B)
{
  typedef typename mathmatrix<T,S>::size_type size_type;

  size_type ma = A.dim1();
  size_type na = A.dim2();
  size_type nb = B.dim2();

  MATRIX_ASSERT(na == B.dim1());

  mathmatrix<T,S> res(ma,nb);

  for (size_type i = 0; i < ma; ++i)
    {
      for (size_type j = 0; j < nb; ++j)
	{
	  for (size_type k = 0; k < na; ++k) res(i,j) += A(i,k)*B(k,j);
	}
    }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> identity_matrix(typename mathmatrix<T,S>::size_type n)
{
  mathmatrix<T,S> id(n,n);

  for (typename mathmatrix<T,S>::size_type i = 0; i < n; ++i) id(i,i) = 1;

  return id;
}

template<class T>
inline mathmatrix<T> identity_matrix(typename mathmatrix<T>::size_type n)
{
  mathmatrix<T> id(n,n);

  for (typename mathmatrix<T>::size_type i = 0; i < n; ++i) id(i,i) = 1;

  return id;
}

template<class T, class S>
inline mathmatrix<T,S> diagonal_matrix(const mathvector<T,S>& v)
{
  typename mathmatrix<T,S>::size_type n = v.size();
  mathmatrix<T,S> diag(n,n);

  for (typename mathmatrix<T,S>::size_type i = 0; i < n; ++i) diag(i,i) = v[i];

  return diag;
}

template<class T, class S>
inline mathmatrix<T,S> diagonal_matrix(const mathvector<T,S>& v,
				       typename mathmatrix<T>::size_type m,
				       typename mathmatrix<T>::size_type n)
{
  typename mathmatrix<T>::size_type d = std::min(m,n);
  assert(v.size() == d);
  mathmatrix<T,S> diag(m,n);

  for (typename mathmatrix<T,S>::size_type i = 0; i < d; ++i) diag(i,i) = v[i];

  return diag;
}

} // namespace jlt

#endif // JLT_MATHMATRIX_HPP
