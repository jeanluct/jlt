#ifndef JLT_MATH_HPP
#define JLT_MATH_HPP

//
// Overloaded definitions of C math funstions.
//
// The default GNU C math functions are double precision, but the
// library always provide a single precision and a long double version
// by appending f and l to the name, respectively.
//

#ifdef __PGI
#define PGCC_NO_LONG_DOUBLE \
    { cerr << "pgCC does not implement long double type.\n"; \
      std::exit(1); \
      return 0; }
#else
#include <cmath>
#endif
#include <iostream>

namespace jlt {

template<class T>
inline T Abs(const T a)
{
  return (a > 0 ? a : -a);
}

inline float Abs(const float a)
{
  return ::fabsf(a);
}

inline double Abs(const double a)
{
  return ::fabs(a);
}

inline long double Abs(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::fabsl(a);
# endif
}


template<class T>
inline T Log(const T a)
{
  std::cerr << "Log: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Log(const float a)
{
  return ::logf(a);
}

inline double Log(const double a)
{
  return ::log(a);
}

inline long double Log(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::logl(a);
# endif
}


template<class T>
inline T Log1p(const T a)
{
  std::cerr << "Log1p: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Log1p(const float a)
{
  return ::log1pf(a);
}

inline double Log1p(const double a)
{
  return ::log1p(a);
}

inline long double Log1p(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::log1pl(a);
# endif
}


template<class T>
inline T Log10(const T a)
{
  std::cerr << "Log10: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Log10(const float a)
{
  return ::log10f(a);
}

inline double Log10(const double a)
{
  return ::log10(a);
}

inline long double Log10(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::log10l(a);
# endif
}


template<class T>
inline T Exp(const T a)
{
  std::cerr << "Exp: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Exp(const float a)
{
  return ::expf(a);
}

inline double Exp(const double a)
{
  return ::exp(a);
}

inline long double Exp(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::expl(a);
# endif
}


template<class T>
inline T Expm1(const T a)
{
  std::cerr << "Expm1: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Expm1(const float a)
{
  return ::expm1f(a);
}

inline double Expm1(const double a)
{
  return ::expm1(a);
}

inline long double Expm1(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::expm1l(a);
# endif
}


template<class T, class S>
inline T Pow(const T a, const S b)
{
  std::cerr << "Pow: You cannot perform this math operation.\n";
  std::exit(1);
}

template<class S>
inline float Pow(const float a, const S b)
{
  return ::powf(a,b);
}

template<class S>
inline double Pow(const double a, const S b)
{
  return ::pow(a,b);
}

template<class S>
inline long double Pow(const long double a, const S b)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::powl(a,b);
# endif
}


//
// The Mod function differs from the fmod function from <cmath>: it
// behaves like a true mod function, where the returned value always
// has the same sign as b.  Hence, Mod(-1/3,1) = 2/3, and not -1/3.
//

template<class T, class S>
inline T Mod(const T a, const S b)
{
  std::cerr << "Mod: You cannot perform this math operation.\n";
  std::exit(1);
}

template<class S>
inline float Mod(const float a, const S b)
{
  return ::fmodf(::fmodf(a,b) + b,b);
}

template<class S>
inline double Mod(const double a, const S b)
{
  return ::fmod(::fmod(a,b) + b,b);
}

template<class S>
inline long double Mod(const long double a, const S b)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::fmodl(::fmodl(a,b) + b,b);
# endif
}


template<class T>
inline int Sign(const T a)
{
  // This function never returns 0.
  return (a >= 0 ? 1 : -1);
}


template<class T>
inline T Sqrt(const T a)
{
  std::cerr << "Sqrt: You cannot perform this math operation.\n";
  std::exit(1);

  return 0;
}

inline float Sqrt(const float a)
{
  return ::sqrtf(a);
}

inline double Sqrt(const double a)
{
  return ::sqrt(a);
}

inline long double Sqrt(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::sqrtl(a);
# endif
}


template<class T>
inline T Sin(const T a)
{
  std::cerr << "Sin: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Sin(const float a)
{
  return ::sinf(a);
}

inline double Sin(const double a)
{
  return ::sin(a);
}

inline long double Sin(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::sinl(a);
# endif
}


template<class T>
inline T Cos(const T a)
{
  std::cerr << "Cos: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Cos(const float a)
{
  return ::cosf(a);
}

inline double Cos(const double a)
{
  return ::cos(a);
}

inline long double Cos(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::cosl(a);
# endif
}


template<class T>
inline T Tan(const T a)
{
  std::cerr << "Tan: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Tan(const float a)
{
  return ::tanf(a);
}

inline double Tan(const double a)
{
  return ::tan(a);
}

inline long double Tan(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::tanl(a);
# endif
}


template<class T>
inline T Sinh(const T a)
{
  std::cerr << "Sinh: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Sinh(const float a)
{
  return ::sinhf(a);
}

inline double Sinh(const double a)
{
  return ::sinh(a);
}

inline long double Sinh(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::sinhl(a);
# endif
}


template<class T>
inline T Cosh(const T a)
{
  std::cerr << "Cosh: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Cosh(const float a)
{
  return ::coshf(a);
}

inline double Cosh(const double a)
{
  return ::cosh(a);
}

inline long double Cosh(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::coshl(a);
# endif
}


template<class T>
inline T Tanh(const T a)
{
  std::cerr << "Tanh: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Tanh(const float a)
{
  return ::tanhf(a);
}

inline double Tanh(const double a)
{
  return ::tanh(a);
}

inline long double Tanh(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::tanhl(a);
# endif
}


template<class T>
inline T Asin(const T a)
{
  std::cerr << "Asin: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Asin(const float a)
{
  return ::asinf(a);
}

inline double Asin(const double a)
{
  return ::asin(a);
}

inline long double Asin(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::asinl(a);
# endif
}


template<class T>
inline T Acos(const T a)
{
  std::cerr << "Acos: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Acos(const float a)
{
  return ::acosf(a);
}

inline double Acos(const double a)
{
  return ::acos(a);
}

inline long double Acos(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::acosl(a);
# endif
}


template<class T>
inline T Atan(const T a)
{
  std::cerr << "Atan: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Atan(const float a)
{
  return ::atanf(a);
}

inline double Atan(const double a)
{
  return ::atan(a);
}

inline long double Atan(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::atanl(a);
# endif
}


template<class T>
inline T Atan2(const T y, const T x)
{
  std::cerr << "Atan: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Atan2(const float y, const float x)
{
  return ::atan2f(y,x);
}

inline double Atan2(const double y, const double x)
{
  return ::atan2(y,x);
}

inline long double Atan2(const long double y, const long double x)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::atan2l(y,x);
# endif
}


template<class T>
inline T Asinh(const T a)
{
  std::cerr << "Asinh: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Asinh(const float a)
{
  return ::asinhf(a);
}

inline double Asinh(const double a)
{
  return ::asinh(a);
}

inline long double Asinh(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::asinhl(a);
# endif
}


template<class T>
inline T Acosh(const T a)
{
  std::cerr << "Acosh: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Acosh(const float a)
{
  return ::acoshf(a);
}

inline double Acosh(const double a)
{
  return ::acosh(a);
}

inline long double Acosh(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::acoshl(a);
# endif
}


template<class T>
inline T Atanh(const T a)
{
  std::cerr << "Atanh: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Atanh(const float a)
{
  return ::atanhf(a);
}

inline double Atanh(const double a)
{
  return ::atanh(a);
}

inline long double Atanh(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::atanhl(a);
# endif
}


template<class T>
inline T Floor(const T a)
{
  std::cerr << "Floor: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Floor(const float a)
{
  return ::floorf(a);
}

inline double Floor(const double a)
{
  return ::floor(a);
}

inline long double Floor(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::floorl(a);
# endif
}


template<class T>
inline T Ceil(const T a)
{
  std::cerr << "Ceil: You cannot perform this math operation.\n";
  std::exit(1);
}

inline float Ceil(const float a)
{
  return ::ceilf(a);
}

inline double Ceil(const double a)
{
  return ::ceil(a);
}

inline long double Ceil(const long double a)
{
# ifdef __PGI
    PGCC_NO_LONG_DOUBLE
# else
    return ::ceill(a);
# endif
}

} // namespace jlt

#endif // JLT_MATH_HPP
