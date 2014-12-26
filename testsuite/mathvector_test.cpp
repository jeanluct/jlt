//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <cassert>
#include <iostream>
#include <complex>
#include <jlt/mathvector.hpp>
#include <jlt/mathmatrix.hpp>


int main()
{
  using std::cin;
  using std::cout;
  using std::cerr;
  using std::endl;
  using jlt::mathvector;
  using jlt::mathmatrix;

  mathvector<double> a(3);
  mathvector<double> b(3);
  mathvector<double> ma(3);

  cout << a << endl << ma << endl;

  cout << ma.isZero() << endl;

  a[0] = 1.;
  a[1] = -1.;
  a[2] = 1.;

  b[0] = 1.;
  b[1] = 0.;
  b[2] = -1.;

  cout << dot(a,b) << endl << cross(a,b) << endl << 3.*a << endl;
  cout << a + b << endl << a - b << endl;
  a+=b;
  cout << a << endl;
  a-=b;
  cout << a << endl;
  a*=3.;
  cout << a << endl << mag2(a) << endl << endl;
  a/=3.;
  cout << a << endl << abs(a) << endl << endl;

  mathvector<double> c = cross(a,b);
  cout << endl << c << endl;

  cout << "\nNow for vectors of vectors:\n\n";

  // A vector of vectors.
  // Scalar type is double.
  mathvector<mathvector<double>,double> aa(2);
  for (int i = 0; i < 2; ++i) {
    aa[i] = std::vector<double>(4);
    for (int j = 0; j < 4; ++j) aa[i][j] = (i+1)*(j+1);
  }

  cout << aa << endl;

  cout << mag2(aa) << endl;

  cout << dot(aa,aa) << endl;

  cout << aa+aa << endl;

  mathmatrix<double> AA(3,3,1);
  cout << AA;
  mathmatrix<double> BB(3,3,2);
  cout << AA+BB;
  cout << AA*a << endl;
  cout << AA.det() << endl;

  // A matrix of matrices.
  // Scalar type is double.
  mathmatrix<mathmatrix<double>,double> MM(2,2,mathmatrix<double>(4,4));

  // Can multiply by mathvector of mathvectors.
  cout << endl << MM*aa << endl;

  MM += 1.;
  cout << MM << endl;

  cout << "Trying bounds-checked access...\n";
  try {
    a.at(3) = 1;
  } catch(std::out_of_range) {
    cout << "Out of range exception! ...Good\n";
  }

  cout << "Enter 3 numbers: ";
  cin >> a;
  cout << a << endl;

  cout << "identity_matrix(5) =" << endl;
  cout << jlt::identity_matrix<double>(5) << endl;

  mathmatrix<std::complex<double> > Ac(3,3,std::complex<double>(1,1));
  Ac(0,0) = 0.;
  Ac(1,1) = 0.;
  Ac(2,2) = 0.;
  cout << "\nComplex matrix:\n";
  cout << Ac << endl;
  mathmatrix<std::complex<double> > Aci(Ac.inverse());
  cout << "and its inverse:\n";
  cout << Aci << endl;
  cout << "Check:\n";
  cout << Ac*Aci << endl;

  mathvector<std::complex<double> > v(3,std::complex<double>(1,1));
  cout << "\nComplex vector (complex double scalar type):\n";
  cout << v << endl;
  cout << "and its magnitude: |v| = ";
  cout << abs(v) << endl;

  mathvector<std::complex<double>,double> v2(3,std::complex<double>(1,1));
  cout << "\nComplex vector (double scalar type):\n";
  cout << v2 << endl;
  cout << "and its magnitude: |v| = ";
  cout << abs(v2) << endl;
}
