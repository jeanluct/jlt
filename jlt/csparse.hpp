#ifndef JLT_CSPARSE_HPP
#define JLT_CSPARSE_HPP

#include <jlt/mathmatrix.hpp>

namespace csparse {
extern "C" {
#include "cs.h"
}
} // namespace csparse

//
// Routines to interface with the CSparse library.
//
// CSparse is Copyright (c) Timothy A. Davis, 2006-2007
//
// See http://www.cise.ufl.edu/research/sparse/CSparse
//
// Compile with -lcsparse flag.

namespace jlt {

template<class T>
csparse::cs* mathmatrix_to_cs_sparse_matrix(const mathmatrix<T>& M)
{
  int m = M.dim1(), n = M.dim2();
  csparse::cs *csM, *csM_comp ;

  csM = csparse::cs_spalloc(0,0,1,1,1) ;

  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
      if (M(i,j) != 0)
	if (!csparse::cs_entry(csM,i,j,M(i,j))) return csparse::cs_spfree(csM);

  // Column-compress the sparse matrix.
  csM_comp = csparse::cs_compress(csM);

  // Free the triplet sparse matrix.
  csparse::cs_spfree(csM);

  return csM_comp;
}

template<class T>
mathmatrix<T> cs_sparse_matrix_to_mathmatrix(const csparse::cs* csM)
{
  if (!csM) return mathmatrix<T>();

  int m = csM->m, n = csM->n;
  int *Ap = csM->p, *Ai = csM->i;
  int nz = csM->nz;
  double *Ax = csM->x;

  mathmatrix<T> M(m,n);

  if (nz < 0)
    {
      // Matrix is in column-compressed form.
      for (int j = 0; j < n; ++j)
	  for (int p = Ap[j]; p < Ap[j+1]; ++p)
	      M(Ai[p],j) = T(Ax[p]);
    }
  else
    {
      // Matrix is in triplet form.
      for (int p = 0; p < nz; ++p)
	M(Ai[p],Ap[p]) = T(Ax[p]);
    }

  return M;
}

} // namespace jlt

#endif // JLT_CSPARSE_HPP
