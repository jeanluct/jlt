#include <iostream>
#include <jlt/mathvector.hpp>
#include <jlt/mathmatrix.hpp>
#include <jlt/svdecomp.hpp>
#include <jlt/stlio.hpp>

using namespace jlt;

int main()
{
  using std::cout;
  using std::endl;

  int m = 3, n = 4;
  mathmatrix<double> A(m,n), U(m,m), Vt(n,n);
  mathvector<double> w(std::min(m,n));

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

  cout.precision(6);
  cout.setf(std::ios::fixed);

  cout << "Original matrix A =\n";
  A.printMatrixForm(cout);

  singular_value_decomp(A,U,Vt,w);

  cout << "\nA = U.diag(w).Vt, where\n";
  cout << "\nU =\n"; U.printMatrixForm(cout);
  cout << "\nVt =\n"; Vt.printMatrixForm(cout);
  cout << "\nw = " << w << endl;

  cout << "\nU.diag(w).Vt =\n";
  (U*diagonal_matrix(w,m,n)*Vt).printMatrixForm(cout);
}
