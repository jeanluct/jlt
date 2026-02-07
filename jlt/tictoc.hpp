//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_TICTOC_HPP
#define JLT_TICTOC_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <boost/timer/timer.hpp>
#include <jlt/mathvector.hpp>

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
  mathvector<double, double> last_mark;  // [wall, user, system] (size 3)
  std::ostream ostr;

  static constexpr long long int nanoseconds_per_second = 1000000000LL;

  // Convert boost::timer::cpu_times to mathvector<double, double> in seconds
  static mathvector<double, double> to_seconds(const boost::timer::cpu_times& times)
  {
    mathvector<double, double> result(3);
    result[0] = static_cast<double>(times.wall) / nanoseconds_per_second;
    result[1] = static_cast<double>(times.user) / nanoseconds_per_second;
    result[2] = static_cast<double>(times.system) / nanoseconds_per_second;
    return result;
  }

public:
  // Constructor: specify output stream
  // Outputs wall, user, and system times (similar to boost::timer default)
  tictoc(std::ostream& ostr_ = std::cerr) :
    dangling(false), last_mark(3, 0.0), ostr(ostr_.rdbuf())
  {
  }

  void restart()
  {
    t.start();
    last_mark = mathvector<double, double>(3, 0.0);
  }

  void tic(const char what[] = 0)
  {
    last_mark = to_seconds(t.elapsed());

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
    auto current = to_seconds(t.elapsed());
    auto elapsed = current - last_mark;  // Vector subtraction!

    // Output format: 0.5w 0.5u 0s  (1.2w 1.2u 0s)
    // First group: elapsed times, second group (in parens): cumulative times
    // w=wall, u=user, s=system
    ostr << elapsed[0] << "w " << elapsed[1] << "u " << elapsed[2] << "s  ("
         << current[0] << "w " << current[1] << "u " << current[2] << "s)" << std::endl;

    last_mark = current;
    dangling = false;
  }

  // Get last elapsed times (for programmatic access)
  // Returns: {wall_elapsed, wall_cumulative, user_elapsed, user_cumulative, system_elapsed, system_cumulative}
  struct timing_info {
    double wall_elapsed;
    double wall_cumulative;
    double user_elapsed;
    double user_cumulative;
    double system_elapsed;
    double system_cumulative;
  };

  timing_info get_timing() const
  {
    auto current = to_seconds(t.elapsed());
    auto elapsed = current - last_mark;

    timing_info info;
    info.wall_elapsed = elapsed[0];
    info.wall_cumulative = current[0];
    info.user_elapsed = elapsed[1];
    info.user_cumulative = current[1];
    info.system_elapsed = elapsed[2];
    info.system_cumulative = current[2];

    return info;
  }
};

} // namespace jlt

#endif // JLT_TICTOC_HPP
