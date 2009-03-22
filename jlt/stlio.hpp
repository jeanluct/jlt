#ifndef JLT_STLIO_HPP
#define JLT_STLIO_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <valarray>
#include <list>
#include <map>


namespace jlt {

  /* Problem: The "modern" gcc (>2.96) does not like the string
     literal definition of field_sep.  Unless the macro
     __STATIC_INIT_FIXED__ is defined, a #define directive is used
     instead. */
#undef __STATIC_INIT_FIXED__


//
// Formatting specifications.
//
template<class T>
struct format_traits {

  // String to separate vector entries.
# if defined(__PGI) || defined(__KCC) || !defined(__STATIC_INIT_FIXED__)
#   define field_sep "  "
# else
    static const char field_sep[] = "  ";
# endif

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

template<>
struct format_traits<int> {

  // String to separate vector entries.
# if defined(__PGI) || defined(__KCC) || !defined(__STATIC_INIT_FIXED__)
#   define field_sep "  "
# else
    static const char field_sep[] = "  ";
# endif

  static const int extra_width_scientific = 0;

  // For short ints, assuming 5 digits plus sign.
  static const int field_width = 6;
};

template<>
struct format_traits<float> {

  // String to separate vector entries.
# if defined(__PGI) || defined(__KCC) || !defined(__STATIC_INIT_FIXED__)
#   define field_sep "  "
# else
    static const char field_sep[] = "  ";
# endif

  static const int extra_width_scientific = 7;

  // For single precision, assuming about 11 digits (being overly
  // generous, but this minimmum width is necessary if scientific
  // notation is needed) plus sign and decimal point.
  static const int field_width = 13;
};

template<>
struct format_traits<long double> {

  // String to separate vector entries.
# if defined(__PGI) || defined(__KCC) || !defined(__STATIC_INIT_FIXED__)
#   define field_sep "  "
# else
    static const char field_sep[] = "  ";
# endif

  static const int extra_width_scientific = 7;

  // For long doubles, assuming about 20 digits of accuracy plus sign
  // and decimal point.
  static const int field_width = 22;
};

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
#     if defined(__PGI) || defined(__KCC) || !defined(__STATIC_INIT_FIXED__)
        strm << std::setw(wid) << vv[i] << field_sep;
#     else
        strm << std::setw(wid) << vv[i] << format_traits<T>::field_sep;
#     endif
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
#     if defined(__PGI) || defined(__KCC) || !defined(__STATIC_INIT_FIXED__)
        strm << std::setw(wid) << vv[i] << field_sep;
#     else
        strm << std::setw(wid) << vv[i] << format_traits<T>::field_sep;
#     endif
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

#ifdef __PGI
  copy(ll.begin(), ll.end(), std::ostream_iterator<T,char>(strm, "\t"));
#else
  copy(ll.begin(), ll.end(), std::ostream_iterator<T>(strm, "\t"));
#endif
  return strm;
}


template<class K, class T>
std::ostream& operator<<(std::ostream& strm, const std::map<K,T>& mm)
  {
    for (typename std::map<K,T>::const_iterator it = mm.begin();
	 it != mm.end(); ++it)
      {
#       if defined(__PGI) || defined(__KCC) || !defined(__STATIC_INIT_FIXED__)
	  strm << it->first << field_sep << it->second << std::endl;
#       else
	  strm << it->first << format_traits<T>::field_sep << it->second
	       << std::endl;
#       endif
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

#       if defined(__PGI) || defined(__KCC) || !defined(__STATIC_INIT_FIXED__)
	  strm << std::setw(wid) << it->first << field_sep;
#       else
	  strm << std::setw(wid) << it->first << format_traits<T>::field_sep;
#       endif
	strm << it->second  << std::endl;
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
