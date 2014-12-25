//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <cstdlib>
#include <jlt/mathvector.hpp>
#include <jlt/math.hpp>
#include <jlt/finitediff.hpp>

using namespace jlt;

typedef double Real;

int main()
{
  using std::cout;
  using std::endl;

  const int N = 1000;
  Real dx = (Real)1./N;
  Real fuzz = .25;

  mathvector<Real> x(N), y(N);
  mathvector<Real> yp2(N), yp4(N), ypexact(N);
  mathvector<Real> err2(N), err4(N);

  for (int i = 0; i < N; ++i) {
    x[i] = (i + fuzz*((Real)rand()/RAND_MAX - 0.5))*dx;
    y[i] = Sin(2*M_PI*x[i]);
    ypexact[i] = 2*M_PI*Cos(2*M_PI*x[i]);
  }

  finitediff2(x,y,yp2,err2);
  finitediff4(x,y,yp4,err4);

  for (int i = 0; i < N; ++i) {
    yp2[i] = Log10(Abs(yp2[i] - ypexact[i]));
    yp4[i] = Log10(Abs(yp4[i] - ypexact[i]));
    cout << x[i];
    cout << "\t" << yp2[i] << "\t" << Log10(err2[i]);
    cout << "\t" << yp4[i] << "\t" << Log10(err4[i]);
    cout << endl;
  }
}
