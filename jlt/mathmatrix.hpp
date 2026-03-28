//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_MATHMATRIX_HPP
#define JLT_MATHMATRIX_HPP

//
// mathmatrix.hpp
//

#include <cassert>
#include <complex>
#include <limits>
#include <jlt/mathvector.hpp>
#include <jlt/matrix.hpp>
#include <jlt/matrixutil.hpp>
#include <jlt/polynomial.hpp>

namespace jlt {

//
// Implementation details - not part of public API
//
namespace detail {

// Trait to compute complex conjugate of an element
// For real types: conjugate is identity
// For complex types: use std::conj
template<class T>
struct conj_traits
{
  static T compute(const T& val)
  {
    return val;
  }
};

template<class T>
struct conj_traits<std::complex<T>>
{
  static std::complex<T> compute(const std::complex<T>& val)
  {
    return std::conj(val);
  }
};

} // namespace detail

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

template<class T, class S = detail::scalar_type_t<T>>
class mathmatrix : public matrix<T>
{
public:
  typedef typename matrix<T>::size_type		size_type;
  using reference = typename matrix<T>::reference;
  using const_reference = typename matrix<T>::const_reference;
  using iterator = typename matrix<T>::iterator;
  using const_iterator = typename matrix<T>::const_iterator;

  using scalar_type = S;
  using const_scalar_type = const S;
  using scalar_reference = S &;
  using const_scalar_reference = const S &;

  using matrix<T>::begin;
  using matrix<T>::end;
  using matrix<T>::rows;
  using matrix<T>::columns;

  //
  // Constructors
  //

// Use C++11-style argument forwarding.
template<typename... Args>
mathmatrix(Args&&... _args) : matrix<T>(std::forward<Args>(_args)...) {}

// Forward initializer list as well.
mathmatrix(size_type _m, size_type _n, std::initializer_list<T> _l)
  : matrix<T>(_m,_n,_l) {}

  //
  // Elementary Matrix Operations
  //

  // These are all inefficient.
  // Only use when abstraction is more important than speed.

  mathmatrix<T,S>& operator+=(const mathmatrix<T,S>& A)
    {
      for (auto i = begin(), j = A.cbegin(); i != end(); ++i, ++j)
	{
	  *i += *j;
	}

      return *this;
    }

  // Adds a*Identity to matrix.
  mathmatrix<T,S>& operator+=(const_scalar_reference a)
    {
      JLT_MATRIX_ASSERT(is_square());

      for (size_type i = 0; i < rows(); ++i)
	{
	  (*this)(i,i) += a;
	}

      return *this;
    }

  mathmatrix<T,S>& operator-=(const mathmatrix<T,S>& A)
    {
      for (auto i = begin(), j = A.cbegin(); i != end(); ++i, ++j)
	{
	  *i -= *j;
	}

      return *this;
    }

  // Subtracts a*Identity from matrix.
  mathmatrix<T,S>& operator-=(const_scalar_reference a)
    {
      JLT_MATRIX_ASSERT(is_square());

      for (size_type i = 0; i < rows(); ++i)
	{
	  (*this)(i,i) -= a;
	}

      return *this;
    }

  // Equate to a*identity
  mathmatrix<T,S>& operator=(const_scalar_reference a)
    {
      JLT_MATRIX_ASSERT(is_square());

      identity(a);

      return *this;
    }

  // Multiply matrix by a*Identity.
  mathmatrix<T,S>& operator*=(const_scalar_reference a)
    {
      for (size_type i = 0; i < rows(); ++i)
	{
	  (*this)(i,i) *= a;
	}

      return *this;
    }

  // Divide matrix by a*Identity.
  mathmatrix<T,S>& operator/=(const_scalar_reference a)
    {
      for (size_type i = 0; i < rows(); ++i)
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
      for (auto i = this->cbegin(), j = A.cbegin();
	   i != this->cend(); ++i, ++j)
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
      for (auto i = cbegin(); i != cend(); ++i)
	{
	  if (*i != a) return false;
	}

      return true;
    }
#endif

  // Check if all matrix elements are non-negative.
  [[nodiscard]] bool is_nonnegative() const
    {
      for (auto i = this->cbegin(); i != this->cend(); ++i)
	{
	  if (*i < T()) return false;
	}
      return true;
    }

  // Primitive matrix: some power has all strictly positive entries.
  // Tests if A^k has all positive entries for k = n²-2n+2 (the maximum
  // exponent of primitivity for an n×n matrix).
  // Requires: matrix must be non-negative (throws std::domain_error otherwise).
  [[nodiscard]] bool is_primitive() const
    {
      JLT_MATRIX_ASSERT(is_square());

      // Check that matrix is non-negative (required for primitivity)
      if (!is_nonnegative())
	{
	  JLT_THROW(std::domain_error(
	    "is_primitive() requires non-negative matrix"));
	}

      size_type n = rows();

      if (n == 0) return false;

      // Compute A^(n²-2n+2) using repeated squaring with renormalization.
      // Take log2 since we nest the multiplications.
      auto pmax = static_cast<size_type>(ceil(log2(n*n - 2*n + 2)));

      // Take powers of matrix.  Do this in place since we need to
      // renormalise to avoid blow-up.
      mathmatrix<T,S> Mp(n,n), M(*this);
      for (size_type p = 1; p < pmax; ++p)
	{
	  for (size_type i = 0; i < n; ++i)
	    {
	      // Mp = M*M.
	      for (size_type j = 0; j < n; ++j)
		{
		  Mp(i,j) = M(i,0)*M(0,j);
		  for (size_type k = 1; k < n; ++k)
		    {
		      Mp(i,j) += M(i,k)*M(k,j);
		    }
		  // Renormalise: all that matters is whether an element
		  // is zero or not.
		  if (Mp(i,j) != T()) Mp(i,j) = 1;
		}
	    }
	  // Copy the result back to M.
	  M = Mp;
	}

      // Now look for zeros.
      for (auto i = M.cbegin(); i != M.cend(); ++i)
	{
	  if (*i == T()) return false;  // Found zero = NOT primitive
	}

      return true;  // No zeros = IS primitive
    }

  // Reducible matrix: can be permuted to block upper-triangular form.
  // Uses Frobenius criterion: A is irreducible iff (I+A)^(n-1) > 0 elementwise.
  // References:
  //   - Horn & Johnson (1985), Matrix Analysis, Theorem 8.5.2, p. 531.
  //   - Seneta (2006), Non-negative Matrices and Markov Chains, §1.2, p. 5.
  //   - Berman & Plemmons (1994), Nonnegative Matrices, Theorem 2.5, p. 29.
  // Returns true if reducible (NOT irreducible).
  // Requires: matrix must be non-negative (throws std::domain_error otherwise).
  [[nodiscard]] bool is_reducible() const
    {
      JLT_MATRIX_ASSERT(is_square());

      // Check that matrix is non-negative (required for Frobenius criterion)
      if (!is_nonnegative())
	{
	  JLT_THROW(std::domain_error(
	    "is_reducible() requires non-negative matrix"));
	}

      size_type n = rows();

      if (n == 0) return false;  // Empty matrix considered irreducible

      // Compute (I + A)
      mathmatrix<T,S> M(*this);
      for (size_type i = 0; i < n; ++i)
	{
	  M(i,i) += 1;  // Add identity
	}

      // Compute M^(n-1) using repeated squaring with renormalization
      // We only care about zero vs non-zero, so renormalize to avoid overflow
      if (n == 1) return false;  // 1x1 matrix is irreducible

      // Compute (I+A)^(n-1) efficiently
      // For simplicity, we'll use the same approach as is_primitive
      // but with power (n-1) instead of (n²-2n+2)
      auto power = n - 1;

      mathmatrix<T,S> Mp(n,n), Mpower = M;

      // Build up M^(n-1) by repeated squaring
      // Start with result = I
      mathmatrix<T,S> result(n,n);
      result.identity();

      // Binary exponentiation
      while (power > 0)
	{
	  if (power & 1)  // If power is odd
	    {
	      // result = result * Mpower
	      for (size_type i = 0; i < n; ++i)
		{
		  for (size_type j = 0; j < n; ++j)
		    {
		      Mp(i,j) = result(i,0)*Mpower(0,j);
		      for (size_type k = 1; k < n; ++k)
			{
			  Mp(i,j) += result(i,k)*Mpower(k,j);
			}
		      // Renormalise
		      if (Mp(i,j) != T()) Mp(i,j) = 1;
		    }
		}
	      result = Mp;
	    }

	  power >>= 1;  // Divide power by 2

	  if (power > 0)
	    {
	      // Mpower = Mpower * Mpower
	      for (size_type i = 0; i < n; ++i)
		{
		  for (size_type j = 0; j < n; ++j)
		    {
		      Mp(i,j) = Mpower(i,0)*Mpower(0,j);
		      for (size_type k = 1; k < n; ++k)
			{
			  Mp(i,j) += Mpower(i,k)*Mpower(k,j);
			}
		      // Renormalise
		      if (Mp(i,j) != T()) Mp(i,j) = 1;
		    }
		}
	      Mpower = Mp;
	    }
	}

      // Now look for zeros in result = (I+A)^(n-1)
      for (auto i = result.cbegin(); i != result.cend(); ++i)
	{
	  if (*i == T()) return true;  // Found zero = IS reducible
	}

      return false;  // No zeros = NOT reducible (i.e., irreducible)
    }

  // Replace nonzero entries by 1.
  mathmatrix<T,S>& ones_and_zeros()
  {
    for (auto i = begin(); i != end(); ++i)
      {
	if (*i != T()) *i = 1;
      }
    return *this;
  }

  //
  // Friends
  //

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
      JLT_MATRIX_ASSERT(is_square());
      unsigned int n = rows();

      int perm;
      std::vector<int> row_index(n);

      LUdecomp<T,mathmatrix<T,S>>(*this, row_index.data(), &perm);

      std::vector<T> col(n);
      mathmatrix<T,S> Ainv(n,n);

      for (unsigned int j = 0; j < n; ++j)
	{
	  for (unsigned int i = 0; i < n; ++i) col[i] = 0.;
	  col[j] = 1.;
	  LUbacksub<T,mathmatrix<T,S>>(*this, row_index.data(), col.data());
	  for (unsigned int i = 0; i < n; ++i) Ainv(i,j) = col[i];
	}

      // Copy Ainv to A, without reallocating.
      auto j = begin();
      auto i = Ainv.cbegin();
      while (j != end()) *j++ = *i++;
    }

  // Replaces matrix Ainv by inverse, detroying *this.
  // Ainv has to be the same size as *this.
  // This should be the fastest method, with the least temporaries.
  void invert(mathmatrix<T,S>& Ainv)
    {
      JLT_MATRIX_ASSERT(is_square() && rows() == Ainv.rows());
      unsigned int n = rows();

      int perm;
      std::vector<int> row_index(n);

      LUdecomp<T,mathmatrix<T,S>>(*this, row_index.data(), &perm);

      std::vector<T> col(n);

      for (unsigned int j = 0; j < n; ++j)
	{
	  for (unsigned int i = 0; i < n; ++i) col[i] = 0.;
	  col[j] = 1.;
	  LUbacksub<T,mathmatrix<T,S>>(*this, row_index.data(), col.data());
	  for (unsigned int i = 0; i < n; ++i) Ainv(i,j) = col[i];
	}
    }

  // Does not alter matrix.
  [[nodiscard]] mathmatrix<T,S> inverse() const
    {
      JLT_MATRIX_ASSERT(is_square());
      unsigned int n = rows();

      int perm;
      std::vector<int> row_index(n);

      mathmatrix<T,S> A_LU(*this);

      LUdecomp<T,mathmatrix<T,S>>(A_LU, row_index.data(), &perm);

      std::vector<T> col(n);
      mathmatrix<T,S> Ainv(n,n);

      for (unsigned int j = 0; j < n; ++j)
	{
	  for (unsigned int i = 0; i < n; ++i) col[i] = 0.;
	  col[j] = 1.;
	  LUbacksub<T,mathmatrix<T,S>>(A_LU, row_index.data(), col.data());
	  for (unsigned int i = 0; i < n; ++i) Ainv(i,j) = col[i];
	}

      return Ainv;
    }

  // Replaces matrix Ainv by inverse, without altering *this.
  // Ainv has to be the same size as *this.
  mathmatrix<T,S> inverse(mathmatrix<T,S>& Ainv) const
    {
      JLT_MATRIX_ASSERT(m == Ainv.m && m == Ainv.n && is_square());
      unsigned int n = rows();

      int perm;
      std::vector<int> row_index(n);

      mathmatrix<T,S> A_LU(*this);

      LUdecomp<T,mathmatrix<T,S>>(A_LU, row_index.data(), &perm);

      std::vector<T> col(n);

      for (unsigned int j = 0; j < n; ++j)
	{
	  for (unsigned int i = 0; i < n; ++i) col[i] = 0.;
	  col[j] = 1.;
	  LUbacksub<T,mathmatrix<T,S>>(A_LU, row_index.data(), col.data());
	  for (unsigned int i = 0; i < n; ++i) Ainv(i,j) = col[i];
	}

      return Ainv;
    }

  //
  // Determinant and trace
  //

  [[nodiscard]] T det() const
    {
      JLT_MATRIX_ASSERT(is_square());

      T det = 1;
      int perm;
      std::vector<int> row_index(columns());

      // The price to pay to leave the object intact is creating a temporary.
      mathmatrix<T,S> A_LU(*this);

      LUdecomp<T,mathmatrix<T,S>>(A_LU, row_index.data(), &perm);

      for (size_type i = 0; i < columns(); ++i) det *= A_LU(i,i);

      return (perm*det);
    }

  [[nodiscard]] T trace() const
    {
      JLT_MATRIX_ASSERT(is_square());

      T tr = 0;

      for (size_type i = 0; i < rows(); ++i)
	{
	  tr += this->operator()(i,i);
	}

      return tr;
    }

  // Frobenius norm: sqrt(sum of |a_ij|^2)
  // Returns real scalar type S, even for complex matrices
  [[nodiscard]] S frobenius_norm() const
    {
      S norm_sq = S();

      for (auto i = this->cbegin(); i != this->cend(); ++i)
	{
	  norm_sq += detail::mag2_traits<T>::compute(*i);
	}

      return std::sqrt(norm_sq);
    }

  [[nodiscard]] polynomial<T> charpoly() const
    {
      JLT_MATRIX_ASSERT(is_square());
      size_type n = rows();
      T t0;
      mathmatrix<T,S> B(n,n), C(n,n);
      polynomial<T> p;

      p[0] = (n % 2 == 0 ? 1 : -1);

      for (size_type l = 0; l < n; ++l)
	{
	  if (l == 0)
	    {
	      for (size_type i = 0; i < n; ++i)
		for (size_type j = 0; j < n; ++j)
		  C(i,j) = this->operator()(i,j);
	    }
	  else
	    {
	      for (size_type i = 0; i < n; ++i)
		{
		  for (size_type j = 0; j < n; ++j)
		    {
		      C(i,j) = 0;
		      for (size_type k = 0; k < n; ++k)
			C(i,j) += B(i,k)*this->operator()(k,j);
		    }
		}
	    }
	  t0 = C.trace()/T(l+1);
	  p[l+1] = -t0*p[0];
	  if (l < n)
	    {
	      for (size_type i = 0; i < n; ++i)
		{
		  for (size_type j = 0; j < n; ++j)
		    {
		      if (j == i)
			B(i,j) = C(i,j)-t0;
		      else
			B(i,j) = C(i,j);
		    }
		}
	    }
	}
      return p;
    }

  //
  // Transpose
  //
  const mathmatrix<T,S>& transpose()
    {
      matrix<T>::transpose();

      return *this;
    }

  // Adjoint (conjugate transpose) - in-place operation
  const mathmatrix<T,S>& adjoint()
    {
      // First transpose
      transpose();

      // Then conjugate each element
      for (auto i = begin(); i != end(); ++i)
	{
	  *i = detail::conj_traits<T>::compute(*i);
	}

      return *this;
    }

  // Aliases for adjoint
  const mathmatrix<T,S>& hermitian_transpose()
    {
      return adjoint();
    }

  const mathmatrix<T,S>& hermitian_conjugate()
    {
      return adjoint();
    }

  // Check if matrix is symmetric (A == A^T)
  // tolerance: absolute tolerance for element comparisons
  [[nodiscard]] bool is_symmetric(S tolerance = S()) const
    {
      JLT_MATRIX_ASSERT(is_square());

      // Use default tolerance based on Frobenius norm if not specified
      if (tolerance == S())
	{
	  tolerance = std::numeric_limits<S>::epsilon() * frobenius_norm();
	}

      for (size_type i = 0; i < rows(); ++i)
	{
	  for (size_type j = i+1; j < columns(); ++j)
	    {
	      // For real matrices: check A(i,j) == A(j,i)
	      // For complex matrices: this checks without conjugation
	      T diff = (*this)(i,j) - (*this)(j,i);
	      if (detail::mag2_traits<T>::compute(diff) > tolerance*tolerance)
		{
		  return false;
		}
	    }
	}

      return true;
    }

  // Check if matrix is Hermitian (A == A^H, where A^H is conjugate transpose)
  // For real matrices, this is the same as is_symmetric
  // tolerance: absolute tolerance for element comparisons
  [[nodiscard]] bool is_hermitian(S tolerance = S()) const
    {
      JLT_MATRIX_ASSERT(is_square());

      // Use default tolerance based on Frobenius norm if not specified
      if (tolerance == S())
	{
	  tolerance = std::numeric_limits<S>::epsilon() * frobenius_norm();
	}

      for (size_type i = 0; i < rows(); ++i)
	{
	  for (size_type j = i+1; j < columns(); ++j)
	    {
	      // Check A(i,j) == conj(A(j,i))
	      T diff = (*this)(i,j) - detail::conj_traits<T>::compute((*this)(j,i));
	      if (detail::mag2_traits<T>::compute(diff) > tolerance*tolerance)
		{
		  return false;
		}
	    }
	}

      return true;
    }

  //
  // Some common matrices
  //

  void identity(const S& a = 1)
    {
      JLT_MATRIX_ASSERT(is_square());

      for (size_type i = 0; i < rows(); ++i)
	{
	  for (size_type j = 0; j < columns(); ++j)
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
  mathmatrix<T,S> res(A.rows(),A.columns());

  {
    auto k = res.begin();
    for (auto i = A.cbegin(); i != A.cend(); ++i, ++k)
      {
	*k = -(*i);
      }
  }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator+(const mathmatrix<T,S>& A,
				 const mathmatrix<T,S>& B)
{
  mathmatrix<T,S> res(A.rows(),A.columns());

  {
    auto k = res.begin();
    for (auto i = A.cbegin(), j = B.cbegin();
	 i != A.cend(); ++i, ++j, ++k)
      {
	*k = *i + *j;
      }
  }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator-(const mathmatrix<T,S>& A,
				 const mathmatrix<T,S>& B)
{
  mathmatrix<T,S> res(A.rows(),A.columns());

  {
    auto k = res.begin();
    for (auto i = A.cbegin(), j = B.cbegin();
	 i != A.cend(); ++i, ++j, ++k)
      {
	*k = *i - *j;
      }
  }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator*(const S& a, const mathmatrix<T,S>& A)
{
  mathmatrix<T,S> res(A.rows(),A.columns());

  {
    auto k = res.begin();
    for (auto i = A.cbegin(); i != A.cend(); ++i, ++k)
      {
	*k = a * (*i);
      }
  }

  return res;
}

template<class T, class S_T, class V, class S_V>
inline mathvector<V,S_V> operator*(const mathmatrix<T,S_T>& A,
				   const mathvector<V,S_V>& v)
{
  auto m = A.rows();
  auto n = A.columns();

  JLT_MATRIX_ASSERT(n == v.size());

  mathvector<V,S_V> res(m);

  for (auto i = 0u; i < m; ++i)
    {
      // Multiplication of a type T and type V must be defined.
      res[i] = A(i,0)*v[0];
      for (auto k = 1u; k < n; ++k)
	res[i] += A(i,k)*v[k];
    }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator*(const mathmatrix<T,S>& A, const S& a)
{
  mathmatrix<T,S> res(A.rows(),A.columns());

  {
    auto k = res.begin();
    for (auto i = A.cbegin(); i != A.cend(); ++i, ++k)
      {
	*k = a * (*i);
      }
  }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator/(const mathmatrix<T,S>& A, const S& a)
{
  mathmatrix<T,S> res(A.rows(),A.columns());

  {
    auto k = res.begin();
    for (auto i = A.cbegin(); i != A.cend(); ++i, ++k)
      {
	*k = (*i) / a;
      }
  }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> operator*(const mathmatrix<T,S>& A,
				 const mathmatrix<T,S>& B)
{
  auto ma = A.rows();
  auto na = A.columns();
  auto nb = B.columns();

  JLT_MATRIX_ASSERT(na == B.rows());

  mathmatrix<T,S> res(ma,nb);

  for (auto i = 0u; i < ma; ++i)
    {
      for (auto j = 0u; j < nb; ++j)
	{
	  for (auto k = 0u; k < na; ++k) res(i,j) += A(i,k)*B(k,j);
	}
    }

  return res;
}

template<class T, class S>
inline mathmatrix<T,S> identity_matrix(typename mathmatrix<T,S>::size_type n)
{
  mathmatrix<T,S> id(n,n);

  for (auto i = 0u; i < n; ++i) id(i,i) = 1;

  return id;
}

template<class T>
inline mathmatrix<T> identity_matrix(typename mathmatrix<T>::size_type n)
{
  mathmatrix<T> id(n,n);

  for (auto i = 0u; i < n; ++i) id(i,i) = 1;

  return id;
}

template<class T, class S>
inline mathmatrix<T,S> diagonal_matrix(const mathvector<T,S>& v)
{
  auto n = v.size();
  mathmatrix<T,S> diag(n,n);

  for (auto i = 0u; i < n; ++i) diag(i,i) = v[i];

  return diag;
}

template<class T, class S>
inline mathmatrix<T,S> diagonal_matrix(const mathvector<T,S>& v,
				       typename mathmatrix<T>::size_type m,
				       typename mathmatrix<T>::size_type n)
{
  auto d = std::min(m,n);
  assert(v.size() == d);
  mathmatrix<T,S> diag(m,n);

  for (auto i = 0u; i < d; ++i) diag(i,i) = v[i];

  return diag;
}

// Standalone transpose function - returns a transposed copy of the matrix
template<class T, class S>
[[nodiscard]]
inline mathmatrix<T,S> transpose(const mathmatrix<T,S>& M)
{
  mathmatrix<T,S> result(M);
  result.transpose();
  return result;
}

// Standalone adjoint function - returns conjugate transpose of the matrix
template<class T, class S>
[[nodiscard]]
inline mathmatrix<T,S> adjoint(const mathmatrix<T,S>& M)
{
  mathmatrix<T,S> result(M);
  result.adjoint();
  return result;
}

// Aliases for adjoint
template<class T, class S>
[[nodiscard]]
inline mathmatrix<T,S> hermitian_transpose(const mathmatrix<T,S>& M)
{
  return adjoint(M);
}

template<class T, class S>
[[nodiscard]]
inline mathmatrix<T,S> hermitian_conjugate(const mathmatrix<T,S>& M)
{
  return adjoint(M);
}

// Check if matrix is symmetric
template<class T, class S>
[[nodiscard]]
inline bool is_symmetric(const mathmatrix<T,S>& M, S tolerance = S())
{
  return M.is_symmetric(tolerance);
}

// Check if matrix is hermitian
template<class T, class S>
[[nodiscard]]
inline bool is_hermitian(const mathmatrix<T,S>& M, S tolerance = S())
{
  return M.is_hermitian(tolerance);
}

} // namespace jlt

#endif // JLT_MATHMATRIX_HPP
