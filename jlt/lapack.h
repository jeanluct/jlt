//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_LAPACK_H
#define JLT_LAPACK_H

//
// Fortran routines from LAPACK
//

// This header file can be included from a C program.

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

#endif // JLT_LAPACK_H
