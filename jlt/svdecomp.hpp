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
  char jobz = 'A';			// 'A' - all M columns of U
					// and all N rows of V^T are
					// returned in the matrices U
					// and Vt.

  int M = A.dim1(), N = A.dim2();	// Dimensions of matrix.
  int info;

  std::vector<int> iwork(8*std::min(M,N));

  // Call the routine with worksize = -1, to get the ideal size of workspace.
  int worksize = -1;
  T tmpwork[1];
  lapack::gesdd(&jobz, &N, &M, &(*A.begin()), &N, &(*w.begin()),
		&(*Vt.begin()), &N, &(*U.begin()), &M,
		tmpwork, &worksize, &(*iwork.begin()), &info);

  worksize = (int)tmpwork[0];
  std::vector<T> work(worksize);

  lapack::gesdd(&jobz, &N, &M, &(*A.begin()), &N, &(*w.begin()),
		&(*Vt.begin()), &N, &(*U.begin()), &M,
		&(*work.begin()), &worksize, &(*iwork.begin()), &info);

  return info;
}


template<class T>
int singular_value_decomp(matrix<T>& A, std::vector<T>& w)
{
  char jobz = 'N';			// 'N' - only singular values
					// are computed.

  int M = A.dim1(), N = A.dim2();	// Dimensions of matrix.
  int info;

  std::vector<int> iwork(8*std::min(M,N));

  int worksize = 3*std::min(M,N) +  std::max(std::max(M,N),6*std::min(M,N));
  std::vector<T> work(worksize);

  lapack::gesdd(&jobz, &N, &M, &(*A.begin()), &N, &(*w.begin()),
		0, &N, 0, &M,
		&(*work.begin()), &worksize, &(*iwork.begin()), &info);

  return info;
}

} // namespace jlt

#endif // JLT_SVDECOMP_HPP
