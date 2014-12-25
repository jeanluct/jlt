//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <iostream>
#include <complex>
#include <jlt/mathvector.hpp>
#include <jlt/mathmatrix.hpp>
#include <jlt/stlio.hpp>
#include <jlt/eigensystem.hpp>


int main()
{
  using std::cout;
  using std::endl;
  using jlt::mathmatrix;
  using jlt::mathvector;

  int n = 3;
  mathmatrix<double> U(n,n);
  mathvector<double> w(n);

  // Upper half of M.
  U(0,0) = 1;
  U(0,1) = -2;
  U(0,2) = 1;
  U(1,1) = 1;
  U(1,2) = 3;
  U(2,2) = 1;

  // Symmetrise.
  for (int i = 1; i < n; ++i)
    {
      for (int j = 0; j < i; ++j)
	{
	  U(i,j) = U(j,i);
	}
    }

  cout << "Symmetric matrix M =\n";
  cout.precision(3);
  cout.setf(std::ios::fixed);
  U.printMatrixForm(cout);

  symmetric_matrix_eigensystem(U,w);

  cout << "\nM = inverse(U).diag(w).U, where\n";
  cout << "\nU =\n"; U.printMatrixForm(cout);
  cout << "\nw = " << w << endl;

  cout << "\ninverse(U).diag(w).U =\n";
  mathmatrix<double> M = U.inverse()*diagonal_matrix(w)*U;
  M.printMatrixForm(cout);

  mathvector<std::complex<double> > wc(n);

  // Nonsymmetric U.
  U(0,0) = 1;
  U(0,1) = -2;
  U(0,2) = 1;
  U(1,0) = -1;
  U(1,1) = -2;
  U(1,2) = 3;
  U(2,0) = 0;
  U(2,1) = 1;
  U(2,2) = 1;

  cout << "\n\nNonsymmetric matrix M =\n";
  U.printMatrixForm(cout);

  matrix_eigenvalues(U,wc);

  cout << "\nEigenvalues w = " << wc << endl;

  mathmatrix<std::complex<double> > Uc(n,n);
  const std::complex<double> i(0,1);

  Uc(0,0) = i;
  Uc(0,1) = std::complex<double>(-2,1);
  Uc(0,2) = 1;
  Uc(1,0) = -1;
  Uc(1,1) = -2;
  Uc(1,2) = 0;
  Uc(2,0) = std::complex<double>(1,-1);
  Uc(2,1) = -i;
  Uc(2,2) = 1;
 
  cout << "\n\nComplex nonsymmetric matrix M =\n";
  Uc.printMatrixForm(cout);

  matrix_eigenvalues(Uc,wc);

  cout << "\nEigenvalues w = " << wc << endl;
}
