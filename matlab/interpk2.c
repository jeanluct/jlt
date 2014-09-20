#include <math.h>
#include <stdio.h>
#include "mex.h"

/*
  Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>

  See the file LICENSE for copying permission.
*/

/*

Build with

mex -largeArrayDims interpk2.c

*/

/* Usage: interpk2(fk,X) */

/* fk is an N1 by N2 complex array containing 2D Fourier transform data. */

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  /* Dimensions of grid */
  mwSize N1, N2;
  /* Counters */
  mwSignedIndex n1, k1, k2;
  /* Fourier transform data */
  double *fkr, *fki;
  /* The point to interpolate */
  double *X, X1, X2;
  /* min/max mode numbers */
  mwSignedIndex k1min, k1max, k2max;

  if (nrhs < 2)
    {
      mexErrMsgTxt("2 input arguments required.");
    }

  /* The row-col dimensions of the first argument. */
  N1 = mxGetM(prhs[0]);
  N2 = mxGetN(prhs[0]);
  /* Pointers to the real part of the velocity data. */
  fkr = mxGetPr(prhs[0]);
  /* Only use the imaginary part if the matrix is complex. */
  if (mxIsComplex(prhs[0])) fki = mxGetPi(prhs[0]);

  /* The point to be interpolated */
  /* Note that the array fk is assumed to be stored in 'meshgrid'
     style, where the rows correspond to Y and the columns to X. */
  X = mxGetPr(prhs[1]);
  X1 = 2*M_PI*X[1];
  X2 = 2*M_PI*X[0];

  k1min = (mwSignedIndex)floor(-((mwSignedIndex)N1-1)/2.);
  k1max = (mwSignedIndex)floor( ((mwSignedIndex)N1-1)/2.);
  k2max = (mwSignedIndex)floor( ((mwSignedIndex)N2-1)/2.);

  /* ik1 is an array that translates from unsigned to signed vector
     components, used for quick indexing. */
  mwSignedIndex *ik1;
  ik1 = (mwIndex *)malloc(N1*sizeof(mwIndex));
  for (n1 = 0; n1 <= k1max; ++n1) ik1[n1] = n1;
  for (n1 = -1; n1 >= k1min; --n1) ik1[(mwIndex)(N1+n1)] = n1;

  /* For even N1, N2, the kmin mode is not included at all. */
  /* The Fourier spectrum is assumed symmetric. */

  double fx = fkr[0];
  if (mxIsComplex(prhs[0]))
    {
      for (k1 = 1; k1 <= k1max; ++k1)
	{
	  double Xk = X1*k1;
	  double exr = 2*cos(Xk), exi = 2*sin(Xk);
	  fx += fkr[k1]*exr - fki[k1]*exi;
	}
      for (k2 = 1; k2 <= k2max; ++k2)
	{
	  for (n1 = 0; n1 < N1; ++n1)
	    {
	      k1 = ik1[n1];
	      double Xk = X1*k1 + X2*k2;
	      double exr = 2*cos(Xk), exi = 2*sin(Xk);
	      mwIndex N = n1 + k2*N2;
	      fx += fkr[N]*exr - fki[N]*exi;
	    }
	}
    }
  else
    {
      for (k1 = 1; k1 <= k1max; ++k1)
	{
	  double Xk = X1*k1;
	  double exr = 2*cos(Xk);
	  fx += fkr[k1]*exr;
	}
      for (k2 = 1; k2 <= k2max; ++k2)
	{
	  for (n1 = 0; n1 < N1; ++n1)
	    {
	      k1 = ik1[n1];
	      double Xk = X1*k1 + X2*k2;
	      double exr = 2*cos(Xk);
	      mwIndex N = n1 + k2*N2;
	      fx += fkr[N]*exr;
	    }
	}
    }
  fx /= N1*N2;

  plhs[0] = mxCreateDoubleScalar(fx);

  free(ik1);

  return;
}
