//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_FINITEDIFF_HPP
#define JLT_FINITEDIFF_HPP

#include <vector>
#include <cassert>

//
// Finite-differentiation of discrete data.
//

//
// To do:
//
// - Add equal-spacing stencils (overload routines: T dx).
//
// - Order-3 Forward2-Backward1 and Forward1-Backward2 routines, which
// combine to give a fourth-order result and an error estimate.
//


namespace jlt {

template<class T, class S>
void finitediff1(const std::vector<T>& x, const std::vector<S>& y,
		 std::vector<S>& dydx)
{
  int n = x.size();
  std::vector<T> dx(n);

  for (int i = 1; i < n; ++i) {
    dx[i] = x[i] - x[i-1];
    assert(dx[i] > 0);
  }

  for (int i = 0; i < n-1; ++i)
    dydx[i] = ForwardDiff1Stencil(dx,y,i);

  dydx[n-1] = BackwardDiff1Stencil(dx,y,n-1);
}

template<class T, class S>
void finitediff2(const std::vector<T>& x, const std::vector<S>& y,
		 std::vector<S>& dydx)
{
  int n = x.size();
  std::vector<T> dx(n);

  for (int i = 1; i < n; ++i) {
    dx[i] = x[i] - x[i-1];
    assert(dx[i] > 0);
  }

  dydx[0] = ForwardDiff2Stencil(dx,y,0);

  for (int i = 1; i < n-1; ++i)
    dydx[i] = CentralDiff2Stencil(dx,y,i);

  dydx[n-1] = BackwardDiff2Stencil(dx,y,n-1);
}

template<class T, class S>
void finitediff2(const std::vector<T>& x, const std::vector<S>& y,
		 std::vector<S>& dydx, std::vector<S>& err)
{
  int n = x.size();
  S dydx2;
  std::vector<T> dx(n);

  for (int i = 1; i < n; ++i) {
    dx[i] = x[i] - x[i-1];
    assert(dx[i] > 0);
  }

  // At the endpoints only get an error on the first-order result.
  dydx[0] = ForwardDiff2Stencil(dx,y,0);
  dydx2 = ForwardDiff1Stencil(dx,y,0);
  err[0] = dydx[0] - dydx2;

  for (int i = 1; i < n-2; ++i) {
    dydx[i] = CentralDiff2Stencil(dx,y,i);
    dydx2 = ForwardDiff2Stencil(dx,y,i);
    err[i] = dydx[i] - dydx2;
  }

  // Start using backward differences near the last point.
  dydx[n-2] = CentralDiff2Stencil(dx,y,n-2);
  dydx2 = BackwardDiff2Stencil(dx,y,n-2);
  err[n-2] = dydx[n-2] - dydx2;

  // At the endpoints only get an error on the first-order result.
  dydx[n-1] = BackwardDiff2Stencil(dx,y,n-1);
  dydx2 = BackwardDiff1Stencil(dx,y,n-1);
  err[n-1] = dydx[n-1] - dydx2;
}

template<class T, class S>
void finitediff4(const std::vector<T>& x, const std::vector<S>& y,
		 std::vector<S>& dydx)
{
  int n = x.size();
  std::vector<T> dx(n);

  for (int i = 1; i < n; ++i) dx[i] = x[i] - x[i-1];

  dydx[0] = ForwardDiff4Stencil(dx,y,0);
  dydx[1] = ForwardDiff4Stencil(dx,y,1);

  for (int i = 2; i < n-2; ++i) dydx[i] = CentralDiff4Stencil(dx,y,i);

  dydx[n-2] = BackwardDiff4Stencil(dx,y,n-2);
  dydx[n-1] = BackwardDiff4Stencil(dx,y,n-1);
}

template<class T, class S>
void finitediff4(const std::vector<T>& x, const std::vector<S>& y,
		 std::vector<S>& dydx, std::vector<S>& err)
{
  int n = x.size();
  S dydx2;
  std::vector<T> dx(n);

  for (int i = 1; i < n; ++i) {
    dx[i] = x[i] - x[i-1];
    assert(dx[i] > 0);
  }

  // At the endpoints only get an error on the third-order result.
  dydx[0] = ForwardDiff4Stencil(dx,y,0);
  dydx2 = ForwardDiff3Stencil(dx,y,0);
  err[0] = dydx[0] - dydx2;

  dydx[1] = ForwardDiff4Stencil(dx,y,1);
  dydx2 = ForwardDiff3Stencil(dx,y,1);
  err[1] = dydx[1] - dydx2;

  for (int i = 2; i < n-4; ++i) {
    dydx[i] = CentralDiff4Stencil(dx,y,i);
    dydx2 = ForwardDiff4Stencil(dx,y,i);
    err[i] = dydx[i] - dydx2;
  }

  // Start using backward differences near the last point.
  dydx[n-4] = CentralDiff4Stencil(dx,y,n-4);
  dydx2 = BackwardDiff4Stencil(dx,y,n-4);
  err[n-4] = dydx[n-4] - dydx2;

  dydx[n-3] = CentralDiff4Stencil(dx,y,n-3);
  dydx2 = BackwardDiff4Stencil(dx,y,n-3);
  err[n-3] = dydx[n-3] - dydx2;

  // At the endpoints only get an error on the third-order result.
  dydx[n-2] = BackwardDiff4Stencil(dx,y,n-2);
  dydx2 = BackwardDiff3Stencil(dx,y,n-2);
  err[n-2] = dydx[n-2] - dydx2;

  dydx[n-1] = BackwardDiff4Stencil(dx,y,n-1);
  dydx2 = BackwardDiff3Stencil(dx,y,n-1);
  err[n-1] = dydx[n-1] - dydx2;
}

//
// Stencils
//

template<class T, class S>
inline
S ForwardDiff1Stencil(const std::vector<T>& dx, const std::vector<S>& y, int i)
{
  return (y[i+1] - y[i])/dx[i+1];
}

template<class T, class S>
inline
S BackwardDiff1Stencil(const std::vector<T>& dx, const std::vector<S>& y, int i)
{
  return (y[i] - y[i-1])/dx[i];
}

template<class T, class S>
inline
S CentralDiff2Stencil(const std::vector<T>& dx, const std::vector<S>& y, int i)
{
  T a2 = dx[i]*dx[i], b2 = dx[i+1]*dx[i+1];

  return (y[i+1]*a2 - y[i-1]*b2 + y[i]*(b2 - a2)) /
    (dx[i+1]*dx[i]*(dx[i] + dx[i+1]));
}

template<class T, class S>
inline
S ForwardDiff2Stencil(const std::vector<T>& dx, const std::vector<S>& y, int i)
{
  T a = (dx[i+1] + dx[i+2]);

  return (-y[i+2]*dx[i+1]*dx[i+1] + y[i+1]*a*a
	  - y[i]*dx[i+2]*(2*dx[i+1] + dx[i+2])) / (dx[i+1]*dx[i+2]*a);
}

template<class T, class S>
inline
S BackwardDiff2Stencil(const std::vector<T>& dx, const std::vector<S>& y, int i)
{
  T a = (dx[i-1] + dx[i]);

  return (y[i-2]*dx[i]*dx[i] - y[i-1]*a*a
	  + y[i]*dx[i-1]*(2*dx[i] + dx[i-1])) / (dx[i-1]*dx[i]*a);
}

template<class T, class S>
inline
S ForwardDiff3Stencil(const std::vector<T>& dx, const std::vector<S>& y, int i)
{
  return
      y[i+3] * (dx[i+1]*(dx[i+1] + dx[i+2])) /
		(dx[i+3]*(dx[i+2] + dx[i+3])*(dx[i+1] + dx[i+2] + dx[i+3]))
    - y[i+2] * (dx[i+1]*(dx[i+1] + dx[i+2] + dx[i+3])) /
		(dx[i+2]*(dx[i+1] + dx[i+2])*dx[i+3])
    + y[i+1] * ((dx[i+1] + dx[i+2])*(dx[i+1] + dx[i+2] + dx[i+3])) /
		(dx[i+1]*dx[i+2]*(dx[i+2] + dx[i+3]))
    - y[i] * (3*dx[i+1]*dx[i+1] + dx[i+2]*(dx[i+2] + dx[i+3])
	       + 2*dx[i+1]*(2*dx[i+2] + dx[i+3])) /
              (dx[i+1]*(dx[i+1] + dx[i+2])*(dx[i+1] + dx[i+2] + dx[i+3]));
}

template<class T, class S>
inline
S BackwardDiff3Stencil(const std::vector<T>& dx, const std::vector<S>& y, int i)
{
  return
    - y[i-3] * (dx[i]*(dx[i] + dx[i-1])) /
                (dx[i-2]*(dx[i-1] + dx[i-2])*(dx[i] + dx[i-1] + dx[i-2]))
    + y[i-2] * (dx[i]*(dx[i] + dx[i-1] + dx[i-2])) /
                (dx[i-1]*dx[i-2]*(dx[i] + dx[i-1]))
    - y[i-1] * ((dx[i] + dx[i-1])*(dx[i] + dx[i-1] + dx[i-2])) /
                (dx[i]*dx[i-1]*(dx[i-1] + dx[i-2]))
    + y[i] * (3*dx[i]*dx[i] + dx[i-1]*(dx[i-1] + dx[i-2])
	      + 2*dx[i]*(2*dx[i-1] + dx[i-2])) /
              (dx[i]*(dx[i] + dx[i-1])*(dx[i] + dx[i-1] + dx[i-2]));
}

template<class T, class S>
inline
S CentralDiff4Stencil(const std::vector<T>& dx, const std::vector<S>& y, int i)
{
  return
    y[i+1] * (dx[i]*(dx[i-1] + dx[i])*(dx[i+1] + dx[i+2])) /
    (dx[i+1]*(dx[i] + dx[i+1])*(dx[i-1] + dx[i] + dx[i+1])*dx[i+2])
    - y[i-1] * (dx[i+1]*(dx[i-1] + dx[i])*(dx[i+1] + dx[i+2])) /
    (dx[i-1]*dx[i]*(dx[i] + dx[i+1])*(dx[i] + dx[i+1] + dx[i+2]))
    - y[i+2] * (dx[i]*dx[i+1]*(dx[i-1] + dx[i])) /
    (dx[i+2]*(dx[i+1] + dx[i+2])*(dx[i] + dx[i+1] + dx[i+2])
     *(dx[i-1] + dx[i] + dx[i+1] + dx[i+2]))
    + y[i-2] * (dx[i]*dx[i+1]*(dx[i+1] + dx[i+2])) /
    (dx[i-1]*(dx[i-1] + dx[i])*(dx[i-1] + dx[i] + dx[i+1])
     *(dx[i-1] + dx[i] + dx[i+1] + dx[i+2]))
    + y[i] * (dx[i-1]*(dx[i+1]*(dx[i+1] + dx[i+2])
		       - dx[i]*(2*dx[i+1] + dx[i+2]))
	    + dx[i]*(2*dx[i+1]*(dx[i+1] + dx[i+2])
		     - dx[i]*(2*dx[i+1] + dx[i+2]))) /
    (dx[i]*dx[i+1]*(dx[i-1] + dx[i])*(dx[i+1] + dx[i+2]));
}

template<class T, class S>
inline
S ForwardDiff4Stencil(const std::vector<T>& dx, const std::vector<S>& y, int i)
{
  return
    -y[i+4] * (dx[i+1]*(dx[i+1] + dx[i+2])*(dx[i+1] + dx[i+2] + dx[i+3])) /
    (dx[i+4]*(dx[i+3] + dx[i+4])*(dx[i+2] + dx[i+3] + dx[i+4])
     *(dx[i+1] + dx[i+2] + dx[i+3] + dx[i+4]))
    + y[i+3] * (dx[i+1]*(dx[i+1] + dx[i+2])
		*(dx[i+1] + dx[i+2] + dx[i+3] + dx[i+4])) /
    (dx[i+3]*(dx[i+2] + dx[i+3])*(dx[i+1] + dx[i+2] + dx[i+3])*dx[i+4])
    - y[i+2] * (dx[i+1]*(dx[i+1] + dx[i+2] + dx[i+3])
		*(dx[i+1] + dx[i+2] + dx[i+3] + dx[i+4])) /
    (dx[i+2]*(dx[i+1] + dx[i+2])*dx[i+3]*(dx[i+3] + dx[i+4]))
    + y[i+1] * ((dx[i+1] + dx[i+2])*(dx[i+1] + dx[i+2] + dx[i+3])
		*(dx[i+1] + dx[i+2] + dx[i+3] + dx[i+4])) /
    (dx[i+1]*dx[i+2]*(dx[i+2] + dx[i+3])*(dx[i+2] + dx[i+3] + dx[i+4]))
    - y[i] * (4*dx[i+1]*dx[i+1]*dx[i+1] + dx[i+2]*(dx[i+2] + dx[i+3])
	      *(dx[i+2] + dx[i+3] + dx[i+4])
	       + 3*dx[i+1]*dx[i+1]*(3*dx[i+2] + 2*dx[i+3] + dx[i+4])
	      + 2*dx[i+1]*(3*dx[i+2]*dx[i+2] + dx[i+3]*(dx[i+3] + dx[i+4])
			   + 2*dx[i+2]*(2*dx[i+3] + dx[i+4]))) /
    (dx[i+1]*(dx[i+1] + dx[i+2])*(dx[i+1] + dx[i+2] + dx[i+3])
     *(dx[i+1] + dx[i+2] + dx[i+3] + dx[i+4]));
}

template<class T, class S>
inline
S BackwardDiff4Stencil(const std::vector<T>& dx, const std::vector<S>& y, int i)
{
  return
    y[i-4] * (dx[i]*(dx[i-1] + dx[i])*(dx[i-2] + dx[i-1] + dx[i])) /
    (dx[i-3]*(dx[i-2] + dx[i-3])*(dx[i-1] + dx[i-2] + dx[i-3])
     *(dx[i] + dx[i-1] + dx[i-2] + dx[i-3]))
    - y[i-3] * (dx[i]*(dx[i] + dx[i-1])
		*(dx[i] + dx[i-1] + dx[i-2] + dx[i-3])) /
    (dx[i-2]*(dx[i-1] + dx[i-2])*(dx[i] + dx[i-1] + dx[i-2])*dx[i-3])
    + y[i-2] * (dx[i]*(dx[i] + dx[i-1] + dx[i-2])
		*(dx[i] + dx[i-1] + dx[i-2] + dx[i-3])) /
    (dx[i-1]*(dx[i] + dx[i-1])*dx[i-2]*(dx[i-2] + dx[i-3]))
    - y[i-1] * ((dx[i] + dx[i-1])
		*(dx[i] + dx[i-1] + dx[i-2])
		*(dx[i] + dx[i-1] + dx[i-2] + dx[i-3])) /
    (dx[i]*dx[i-1]*(dx[i-1] + dx[i-2])*(dx[i-1] + dx[i-2] + dx[i-3]))
    + y[i] * (4*dx[i]*dx[i]*dx[i] + dx[i-1]*(dx[i-1] + dx[i-2])
	      *(dx[i-1] + dx[i-2] + dx[i-3])
	      + 3*dx[i]*dx[i]*(3*dx[i-1] + 2*dx[i-2] + dx[i-3])
	      + 2*dx[i]*(3*dx[i-1]*dx[i-1] + dx[i-2]*(dx[i-2] + dx[i-3])
			 + 2*dx[i-1]*(2*dx[i-2] + dx[i-3]))) /
    (dx[i]*(dx[i] + dx[i-1])*(dx[i] + dx[i-1] + dx[i-2])
     *(dx[i] + dx[i-1] + dx[i-2] + dx[i-3]));
}

} // namespace jlt

#endif // JLT_FINITEDIFF_HPP
