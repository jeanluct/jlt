#ifndef JLT_FIXMATRIX_HPP
#define JLT_FIXMATRIX_HPP

//
// fixmatrix.hpp
//

#include <matrix.hpp>
#include <mathmatrix.hpp>

namespace jlt {

template<class T, unsigned int M, unsigned int N>
class fixmatrix : public matrix<T>
{
public:

  //
  // Constructors
  //

  // Matrix of size M*N filled with _x.
  explicit fixmatrix(const_reference _x = T()) : matrix<T>(M,N,_x) {}

  // Copy constructor.
  fixmatrix(const matrix<T>& _v) : matrix<T>(_v) {}
}; // class fixmatrix

template<class T, unsigned int M, unsigned int N, class S = T>
class fixmathmatrix : public mathmatrix<T,S>
{
public:

  //
  // Constructors
  //

  // Matrix of size M*N filled with _x.
  explicit fixmathmatrix(const_reference _x = T()) : mathmatrix<T,S>(M,N,_x) {}

  // Copy constructor.
  fixmathmatrix(const mathmatrix<T,S>& _v) : mathmatrix<T,S>(_v) {}
}; // class fixmathmatrix

} // namespace jlt

#endif // JLT_FIXMATRIX_HPP
