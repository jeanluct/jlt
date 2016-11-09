This is such an annoying bug.

The problem is that in `stlio.hpp`, I have a full template
specialization involving static non-integral data members.

```C++
/* The template */
template<class T>
struct format_traits {

  // String to separate vector entries.
  static const char field_sep[];
  ...
};
template<class T>
const char format_traits<T>::field_sep[] = "  ";
```

```C++
/* The specialization */
template<>
struct format_traits<float> {

  // String to separate vector entries.
  static const char field_sep[];
  ...
};
const char format_traits<float>::field_sep[] = "  ";
```
The second one of these is no longer a template, but is a bona fide
class, so cannot be included more than once when linking.  Thus when
there are enough files I get [a linking error](http://stackoverflow.com/questions/9656941/why-cant-i-initialize-non-const-static-member-or-static-array-in-class).  (See also [here](http://stackoverflow.com/questions/22304063/static-variable-initialization-for-template-specialization-in-header).)

Solution 1: make `stlio.cpp` containing only the `field_sep`
definitions.  Very annoying, since there are not other cpp files.

Solution 2: get rid of the custom definitons of `field_sep`.  They are
all set equal to the default anyways.  If a user really wants to
customize their own, they can declare it in a cpp file somewhere.

Solution 3: make `field_sep` an integer, denoting the number of
spaces.  Not ideal since sometimes I'd like to use a tab, maybe.

Solution 4: use `constexpr` rather than `const`, and make these an
in-class definition.  [Only possible with C++11](http://stackoverflow.com/questions/20279945/error-invalid-in-class-initialization-of-static-data-member-of-non-integral-typ).  The problem is it
doesn't seem to work.

