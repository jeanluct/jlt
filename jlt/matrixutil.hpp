#ifndef JLT_MATRIXUTIL_HPP
#define JLT_MATRIXUTIL_HPP

#include <iostream>
#include <algorithm>
#include <jlt/math.hpp>

#ifndef MATRIX_ASSERT
#  define MATRIX_ASSERT(x)
#endif

namespace jlt {

template<class T, class T_Matrix>
void LUdecomp(T_Matrix& A, int* row_index, int* perm)
{
  int	imax = 0;
  int	n = A.dim();
  T	big, dum, sum, temp;

  const T tiny = 1.e-20;

  T* vv = new T[n];

  *perm=1;

  for (int i = 0; i < n; ++i)
    {
      big = 0.0;
      for (int j = 0; j < n; ++j)
	if (Abs(temp = Abs(A(i,j))) > Abs(big)) big = temp;
      if (big == 0.0)
	{
	  std::cerr << "Singular Matrix in LUdecomp\n";
	  exit(1);
	}
      vv[i]=1.0/big;
    }
  for (int j = 0; j < n; ++j) {
    for (int i = 0; i < j; ++i) {
      sum = A(i,j);
      for (int k = 0; k < i ; ++k) sum -= A(i,k)*A(k,j);
      A(i,j) = sum;
    }
    big = 0.0;
    for (int i = j; i < n; ++i)
      {
	sum=A(i,j);
	for (int k = 0; k < j; ++k)
	  sum -= A(i,k)*A(k,j);
	A(i,j) = sum;
	if (Abs(dum = vv[i]*Abs(sum)) >= Abs(big)) {
	  big = dum;
	  imax = i;
	}
      }
    if (j != imax)
      {
	for (int k = 0; k < n; ++k)
	  {
	    dum = A(imax,k);
	    A(imax,k) = A(j,k);
	    A(j,k) = dum;
	  }
	*perm = -(*perm);
	vv[imax] = vv[j];
      }
    row_index[j] = imax;
    if (A(j,j) == 0.0) A(j,j) = tiny;
    if (j != n) {
      dum = 1.0/(A(j,j));
      for (int i = j+1; i < n; ++i) A(i,j) *= dum;
    }
  }
  delete[] vv;
}

template<class T, class T_Matrix>
void LUbacksub(T_Matrix& A, int* row_index, T* b)
{
  int n = A.dim();
  int i_nonzero = -1, i_perm;
  T sum;

  for (int i = 0; i < n; ++i)
    {
      i_perm = row_index[i];
      sum = b[i_perm];
      b[i_perm] = b[i];

      if (i_nonzero >= 0)
	for (int j = i_nonzero; j < i; ++j) sum -= A(i,j)*b[j];
      else
	if (Abs(sum)) i_nonzero = i;

      b[i] = sum;
    }

  for (int i = n-1; i >= 0 ; --i)
    {
      sum = b[i];
      for (int j = i+1; j < n; ++j) sum -= A(i,j)*b[j];
      b[i] = sum/A(i,i);
    }
}

template<class T, class T_Matrix>
T_Matrix inverse(T_Matrix& A)
{
  int n = A.dim();
  int perm;

  int* row_index = new int[n];

  LUdecomp<T,T_Matrix>(A, row_index, &perm);

  T* col = new T[n];
  T_Matrix Ainv(n,n);

  for (int j = 0; j < n; ++j)
    {
      for (int i = 0; i < n; ++i) col[i] = 0.;
      col[j] = 1.;
      LUbacksub<T,T_Matrix>(A, row_index, col);
      for (int i = 0; i < n; ++i) Ainv(i,j) = col[i];
    }

  delete[] row_index;
  delete[] col;

  return Ainv;
}


template<class T, class T_Matrix, class T_Vector>
bool QRdecomp(T_Matrix& A, T_Vector& c, T_Vector& d, int m)
{
  //
  // Constructs the QR decomposition of A(0..n-1, 0..n-1).
  //
  // The upper triangular matrix R is returned in the upper triangle
  // of A, except for the diagonal elements of R which are returned in
  // d(0..n-1). The orthogonal matrix Q is represented as a product of
  // n-1 Householder matrices Q_0 ... Q_{n-2} , where
  //
  // 	Q_j = 1 - (u_j u_j) / c_j ,
  //
  // so c_j = 0.5*|u_j|^2.  The ith component of u_j is zero for i =
  // 0...j-2 while the nonzero components are returned in A(i,j) for i
  // = j...n-1. The function returns true if singularity is
  // encountered during the decomposition, but the decomposition is
  // still completed in this case; otherwise it returns false.
  //

  bool sing = false;
  T scale = 0.;

  for (int k = 0; k < m-1; ++k)
    {
      for (int i = k; i < m; ++i) scale = std::max(scale, Abs(A(i,k)));
      if (scale == 0.)
	{
	  sing = true;
	  c[k] = d[k] = 0.;
	} else {
	  for (int i = k; i < m; ++i) A(i,k) /= scale;

	  T sum = 0.;
	  for (int i = k; i < m; ++i) sum += A(i,k)*A(i,k);

	  T sigma = (A(k,k) >= 0. ? Abs(Sqrt(sum)) : -Abs(Sqrt(sum)));

	  A(k,k) += sigma;
	  c[k] = sigma*A(k,k);
	  d[k] = -scale*sigma;

	  for (int j = k+1; j < m; ++j)
	    {
	      sum = 0.;
	      for (int i = k; i < m; ++i) sum += A(i,k)*A(i,j);
	      T tau = sum/c[k];
	      for (int i = k; i < m; ++i) A(i,j) -= tau*A(i,k);
	    }
	}
    }

  d[m-1] = A(m-1,m-1);

  if (d[m-1] == 0.) sing = true;

  return sing;
}


// If no subblock size is specified, the default is the size of d.
template<class T, class T_Matrix, class T_Vector>
bool QRdecomp(T_Matrix& A, T_Vector& c, T_Vector& d)
{
  return (QRdecomp<T,T_Matrix,T_Vector>(A,c,d,d.size()));
}


template<class T, class T_Matrix, class T_Vector>
void QRextract(const T_Matrix& A, const T_Vector& c, const T_Vector& d,
	       T_Matrix& Q, T_Matrix& R, int m)
{
  //
  // QRdecomp returns a product of Householder matrices.  Need to
  // multiply these to obtain Q.
  //
  // Inspired by the NRC algorithm qrsolv, p. 100 of Second edition.
  //

  for (int k = 0; k < m; ++k) {
    for (int l = 0; l < m; ++l) Q(k,l) = (k == l ? 1 : 0);
    for (int j = 0; j < m-1; ++j) {
      T sum = 0.0;
      for (int i = j; i < m; ++i) sum += A(i,j)*Q(k,i);
      T tau = sum/c[j];
      for (int i = j; i < m; ++i) Q(k,i) -= tau*A(i,j);
    }
  }

  for (int i = 0; i < m; ++i) {
    R(i,i) = d[i];
    for (int j = 0; j < i; ++j) R(i,j) = 0.;
    for (int j = i+1; j < m; ++j) R(i,j) = A(i,j);
  }
}

// If no subblock size is specified, the default is the size of d.
template<class T, class T_Matrix, class T_Vector>
void QRextract(const T_Matrix& A, const T_Vector& c, const T_Vector& d,
	       T_Matrix& Q, T_Matrix& R)
{
  QRextract<T,T_Matrix,T_Vector>(A,c,d,Q,R,d.size());
}


// This is generally the only function one ever needs: given A, return
// Q and R.  Not the most efficient routine because of a few
// temporaries, but nice and clean.  A is destroyed in the process,
// though.
template<class T, class T_Matrix, class T_Vector>
bool QRdecomp(T_Matrix& A, T_Matrix& Q, T_Matrix& R)
{
  bool sing;
  int m = A.dim1();
  MATRIX_ASSERT(m == A.dim2());		// It's hip to be square.

  T_Vector c(m), d(m);

  sing = QRdecomp<T,T_Matrix,T_Vector>(A,c,d,m);

  QRextract<T,T_Matrix,T_Vector>(A,c,d,Q,R,m);

  // Make diagonal elements of R positive.
  for (int j = 0; j < m; ++j) {
    if (d[j] < 0) {
      d[j] = -d[j];
      // Change sign of row in R.
      for (int i = j; i < m; ++i) R(j,i) = -R(j,i);
      // Must also change sign of corresponding column in Q.
      for (int i = 0; i < m; ++i) Q(i,j) = -Q(i,j);
    }
  }

  return sing;
}


//
// Gram-Schmidt Orthonormalization.
//
//  Start from the first row, work our way down.
//  Each row is normalized to 1 by the routine.
//
template<class T_Matrix>
void GramSchmidtOrthonorm(T_Matrix& A)
{
  int	n = A.dim();

  for (int i = 0; i < n; ++i) {

      // Normalize the vector (row)
      double norm = 0;
      for (int j = 0; j < n; ++j) norm += A(i,j)*A(i,j);
      norm = Sqrt(norm);
      for (int j = 0; j < n; ++j) A(i,j) /= norm;

      // Subtract component of ith vector (row) from subsequent ones.
      for (int j = i+1; j < n; ++j) {
	// Projection of vector i onto j (vector i is normalized)
	double proj = 0;
	for (int k = 0; k < n; ++k) proj += A(i,k)*A(j,k);
	// Subtract off projection
	for (int k = 0; k < n; ++k) A(j,k) -= proj*A(i,k);
      }
    }
}

// Save the norms of each vector.
template<class T_Matrix, class T_Vector>
void GramSchmidtOrthonorm(T_Matrix& A, T_Vector& norm)
{
  int	n = A.dim();

  for (int i = 0; i < n; ++i) {

      // Normalize the vector (row)
      norm[i] = 0;
      for (int j = 0; j < n; ++j) norm[i] += A(i,j)*A(i,j);
      norm[i] = Sqrt(norm[i]);
      for (int j = 0; j < n; ++j) A(i,j) /= norm[i];

      // Subtract component of ith vector (row) from subsequent ones.
      for (int j = i+1; j < n; ++j) {
	// Projection of vector i onto j (vector i is normalized)
	double proj = 0;
	for (int k = 0; k < n; ++k) proj += A(i,k)*A(j,k);
	// Subtract off projection
	for (int k = 0; k < n; ++k) A(j,k) -= proj*A(i,k);
      }
    }
}

// Save the projections onto the unit vectors.
template<class T_Matrix>
void GramSchmidtOrthonorm(T_Matrix& A, T_Matrix& proj)
{
  int	n = A.dim();

  for (int i = 0; i < n; ++i) {

      // Normalize the vector (row)
      proj(i,i) = 0;
      for (int j = 0; j < n; ++j) proj(i,i) += A(i,j)*A(i,j);
      proj(i,i) = Sqrt(proj(i,i));
      for (int j = 0; j < n; ++j) A(i,j) /= proj(i,i);

      // Subtract component of ith vector (row) from subsequent ones.
      for (int j = i+1; j < n; ++j) {
	// Projection of vector i onto j (vector i is normalized)
	proj(j,i) = 0;
	for (int k = 0; k < n; ++k) proj(j,i) += A(i,k)*A(j,k);
	// Subtract off projection
	for (int k = 0; k < n; ++k) A(j,k) -= proj(j,i)*A(i,k);
      }
    }
}

} // namespace jlt

#endif // JLT_MATRIXUTIL_HPP
