#include <iostream>
#include <iomanip>
#include <jlt/math.hpp>

using namespace jlt;
using namespace std;

int main()
{
  long double al = 1./9.L;
  long double bl = 1./3.L;
  long double cl = 1.e-30L;

  long double aal=.9L;
  double aad=.9;
  float aaf=.9;

  cout.setf(ios::fixed);
  cout.precision(40);

  cout << ".9l = " << aal << endl;
  cout << ".9  = " << aad << endl;
  cout << ".9f = " << aaf << endl << endl;

  cout << "Sqrt:\n";
  cout << Sqrt(al) << endl;
  cout << Sqrt((double)al) << endl;
  cout << Sqrt((float)al) << endl;
  cout << endl;
  cout << "Pow:\n";
  cout << Pow(Pow(1./al,bl),1./bl) << endl;
  cout << Pow(Pow((double)(1./al),(double)bl),(double)(1./bl)) << endl;
  cout << Pow(Pow((float)(1./al),(float)bl),(float)(1./bl)) << endl;

  for (int i = 1; i <= 4; ++i) {
    cout << endl;
    cout << Pow(10.L,-i) << endl;
    cout << Pow(10.,-i) << endl;
  }
  for (int i = 1; i <= 4; ++i) {
    cout << endl;
    cout << Pow(10.L,-(long double)i) << endl;
    cout << Pow(10.,-(double)i) << endl;
  }

  cout.setf(ios::scientific);

  cout << endl;
  cout << "Sin:\n";
  cout << Sin(cl) << endl;
  cout << Sin((double)cl) << endl;
  cout << Sin((float)cl) << endl;

  cout << endl;
  cout << "Tan:\n";
  cout << Tan(cl) << endl;
  cout << Tan((double)cl) << endl;
  cout << Tan((float)cl) << endl;

  cout << endl;
  cout << "Sinh:\n";
  cout << Sinh(cl) << endl;
  cout << Sinh((double)cl) << endl;
  cout << Sinh((float)cl) << endl;

  cout << endl;
  cout << "Tanh:\n";
  cout << Tanh(cl) << endl;
  cout << Tanh((double)cl) << endl;
  cout << Tanh((float)cl) << endl;

  cout << endl;
  cout << "Asin:\n";
  cout << Asin(cl) << endl;
  cout << Asin((double)cl) << endl;
  cout << Asin((float)cl) << endl;

  cout << endl;
  cout << "Atan:\n";
  cout << Atan(cl) << endl;
  cout << Atan((double)cl) << endl;
  cout << Atan((float)cl) << endl;

  cout << endl;
  cout << "Asinh:\n";
  cout << Asinh(cl) << endl;
  cout << Asinh((double)cl) << endl;
  cout << Asinh((float)cl) << endl;

  cout << endl;
  cout << "Atanh:\n";
  cout << Atanh(cl) << endl;
  cout << Atanh((double)cl) << endl;
  cout << Atanh((float)cl) << endl;

  cout << endl;
  cout << "Expm1:\n";
  cout << Expm1(cl) << endl;
  cout << Expm1((double)cl) << endl;
  cout << Expm1((float)cl) << endl;
}
