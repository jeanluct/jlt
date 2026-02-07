//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_SVDECOMP_HPP
#define JLT_SVDECOMP_HPP

#include <jlt/matrix.hpp>
#include <jlt/lapack.hpp>
#include <cassert>
#include <algorithm>
#include <complex>

namespace jlt {

//
// Given a matrix A, compute the singular value decomposition
//
//   A = U.diag(w).transp(V) = U.diag(w).Vt
//
// where U and V are orthogonal, and w is a vector of singular values.
// The singular values are positive and sorted by decreasing size.
//
// The M by N matrix A is the input, is destroyed on return.
//

template<class T>
int SVdecomp(matrix<T>& A,
	     matrix<T>& U,
	     matrix<T>& Vt,
	     std::vector<T>& w)
{
  using std::min;
  using std::max;

  char jobu = 'A';			// 'A' - all M columns of U
					// are returned in the matrix U.
  char jobvt = 'A';			// 'A' - all M columns of V
					// are returned in the matrix Vt.

  int M = A.rows(), N = A.columns();	// Dimensions of matrix.
  int info;

#ifdef JLT_MIN_WORKSIZE
  // Use the smallest possible workspace.
  int worksize = max(3*min(M,N)+max(M,N),5*min(M,N));
  std::vector<T> work(worksize);
#else
  // Call the routine with worksize = -1, to get the ideal size of workspace.
  int worksize = -1;
  T tmpwork[1];

  lapack::gesvd(&jobu, &jobvt, &N, &M, A.data(), &N, w.data(),
		Vt.data(), &N, U.data(), &M, tmpwork, &worksize, &info);

  worksize = static_cast<int>(tmpwork[0]);

#ifdef JLT_DEBUG
  std::cerr << "jlt::svdecomp:     worksize = " << worksize << std::endl;
  std::cerr << "jlt::svdecomp: min worksize = ";
  std::cerr << max(3*min(M,N)+max(M,N),5*min(M,N)) << std::endl;
#endif

  std::vector<T> work(worksize);
#endif

  lapack::gesvd(&jobu, &jobvt, &N, &M, A.data(), &N, w.data(), Vt.data(), &N,
		U.data(), &M, work.data(), &worksize, &info);

  return info;
}


template<class T>
int SVdecomp(matrix<T>& A, std::vector<T>& w)
{
  using std::min;
  using std::max;

  char jobu = 'N', jobvt = 'N';		// 'N' - only singular values
					// are computed.

  int M = A.rows(), N = A.columns();	// Dimensions of matrix.
  int info;

#ifdef JLT_MIN_WORKSIZE
  // Use the smallest possible workspace.
  int worksize = max(3*min(M,N)+max(M,N),5*min(M,N));
  std::vector<T> work(worksize);
#else
  // Call the routine with worksize = -1, to get the ideal size of workspace.
  int worksize = -1;
  T tmpwork[1];

  lapack::gesvd(&jobu, &jobvt, &N, &M, A.data(), &N, w.data(),
		nullptr, &N, nullptr, &M, tmpwork, &worksize, &info);

  worksize = static_cast<int>(tmpwork[0]);

#ifdef JLT_DEBUG
  std::cerr << "jlt::svdecomp:     worksize = " << worksize << std::endl;
  std::cerr << "jlt::svdecomp: min worksize = ";
  std::cerr << max(3*min(M,N)+max(M,N),5*min(M,N)) << std::endl;
#endif

  std::vector<T> work(worksize);
#endif

  lapack::gesvd(&jobu, &jobvt, &N, &M, A.data(), &N, w.data(), nullptr, &N, nullptr, &M,
		work.data(), &worksize, &info);

  return info;
}


//
// Complex matrix SVD overloads
//
// For complex matrices, the singular values are real, but the matrix A
// and singular vectors U, Vt are complex. An additional real workspace
// (rwork) is required.
//

template<class T>
int SVdecomp(matrix<std::complex<T>>& A,
	     matrix<std::complex<T>>& U,
	     matrix<std::complex<T>>& Vt,
	     std::vector<T>& w)
{
  using std::min;
  using std::max;

  char jobu = 'A';			// 'A' - all M columns of U
					// are returned in the matrix U.
  char jobvt = 'A';			// 'A' - all M columns of V
					// are returned in the matrix Vt.

  int M = A.rows(), N = A.columns();	// Dimensions of matrix.
  int info;

  // Real workspace for complex SVD
  int rworksize = 5 * min(M, N);
  std::vector<T> rwork(rworksize);

  // Call the routine with cworksize = -1, to get the ideal size of workspace.
  int cworksize = -1;
  std::complex<T> ctmpwork[1];

  lapack::gesvd(&jobu, &jobvt, &N, &M, A.data(), &N, w.data(),
		Vt.data(), &N, U.data(), &M, ctmpwork, &cworksize,
		rwork.data(), &info);

  cworksize = static_cast<int>(ctmpwork[0].real());

#ifdef JLT_DEBUG
  std::cerr << "jlt::svdecomp (complex):     cworksize = " << cworksize << std::endl;
  std::cerr << "jlt::svdecomp (complex):     rworksize = " << rworksize << std::endl;
#endif

  std::vector<std::complex<T>> cwork(cworksize);

  lapack::gesvd(&jobu, &jobvt, &N, &M, A.data(), &N, w.data(), Vt.data(), &N,
		U.data(), &M, cwork.data(), &cworksize, rwork.data(), &info);

  return info;
}


template<class T>
int SVdecomp(matrix<std::complex<T>>& A, std::vector<T>& w)
{
  using std::min;
  using std::max;

  char jobu = 'N', jobvt = 'N';		// 'N' - only singular values
					// are computed.

  int M = A.rows(), N = A.columns();	// Dimensions of matrix.
  int info;

  // Real workspace for complex SVD
  int rworksize = 5 * min(M, N);
  std::vector<T> rwork(rworksize);

  // Call the routine with cworksize = -1, to get the ideal size of workspace.
  int cworksize = -1;
  std::complex<T> ctmpwork[1];

  lapack::gesvd(&jobu, &jobvt, &N, &M, A.data(), &N, w.data(),
		nullptr, &N, nullptr, &M, ctmpwork, &cworksize,
		rwork.data(), &info);

  cworksize = static_cast<int>(ctmpwork[0].real());

#ifdef JLT_DEBUG
  std::cerr << "jlt::svdecomp (complex):     cworksize = " << cworksize << std::endl;
  std::cerr << "jlt::svdecomp (complex):     rworksize = " << rworksize << std::endl;
#endif

  std::vector<std::complex<T>> cwork(cworksize);

  lapack::gesvd(&jobu, &jobvt, &N, &M, A.data(), &N, w.data(), nullptr, &N,
		nullptr, &M, cwork.data(), &cworksize, rwork.data(), &info);

  return info;
}

} // namespace jlt

#endif // JLT_SVDECOMP_HPP
