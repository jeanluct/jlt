#ifndef JLT_EIGENSYSTEM_HPP
#define JLT_EIGENSYSTEM_HPP

#include <vector>
#include <complex>
#include <jlt/matrix.hpp>

#if defined(__PGI)
#  include <assert.h>
#else
#  include <cassert>
#endif
#include <algorithm>

// Symmetric real matrices
#define F77_SSYEV   ssyev_
#define F77_DSYEV   dsyev_
// Nonymmetric real matrices
#define F77_SGEEV   sgeev_
#define F77_DGEEV   dgeev_
// Hermitian complex matrices
#define F77_CHEEV   cheev_
#define F77_ZHEEV   zheev_
// Nonsymmetric complex matrices
#define F77_CGEEV   cgeev_
#define F77_ZGEEV   zgeev_

// Fortran routines from LAPACK
extern "C"
{
  // Eigenvalues and eigenvectors routines

  // Symmetric real matrix (single precision)
  void F77_SSYEV(char* jobz, char* uplo, int* N, float* A, int* ldA,
		 float* W, float* work, int* lwork, int* info);

  // Symmetric real matrix (double precision)
  void F77_DSYEV(char* jobz, char* uplo, int* N, double* A, int* ldA,
		 double* W, double* work, int* lwork, int* info);

  // Nonsymmetric real matrix (double precision)
  void F77_DGEEV(char* jobVL, char* jobVR, int* N, double* A, int* ldA,
		 double* Wr, double* Wi,
		 double *VL, int *ldVL, double *VR, int *ldVR,
		 double* work, int* lwork, int* info);

  // Nonsymmetric complex matrix (double precision)
  void F77_ZGEEV(char* jobVL, char* jobVR, int* N, double* A, int* ldA,
		 double* W, double *VL, int *ldVL, double *VR, int *ldVR,
		 double* cwork, int* lwork, double* rwork, int* info);
}


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

template<class T>
int matrix_eigenvalues(matrix<T>& A,
		       std::vector<T>& eigvals,
		       std::vector<T>& work)
{
  std::cerr << "matrix_eigenvalues:\n";
  std::cerr << "You cannot perform this math operation on this type.\n";
  exit(1);
}

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
    assert((int)work.size() >= std::max(1,3*N-1));
# endif

  int info;

  std::vector<float> eigs(N);

  F77_SSYEV(&jobz, &uplo, &N, &(*A.begin()), &N,
	    &(*eigs.begin()), &(*work.begin()), &worksize, &info);

  // Output eigenvalues in *descending* order.
  for (int i = 0; i < N; ++i) eigvals[i] = eigs[N-i-1];

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
    assert((int)work.size() >= std::max(1,3*N-1));
# endif

  int info;

  std::vector<double> eigs(N);

  F77_DSYEV(&jobz, &uplo, &N, &(*A.begin()), &N,
	    &(*eigs.begin()), &(*work.begin()), &worksize, &info);

  // Output eigenvalues in *descending* order.
  for (int i = 0; i < N; ++i) eigvals[i] = eigs[N-i-1];

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


int matrix_eigenvalues(matrix<double>& A,
		       std::vector<std::complex<double> >& eigvals,
		       std::vector<double>& work)
{
  char jobVL = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  char jobVR = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  int N = A.dim1();	// Dimensions of matrix.
  int worksize = work.size();

# ifdef __PGI
# else
    assert(N == (int)A.dim2() && N == (int)eigvals.size());
    /*
    if (jobVL == 'V' || jobVR == 'V')
      assert((int)work.size() >= std::max(1,4*N));
    else
    */
    assert((int)work.size() >= std::max(1,3*N));
# endif

  int info, ldVL = 1, ldVR = 1;

  std::vector<double> evr(N), evi(N);

  F77_DGEEV(&jobVL, &jobVR, &N, &(*A.begin()), &N,
	    &(*evr.begin()), &(*evi.begin()), 0, &ldVL, 0, &ldVR,
	    &(*work.begin()), &worksize, &info);

  for (int n = 0; n < N; ++n)
    {
      eigvals[n] = std::complex<double>(evr[n],evi[n]);
    }

  return info;
}

int matrix_eigenvalues(matrix<std::complex<double> >& A,
		       std::vector<std::complex<double> >& eigvals,
		       std::vector<std::complex<double> >& cwork,
		       std::vector<double>& rwork)
{
  char jobVL = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  char jobVR = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  int N = A.dim1();	// Dimensions of matrix.
  int cworksize = cwork.size();

# ifdef __PGI
# else
    assert((int)cwork.size() >= std::max(1,2*N));
    assert((int)rwork.size() >= std::max(1,2*N));
# endif

  int info, ldVL = 1, ldVR = 1;

  F77_ZGEEV(&jobVL, &jobVR, &N, (double *)&(*A.begin()), &N,
	    (double *)&(*eigvals.begin()), 0, &ldVL, 0, &ldVR,
	    (double *)&(*cwork.begin()), &cworksize, &(*rwork.begin()), &info);

  return info;
}

int matrix_eigenvalues(matrix<double>& A,
		       std::vector<std::complex<double> >& eigvals)
{
  // See comment in float version.
  int worksize = 3 * A.dim1();
  std::vector<double> work(worksize);

  return matrix_eigenvalues(A,eigvals,work);
}

int matrix_eigenvalues(matrix<std::complex<double> >& A,
		       std::vector<std::complex<double> >& eigvals)
{
  // See comment in float version.
  int cworksize = 2 * A.dim1();
  std::vector<std::complex<double> > cwork(cworksize);
  int rworksize = 2 * A.dim1();
  std::vector<double> rwork(rworksize);

  return matrix_eigenvalues(A,eigvals,cwork,rwork);
}

} // namespace jlt

#endif // JLT_EIGENSYSTEM_HPP
