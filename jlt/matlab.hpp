#ifndef JLT_MATLAB_HPP
#define JLT_MATLAB_HPP

#ifdef JLT_MATLAB_SUPPORT
#include "mat.h"

namespace jlt {

void printMatlabForm(MATFile *pmat, const char name[], const double var)
{
  mxArray *A = mxCreateDoubleMatrix(1,1,mxREAL);
  double *Ap = mxGetPr(A);
  Ap[0] = var;
  matPutVariable(pmat, name, A);
  mxDestroyArray(A);
}

} // namespace jlt

#endif // JLT_MATLAB_SUPPORT

#endif // JLT_MATLAB_HPP
