#include <iostream>
#include <jlt/mathvector.hpp>
#include <jlt/mathmatrix.hpp>
#include <jlt/svdecomp.hpp>
#include <jlt/stlio.hpp>

using namespace jlt;
using namespace std;

int main()
{
  int m = 3, n = 2;
  mathmatrix<double> U(m,n), V(n,n);
  mathvector<double> w(n);

  U(0,0) = 1;
  U(0,1) = -2;
  U(1,0) = -1;
  U(1,1) = 1;
  U(2,0) = 1;
  U(2,1) = 2;

  cout << "Original matrix M =\n";
  U.printMatrixForm(cout);

  singular_value_decomp(U,V,w);

  cout << "\nM = U.diag(w).transp(V), where\n";
  cout << "\nU =\n"; U.printMatrixForm(cout);
  cout << "\nV =\n"; V.printMatrixForm(cout);
  cout << "\nw = " << w<< endl;

  cout << "\nU.diag(w).transp(V) =\n";
  mathmatrix<double> M = U*diagonal_matrix(w)*V.transpose();
  M.printMatrixForm(cout);
}
