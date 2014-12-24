//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <iostream>
#include <jlt/time_mark.hpp>

int main()
{
  const int N = 100000000;

  // Timer: starts at 0 seconds upon creation.
  // Output times to stdout.
  jlt::time_mark timer(std::cout);

  // Mark the start of a timed block.
  timer.mark_start("Summing harmonic series...");
  // Do something long...
  double sum = 0;
  for (int i = 1; i < N; ++i)
    {
      sum += 1/(double)i;
    }
  // Print elapsed time, cumulative time.
  timer.mark_end();
  std::cout << "sum = " << sum << std::endl;

  // Mark the start of another timed block.
  timer.mark_start("Summing Basel problem series...");
  // Do something long again...
  sum = 0;
  for (int i = 1; i < N; ++i)
    {
      sum += 1/((double)i*i);
    }
  // Print elapsed time, cumulative time.
  timer.mark_end();
  std::cout << "sum = " << sum << std::endl;
}
