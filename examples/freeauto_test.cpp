//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <cassert>
#include "jlt/freeauto.hpp"


int main()
{
  using jlt::freeauto;
  using jlt::freeword;

  freeauto<int> id(3);
  freeauto<int> a(3);
  a[1] = {2};
  a[2] = {-1,3};
  a[3] = {3};

  freeauto<int> aid = a * id;
  assert((aid[1] == a[1]));
  assert((aid[2] == a[2]));
  assert((aid[3] == a[3]));

  freeword<int> w = a.get_action(-2);
  assert((w == freeword<int>({-3,1})));

  return 0;
}
