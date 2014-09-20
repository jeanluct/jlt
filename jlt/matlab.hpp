//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_MATLAB_HPP
#define JLT_MATLAB_HPP

#ifdef JLT_MATLAB_LIB_SUPPORT
#  include "mat.h"
#endif

namespace jlt {

// TODO: add the iostream versions (no need to have
// JLT_MATLAB_LIB_SUPPORT defined).

#ifdef JLT_MATLAB_LIB_SUPPORT

void printMatlabForm(MATFile *pmat, const char name[], const double var)
{
  mxArray *A = mxCreateDoubleMatrix(1,1,mxREAL);
  double *Ap = mxGetPr(A);
  Ap[0] = var;
  matPutVariable(pmat, name, A);
  mxDestroyArray(A);
}

void printMatlabForm(MATFile *pmat, const char name[], const char str[])
{
  mxArray *A = mxCreateString(str);
  matPutVariable(pmat,"name",A);
  mxDestroyArray(A);
}

} // namespace jlt

#endif // JLT_MATLAB_LIB_SUPPORT

#endif // JLT_MATLAB_HPP
