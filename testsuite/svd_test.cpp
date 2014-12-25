//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <iostream>
#include <jlt/mathvector.hpp>
#include <jlt/mathmatrix.hpp>
#include <jlt/svdecomp.hpp>
#include <jlt/stlio.hpp>


int main()
{
  using std::cout;
  using std::endl;
  using jlt::matrix;
  using jlt::mathmatrix;
  using jlt::mathvector;

  typedef double Real;

  int m = 3, n = 4;
  mathmatrix<Real> A(m,n), U(m,m), Vt(n,n);
  mathvector<Real> w(std::min(m,n));

  A(0,0) = 1;
  A(0,1) = -4;
  A(0,2) = 1;
  A(0,3) = 1;

  A(1,0) = -1;
  A(1,1) = 3;
  A(1,2) = -1;
  A(1,3) = -2;

  A(2,0) = 1;
  A(2,1) = 2;
  A(2,2) = 1;
  A(2,3) = 0;

  mathmatrix<Real> A2(A); // Make a copy, since A is destroyed.

  cout.precision(6);
  cout.setf(std::ios::fixed);

  cout << "Original matrix A =\n";
  A.printMatrixForm(cout);

  jlt::singular_value_decomp(A,U,Vt,w);

  cout << "\nA = U.diag(w).Vt, where\n";
  cout << "\nU =\n"; U.printMatrixForm(cout);
  cout << "\nVt =\n"; Vt.printMatrixForm(cout);
  cout << "\nw = " << w;
  // Compute only the singular values.
  singular_value_decomp(A2,w);
  cout << "\nw = " << w << endl;

  cout << "\nU.diag(w).Vt =\n";
  (U*diagonal_matrix(w,m,n)*Vt).printMatrixForm(cout);
}
