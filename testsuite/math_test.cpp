//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <iostream>
#include <iomanip>
#include <cmath>

int main()
{
  using std::cout;
  using std::endl;
  using namespace std;

  long double al = 1./9.L;
  long double bl = 1./3.L;
  long double cl = 1.e-30L;

  long double aal=.9L;
  double aad=.9;
  float aaf=.9;

  cout.setf(std::ios::fixed);
  cout.precision(40);

  cout << ".9l = " << aal << endl;
  cout << ".9  = " << aad << endl;
  cout << ".9f = " << aaf << endl << endl;

  cout << "sqrt:\n";
  cout << sqrt(al) << endl;
  cout << sqrt((double)al) << endl;
  cout << sqrt((float)al) << endl;
  cout << endl;
  cout << "pow:\n";
  cout << pow(pow(1./al,bl),1./bl) << endl;
  cout << pow(pow((double)(1./al),(double)bl),(double)(1./bl)) << endl;
  cout << pow(pow((float)(1./al),(float)bl),(float)(1./bl)) << endl;

  for (int i = 1; i <= 4; ++i) {
    cout << endl;
    cout << pow(10.L,-i) << endl;
    cout << pow(10.,-i) << endl;
  }
  for (int i = 1; i <= 4; ++i) {
    cout << endl;
    cout << pow(10.L,-(long double)i) << endl;
    cout << pow(10.,-(double)i) << endl;
  }

  cout.setf(std::ios::scientific);

  cout << endl;
  cout << "sin:\n";
  cout << sin(cl) << endl;
  cout << sin((double)cl) << endl;
  cout << sin((float)cl) << endl;

  cout << endl;
  cout << "tan:\n";
  cout << tan(cl) << endl;
  cout << tan((double)cl) << endl;
  cout << tan((float)cl) << endl;

  cout << endl;
  cout << "sinh:\n";
  cout << sinh(cl) << endl;
  cout << sinh((double)cl) << endl;
  cout << sinh((float)cl) << endl;

  cout << endl;
  cout << "tanh:\n";
  cout << tanh(cl) << endl;
  cout << tanh((double)cl) << endl;
  cout << tanh((float)cl) << endl;

  cout << endl;
  cout << "Asin:\n";
  cout << asin(cl) << endl;
  cout << asin((double)cl) << endl;
  cout << asin((float)cl) << endl;

  cout << endl;
  cout << "Atan:\n";
  cout << atan(cl) << endl;
  cout << atan((double)cl) << endl;
  cout << atan((float)cl) << endl;

  cout << endl;
  cout << "Asinh:\n";
  cout << asinh(cl) << endl;
  cout << asinh((double)cl) << endl;
  cout << asinh((float)cl) << endl;

  cout << endl;
  cout << "Atanh:\n";
  cout << atanh(cl) << endl;
  cout << atanh((double)cl) << endl;
  cout << atanh((float)cl) << endl;

  cout << endl;
  cout << "expm1:\n";
  cout << expm1(cl) << endl;
  cout << expm1((double)cl) << endl;
  cout << expm1((float)cl) << endl;
}
