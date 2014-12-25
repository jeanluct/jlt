//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <cstdlib>
#include <jlt/mathvector.hpp>
#include <jlt/math.hpp>
#include <jlt/finitediff.hpp>
#include <jlt/fixvector.hpp>


typedef double Real;

int main()
{
  using std::cout;
  using std::endl;
  using jlt::mathvector;
  using jlt::Log10;
  using jlt::Sin;
  using jlt::Cos;
  using jlt::Abs;

  // finitediff_test
  {
    const int N = 10;
    Real dx = (Real)1./N;
    Real fuzz = .1;

    mathvector<Real> x(N), y(N);
    mathvector<Real> yp1(N), yp2(N), yp4(N), ypexact(N);

    for (int i = 0; i < N; ++i) {
      x[i] = (i + fuzz*(rand()/RAND_MAX - 0.5))*dx;
      y[i] = Sin(2*M_PI*x[i]);
      ypexact[i] = 2*M_PI*Cos(2*M_PI*x[i]);
    }

    finitediff1(x,y,yp1);
    finitediff2(x,y,yp2);
    finitediff4(x,y,yp4);

    for (int i = 0; i < N; ++i) {
      yp1[i] = Log10(Abs(yp1[i] - ypexact[i]));
      yp2[i] = Log10(Abs(yp2[i] - ypexact[i]));
      yp4[i] = Log10(Abs(yp4[i] - ypexact[i]));
      cout << x[i] << "\t" << yp1[i] << "\t";
      cout << yp2[i] << "\t" << yp4[i] << endl;
    }
  }

  // finitediff_vec_test
  {
    const int N = 100;
    Real dx = (Real)1./N;

    std::vector<Real> x(N);
    std::vector<jlt::fixmathvector<Real,2> > y(N), yp(N), ypexact(N);

    for (int i = 0; i < N; ++i) {
      x[i] = i*dx;
      y[i][0] = Sin(2*M_PI*x[i]);
      y[i][1] = Cos(2*M_PI*x[i]);
      ypexact[i][0] = 2*M_PI*Cos(2*M_PI*x[i]);
      ypexact[i][1] = -2*M_PI*Sin(2*M_PI*x[i]);
    }

    finitediff4(x,y,yp);

    for (int i = 0; i < N; ++i) {
      yp[i][0] = Log10(Abs(yp[i][0] - ypexact[i][0]));
      yp[i][1] = Log10(Abs(yp[i][1] - ypexact[i][1]));
      cout << x[i] << "\t" << yp[i] << endl;
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
}
