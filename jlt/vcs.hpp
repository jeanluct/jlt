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

// TODO: * print "+" when there are changes to repo.
//       * figure out git vs hg?

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


//
// Extract Mercurial version information at runtime.
//

// Short (7 char) hash string of the current commit.
std::string getHgRevision()
{
  return get_command_output("hg parent --template '{node}' | cut -b -7");
}

// Date of the current commit (YYYY-MM-DD).
std::string getHgDate()
{
  return get_command_output("hg parent --template \"{date|shortdate}\n\"");
}

// Form revision and date string: e.g., "rev a98ad79 (2014-12-24)".
// This function duplicates too much code from Git version.
std::string getHgRevDate()
{
  std::string gitRevision(jlt::getHgRevision());
  std::string gitDate(jlt::getHgDate());
  std::string revdate = "";

  if (gitRevision != "") revdate += "rev " + gitRevision + " ";
  if (gitDate != "") revdate += "(" + gitDate + ")";

  return revdate;
}

// Print a banner with revision information.
// This function duplicates too much code from Git version.
void printHgBanner(const std::string s, std::ostream& ostr = std::cout)
{
  using std::string;
  using std::endl;

  string halfdashes(22,'-');
  string banner = halfdashes + " " + s + " ";
  banner += getHgRevDate() + " ";
  banner += halfdashes;
  string fulldashes(banner.length(),'-');

  ostr << fulldashes << endl << banner << endl << fulldashes << endl;
}


//
// Extract data from Subversion keyword strings.
//

// Typical svn LastChangedRevision string:
// "$LastChangedRevision: 6 $"
// This function extracts just the revision number (as an integer).
std::string getSVNRevision(std::string LastChangedRevision)
{
  // Need at least 25 characters, otherwise the string was not expanded.
  if (LastChangedRevision.length() >= 25)
    {
      // Erase ""$LastChangedRevision: " at the start of the string.
      LastChangedRevision.erase(0,22);
      // Then erase the trailing " $".
      LastChangedRevision.erase(LastChangedRevision.length()-2,2);
    }
  else
    {
      LastChangedRevision = "";
    }
  return LastChangedRevision;
}

// Typical svn LastChangedDate string:
// "$LastChangedDate: 2011-08-31 14:10:23 -0500 (Wed, 31 Aug 2011) $"
// This function extracts just the date.
std::string getSVNDate(std::string LastChangedDate)
{
  // Need at least 28 characters, otherwise the string was not expanded.
  if (LastChangedDate.length() >= 28)
    {
      // The date consists of 10 characters starting from position 18.
      LastChangedDate = LastChangedDate.substr(18,10);
    }
  else
    {
      LastChangedDate = "";
    }
  return LastChangedDate;
}

} // namespace jlt

#endif // JLT_VCS_HPP
