//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_VCS_HPP
#define JLT_VCS_HPP

//
// vcs.hpp
//

// Functions for Version Control Systems.


//
// Extract Git version information at runtime.
//

#include <iostream>
#include <string>
#include <jlt/command.hpp>

namespace jlt {

// Short (7 char) hash string of the current commit.
std::string getGitRevision()
{
  return get_command_output("git log -1 --format=%h");
}

// Date of the current commit (YYYY-MM-DD).
std::string getGitDate()
{
  return get_command_output("git log -1 --date=short --format=%cd");
}

// Form revision and date string: e.g., "rev a98ad79 (2014-12-24)".
std::string getGitRevDate()
{
  std::string gitRevision(jlt::getGitRevision());
  std::string gitDate(jlt::getGitDate());
  std::string revdate = "";

  if (gitRevision != "") revdate += "rev " + gitRevision + " ";
  if (gitDate != "") revdate += "(" + gitDate + ")";

  return revdate;
}

// Print a banner with revision information.
void printGitBanner(const std::string s, std::ostream& ostr = std::cout)
{
  using std::string;
  using std::endl;

  string halfdashes(22,'-');
  string banner = halfdashes + " " + s + " ";
  banner += getGitRevDate() + " ";
  banner += halfdashes;
  string fulldashes(banner.length(),'-');

  ostr << fulldashes << endl << banner << endl << fulldashes << endl;
}

} // namespace jlt

#endif // JLT_VCS_HPP
