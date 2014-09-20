//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_TIME_MARK_HPP
#define JLT_TIME_MARK_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <boost/timer.hpp>

//
// TODO:
//
// - Implement sublevels of timing using a stack.
//
// - More control over formatting
//

namespace jlt {

class time_mark
{
private:
  boost::timer t;
  bool dangling;
  double last_mark;
  std::ostream ostr;

public:
  time_mark(std::ostream& ostr_ = std::cerr) :
    dangling(false), last_mark(0), ostr(ostr_.rdbuf())
  {
  }

  void restart()
  {
    t.restart();
    last_mark = 0;
  }

  void mark_start(const std::string& what)
  {
    last_mark = t.elapsed();

    // If mark_end wasn't called after mark_start, we are left dangling.
    if (dangling) ostr << std::endl;
    dangling = true;

    ostr << std::setiosflags(std::ios::left) << std::setw(60) << what << "\t";
  }

  void mark_end()
  {
    double tel = t.elapsed();

    ostr << tel - last_mark << "\t";
    ostr << tel << std::endl;

    last_mark = tel;
    dangling = false;
  }
};

} // namespace jlt

#endif // JLT_TIME_MARK_HPP
