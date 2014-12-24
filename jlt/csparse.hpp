//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_CSPARSE_HPP
#define JLT_CSPARSE_HPP

#include <iostream>
#include <cstdio>
#include <memory>
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

// auto_ptr wrappers for csparse pointers that will take care of
// freeing the memory when we're done with a matrix.

// Derived wrapper for csparse::cs pointers.
class cs_auto_ptr : public std::auto_ptr<csparse::cs>
{
public:
  cs_auto_ptr(csparse::cs* p_ = 0) : std::auto_ptr<csparse::cs>(p_) {}

  // Conversion to normal dumb pointer.
  operator csparse::cs*() { return get(); }

  // Returns true if pointer is null.
  bool operator!() const { return (get() == 0); }

  ~cs_auto_ptr()
  {
    // Free the internals of the csparse pointer.
    //
    // From csparse::cs_spfree defined in cs_util.c
    //
    // We do not call this function directly, since we have to let the
    // base auto_ptr<> free the actual pointer.
    if (!get())
      {
	csparse::cs_free(get()->p);
	csparse::cs_free(get()->i);
	csparse::cs_free(get()->x);
      }
  }
};

// Derived wrapper for csparse::csd pointers.
class csd_auto_ptr : public std::auto_ptr<csparse::csd>
{
public:
  csd_auto_ptr(csparse::csd* p_ = 0) : std::auto_ptr<csparse::csd>(p_) {}

  // Conversion to normal dumb pointer.
  operator csparse::csd*() { return get(); }

  // Returns true if pointer is null.
  bool operator!() const { return (get() == 0); }

  ~csd_auto_ptr()
  {
    // Free the internals of the csparse pointer.
    //
    // From csparse::cs_dfree defined in cs_util.c
    //
    // We do not call this function directly, since we have to let the
    // base auto_ptr<> free the actual pointer.
    if (!get())
      {
	csparse::cs_free(get()->p);
	csparse::cs_free(get()->q);
	csparse::cs_free(get()->r);
	csparse::cs_free(get()->s);
      }
  }
};

// Note that we return a pointer, not an auto_ptr, since the return
// value is a temporary.
template<class T>
csparse::cs* mathmatrix_to_cs_sparse_matrix(const mathmatrix<T>& M)
{
  int m = M.rows(), n = M.columns();
  cs_auto_ptr csM(csparse::cs_spalloc(0,0,1,1,1));

  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
      if (M(i,j) != 0)
	csparse::cs_entry(csM,i,j,M(i,j));

  // Column-compress the sparse matrix.
  return csparse::cs_compress(csM);
}

template<class T>
mathmatrix<T> cs_sparse_matrix_to_mathmatrix(const cs_auto_ptr& csM)
{
  if (!csM) return mathmatrix<T>();

  int m = csM->m, n = csM->n;
  long int *Ap = csM->p, *Ai = csM->i;
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
