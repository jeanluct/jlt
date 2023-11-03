//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_TICTOC_HPP
#define JLT_TICTOC_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <boost/timer/timer.hpp>

//
// TODO:
//
// - Implement sublevels of timing using a stack.
//
// - More control over formatting
//

namespace jlt {

class tictoc
{
private:
  boost::timer::cpu_timer t;
  bool dangling;
  boost::timer::nanosecond_type last_mark;
  std::ostream ostr;

public:
  tictoc(std::ostream& ostr_ = std::cerr) :
    dangling(false), last_mark(0), ostr(ostr_.rdbuf())
  {
  }

  void restart()
  {
    t.start();
    last_mark = 0;
  }

  void tic(const char what[] = 0)
  {
    last_mark = t.elapsed().user;

    // If mark_end wasn't called after mark_start, we are left dangling.
    if (dangling) ostr << std::endl;
    dangling = true;

    if (what)
      {
	ostr << std::setiosflags(std::ios::left) << std::setw(60)
	     << what << "\t";
      }
  }

  void toc()
  {
    boost::timer::nanosecond_type tel = t.elapsed().user;

    const long long int onesecond = 1000000000LL;

    ostr << (double)(tel - last_mark)/onesecond << "\t";
    ostr << (double)tel/onesecond << std::endl;

    last_mark = tel;
    dangling = false;
  }
};

} // namespace jlt

#endif // JLT_TICTOC_HPP
