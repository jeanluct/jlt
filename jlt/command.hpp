//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_COMMAND_HPP
#define JLT_COMMAND_HPP

//
// command.hpp
//

// Run Unix commands and get output.

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

namespace jlt {

// Get output of Unix command.
std::string get_command_output(const std::string& commandstr)
{
  // Run the command, redirecting stderr to /dev/null.
  FILE* pipe(popen((commandstr + " 2>&1").c_str(), "r"));

  // Should be more graceful here.
  if (!pipe)
    {
      std::cerr << "Something went wrong in jlt::get_command_output.\n";
      std::exit(1);
    }

  std::string output;
  char buffer[256];
  while(fgets(buffer,sizeof(buffer),pipe) != NULL)
    {
      std::string file = buffer;
      output += file.substr(0,file.size()-1);
    }

  pclose(pipe);

  return output;
}

} // namespace jlt

#endif // JLT_COMMAND_HPP
