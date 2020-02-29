//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <cstdlib>
#include <jlt/mathvector.hpp>
#include <jlt/finitediff.hpp>


typedef double Real;

int main()
{
  using std::cout;
  using std::endl;
  using std::log10;
  using std::sin;
  using std::cos;
  using std::abs;
  using jlt::mathvector;

  // finitediff_test
  {
    const int N = 10;
    Real dx = (Real)1./N;
    Real fuzz = .1;

    mathvector<Real> x(N), y(N);
    mathvector<Real> yp1(N), yp2(N), yp4(N), ypexact(N);

    for (int i = 0; i < N; ++i) {
      x[i] = (i + fuzz*(rand()/RAND_MAX - 0.5))*dx;
      y[i] = sin(2*M_PI*x[i]);
      ypexact[i] = 2*M_PI*cos(2*M_PI*x[i]);
    }

    finitediff1(x,y,yp1);
    finitediff2(x,y,yp2);
    finitediff4(x,y,yp4);

    for (int i = 0; i < N; ++i) {
      yp1[i] = log10(abs(yp1[i] - ypexact[i]));
      yp2[i] = log10(abs(yp2[i] - ypexact[i]));
      yp4[i] = log10(abs(yp4[i] - ypexact[i]));
      cout << x[i] << "\t" << yp1[i] << "\t";
      cout << yp2[i] << "\t" << yp4[i] << endl;
    }
  }

  // finitediff_err_test
  {
    const int N = 1000;
    Real dx = (Real)1./N;
    Real fuzz = .25;

    mathvector<Real> x(N), y(N);
    mathvector<Real> yp2(N), yp4(N), ypexact(N);
    mathvector<Real> err2(N), err4(N);

    for (int i = 0; i < N; ++i) {
      x[i] = (i + fuzz*((Real)rand()/RAND_MAX - 0.5))*dx;
      y[i] = sin(2*M_PI*x[i]);
      ypexact[i] = 2*M_PI*cos(2*M_PI*x[i]);
    }

    finitediff2(x,y,yp2,err2);
    finitediff4(x,y,yp4,err4);

    for (int i = 0; i < N; ++i) {
      yp2[i] = log10(abs(yp2[i] - ypexact[i]));
      yp4[i] = log10(abs(yp4[i] - ypexact[i]));
      cout << x[i];
      cout << "\t" << yp2[i] << "\t" << log10(err2[i]);
      cout << "\t" << yp4[i] << "\t" << log10(err4[i]);
      cout << endl;
    }
  }
}
