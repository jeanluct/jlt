#ifndef JLT_SVDECOMP_HPP
#define JLT_SVDECOMP_HPP

#include <nr.h>
#include <nrutil.h>

#if defined(__PGI)
#  include <assert.h>
#else
#  include <cassert>
#endif

using namespace numrec;

namespace jlt {

//
// Given a matrix A, compute the singular value decomposition
//
//   M = U.diag(w).transp(V)
//
// where U and V are orthogonal, and w is a vector of singular values.
// The singular values are positive and sorted by decreasing size.
//
// The m by n matrix A is the input (m>n), and is overwritten by U
// on return.
//

int singular_value_decomp(matrix<double>& A,
			  matrix<double>& V,
			  vector<double>& W)
{
  int m = A.dim1(), n = A.dim2();	// Dimensions of matrix.

  assert(m >= n);
  assert(n == (int)W.size());
  assert(n == (int)V.dim1() && n == (int)V.dim2());

  // Allocate NRC-style matrices and vectors.
  double **a = dmatrix(1,m,1,n);
  double **v = dmatrix(1,n,1,n);
  double *w = dvector(1,n);

  // Copy the matrix.
  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      a[i][j] = A(i-1,j-1);
    }
  }

  svdcmp(a,m,n,w,v);
  svdsrt(a,m,n,w,v);

  // Copy the result.
  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      A(i-1,j-1) = a[i][j];
    }
  }
  for (int i = 1; i <= n; ++i) {
    W[i-1] = w[i];
    for (int j = 1; j <= n; ++j) {
      V(i-1,j-1) = v[i][j];
    }
  }

  free_dmatrix(a,1,n,1,n);
  free_dmatrix(v,1,n,1,n);
  free_dvector(w,1,n);

  return 0;
}


int singular_value_decomp(matrix<long double>& A,
			  matrix<long double>& V,
			  vector<long double>& W)
{
  int m = A.dim1(), n = A.dim2();	// Dimensions of matrix.

  assert(m >= n);
  assert(n == (int)W.size());
  assert(n == (int)V.dim1() && n == (int)V.dim2());

  // Allocate NRC-style matrices and vectors.
  long double **a = ldmatrix(1,m,1,n);
  long double **v = ldmatrix(1,n,1,n);
  long double  *w = ldvector(1,n);

  // Copy the matrix.
  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      a[i][j] = A(i-1,j-1);
    }
  }

  svdcmpl(a,m,n,w,v);
  svdsrtl(a,m,n,w,v);

  // Copy the result.
  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      A(i-1,j-1) = a[i][j];
    }
  }
  for (int i = 1; i <= n; ++i) {
    W[i-1] = w[i];
    for (int j = 1; j <= n; ++j) {
      V(i-1,j-1) = v[i][j];
    }
  }

  free_ldmatrix(a,1,n,1,n);
  free_ldmatrix(v,1,n,1,n);
  free_ldvector(w,1,n);

  return 0;
}

} // namespace jlt

#endif // JLT_SVDECOMP_HPP
