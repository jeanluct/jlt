//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <cassert>
#include "jlt/freeword.hpp"


int main()
{
  using jlt::freeword;

  freeword<int> w({1,2,-2,3,4,-4,-3});
  w.reduce();
  assert((w == freeword<int>({1})));

  freeword<int> wi = w.inverse();
  assert((wi == freeword<int>({-1})));

  return 0;
}
