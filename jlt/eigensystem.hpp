//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_EIGENSYSTEM_HPP
#define JLT_EIGENSYSTEM_HPP

#include <vector>
#include <complex>
#include <jlt/matrix.hpp>
#include <jlt/lapack.hpp>

#include <cassert>
#include <algorithm>

// No data() method in std::vector prior to GCC 4.1.
#if (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 1))
#  define JLT_NO_VECTOR_DATA_METHOD
#endif

//
// All these routines destroy the data in A!
//

namespace jlt {

template<class T>
int symmetric_matrix_eigensystem(matrix<T>& A,
				 std::vector<T>& eigvals)
{
  char jobz = 'V';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  char uplo = 'L';	// 'L'ower or 'U'pper triangle stored (opposite)
  int N = A.rows();	// Dimensions of matrix.

  assert(N == (int)A.columns() && N == (int)eigvals.size());

  int info;

  // Use temporary vector and copy of A, since we need to reverse the order.
  std::vector<T> eigs(N);
  matrix<T> U(A);

  // Call the routine with worksize = -1, to get the ideal size of workspace.
  int worksize = -1;
  T tmpwork[1];

# if !defined(JLT_NO_VECTOR_DATA_METHOD)
  lapack::syev(&jobz, &uplo, &N, U.data(), &N, eigs.data(),
	       tmpwork, &worksize, &info);
# else
  lapack::syev(&jobz, &uplo, &N, &(*U.begin()), &N,
	       &(*eigs.begin()), tmpwork, &worksize, &info);
# endif

  // Now allocate the memory for the workspace.
  worksize = (int)tmpwork[0];
  std::vector<T> work(worksize);

# if !defined(JLT_NO_VECTOR_DATA_METHOD)
  lapack::syev(&jobz, &uplo, &N, U.data(), &N, eigs.data(),
	       work.data(), &worksize, &info);
# else
  lapack::syev(&jobz, &uplo, &N, &(*U.begin()), &N,
	       &(*eigs.begin()), &(*work.begin()), &worksize, &info);
# endif

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
  int N = A.rows();	// Dimensions of matrix.

  assert(N == (int)A.columns() && N == (int)eigvals.size());

  int info, ldVL = 1, ldVR = 1;

  std::vector<T> evr(N), evi(N);

  // Call the routine with worksize = -1, to get the ideal size of workspace.
  int worksize = -1;
  T tmpwork[1];

# if !defined(JLT_NO_VECTOR_DATA_METHOD)
  lapack::geev(&jobVL, &jobVR, &N, A.data(), &N,
	       evr.data(), evi.data(), 0, &ldVL, 0, &ldVR,
	       tmpwork, &worksize, &info);
# else
  lapack::geev(&jobVL, &jobVR, &N, &(*A.begin()), &N,
	       &(*evr.begin()), &(*evi.begin()), 0, &ldVL, 0, &ldVR,
	       tmpwork, &worksize, &info);
# endif

  // Now allocate the memory for the workspace.
  worksize = (int)tmpwork[0];
  std::vector<T> work(worksize);

# if !defined(JLT_NO_VECTOR_DATA_METHOD)
  lapack::geev(&jobVL, &jobVR, &N, A.data(), &N,
	       evr.data(), evi.data(), 0, &ldVL, 0, &ldVR,
	       work.data(), &worksize, &info);
# else
  lapack::geev(&jobVL, &jobVR, &N, &(*A.begin()), &N,
	       &(*evr.begin()), &(*evi.begin()), 0, &ldVL, 0, &ldVR,
	       &(*work.begin()), &worksize, &info);
# endif

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
  int N = A.rows();	// Dimensions of matrix.
  int rworksize = 2 * A.rows();
  std::vector<T> rwork(rworksize);

  assert(N == (int)A.columns() && N == (int)eigvals.size());

  int info, ldVL = 1, ldVR = 1;

  // Call the routine with cworksize = -1, to get the ideal size of workspace.
  int cworksize = -1;
  std::complex<T> ctmpwork[1];

# if !defined(JLT_NO_VECTOR_DATA_METHOD)
  lapack::geev(&jobVL, &jobVR, &N, A.data(), &N,
	       eigvals.data(), 0, &ldVL, 0, &ldVR,
	       ctmpwork, &cworksize, rwork.data(), &info);
# else
  lapack::geev(&jobVL, &jobVR, &N, &(*A.begin()), &N,
	       &(*eigvals.begin()), 0, &ldVL, 0, &ldVR,
	       ctmpwork, &cworksize, &(*rwork.begin()), &info);
# endif

  // Now allocate the memory for the workspace.
  cworksize = (int)ctmpwork[0].real();
  std::vector<std::complex<T> > cwork(cworksize);

# if !defined(JLT_NO_VECTOR_DATA_METHOD)
  lapack::geev(&jobVL, &jobVR, &N, A.data(), &N,
	       eigvals.data(), 0, &ldVL, 0, &ldVR,
	       cwork.data(), &cworksize, rwork.data(), &info);
# else
  lapack::geev(&jobVL, &jobVR, &N, &(*A.begin()), &N,
	       &(*eigvals.begin()), 0, &ldVL, 0, &ldVR,
	       &(*cwork.begin()), &cworksize, &(*rwork.begin()), &info);
# endif

  return info;
}


/* The spectral_radius function is inefficient: should only require
   the largest eigenvalue in magniture, but it finds them all. */
template<class T>
T spectral_radius(matrix<T>& A)
{
  typedef typename std::vector<std::complex<T> >::const_iterator it;
  std::vector<std::complex<T> > ev(A.rows());
  matrix_eigenvalues(A,ev);

  // Find the eigenvalue with the largest magnitude.
  T spec = 0;
  for (it i = ev.begin(); i != ev.end(); ++i)
    {
      if (Abs(*i) > spec) spec = Abs(*i);
    }

  return spec;
}


template<class T>
T spectral_radius(matrix<std::complex<T> >& A)
{
  typedef typename std::vector<std::complex<T> >::const_iterator it;
  std::vector<std::complex<T> > ev(A.rows());
  matrix_eigenvalues(A,ev);

  // Find the eigenvalue with the largest magnitude.
  T spec = 0;
  for (it i = ev.begin(); i != ev.end(); ++i)
    {
      if (Abs(*i) > spec) spec = Abs(*i);
    }

  return spec;
}


} // namespace jlt

#endif // JLT_EIGENSYSTEM_HPP
