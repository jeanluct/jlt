#ifndef JLT_SVDECOMP_HPP
#define JLT_SVDECOMP_HPP

#include <nr.h>
#include <nrutil.h>
#include <jlt/matrix.hpp>

#if defined(__PGI)
#  include <assert.h>
#else
#  include <cassert>
#endif

// Symmetric real matrices
#define F77_SGESDD   sgesdd_
#define F77_DGESDD   dgesdd_

// Fortran routines from LAPACK
extern "C"
{
  // Singular value decomposition routines

  // M by N real matrix (single precision)
  void F77_SGESDD(char* jobz, int* M, int* N, float* A, int* ldA,
		  float* S, float* U, int* ldU, float* VT, int* ldVT,
		  float* work, int* lwork, int* iwork, int* info);

  // M by N real matrix (double precision)
  void F77_DGESDD(char* jobz, int* M, int* N, double* A, int* ldA,
		  double* S, double* U, int* ldU, double* VT, int* ldVT,
		  double* work, int* lwork, int* iwork, int* info);
}

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
  std::cerr << "singular_value_decomp:\n";
  std::cerr << "You cannot perform this math operation on this type.\n";
  exit(1);
}

int singular_value_decomp(matrix<float>& A,
			  matrix<float>& U,
			  matrix<float>& Vt,
			  std::vector<float>& w)
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
  float tmpwork[1];
  F77_SGESDD(&jobz, &N, &M, &(*A.begin()), &N, &(*w.begin()),
	     &(*Vt.begin()), &N, &(*U.begin()), &M,
	     tmpwork, &worksize, &(*iwork.begin()), &info);

  worksize = (int)tmpwork[0];
  std::vector<float> work(worksize);

  F77_SGESDD(&jobz, &N, &M, &(*A.begin()), &N, &(*w.begin()),
	     &(*Vt.begin()), &N, &(*U.begin()), &M,
	     &(*work.begin()), &worksize, &(*iwork.begin()), &info);

  return info;
}


int singular_value_decomp(matrix<float>& A, std::vector<float>& w)
{
  char jobz = 'N';			// 'N' - only singular values
					// are computed.

  int M = A.dim1(), N = A.dim2();	// Dimensions of matrix.
  int info;

  std::vector<int> iwork(8*std::min(M,N));

  int worksize = 3*std::min(M,N) +  std::max(std::max(M,N),6*std::min(M,N));
  std::vector<float> work(worksize);

  F77_SGESDD(&jobz, &N, &M, &(*A.begin()), &N, &(*w.begin()), 0, &N, 0, &M,
	     &(*work.begin()), &worksize, &(*iwork.begin()), &info);

  return info;
}


int singular_value_decomp(matrix<double>& A,
			  matrix<double>& U,
			  matrix<double>& Vt,
			  std::vector<double>& w)
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
  double tmpwork[1];
  F77_DGESDD(&jobz, &N, &M, &(*A.begin()), &N, &(*w.begin()),
	     &(*Vt.begin()), &N, &(*U.begin()), &M,
	     tmpwork, &worksize, &(*iwork.begin()), &info);

  worksize = (int)tmpwork[0];
  std::vector<double> work(worksize);

  F77_DGESDD(&jobz, &N, &M, &(*A.begin()), &N, &(*w.begin()),
	     &(*Vt.begin()), &N, &(*U.begin()), &M,
	     &(*work.begin()), &worksize, &(*iwork.begin()), &info);

  return info;
}


int singular_value_decomp(matrix<double>& A, std::vector<double>& w)
{
  char jobz = 'N';			// 'N' - only singular values
					// are computed.

  int M = A.dim1(), N = A.dim2();	// Dimensions of matrix.
  int info;

  std::vector<int> iwork(8*std::min(M,N));

  int worksize = 3*std::min(M,N) +  std::max(std::max(M,N),6*std::min(M,N));
  std::vector<double> work(worksize);

  F77_DGESDD(&jobz, &N, &M, &(*A.begin()), &N, &(*w.begin()), 0, &N, 0, &M,
	     &(*work.begin()), &worksize, &(*iwork.begin()), &info);

  return info;
}

} // namespace jlt

#endif // JLT_SVDECOMP_HPP
