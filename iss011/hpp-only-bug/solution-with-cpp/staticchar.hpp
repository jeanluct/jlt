#ifndef STATICCHAR_HPP
#define STATICCHAR_HPP

template<class T>
struct A {
  static const char str[];
};

template<class T>
const char A<T>::str[] = "T string";

template<>
struct A<int> {
  static const char str[];
};

#endif // STATICCHAR_HPP
