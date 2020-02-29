//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
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
//       * this breaks if the command is run from outside the repo.  Fix?
//       * width of banner

//
// Extract Git or Mercurial version information at runtime.
//

#include <iostream>
#include <string>
#include <jlt/command.hpp>

namespace jlt {

bool detectGitRepo()
{
  std::string g = get_command_output("git rev-parse --is-inside-work-tree");

  return (g == "true" || g == "false");
}

bool detectHgRepo()
{
  std::string g = get_command_output("hg root");
  std::string prefix("abort");

  // Note that this returns 0 if they're equal (C-style).
  if (g.compare(0,prefix.size(),prefix) == 0)
    return false;
  else
    return true;
}

std::string getRepo()
{
  // Check for Git first, since we can have git subrepos inside hg,
  // but not the other way around.
  if (detectGitRepo())
    return std::string("git");
  else if (detectHgRepo())
    return std::string("hg");
  else
    return std::string("none");
}

// Short (7 char) hash string of the current commit.
std::string getVCSRevision()
{
  std::string r = getRepo();

  if (r == "git")
    return get_command_output("git log -1 --format=%h");
  else if (r == "hg")
    return get_command_output("hg parent --template '{node}' | cut -b -7");
  else
    return r;  // not an error if no repo
}

// Date of the current commit (YYYY-MM-DD).
std::string getVCSDate()
{
  std::string r = getRepo();

  if (r == "git")
    return get_command_output("git log -1 --date=short --format=%cd");
  else if (r == "hg")
    return get_command_output("hg parent --template \"{date|shortdate}\n\"");
  else
    return r;  // not an error if no repo
}

// Form revision and date string: e.g., "rev a98ad79 (2014-12-24)".
std::string getVCSRevDate()
{
  std::string vcsRevision(jlt::getVCSRevision());
  std::string vcsDate(jlt::getVCSDate());
  std::string revdate = "";

  if (vcsRevision != "none" && vcsDate != "none")
    {
      revdate += "rev " + vcsRevision + " ";
      revdate += "(" + vcsDate + ")";
    }

  return revdate;
}

// Print a banner with revision information.
void printVCSBanner(std::ostream& ostr, const std::string s)
{
  using std::string;
  using std::endl;

  string revdate = getVCSRevDate();

  // Do not print a banner if we can't find repo.
  // No complaints, just don't do it, in case the user is running the
  // command from outside the repo.
  if (revdate == "") return;

  string halfdashes(22,'-');
  string banner = halfdashes + " " + s + " ";
  banner += revdate + " ";
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
