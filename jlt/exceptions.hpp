//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_EXCEPTIONS_HPP
#define JLT_EXCEPTIONS_HPP

#ifdef __EXCEPTIONS

#include <stdexcept>			// Include standard library exceptions.
#include <string>

#define JLT_TRY try
#define JLT_CATCH_ALL catch(...)
#define JLT_THROW(x) throw x
#define JLT_RETHROW throw
#define JLT_NOTHROW throw()
#define JLT_UNWIND(action) catch(...) { action; throw; }
#define JLT_CATCH_ALL_RETHROW catch(...) { throw; }

namespace jlt {

class too_many_steps : public std::runtime_error {
  unsigned long int nsteps;
public:
  too_many_steps(const std::string& _descr, unsigned long int _nsteps)
    : std::runtime_error(_descr), nsteps(_nsteps) {}

  [[nodiscard]] unsigned long int how_many() const { return nsteps; }
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
#define JLT_TRY
#define JLT_CATCH_ALL if (false)
#define JLT_THROW(x)
#define JLT_RETHROW
#define JLT_NOTHROW
#define JLT_UNWIND(action)
#define JLT_CATCH_ALL_RETHROW

#endif // __EXCEPTIONS

#endif // JLT_EXCEPTIONS_HPP
