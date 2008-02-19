#ifndef JLT_SVDECOMP_HPP
#define JLT_SVDECOMP_HPP

#include <jlt/matrix.hpp>
#include <jlt/lapack.hpp>

#if defined(__PGI)
#  include <assert.h>
#else
#  include <cassert>
#endif

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
int singular_value_decomp(matrix<T>& A,
			  matrix<T>& U,
			  matrix<T>& Vt,
			  std::vector<T>& w)
{
  using std::min;
  using std::max;

  char jobz = 'A';			// 'A' - all M columns of U
					// and all N rows of V^T are
					// returned in the matrices U
					// and Vt.

  int M = A.rows(), N = A.columns();	// Dimensions of matrix.
  int info;

  std::vector<int> iwork(8*min(M,N));

  // For some reason, calling the routine with worksize=-1 first to
  // get the ideal worksize no longer works.  Removed as of r22.
  int worksize = 3*min(M,N)*min(M,N)
                 + max(max(M,N),4*min(M,N)*min(M,N) + 4*min(M,N));
  std::vector<T> work(worksize);

  lapack::gesdd(&jobz, &N, &M, A.data(), &N, w.data(), Vt.data(), &N,
		U.data(), &M, work.data(), &worksize, iwork.data(), &info);

  return info;
}


template<class T>
int singular_value_decomp(matrix<T>& A, std::vector<T>& w)
{
  using std::min;
  using std::max;

  char jobz = 'N';			// 'N' - only singular values
					// are computed.

  int M = A.rows(), N = A.columns();	// Dimensions of matrix.
  int info;

  std::vector<int> iwork(8*min(M,N));

  int worksize = 3*min(M,N) +  max(max(M,N),6*min(M,N));
  std::vector<T> work(worksize);

  lapack::gesdd(&jobz, &N, &M, A.data(), &N, w.data(), 0, &N, 0, &M,
		work.data(), &worksize, iwork.data(), &info);

  return info;
}

} // namespace jlt

#endif // JLT_SVDECOMP_HPP
