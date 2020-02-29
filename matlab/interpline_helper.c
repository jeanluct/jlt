#include <math.h>
#include <stdio.h>
#include "mex.h"

/* Interpolate (refine) a material line */

/*
  Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>

  See the file LICENSE for copying permission.
*/

double shortestdir(double x1, double x2, double L);
double fmodp(double x, double y);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  mwIndex i, j, d;

  if (nrhs < 2)
    {
      mexErrMsgTxt("2 input arguments required.");
      exit(1);
    }

  const mxArray *XA = prhs[0];
  double dmax = mxGetScalar(prhs[1]);     /* Maximum gap allowed. */
  mwSize Nr = 1;

  mwSize N = mxGetM(XA);  /* Number of points in original list. */
  mwSize n = mxGetN(XA);  /* Dimension of vectors. */

  /* Pointer to the X data. */
  double *X = mxGetPr(XA);

  double *dX = (double*)malloc(n*(N-1)*sizeof(double));
  mwSize *nr = (mwSize*)malloc((N-1)*sizeof(mwSize));

  /* Go once through, counting how many interpolation points we'll need. */
  for (i = 0; i < N-1; ++i)
    {
      double dist2 = 0;
      for (d = 0; d < n; ++d)
	{
	  mwIndex idx = i + d*(N-1);
	  dX[idx] = X[i+1 + d*N] - X[i + d*N];
	  dist2 += dX[idx]*dX[idx];
	}
      double dist = sqrt(dist2);
      nr[i] = ceil(dist/dmax);
      Nr += nr[i];
    }

  /* Create an mxArray for the output data */
  plhs[0] = mxCreateDoubleMatrix(Nr, n, mxREAL);
  double *Xr = mxGetPr(plhs[0]);

  mwIndex k = 0;
  for (i = 0; i < N-1; ++i)
    {
      /* Interpolate nr[i]-1 points between X[i] and X[i+1]. */
      for (j = 0; j < nr[i]; ++j)
	{
	  for (d = 0; d < n; ++d)
	    Xr[k + d*Nr] = X[i + d*N] + dX[i + d*(N-1)]/nr[i] * j;
	  ++k;
	}
    }
  /* Last point (no interpolation) */
  for (d = 0; d < n; ++d) Xr[k + d*Nr] = X[N-1 + d*N];

  free(dX);
  free(nr);

  return;
}
