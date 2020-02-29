//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_STLIO_HPP
#define JLT_STLIO_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <iterator>
#include <vector>
#include <valarray>
#include <list>
#include <map>


namespace jlt {

//
// Formatting specifications.
//
template<class T>
struct format_traits {

#ifdef JLT_FIELD_SEP_STRING
  // String to separate vector entries.
  static const char field_sep[];
#else
  // Number of spaces between entries.
  static const int field_sep = 2;
#endif

  // Scientific notation takes seven extra characters:
  // sign = 1, decimal point = 1, e = 1, exponent sign = 1, exponent = 2,
  // plus one because the precision does not include the leading digit.
  // (This will be wrong if the exponent has three digits,
  // but then things probably need to be rescaled anyways...)
  static const int extra_width_scientific = 7;

  // For unformatted and fixed notation, use a fixed field width.
  // This is the default for double precision, assuming about 13
  // digits of accuracy plus sign and decimal point.  Okay to be
  // conservative, since large numbers will just overflow field, not
  // be truncated.
  static const int field_width = 15;
};

#ifdef JLT_FIELD_SEP_STRING
template<class T>
const char format_traits<T>::field_sep[] = "  ";
#endif

template<>
struct format_traits<int> {

#ifdef JLT_FIELD_SEP_STRING
  // String to separate vector entries.
  static const char field_sep[];
#else
  // Number of spaces between entries.
  static const int field_sep = 2;
#endif

  static const int extra_width_scientific = 0;

  // For short ints, assuming 5 digits plus sign.
  static const int field_width = 6;
};

#ifdef JLT_FIELD_SEP_STRING
const char format_traits<int>::field_sep[] = "  ";
#endif

template<>
struct format_traits<float> {

#ifdef JLT_FIELD_SEP_STRING
  // String to separate vector entries.
  static const char field_sep[];
#else
  // Number of spaces between entries.
  static const int field_sep = 2;
#endif

  static const int extra_width_scientific = 7;

  // For single precision, assuming about 11 digits (being overly
  // generous, but this minimmum width is necessary if scientific
  // notation is needed) plus sign and decimal point.
  static const int field_width = 13;
};

#ifdef JLT_FIELD_SEP_STRING
const char format_traits<float>::field_sep[] = "  ";
#endif

template<>
struct format_traits<long double> {

#ifdef JLT_FIELD_SEP_STRING
  // String to separate vector entries.
  static const char field_sep[];
#else
  // Number of spaces between entries.
  static const int field_sep = 2;
#endif

  static const int extra_width_scientific = 7;

  // For long doubles, assuming about 20 digits of accuracy plus sign
  // and decimal point.
  static const int field_width = 22;
};

#ifdef JLT_FIELD_SEP_STRING
const char format_traits<long double>::field_sep[] = "  ";
#endif

template<class T>
std::ostream& operator<<(std::ostream& strm, const std::vector<T>& vv)
{
  if (vv.size() == 0) return strm;

  std::ios::fmtflags old_options = strm.flags();
  const int prec = strm.precision();	// Precision (number of digits - 1).
  int wid = format_traits<T>::field_width;	// Width of output field.

  strm.setf(std::ios::showpoint);		// Print trailing zeros.
  strm.setf(std::ios::right,std::ios::adjustfield);	// Adjust to the right.

  // If the notation is scientific we can predict the width, so adjust
  // accordingly.
  if (strm.flags() & std::ios::scientific)
      wid = prec + format_traits<T>::extra_width_scientific;

  for (unsigned int i = 0; i < vv.size()-1; ++i)
    {
      strm << std::setw(wid) << vv[i]
#ifdef JLT_FIELD_SEP_STRING
	   << format_traits<T>::field_sep;
#else
	   << std::string(format_traits<T>::field_sep,' ');
#endif
    }

  strm << std::setw(wid) << vv[vv.size()-1];	// To avoid dangling tab.

  // Restore format flags.
  strm.flags(old_options);

  return strm;
}

template<class T>
std::ostream& operator<<(std::ostream& strm, const std::valarray<T>& vv)
{
  if (vv.size() == 0) return strm;

  std::ios::fmtflags old_options = strm.flags();
  const int prec = strm.precision();	// Precision (number of digits - 1).
  int wid = format_traits<T>::field_width;	// Width of output field.

  strm.setf(std::ios::showpoint);		// Print trailing zeros.
  strm.setf(std::ios::right,std::ios::adjustfield);	// Adjust to the right.

  // If the notation is scientific we can predict the width, so adjust
  // accordingly.
  if (strm.flags() & std::ios::scientific)
      wid = prec + format_traits<T>::extra_width_scientific;

  for (unsigned int i = 0; i < vv.size()-1; ++i)
    {
      strm << std::setw(wid) << vv[i]
#ifdef JLT_FIELD_SEP_STRING
	   << format_traits<T>::field_sep;
#else
	   << std::string(format_traits<T>::field_sep,' ');
#endif
    }

  strm << std::setw(wid) << vv[vv.size()-1];	// To avoid dangling tab.

  // Restore format flags.
  strm.flags(old_options);

  return strm;
}


//
//  A quick-and-dirty way to print lists (or vectors).
//  Not using format info as with other methods yet.
//
template<class T>
std::ostream& operator<<(std::ostream& strm, const std::list<T>& ll)
{
  if (ll.empty()) return strm;

  copy(ll.begin(), ll.end(), std::ostream_iterator<T>(strm, "\t"));

  return strm;
}


template<class K, class T>
std::ostream& operator<<(std::ostream& strm, const std::map<K,T>& mm)
  {
    for (typename std::map<K,T>::const_iterator it = mm.begin();
	 it != mm.end(); ++it)
      {
	strm << it->first
#ifdef JLT_FIELD_SEP_STRING
	     << format_traits<T>::field_sep
#else
	     << std::string(format_traits<T>::field_sep,' ')
#endif
	     << it->second
	     << std::endl;
      }

    return strm;
  }

// Specialization: if the independent variable is of type double,
// print in scientific notation at fixed width and precision.
template<class T>
std::ostream& operator<<(std::ostream& strm, const std::map<double,T>& mm)
  {
    const int prec = 5;		// Precision (number of digits - 1).
    const int wid = prec + 7;	// Extra characters in scientific notation.

    for (typename std::map<double,T>::const_iterator it = mm.begin();
	 it != mm.end(); ++it)
      {
	// Should save ios flags and restore them at the end.
	strm.precision(prec);
	strm.setf(std::ios::scientific);

	strm << std::setw(wid)
	     << it->first
#ifdef JLT_FIELD_SEP_STRING
	     << format_traits<T>::field_sep
#else
	     << std::string(format_traits<T>::field_sep,' ')
#endif
	     << it->second << std::endl;
      }

    return strm;
  }


//
// Input
//

// Read vv.size() elements from strm, overwriting content of vv.
template<class T>
std::istream& operator>>(std::istream& strm, std::vector<T>& vv)
{
  for (typename std::vector<T>::iterator i = vv.begin(); i != vv.end(); ++i)
    {
      strm >> *i;
    }

  return strm;
}

} // namespace jlt

#endif // JLT_STLIO_HPP
