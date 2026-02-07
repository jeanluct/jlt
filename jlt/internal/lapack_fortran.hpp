//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_LAPACK_FORTRAN_HPP
#define JLT_LAPACK_FORTRAN_HPP

#include <complex>

//
// C++ declarations for Fortran LAPACK routines (with C linkage)
//
// INTERNAL HEADER - Do not include directly!
// Use #include <jlt/lapack.hpp> instead.
//
// This file provides extern "C" declarations for LAPACK functions.
// std::complex<T> is used for complex types, which is layout-compatible
// with Fortran COMPLEX types.

//
// Eigenvalues and eigenvectors routines
//

// SSYEV - compute all eigenvalues and, optionally, eigenvectors of a real
//    symmetric matrix A.
// (single precision)
void ssyev_(char* jobz,
	    char* uplo,
	    int* N,
	    float* A,
	    int* ldA,
	    float* W,
	    float* work,
	    int* lwork,
	    int* info);

// DSYEV - compute all eigenvalues and, optionally, eigenvectors of a real
//    symmetric matrix A.
// (double precision)
void dsyev_(char* jobz,
	    char* uplo,
	    int* N,
	    double* A,
	    int* ldA,
	    double* W,
	    double* work,
	    int* lwork,
	    int* info);

// SGEEV - compute for an N-by-N real nonsymmetric matrix A, the eigenvalues
//    and, optionally, the left and/or right eigenvectors.
// (single precision)
void sgeev_(char* jobVL,
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
	    int* info);

// DGEEV - compute for an N-by-N real nonsymmetric matrix A, the eigenvalues
//    and, optionally, the left and/or right eigenvectors.
// (double precision)
void dgeev_(char* jobVL,
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
	    int* info);

// CGEEV - compute for an N-by-N complex nonsymmetric matrix A, the
//    eigenvalues and, optionally, the left and/or right eigenvectors.
// (single precision)
void cgeev_(char* jobVL,
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
	    int* info);

// ZGEEV - compute for an N-by-N complex nonsymmetric matrix A, the
//    eigenvalues and, optionally, the left and/or right eigenvectors.
// (double precision)
void zgeev_(char* jobVL,
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
	    int* info);

//
// Singular value decomposition routines
//

// SGESVD - compute the singular value decomposition (SVD) of a real M-by-N
//    matrix A, optionally computing the left and/or right singular vectors.
// (single precision)
void sgesvd_(char* jobu,
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
	     int* info);

// DGESVD - compute the singular value decomposition (SVD) of a real M-by-N
//    matrix A, optionally computing the left and/or right singular vectors.
// (double precision)
void dgesvd_(char* jobu,
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
	     int* info);

// SGESDD - compute the singular value decomposition (SVD) of a real M-by-N
//    matrix A, optionally computing the left and right singular vectors.
// (single precision)
void sgesdd_(char* jobz,
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
	     int* info);

// DGESDD - compute the singular value decomposition (SVD) of a real M-by-N
//    matrix A, optionally computing the left and right singular vectors.
// (double precision)
void dgesdd_(char* jobz,
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
	     int* info);

// CGESVD - compute the singular value decomposition (SVD) of a complex M-by-N
//    matrix A, optionally computing the left and/or right singular vectors.
// (single precision)
void cgesvd_(char* jobu,
	     char* jobvt,
	     int* M,
	     int* N,
	     std::complex<float>* A,
	     int* ldA,
	     float* S,
	     std::complex<float>* U,
	     int* ldU,
	     std::complex<float>* VT,
	     int* ldVT,
	     std::complex<float>* work,
	     int* lwork,
	     float* rwork,
	     int* info);

// ZGESVD - compute the singular value decomposition (SVD) of a complex M-by-N
//    matrix A, optionally computing the left and/or right singular vectors.
// (double precision)
void zgesvd_(char* jobu,
	     char* jobvt,
	     int* M,
	     int* N,
	     std::complex<double>* A,
	     int* ldA,
	     double* S,
	     std::complex<double>* U,
	     int* ldU,
	     std::complex<double>* VT,
	     int* ldVT,
	     std::complex<double>* work,
	     int* lwork,
	     double* rwork,
	     int* info);

// CGESDD - compute the singular value decomposition (SVD) of a complex M-by-N
//    matrix A, optionally computing the left and right singular vectors.
// (single precision)
void cgesdd_(char* jobz,
	     int* M,
	     int* N,
	     std::complex<float>* A,
	     int* ldA,
	     float* S,
	     std::complex<float>* U,
	     int* ldU,
	     std::complex<float>* VT,
	     int* ldVT,
	     std::complex<float>* work,
	     int* lwork,
	     float* rwork,
	     int* lrwork,
	     int* iwork,
	     int* info);

// ZGESDD - compute the singular value decomposition (SVD) of a complex M-by-N
//    matrix A, optionally computing the left and right singular vectors.
// (double precision)
void zgesdd_(char* jobz,
	     int* M,
	     int* N,
	     std::complex<double>* A,
	     int* ldA,
	     double* S,
	     std::complex<double>* U,
	     int* ldU,
	     std::complex<double>* VT,
	     int* ldVT,
	     std::complex<double>* work,
	     int* lwork,
	     double* rwork,
	     int* lrwork,
	     int* iwork,
	     int* info);

#endif // JLT_LAPACK_FORTRAN_HPP
