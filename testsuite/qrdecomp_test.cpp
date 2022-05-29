//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <iostream>
#include <cstdlib>
#include <vector>
#include <jlt/mathmatrix.hpp>
#include <jlt/matrixutil.hpp>
#include <jlt/stlio.hpp>


int main()
{
  using std::cout;
  using std::endl;
  using jlt::matrix;
  using jlt::mathmatrix;

  int n = 10;
  mathmatrix<double> M(n,n), Q(n,n), R(n,n);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      M(i,j) = (double)rand()/RAND_MAX;
    }
  }

  cout << "Original matrix M =\n";
  M.printMatrixForm(cout);

  mathmatrix<double> Md(M);

#ifndef GSTEST
  // QR decomposition of M.
  jlt::QRdecomp<double,matrix<double>,std::vector<double>>(Md,Q,R);
#else
  // Test Gram-Schmidt instead.
  Md.transpose();
  jlt::GramSchmidtOrthonorm(Md,R);
  Q = Md;
#endif

  cout << "\nM = Q.R, where\n";
  cout << "\nQ =\n"; Q.printMatrixForm(cout);
  cout << "\nR =\n"; R.printMatrixForm(cout);

  cout << "\nQ.R =\n";
#ifndef GSTEST
  Md = Q*R;
#else
  Md = R*Q;
  Md.transpose();
#endif
  Md.printMatrixForm(cout);

  mathmatrix<double> Merr = M - Md;
  double err = 0;
  for (double i : Merr) {
    err += std::abs(i);
  }
  cout << "\nTypical error = " << err/(n*n) << endl;
}
