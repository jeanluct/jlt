//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <iostream>
#include <jlt/tictoc.hpp>

int main()
{
  const int N = 100000000;

  std::cout << "=== tictoc timing (wall, user, system times) ===" << std::endl;
  std::cout << "Output format: <elapsed times>  (<cumulative times>)" << std::endl;
  std::cout << "               0.5w 0.5u 0s  (1.2w 1.2u 0s)" << std::endl;
  std::cout << "               w=wall, u=user, s=system (all in seconds)" << std::endl;
  std::cout << std::endl;

  // Timer: starts at 0 seconds upon creation.
  // Output times to stdout.
  // Prints wall-clock, user CPU, and system CPU times
  jlt::tictoc timer(std::cout);

  // Mark the start of a timed block.
  timer.tic("Summing harmonic series...");
  // Do something long...
  double sum = 0;
  for (int i = 1; i < N; ++i)
    {
      sum += 1/(double)i;
    }
  // Print elapsed time, cumulative time (for each of wall/user/system).
  timer.toc();
  std::cout << "sum = " << sum << std::endl;
  std::cout << std::endl;

  // Mark the start of another timed block.
  timer.tic("Summing Basel problem series...");
  // Do something long again...
  sum = 0;
  for (int i = 1; i < N; ++i)
    {
      sum += 1/((double)i*i);
    }
  // Print elapsed time, cumulative time.
  timer.toc();
  std::cout << "sum = " << sum << std::endl;
  std::cout << std::endl;

  std::cout << "=== Programmatic access to timing info ===" << std::endl;
  // Get timing information programmatically
  jlt::tictoc timer_api(std::cout);
  timer_api.tic();
  sum = 0;
  for (int i = 1; i < 1000000; ++i)
    {
      sum += 1/(double)i;
    }
  timer_api.toc();

  auto info = timer_api.get_timing();
  std::cout << "Via get_timing() API:" << std::endl;
  std::cout << "  Wall elapsed: " << info.wall_elapsed << "s" << std::endl;
  std::cout << "  User elapsed: " << info.user_elapsed << "s" << std::endl;
  std::cout << "  System elapsed: " << info.system_elapsed << "s" << std::endl;
  std::cout << "  Wall cumulative: " << info.wall_cumulative << "s" << std::endl;
  std::cout << "  User cumulative: " << info.user_cumulative << "s" << std::endl;
  std::cout << "  System cumulative: " << info.system_cumulative << "s" << std::endl;
}
