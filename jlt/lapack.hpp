//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_LAPACK_HPP
#define JLT_LAPACK_HPP

#include <complex>

//
// Fortran routines from LAPACK
//

// C++ declarations

namespace jlt {
namespace lapack {

  extern "C" {
# include <jlt/lapack.h>
  }

  //
  // Overloaded Versions
  //

  // For every LAPACK family of routines, define overloaded C++
  // function which just calls the appropriate version.

  //
  // Eigenvalues and eigenvectors routines
  //

  // Symmetric real matrix
  template<class T>
  void syev(char* jobz,
	    char* uplo,
	    int* N,
	    T* A,
	    int* ldA,
	    T* W,
	    T* work,
	    int* lwork,
	    int* info);

  inline
  void syev(char* jobz,
	    char* uplo,
	    int* N,
	    float* A,
	    int* ldA,
	    float* W,
	    float* work,
	    int* lwork,
	    int* info)
  {
    ssyev_(jobz,uplo,N,A,ldA,W,work,lwork,info);
  }

  inline
  void syev(char* jobz,
	    char* uplo,
	    int* N,
	    double* A,
	    int* ldA,
	    double* W,
	    double* work,
	    int* lwork,
	    int* info)
  {
    dsyev_(jobz,uplo,N,A,ldA,W,work,lwork,info);
  }

  // Nonsymmetric real matrix
  template<class T>
  void geev(char* jobVL,
	    char* jobVR,
	    int* N,
	    T* A,
	    int* ldA,
	    T* Wr,
	    T* Wi,
	    T *VL,
	    int *ldVL,
	    T *VR,
	    int *ldVR,
	    T* work,
	    int* lwork,
	    int* info);

  inline
  void geev(char* jobVL,
	    char* jobVR,
	    int* N,
	    float* A,
	    int* ldA,
	    float* Wr,
	    float* Wi,
	    float *VL,
	    int *ldVL,
	    float *VR,
	    int *ldVR,
	    float* work,
	    int* lwork,
	    int* info)
  {
    sgeev_(jobVL,jobVR,N,A,ldA,Wr,Wi,VL,ldVL,VR,ldVR,work,lwork,info);
  }

  inline
  void geev(char* jobVL,
	    char* jobVR,
	    int* N,
	    double* A,
	    int* ldA,
	    double* Wr,
	    double* Wi,
	    double *VL,
	    int *ldVL,
	    double *VR,
	    int *ldVR,
	    double* work,
	    int* lwork,
	    int* info)
  {
    dgeev_(jobVL,jobVR,N,A,ldA,Wr,Wi,VL,ldVL,VR,ldVR,work,lwork,info);
  }

  // Nonsymmetric complex matrix
  template<class T>
  void geev(char* jobVL,
	    char* jobVR,
	    int* N,
	    std::complex<T>* A,
	    int* ldA,
	    std::complex<T>* W,
	    std::complex<T>* VL,
	    int *ldVL,
	    std::complex<T>* VR,
	    int *ldVR,
	    std::complex<T>* cwork,
	    int* lwork,
	    T* rwork,
	    int* info);

  inline
  void geev(char* jobVL,
	    char* jobVR,
	    int* N,
	    std::complex<float>* A,
	    int* ldA,
	    std::complex<float>* W,
	    std::complex<float>* VL,
	    int *ldVL,
	    std::complex<float>* VR,
	    int *ldVR,
	    std::complex<float>* cwork,
	    int* lwork,
	    float* rwork,
	    int* info)
  {
    cgeev_(jobVL,jobVR,N,A,ldA,W,VL,ldVL,VR,ldVR,cwork,lwork,rwork,info);
  }

  inline
  void geev(char* jobVL,
	    char* jobVR,
	    int* N,
	    std::complex<double>* A,
	    int* ldA,
	    std::complex<double>* W,
	    std::complex<double>* VL,
	    int *ldVL,
	    std::complex<double>* VR,
	    int *ldVR,
	    std::complex<double>* cwork,
	    int* lwork,
	    double* rwork,
	    int* info)
  {
    zgeev_(jobVL,jobVR,N,A,ldA,W,VL,ldVL,VR,ldVR,cwork,lwork,rwork,info);
  }

  //
  // Singular value decomposition routines
  //

  // M by N real matrix
  template<class T>
  void gesvd(char* jobu,
	     char* jobvt,
	     int* M,
	     int* N,
	     T* A,
	     int* ldA,
	     T* S,
	     T* U,
	     int* ldU,
	     T* VT,
	     int* ldVT,
	     T* work,
	     int* lwork,
	     int* info);

  inline
  void gesvd(char* jobu,
	     char* jobvt,
	     int* M,
	     int* N,
	     float* A,
	     int* ldA,
	     float* S,
	     float* U,
	     int* ldU,
	     float* VT,
	     int* ldVT,
	     float* work,
	     int* lwork,
	     int* info)
  {
    sgesvd_(jobu,jobvt,M,N,A,ldA,S,U,ldU,VT,ldVT,work,lwork,info);
  }

  inline
  void gesvd(char* jobu,
	     char* jobvt,
	     int* M,
	     int* N,
	     double* A,
	     int* ldA,
	     double* S,
	     double* U,
	     int* ldU,
	     double* VT,
	     int* ldVT,
	     double* work,
	     int* lwork,
	     int* info)
  {
    dgesvd_(jobu,jobvt,M,N,A,ldA,S,U,ldU,VT,ldVT,work,lwork,info);
  }

  // M by N real matrix (divide and conquer)
  /* Warning: When used with Flop, gesdd has been known to return some
     negative singular values. */
  template<class T>
  void gesdd(char* jobz,
	     int* M,
	     int* N,
	     T* A,
	     int* ldA,
	     T* S,
	     T* U,
	     int* ldU,
	     T* VT,
	     int* ldVT,
	     T* work,
	     int* lwork,
	     int* iwork,
	     int* info);

  inline
  void gesdd(char* jobz,
	     int* M,
	     int* N,
	     float* A,
	     int* ldA,
	     float* S,
	     float* U,
	     int* ldU,
	     float* VT,
	     int* ldVT,
	     float* work,
	     int* lwork,
	     int* iwork,
	     int* info)
  {
    sgesdd_(jobz,M,N,A,ldA,S,U,ldU,VT,ldVT,work,lwork,iwork,info);
  }

  inline
  void gesdd(char* jobz,
	     int* M,
	     int* N,
	     double* A,
	     int* ldA,
	     double* S,
	     double* U,
	     int* ldU,
	     double* VT,
	     int* ldVT,
	     double* work,
	     int* lwork,
	     int* iwork,
	     int* info)
  {
    dgesdd_(jobz,M,N,A,ldA,S,U,ldU,VT,ldVT,work,lwork,iwork,info);
  }

} // namespace lapack
} // namespace jlt

#endif // JLT_LAPACK_HPP
