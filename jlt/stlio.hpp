//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
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
#include <complex>


namespace jlt {

//
// Formatting specifications.
//

// Base class for field separator (avoids repetition in specializations)
struct format_traits_base {
#ifdef JLT_FIELD_SEP_STRING
  // String to separate vector entries.
  static const char field_sep[];
#else
  // Number of spaces between entries.
  static const int field_sep = 2;
#endif
};

#ifdef JLT_FIELD_SEP_STRING
const char format_traits_base::field_sep[] = "  ";
#endif

// Default format traits for type T
template<class T>
struct format_traits : format_traits_base {
  // Scientific notation takes seven extra characters:
  // sign = 1, decimal point = 1, e = 1, exponent sign = 1, exponent = 2,
  // plus one because the precision does not include the leading digit.
  static const int extra_width_scientific = 7;

  // For unformatted and fixed notation, use a fixed field width.
  // This is the default for double precision, assuming about 13
  // digits of accuracy plus sign and decimal point.
  static const int field_width = 15;
};

// Specialization for int
template<>
struct format_traits<int> : format_traits_base {
  static const int extra_width_scientific = 0;
  static const int field_width = 6;  // 5 digits plus sign
};

// Specialization for float
template<>
struct format_traits<float> : format_traits_base {
  static const int extra_width_scientific = 7;
  static const int field_width = 13;  // ~11 digits plus sign and decimal
};

// Specialization for long double
template<>
struct format_traits<long double> : format_traits_base {
  static const int extra_width_scientific = 7;
  static const int field_width = 22;  // ~20 digits plus sign and decimal
};

// Specialization for complex numbers
// Field width accounts for both real and imaginary parts plus formatting
template<class T>
struct format_traits<std::complex<T>> : format_traits_base {
  static const int extra_width_scientific = format_traits<T>::extra_width_scientific;
  // Complex needs space for: (real,imag) = 2*field_width + 3 for parentheses and comma
  static const int field_width = 2 * format_traits<T>::field_width + 3;
};

//
// Implementation details - not part of public API
// These are internal helpers that may change without notice.
//
namespace detail {

// RAII helper to save and restore stream flags
class stream_flags_saver {
  std::ios::fmtflags flags_;
  std::streamsize precision_;
  std::ostream& strm_;
public:
  explicit stream_flags_saver(std::ostream& strm)
    : flags_(strm.flags()), precision_(strm.precision()), strm_(strm) {}

  ~stream_flags_saver() {
    strm_.flags(flags_);
    strm_.precision(precision_);
  }

  // Disable copy and move
  stream_flags_saver(const stream_flags_saver&) = delete;
  stream_flags_saver& operator=(const stream_flags_saver&) = delete;
  stream_flags_saver(stream_flags_saver&&) = delete;
  stream_flags_saver& operator=(stream_flags_saver&&) = delete;
};

// Helper to print field separator
template<class T>
void print_field_sep(std::ostream& strm) {
#ifdef JLT_FIELD_SEP_STRING
  strm << format_traits<T>::field_sep;
#else
  strm << std::string(format_traits<T>::field_sep, ' ');
#endif
}

// Common logic for printing sequential containers (vector, valarray)
template<class Container, class T>
std::ostream& print_sequence(std::ostream& strm, const Container& c) {
  if (c.size() == 0) return strm;

  stream_flags_saver saver(strm);
  const int prec = strm.precision();
  int wid = format_traits<T>::field_width;

  strm.setf(std::ios::showpoint);
  strm.setf(std::ios::right, std::ios::adjustfield);

  if (strm.flags() & std::ios::scientific)
    wid = prec + format_traits<T>::extra_width_scientific;

  auto it = std::begin(c);
  auto end = std::end(c);
  --end;  // Point to last element

  for (; it != end; ++it) {
    strm << std::setw(wid) << *it;
    print_field_sep<T>(strm);
  }
  strm << std::setw(wid) << *end;

  return strm;
}

} // namespace detail

template<class T>
[[nodiscard]]
std::ostream& operator<<(std::ostream& strm, const std::vector<T>& vv)
{
  return detail::print_sequence<std::vector<T>, T>(strm, vv);
}

template<class T>
[[nodiscard]]
std::ostream& operator<<(std::ostream& strm, const std::valarray<T>& vv)
{
  return detail::print_sequence<std::valarray<T>, T>(strm, vv);
}


//
//  Print list with consistent formatting.
//
template<class T>
[[nodiscard]]
std::ostream& operator<<(std::ostream& strm, const std::list<T>& ll)
{
  if (ll.empty()) return strm;

  auto it = ll.begin();
  auto end = ll.end();
  --end;

  for (; it != end; ++it) {
    strm << *it;
    detail::print_field_sep<T>(strm);
  }
  strm << *end;

  return strm;
}


template<class K, class T>
[[nodiscard]]
std::ostream& operator<<(std::ostream& strm, const std::map<K,T>& mm)
  {
    for (auto it = mm.cbegin(); it != mm.cend(); ++it)
      {
	strm << it->first;
	detail::print_field_sep<T>(strm);
	strm << it->second << '\n';
      }

    return strm;
  }

// Specialization: if the independent variable is of type double,
// print in scientific notation at fixed width and precision.
template<class T>
[[nodiscard]]
std::ostream& operator<<(std::ostream& strm, const std::map<double,T>& mm)
  {
    detail::stream_flags_saver saver(strm);

    const int prec = 5;		// Precision (number of digits - 1).
    const int wid = prec + 7;	// Extra characters in scientific notation.

    strm.precision(prec);
    strm.setf(std::ios::scientific);

    for (auto it = mm.cbegin(); it != mm.cend(); ++it)
      {
	strm << std::setw(wid) << it->first;
	detail::print_field_sep<T>(strm);
	strm << it->second << '\n';
      }

    return strm;
  }

// Output operator for complex numbers
// Format: (real, imag)
template<class T>
[[nodiscard]]
std::ostream& operator<<(std::ostream& strm, const std::complex<T>& c)
{
  strm << '(' << c.real() << ',' << c.imag() << ')';
  return strm;
}


//
// Input
//

// Read vv.size() elements from strm, overwriting content of vv.
// Stops reading on first failure and leaves remaining elements unchanged.
template<class T>
[[nodiscard]]
std::istream& operator>>(std::istream& strm, std::vector<T>& vv)
{
  for (auto i = vv.begin(); i != vv.end() && strm.good(); ++i)
    {
      strm >> *i;
    }

  return strm;
}

// Read elements into valarray from strm.
// Stops reading on first failure and leaves remaining elements unchanged.
template<class T>
[[nodiscard]]
std::istream& operator>>(std::istream& strm, std::valarray<T>& vv)
{
  for (std::size_t i = 0; i < vv.size() && strm.good(); ++i)
    {
      strm >> vv[i];
    }

  return strm;
}

// Read elements into list from strm until stream fails or list is full.
// Note: list doesn't have fixed size, so this reads until EOF or failure.
template<class T>
[[nodiscard]]
std::istream& operator>>(std::istream& strm, std::list<T>& ll)
{
  T value;
  while (strm >> value)
    {
      ll.push_back(value);
    }

  return strm;
}

} // namespace jlt

#endif // JLT_STLIO_HPP
