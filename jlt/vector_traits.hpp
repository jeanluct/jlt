#ifndef VECTOR_TRAITS_HPP
#define VECTOR_TRAITS_HPP

#include <vector>
#include <complex>
#include <list>

#include <matrix.hpp>
#include <math.hpp>

/*using namespace std;*/
/*using namespace jlt;*/

namespace rodent {

  //
  // The default template declaration works for all standard library
  // containers of simple types, such as vector<double> and list<float>.
  //
  template<class vecT>
  struct vector_traits
  {
    //
    // vector_type	The trait (vector) type.
    // value_type	The type of the dependent variables (T).
    // step_type	The type of the independent variable ("time step").
    // mag_type		The positive magnitude of vecT.
    // matrix_type	A matrix container (for the Jacobian).
    // mag		A function that returns the positive magnitude of x.
    //
    typedef vecT				vector_type;
    typedef typename vecT::value_type		value_type;
    typedef value_type				step_type;
    typedef value_type				mag_type;
    typedef matrix<value_type>			matrix_type;
#ifdef VECTOR_ITERATOR_LOOPS
    typedef typename vecT::iterator		iterator;
    typedef typename vecT::const_iterator	const_iterator;
#endif

    static inline mag_type	absval(step_type _x) { return Abs(_x); }
    static inline mag_type	mag(value_type _x) { return Abs(_x); }
    static inline vector_type	copy(const vector_type& _v) { return _v; }
  };

  //
  // Need to specialize traits class when the independent variable
  // is not of value_type, such as for complex types.
  //
  template<class float_type>
  struct vector_traits<vector<complex<float_type> > >
  {
    typedef vector<complex<float_type> >	vector_type;
    typedef complex<float_type>			value_type;
    typedef float_type				step_type;
    typedef float_type				mag_type;
    typedef matrix<value_type>			matrix_type;
#ifdef VECTOR_ITERATOR_LOOPS
    typedef typename vector_type::iterator		iterator;
    typedef typename vector_type::const_iterator	const_iterator;
#endif

    static inline mag_type 	absval(step_type _x) { return Abs(_x); }
    static inline mag_type	mag(value_type _x) { return abs(_x); }
    static inline vector_type	copy(const vector_type& _v) { return _v; }
  };

} // namespace rodent

#endif // VECTOR_TRAITS_HPP
