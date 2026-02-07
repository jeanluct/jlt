//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_EIGENSYSTEM_HPP
#define JLT_EIGENSYSTEM_HPP

#include <vector>
#include <cmath>
#include <complex>
#include <jlt/matrix.hpp>
#include <jlt/lapack.hpp>

#include <cassert>
#include <algorithm>

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

  assert(N == static_cast<int>(A.columns()) && N == static_cast<int>(eigvals.size()));

  int info;

  // Use temporary vector and copy of A, since we need to reverse the order.
  std::vector<T> eigs(N);
  matrix<T> U(A);

#ifdef JLT_MIN_WORKSIZE
  // Use the smallest possible workspace.
  int worksize = std::max(1, 3*N-1);
  std::vector<T> work(worksize);
#else
  // Call the routine with worksize = -1, to get the ideal size of workspace.
  int worksize = -1;
  T tmpwork[1];

  lapack::syev(&jobz, &uplo, &N, U.data(), &N, eigs.data(),
	       tmpwork, &worksize, &info);

  // Now allocate the memory for the workspace.
  worksize = static_cast<int>(tmpwork[0]);

#ifdef JLT_DEBUG
  std::cerr << "jlt::symmetric_matrix_eigensystem:     worksize = " << worksize << std::endl;
  std::cerr << "jlt::symmetric_matrix_eigensystem: min worksize = ";
  std::cerr << std::max(1, 3*N-1) << std::endl;
#endif

  std::vector<T> work(worksize);
#endif

  lapack::syev(&jobz, &uplo, &N, U.data(), &N, eigs.data(),
	       work.data(), &worksize, &info);

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
		       std::vector<std::complex<T>>& eigvals)
{
  char jobVL = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  char jobVR = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  int N = A.rows();	// Dimensions of matrix.

  assert(N == static_cast<int>(A.columns()) && N == static_cast<int>(eigvals.size()));

  int info, ldVL = 1, ldVR = 1;

  std::vector<T> evr(N), evi(N);

#ifdef JLT_MIN_WORKSIZE
  // Use the smallest possible workspace.
  int worksize = std::max(1, 3*N);
  std::vector<T> work(worksize);
#else
  // Call the routine with worksize = -1, to get the ideal size of workspace.
  int worksize = -1;
  T tmpwork[1];

  lapack::geev(&jobVL, &jobVR, &N, A.data(), &N,
	       evr.data(), evi.data(), nullptr, &ldVL, nullptr, &ldVR,
	       tmpwork, &worksize, &info);

  // Now allocate the memory for the workspace.
  worksize = static_cast<int>(tmpwork[0]);

#ifdef JLT_DEBUG
  std::cerr << "jlt::matrix_eigenvalues:     worksize = " << worksize << std::endl;
  std::cerr << "jlt::matrix_eigenvalues: min worksize = ";
  std::cerr << std::max(1, 3*N) << std::endl;
#endif

  std::vector<T> work(worksize);
#endif

  lapack::geev(&jobVL, &jobVR, &N, A.data(), &N,
	       evr.data(), evi.data(), nullptr, &ldVL, nullptr, &ldVR,
	       work.data(), &worksize, &info);

  for (int n = 0; n < N; ++n)
    {
      eigvals[n] = std::complex<T>(evr[n],evi[n]);
    }

  return info;
}


template<class T>
int matrix_eigenvalues(matrix<std::complex<T>>& A,
		       std::vector<std::complex<T>>& eigvals)
{
  char jobVL = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  char jobVR = 'N';	// 'N'-eigenvalues only, 'V'-eigenvalues and vectors
  int N = A.rows();	// Dimensions of matrix.
  int rworksize = 2 * A.rows();
  std::vector<T> rwork(rworksize);

  assert(N == static_cast<int>(A.columns()) && N == static_cast<int>(eigvals.size()));

  int info, ldVL = 1, ldVR = 1;

#ifdef JLT_MIN_WORKSIZE
  // Use the smallest possible workspace.
  int cworksize = std::max(1, 2*N);
  std::vector<std::complex<T>> cwork(cworksize);
#else
  // Call the routine with cworksize = -1, to get the ideal size of workspace.
  int cworksize = -1;
  std::complex<T> ctmpwork[1];

  lapack::geev(&jobVL, &jobVR, &N, A.data(), &N,
	       eigvals.data(), nullptr, &ldVL, nullptr, &ldVR,
	       ctmpwork, &cworksize, rwork.data(), &info);

  // Now allocate the memory for the workspace.
  cworksize = static_cast<int>(ctmpwork[0].real());

#ifdef JLT_DEBUG
  std::cerr << "jlt::matrix_eigenvalues (complex):     cworksize = " << cworksize << std::endl;
  std::cerr << "jlt::matrix_eigenvalues (complex): min cworksize = ";
  std::cerr << std::max(1, 2*N) << std::endl;
#endif

  std::vector<std::complex<T>> cwork(cworksize);
#endif

  lapack::geev(&jobVL, &jobVR, &N, A.data(), &N,
	       eigvals.data(), nullptr, &ldVL, nullptr, &ldVR,
	       cwork.data(), &cworksize, rwork.data(), &info);

  return info;
}


/* The spectral_radius function is inefficient: should only require
   the largest eigenvalue in magniture, but it finds them all. */
template<class T>
T spectral_radius(matrix<T>& A)
{
  std::vector<std::complex<T>> ev(A.rows());
  matrix_eigenvalues(A,ev);

  // Find the eigenvalue with the largest magnitude.
  T spec = 0;
  for (auto i = ev.cbegin(); i != ev.cend(); ++i)
    {
      if (std::abs(*i) > spec) spec = std::abs(*i);
    }

  return spec;
}


template<class T>
T spectral_radius(matrix<std::complex<T>>& A)
{
  std::vector<std::complex<T>> ev(A.rows());
  matrix_eigenvalues(A,ev);

  // Find the eigenvalue with the largest magnitude.
  T spec = 0;
  for (auto i = ev.cbegin(); i != ev.cend(); ++i)
    {
      if (Abs(*i) > spec) spec = Abs(*i);
    }

  return spec;
}


} // namespace jlt

#endif // JLT_EIGENSYSTEM_HPP
