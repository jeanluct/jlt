#ifndef JLT_SVDECOMP_HPP
#define JLT_SVDECOMP_HPP

#include <jlt/matrix.hpp>
#include <jlt/lapack.hpp>

#if defined(__PGI)
#  include <assert.h>
#else
#  include <cassert>
#endif

// No data() method in std::vector prior to GCC 4.1.
#if (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 1))
#  define JLT_NO_VECTOR_DATA_METHOD
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

  char jobu = 'A';			// 'A' - all M columns of U
					// are returned in the matrix U.
  char jobvt = 'A';			// 'A' - all M columns of V
					// are returned in the matrix Vt.

  int M = A.rows(), N = A.columns();	// Dimensions of matrix.
  int info;

  // For some reason, calling the routine with worksize=-1 first to
  // get the ideal worksize no longer works.  Removed as of r22.
  /* Try this again with new gesvd. */
  int worksize = max(3*min(M,N)+max(M,N),5*min(M,N));
  std::vector<T> work(worksize);

# if !defined(JLT_NO_VECTOR_DATA_METHOD)
  lapack::gesvd(&jobu, &jobvt, &N, &M, A.data(), &N, w.data(), Vt.data(), &N,
		U.data(), &M, work.data(), &worksize, &info);
# else
  lapack::gesvd(&jobu, &jobvt, &N, &M, &(*A.begin()), &N, &(*w.begin()),
		&(*Vt.begin()), &N, &(*U.begin()), &M,
		&(*work.begin()), &worksize, &info);
# endif

  return info;
}


template<class T>
int singular_value_decomp(matrix<T>& A, std::vector<T>& w)
{
  using std::min;
  using std::max;

  char jobu = 'N', jobvt = 'N';		// 'N' - only singular values
					// are computed.

  int M = A.rows(), N = A.columns();	// Dimensions of matrix.
  int info;

  int worksize = max(3*min(M,N)+max(M,N),5*min(M,N));
  std::vector<T> work(worksize);

# if !defined(JLT_NO_VECTOR_DATA_METHOD)
  lapack::gesvd(&jobu, &jobvt, &N, &M, A.data(), &N, w.data(), 0, &N, 0, &M,
		work.data(), &worksize, &info);
# else
  lapack::gesvd(&jobu, &jobvt, &N, &M, &(*A.begin()), &N, &(*w.begin()),
		0, &N, 0, &M, &(*work.begin()), &worksize, &info);
# endif

  return info;
}

} // namespace jlt

#endif // JLT_SVDECOMP_HPP
