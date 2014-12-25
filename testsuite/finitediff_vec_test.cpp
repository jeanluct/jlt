//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <jlt/math.hpp>
#include <jlt/mathvector.hpp>
#include <jlt/fixvector.hpp>
#include <jlt/finitediff.hpp>


typedef double Real;

int main()
{
  using std::cout;
  using std::endl;
  using jlt::mathvector;

  const int N = 100;
  Real dx = (Real)1./N;

  std::vector<Real> x(N);
  std::vector<jlt::fixmathvector<Real,2> > y(N), yp(N), ypexact(N);

  for (int i = 0; i < N; ++i) {
    x[i] = i*dx;
    y[i][0] = jlt::Sin(2*M_PI*x[i]);
    y[i][1] = jlt::Cos(2*M_PI*x[i]);
    ypexact[i][0] = 2*M_PI*jlt::Cos(2*M_PI*x[i]);
    ypexact[i][1] = -2*M_PI*jlt::Sin(2*M_PI*x[i]);
  }

  finitediff4(x,y,yp);

  for (int i = 0; i < N; ++i) {
    yp[i][0] = jlt::Log10(jlt::Abs(yp[i][0] - ypexact[i][0]));
    yp[i][1] = jlt::Log10(jlt::Abs(yp[i][1] - ypexact[i][1]));
    cout << x[i] << "\t" << yp[i] << endl;
  }
}
