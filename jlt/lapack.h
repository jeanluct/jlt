#ifndef JLT_LAPACK_H
#define JLT_LAPACK_H

//
// Fortran routines from LAPACK
//

// This header file can be included from a C program.

//
// Eigenvalues and eigenvectors routines
//

// Symmetric real matrix (single precision)
void ssyev_(char* jobz,
	    char* uplo,
	    int* N,
	    float* A,
	    int* ldA,
	    float* W,
	    float* work,
	    int* lwork,
	    int* info);

// Symmetric real matrix (double precision)
void dsyev_(char* jobz,
	    char* uplo,
	    int* N,
	    double* A,
	    int* ldA,
	    double* W,
	    double* work,
	    int* lwork,
	    int* info);

// Nonsymmetric real matrix (single precision)
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

// Nonsymmetric real matrix (double precision)
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

// Nonsymmetric complex matrix (single precision)
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

// Nonsymmetric complex matrix (double precision)
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

// M by N real matrix (single precision)
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

// M by N real matrix (double precision)
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
