//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_EXCEPTIONS_HPP
#define JLT_EXCEPTIONS_HPP

#ifdef _EXCEPTIONS			// KCC
# define __EXCEPTIONS			// gcc
#endif

#ifdef __EXCEPTIONS

#include <stdexcept>			// Include standard library exceptions.
#include <string>

#define _TRY try
#define _CATCH_ALL catch(...)
#define _THROW(x) throw x
#define _RETHROW throw
#define _NOTHROW throw()
#define _UNWIND(action) catch(...) { action; throw; }
#define _CATCH_ALL_RETHROW catch(...) { throw; }

namespace jlt {

class too_many_steps : public std::runtime_error {
  unsigned long int nsteps;
public:
  too_many_steps(const std::string& _descr, unsigned long int _nsteps)
    : std::runtime_error(_descr), nsteps(_nsteps) {}

  unsigned long int how_many() const { return nsteps; }
};


template<class T>
class stepsize_too_small : public std::runtime_error {
  T stepsize;
public:
  stepsize_too_small(const std::string& _descr, T _stepsize)
    : std::runtime_error(_descr), stepsize(_stepsize) {}

  T how_small() const { return stepsize; }
};



template<class T>
class failed_to_converge : public std::runtime_error {
  T res;
public:
  failed_to_converge(const std::string& _descr, T _res = 0)
    : std::runtime_error(_descr), res(_res) {}

  T residual() const { return res; }
};

} // namespace jlt

#else // __EXCEPTIONS
#define _TRY
#define _CATCH_ALL if (false)
#define _THROW(x)
#define _RETHROW
#define _NOTHROW
#define _UNWIND(action)
#define _CATCH_ALL_RETHROW

#endif // __EXCEPTIONS

#endif // JLT_EXCEPTIONS_HPP
