#ifndef JLT_EIGENSYSTEM_HPP
#define JLT_EIGENSYSTEM_HPP

#include <vector>
#include <jlt/matrix.hpp>
#if defined(__PGI)
#  include <assert.h>
#else
#  include <cassert>
#endif
#include <algorithm>

#ifdef JLT_USE_LAPACK
#define F77_SSYEV   ssyev_
#define F77_DSYEV   dsyev_

// Fortran routine from LAPACK
extern "C"
{
  // Eigenvalues and eigenvectors routines

  // Symmetric matrix (single precision)
  void F77_SSYEV(char* jobz, char* uplo, int* N, float* A, int* lda, 
		   float* W, float* work, int* lwork, int* info);

  // Symmetric matrix (double precision)
  void F77_DSYEV(char* jobz, char* uplo, int* N, double* A, int* lda, 
		   double* W, double* work, int* lwork, int* info);
}
#else // JLT_USE_LAPACK

#include <nr.h>
#include <nrutil.h>

#endif // JLT_USE_LAPACK

namespace jlt {

template<class T>
int symmetric_matrix_eigensystem(matrix<T>& A,
				 std::vector<T>& eigvals,
				 std::vector<T>& work)
{
  std::cerr << "symmetric_matrix_eigensystem:\n";
  std::cerr << "You cannot perform this math operation on this type.\n";
  exit(1);
}

#ifdef JLT_USE_LAPACK
int symmetric_matrix_eigensystem(matrix<float>& A,
				 std::vector<float>& eigvals,
				 std::vector<float>& work)
{
  char jobz = 'V';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  char uplo = 'L';	// 'L'ower or 'U'pper triangle stored (opposite)
  int N = A.dim1();	// Dimensions of matrix.
  int worksize = work.size();

# ifdef __PGI
# else
    assert(N == (int)A.dim2() && N == (int)eigvals.size());
    assert((int)work.size() >= max(1,3*N-1));
# endif

  int info = 0;		// Output eigenvalues in ascending order.

  F77_SSYEV(&jobz, &uplo, &N, A.begin(), &N, eigvals.begin(), work.begin(),
	    &worksize, &info);

  return info;
}

int symmetric_matrix_eigensystem(matrix<double>& A,
				 std::vector<double>& eigvals,
				 std::vector<double>& work)
{
  char jobz = 'V';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  char uplo = 'L';	// 'L'ower or 'U'pper triangle stored (opposite)
  int N = A.dim1();	// Dimensions of matrix.
  int worksize = work.size();

# ifdef __PGI
# else
    assert(N == (int)A.dim2() && N == (int)eigvals.size());
    assert((int)work.size() >= max(1,3*N-1));
# endif

  int info = 0;		// Output eigenvalues in ascending order.

  F77_DSYEV(&jobz, &uplo, &N, A.begin(), &N, eigvals.begin(), work.begin(),
	    &worksize, &info);

  return info;
}

int symmetric_matrix_eigensystem(matrix<float>& A,
				 std::vector<float>& eigvals)
{
  // Allocate the workspace before calling LAPACK.  Inefficient, but
  // will do if speed not an issue.  The minimum worksize is 3N - 1,
  // but the optimal number is much larger, as returned in work[0]
  // after a call to ssyev or dsyev (see man page for ssyev or dsyev).
  int worksize = 3 * A.dim1();
  std::vector<float> work(worksize);

  return symmetric_matrix_eigensystem(A,eigvals,work);
}

int symmetric_matrix_eigensystem(matrix<double>& A,
				 std::vector<double>& eigvals)
{
  // See comment in float version.
  int worksize = 3 * A.dim1();
  std::vector<double> work(worksize);

  return symmetric_matrix_eigensystem(A,eigvals,work);
}

#else
int symmetric_matrix_eigensystem(matrix<double>& A,
				 std::vector<double>& eigvals)
{
  int n = A.dim1();	// Dimensions of matrix.

# ifdef __PGI
# else
    assert(n == (int)A.dim2() && n == (int)eigvals.size());
# endif

  // Allocate NRC-style matrices and vectors.
  double **a = numrec::dmatrix(1,n,1,n);
  double *e = numrec::dvector(1,n);
  double *d = numrec::dvector(1,n);

  // Copy the matrices.
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      a[i][j] = A(i-1,j-1);
    }
  }

  numrec::tred2(a,n,d,e);
  numrec::tqli(d,e,n,a);

  // Sort eigenvalues in descending order.
  numrec::eigsrt(d,a,n);

  // Copy the result, storing the eigenvectors in the rows.
  for (int i = 1; i <= n; ++i) {
    eigvals[i-1] = d[i];
    for (int j = 1; j <= n; ++j) {
      A(j-1,i-1) = a[i][j];
    }
  }

  numrec::free_dmatrix(a,1,n,1,n);
  numrec::free_dvector(e,1,n);
  numrec::free_dvector(d,1,n);

  return 0;
}

int symmetric_matrix_eigensystem(matrix<long double>& A,
				 std::vector<long double>& eigvals)
{
  int n = A.dim1();	// Dimensions of matrix.

# ifdef __PGI
# else
    assert(n == (int)A.dim2() && n == (int)eigvals.size());
# endif

  // Allocate NRC-style matrices and vectors.
  long double **a = numrec::ldmatrix(1,n,1,n);
  long double *e = numrec::ldvector(1,n);
  long double *d = numrec::ldvector(1,n);

  // Copy the matrices.
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      a[i][j] = A(i-1,j-1);
    }
  }

  numrec::tred2l(a,n,d,e);
  numrec::tqlil(d,e,n,a);

  // Sort eigenvalues in descending order.
  numrec::eigsrtl(d,a,n);

  // Copy the result, storing the eigenvectors in the rows.
  for (int i = 1; i <= n; ++i) {
    eigvals[i-1] = d[i];
    for (int j = 1; j <= n; ++j) {
      A(j-1,i-1) = a[i][j];
    }
  }

  numrec::free_ldmatrix(a,1,n,1,n);
  numrec::free_ldvector(e,1,n);
  numrec::free_ldvector(d,1,n);

  return 0;
}
#endif

} // namespace jlt

#endif // JLT_EIGENSYSTEM_HPP
