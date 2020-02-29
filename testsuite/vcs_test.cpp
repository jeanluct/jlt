//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <iostream>
#include <jlt/vcs.hpp>

int main()
{
  using std::cout;
  using std::endl;

  // Print a banner if command is run from inside the repo.
  // Otherwise print nothing.
  jlt::printVCSBanner(cout,"jlt");

  cout << endl;
  cout << "  Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>\n";
  cout << endl;
}
