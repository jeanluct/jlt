//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <iostream>
#include <jlt/mathmatrix.hpp>
#include <jlt/csparse.hpp>

int main ()
{
  using std::cout;
  using std::endl;
  using jlt::mathmatrix;

  mathmatrix<int> M(10,10);
  M(0,0) = 1;
  M(1,3) = 2;
  M(3,4) = 3;
  M(3,3) = 4;
  M(2,9) = 5;
  M(3,7) = 6;
  M(2,9) = 7;
  M(9,9) = 8;
  M(3,9) = 9;
  M(4,9) = -1;

  jlt::cs_unique_ptr T(jlt::mathmatrix_to_cs_sparse_matrix(M));

  csparse::cs_print(T,1);

  mathmatrix<int> M2(jlt::cs_sparse_matrix_to_mathmatrix<int>(T));

  if (M == M2) cout << "Equal!\n";

  jlt::csd_unique_ptr BT(csparse::cs_dmperm(T,1));

  if (!BT) { std::cerr << "Error\n"; exit(1); }

  cout << "nb = " << BT->nb << endl;

  for (int i = 1; i <= BT->nb; ++i)
    {
      cout << BT->r[i] << " ";
    }
  cout << endl;
}
