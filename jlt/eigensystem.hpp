#ifndef JLT_EIGENSYSTEM_HPP
#define JLT_EIGENSYSTEM_HPP

#include <vector>
#include <complex>
#include <jlt/matrix.hpp>
#include <jlt/lapack.hpp>

#if defined(__PGI)
#  include <assert.h>
#else
#  include <cassert>
#endif
#include <algorithm>


namespace jlt {

template<class T>
int symmetric_matrix_eigensystem(matrix<T>& A,
				 std::vector<T>& eigvals)
{
  char jobz = 'V';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  char uplo = 'L';	// 'L'ower or 'U'pper triangle stored (opposite)
  int N = A.dim1();	// Dimensions of matrix.

# ifdef __PGI
# else
    assert(N == (int)A.dim2() && N == (int)eigvals.size());
# endif

  int info;

  // Use temporary vector and copy of A, since we need to reverse the order.
  std::vector<T> eigs(N);
  matrix<T> U(A);

  // Call the routine with worksize = -1, to get the ideal size of workspace.
  int worksize = -1;
  T tmpwork[1];

  lapack::syev(&jobz, &uplo, &N, &(*U.begin()), &N,
	       &(*eigs.begin()), tmpwork, &worksize, &info);

  // Now allocate the memory for the workspace.
  worksize = (int)tmpwork[0];
  std::vector<T> work(worksize);

  lapack::syev(&jobz, &uplo, &N, &(*U.begin()), &N,
	       &(*eigs.begin()), &(*work.begin()), &worksize, &info);

  // Output eigenvalues in *descending* order.
  for (int i = 0; i < N; ++i) eigvals[i] = eigs[N-i-1];
  // Also need to reverse the eigenvectors, stored as row vectors.
  for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
	{
	  A(i,j) = U(N-i-1,j);
	}
    }

  return info;
}


template<class T>
int matrix_eigenvalues(matrix<T>& A,
		       std::vector<std::complex<T> >& eigvals)
{
  char jobVL = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  char jobVR = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  int N = A.dim1();	// Dimensions of matrix.

# ifdef __PGI
# else
    assert(N == (int)A.dim2() && N == (int)eigvals.size());
# endif

  int info, ldVL = 1, ldVR = 1;

  std::vector<T> evr(N), evi(N);

  // Call the routine with worksize = -1, to get the ideal size of workspace.
  int worksize = -1;
  T tmpwork[1];

  lapack::geev(&jobVL, &jobVR, &N, &(*A.begin()), &N,
	       &(*evr.begin()), &(*evi.begin()), 0, &ldVL, 0, &ldVR,
	       tmpwork, &worksize, &info);

  // Now allocate the memory for the workspace.
  worksize = (int)tmpwork[0];
  std::vector<T> work(worksize);

  lapack::geev(&jobVL, &jobVR, &N, &(*A.begin()), &N,
	       &(*evr.begin()), &(*evi.begin()), 0, &ldVL, 0, &ldVR,
	       &(*work.begin()), &worksize, &info);

  for (int n = 0; n < N; ++n)
    {
      eigvals[n] = std::complex<T>(evr[n],evi[n]);
    }

  return info;
}


template<class T>
int matrix_eigenvalues(matrix<std::complex<T> >& A,
		       std::vector<std::complex<T> >& eigvals)
{
  char jobVL = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  char jobVR = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  int N = A.dim1();	// Dimensions of matrix.
  int rworksize = 2 * A.dim1();
  std::vector<T> rwork(rworksize);

# ifdef __PGI
# else
    assert(N == (int)A.dim2() && N == (int)eigvals.size());
# endif

  int info, ldVL = 1, ldVR = 1;

  // Call the routine with cworksize = -1, to get the ideal size of workspace.
  int cworksize = -1;
  std::complex<T> ctmpwork[1];

  lapack::geev(&jobVL, &jobVR, &N, &(*A.begin()), &N,
	       &(*eigvals.begin()), 0, &ldVL, 0, &ldVR,
	       ctmpwork, &cworksize, &(*rwork.begin()), &info);

  // Now allocate the memory for the workspace.
  cworksize = (int)ctmpwork[0].real();
  std::cerr << cworksize << std::endl;
  std::vector<std::complex<T> > cwork(cworksize);

  lapack::geev(&jobVL, &jobVR, &N, &(*A.begin()), &N,
	       &(*eigvals.begin()), 0, &ldVL, 0, &ldVR,
	       &(*cwork.begin()), &cworksize, &(*rwork.begin()), &info);

  return info;
}


} // namespace jlt

#endif // JLT_EIGENSYSTEM_HPP
