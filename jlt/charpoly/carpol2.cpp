/********************************************************
* Calculate the coefficients of the characteristic poly-*
* nomial P(l)=A-l*I of a real matrix A(i,j)             *
* ----------------------------------------------------- *
* Ref.: "Algèbre, Algorithmes et programmes en Pascal   *
*        By Jean-Louis Jardrin, DUNOD Paris, 1988"      *
*        [BIBLI 10].                                    *
* ----------------------------------------------------- *
* SAMPLE RUN:                                           *
* (Find the caracteristic polynomial of matrix:         *
*           1  0  0  0  0  1                            *
*           1  1  0  0  0 -1                            *
*      A = -1  1  1  0  0  1                            *
*           1 -1  1  1  0 -1                            *
*          -1  1 -1  1  1  1                            *
*           1 -1  1 -1  1 -1  )                         *
*                                                       *
* Input matrix size (max 100): 6                        *
*  Line 1:                                              *
*   Element 1: 1                                        *
*   Element 2: 0                                        *
*   Element 3: 0                                        *
*   Element 4: 0                                        *
*   Element 5: 0                                        *
*   Element 6: 1                                        *
*  Line 2:                                              *
*   Element 1: 1                                        *
*   Element 2: 1                                        *
*   Element 3: 0                                        *
*   Element 4: 0                                        *
*   Element 5: 0                                        *
*   Element 6: -1                                       *
*  Line 3:                                              *
*   Element 1: -1                                       *
*   Element 2: 1                                        *
*   Element 3: 1                                        *
*   Element 4: 0                                        *
*   Element 5: 0                                        *
*   Element 6: 1                                        *
*  Line 4:                                              *
*   Element 1: 1                                        *
*   Element 2: -1                                       *
*   Element 3: 1                                        *
*   Element 4: 1                                        *
*   Element 5: 0                                        *
*   Element 6: -1                                       *
*  Line 5:                                              *
*   Element 1: -1                                       *
*   Element 2: 1                                        *
*   Element 3: -1                                       *
*   Element 4: 1                                        *
*   Element 5: 1                                        *
*   Element 6: 1                                        *
*  Line 6:                                              *
*   Element 1: 1                                        *
*   Element 2: -1                                       *
*   Element 3: 1                                        *
*   Element 4: -1                                       *
*   Element 5: 1                                        *
*   Element 6: -1                                       *
*                                                       *
* Chracteristic polynomial P(l):                        *
* Coefficient for degree 6: 1.000000                    *
* Coefficient for degree 5: -4.000000                   *
* Coefficient for degree 4: 0.000000                    *
* Coefficient for degree 3: 30.000000                   *
* Coefficient for degree 2: -75.000000                  *
* Coefficient for degree 1: 79.000000                   *
* Coefficient for degree 0: -32.000000                  *
*                                                       *
* (The characteristic polynomial of matrix A is:        *
*   P(l)=l^6-4*l^5+30*l^3-75*l^2+79*l-32 )              *
*                                                       *
*                 C++ version by J-P Moreau, Paris      *
*               (with dynamically allocated vectors).   *
*                                                       *
* To be linked with basis_r.cpp and vmblock.cpp.        *
********************************************************/
#include <basis.h>    //used by vmblock.cpp
#include <vmblock.h>  //dynamic allocation of real matrices
                      //and vectors 
int k,n;
REAL **A;
REAL *P; 

void *vmblock = NULL;

//read matrix A from screen
void  Read_data(int n) {
  int i,j;
  for (i=1; i<=n; i++) {
    printf("\n Line %d\n", i);
    for (j=1; j<=n; j++) {
      printf("  Element %d: ", j); scanf("%lf",&A[i][j]);
    }
  }
}

//return trace of real C matrix
REAL TRM(int n, REAL **C) {
  int i; REAL t0;
  t0=0.0;
  for (i=1; i<=n; i++)
    t0+=C[i][i];
  return t0;
}

/****************************************************
* The procedure PCMS calculates the coefficients of *
* the characteristic polynomial P(l)=A-l*I of a real*
* square matrix A(i,j).                             *
*                                                   *
* Note: the roots of P(l) are the eigenvalues of    *
*       matrix A(i,j).                              *
* ------------------------------------------------- *
* INPUTS:                                           *
*          n: zize of matrix (integer)              *
*          A: real matrix of size (n,n)             *
*             (see main program).                   *
* OUTPUT:                                           *
*          P: vector of coefficients of P(l)        *
*                                                   *
* The pointers to a real matrix or a real vector    *
* must be declared and allocated in the calling     *
* program by using the functions of module VMBLOCK. *
****************************************************/
void PCMS(int n, REAL **A, REAL *P) {
  int i,j,k,l;
  REAL s,t0;
  REAL **B, **C;
  void *vmblock1 = NULL;

  //initialize temporary matrices B and C (see vlblock.cpp)
  vmblock1 = vminit();
  B = (REAL **)vmalloc(vmblock1, MATRIX,  n+1, n+1);
  C = (REAL **)vmalloc(vmblock1, MATRIX,  n+1, n+1);

  if (n%2!=0) 
	P[1]=-1.0;
  else 
	P[1]= 1.0;
  
  for (l=1; l<=n; l++) {
    if (l==1) {
      for (i=1; i<=n; i++)
        for (j=1; j<=n; j++)
          C[i][j]=A[i][j];
    }
    else {
      for (i=1; i<=n; i++) {
        for (j=1; j<=n; j++) {
          s=0.0;
          for (k=1; k<=n; k++)
            s += B[i][k]*A[k][j];
          C[i][j]=s;
        }
      }
    }
    t0=TRM(n,C)/l;
    P[l+1]=-t0*P[1];
    if (l<n) 
      for (i=1; i<=n; i++)
        for (j=1; j<=n; j++)
          if (j==i)  B[i][j]=C[i][j]-t0;
          else B[i][j]=C[i][j];
  }
  free(vmblock1);
}


int main()  {

  printf("\n Input matrix size: "); scanf("%d",&n);
  //Initialize A and P (see vmblock.cpp)
  //Note: index 0 not used here.
  vmblock = vminit();
  A = (REAL **)vmalloc(vmblock, MATRIX,  n+1, n+1);
  P = (REAL *) vmalloc(vmblock, VEKTOR,  n+2, 0);
  //read matrix A from screen 
  Read_data(n);
  //call routine to calculate coefficients
  PCMS(n,A,P);
  //print results
  printf("\n\n Characteristic polynomial P(l):\n");
  for (k=1; k<n+2; k++)
    printf(" Coefficient for degree %d: %f\n", n-k+1, P[k]);
  printf("\n\n");
  free(vmblock);

  return 0;
}

//end of file carpol2.cpp
