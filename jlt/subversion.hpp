//
// Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_SUBVERSION_HPP
#define JLT_SUBVERSION_HPP

//
// subversion.hpp
//

// Extract data from subversion keyword strings.

// TODO: have getSVNDate and getSVNRevision accept the Id SVN string.

#include <string>

namespace jlt {

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

} // namespace jlt

#endif // JLT_SUBVERSION_HPP
