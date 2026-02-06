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
#include <sstream>
#include <algorithm>

//
// Supported containers for output operator<<
//
#include <vector>
#include <valarray>
#include <list>
#include <array>
#include <deque>
#include <set>
#include <unordered_set>
#include <map>
#include <utility>   // For std::pair
#include <tuple>
#include <complex>

// C++17 features
#if __cplusplus >= 201703L
#include <string_view>
#include <optional>
#endif


namespace jlt {

//
// Formatting specifications.
//

// Base class for field separator (avoids repetition in specializations)
// C++17+: Use string_view for efficiency and flexibility
// C++11/14: Use constexpr function (can't use string_view or inline variables)
struct format_traits_base {
#if __cplusplus >= 201703L
  // C++17: string_view - efficient, no allocations, supports any string
  static constexpr std::string_view field_sep = "  ";
#else
  // C++11/14: constexpr function - works without inline variables
  static constexpr const char* field_sep() { return "  "; }
#endif
};

// Default format traits for type T
template<class T>
struct format_traits : format_traits_base {
  // Scientific notation takes seven extra characters:
  // sign = 1, decimal point = 1, e = 1, exponent sign = 1, exponent = 2,
  // plus one because the precision does not include the leading digit.
  static const int extra_width_scientific = 7;

  // Default field width for floating-point (double-ish)
  static const int field_width = 15;
};

//
// Integer types (signed)
//
template<>
struct format_traits<short> : format_traits_base {
  static const int extra_width_scientific = 0;
  static const int field_width = 6;
};

template<>
struct format_traits<int> : format_traits_base {
  static const int extra_width_scientific = 0;
  static const int field_width = 6;  // 5 digits plus sign
};

template<>
struct format_traits<long> : format_traits_base {
  static const int extra_width_scientific = 0;
  static const int field_width = 12;
};

template<>
struct format_traits<long long> : format_traits_base {
  static const int extra_width_scientific = 0;
  static const int field_width = 20;  // 19 digits + sign
};

//
// Integer types (unsigned)
//
template<>
struct format_traits<unsigned short> : format_traits_base {
  static const int extra_width_scientific = 0;
  static const int field_width = 5;
};

template<>
struct format_traits<unsigned int> : format_traits_base {
  static const int extra_width_scientific = 0;
  static const int field_width = 10;  // No sign needed
};

template<>
struct format_traits<unsigned long> : format_traits_base {
  static const int extra_width_scientific = 0;
  static const int field_width = 11;
};

template<>
struct format_traits<unsigned long long> : format_traits_base {
  static const int extra_width_scientific = 0;
  static const int field_width = 20;
};

//
// Floating-point types
//
template<>
struct format_traits<float> : format_traits_base {
  static const int extra_width_scientific = 7;
  static const int field_width = 13;  // ~11 digits plus sign and decimal
};

template<>
struct format_traits<long double> : format_traits_base {
  static const int extra_width_scientific = 7;
  static const int field_width = 22;  // ~20 digits plus sign and decimal
};

//
// bool
//
template<>
struct format_traits<bool> : format_traits_base {
  static const int extra_width_scientific = 0;
  // Print booleans in a single column; textual form may be used when boolalpha
  static const int field_width = 1;
};

//
// complex numbers
//
template<class T>
struct format_traits<std::complex<T>> : format_traits_base {
  static const int extra_width_scientific = format_traits<T>::extra_width_scientific;
  // Complex needs space for: (real,imag) = 2*field_width + 3 for parentheses and comma
  static const int field_width = 2 * format_traits<T>::field_width + 3;
};

// std::pair operator<<
//
// Implementation note: provide operator<< for std::pair so streaming of pairs
// uses the library's preferred formatting. This must be visible before the
// internal helpers that call operator<< on temporaries below.
template<class T1, class T2>
std::ostream& operator<<(std::ostream& strm, const std::pair<T1, T2>& pp)
{
  strm << '(' << pp.first << ',' << pp.second << ')';
  return strm;
}

//
// Implementation details - not part of public API
// These are internal helpers that may change without notice.
//
namespace detail {

// print_element: stream the value into a temporary using operator<< so that
// both built-in and user-provided operator<< representations are supported,
// then emit with setw.
template<class U>
void print_element(std::ostream& strm, const U& val, int wid)
{
  std::ostringstream tmp;
  tmp << val;
  strm << std::setw(wid) << tmp.str();
}

inline void print_element(std::ostream& strm, bool val, int wid)
{
  // Adaptive boolean width: use a larger width when std::boolalpha is enabled
  // (so textual "true"/"false" remain readable), otherwise keep the
  // compact width for numeric 0/1.
  int wid_used = wid;
  if (strm.flags() & std::ios::boolalpha) {
    // Reserve at least 5 characters for textual booleans ("false" length = 5)
    wid_used = std::max(wid, 5);
    strm << std::setw(wid_used) << std::boolalpha << val;
  } else {
    strm << std::setw(wid_used) << static_cast<int>(val);
  }
}

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
// C++17: field_sep is a string_view, C++11/14: field_sep() returns const char*
template<class T>
void print_field_sep(std::ostream& strm) {
#if __cplusplus >= 201703L
  strm << format_traits<T>::field_sep;
#else
  strm << format_traits<T>::field_sep();
#endif
}

// Common logic for printing sequential containers (vector, valarray, array, deque)
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

  // Handle first element specially to avoid trailing separator
  detail::print_element(strm, *it, wid);
  ++it;

  for (; it != end; ++it) {
    print_field_sep<T>(strm);
    detail::print_element(strm, *it, wid);
  }

  return strm;
}

} // namespace detail

template<class T>
std::ostream& operator<<(std::ostream& strm, const std::vector<T>& vv)
{
  return detail::print_sequence<std::vector<T>, T>(strm, vv);
}

template<class T>
std::ostream& operator<<(std::ostream& strm, const std::valarray<T>& vv)
{
  return detail::print_sequence<std::valarray<T>, T>(strm, vv);
}

// std::array output
template<class T, std::size_t N>
std::ostream& operator<<(std::ostream& strm, const std::array<T, N>& aa)
{
  return detail::print_sequence<std::array<T, N>, T>(strm, aa);
}

// std::deque output
template<class T>
std::ostream& operator<<(std::ostream& strm, const std::deque<T>& dd)
{
  return detail::print_sequence<std::deque<T>, T>(strm, dd);
}

// std::set output
template<class T>
std::ostream& operator<<(std::ostream& strm, const std::set<T>& ss)
{
  return detail::print_sequence<std::set<T>, T>(strm, ss);
}

// std::unordered_set output
template<class T>
std::ostream& operator<<(std::ostream& strm, const std::unordered_set<T>& ss)
{
  return detail::print_sequence<std::unordered_set<T>, T>(strm, ss);
}

// (std::pair output implemented earlier)

// C++17: Simplified tuple output using fold expressions and std::apply
#if __cplusplus >= 201703L

template<class... Args>
std::ostream& operator<<(std::ostream& strm, const std::tuple<Args...>& tt)
{
  strm << '(';
  if constexpr (sizeof...(Args) > 0) {
    // Use std::apply to unpack tuple into lambda with fold expression
    std::apply([&strm](const auto&... args) {
      std::size_t n = 0;
      // Fold expression: (condition ? separator : "") << element
      ((strm << (n++ ? "," : "") << args), ...);
    }, tt);
  }
  strm << ')';
  return strm;
}

#else

// C++11: Recursive template approach (fallback)
namespace detail {
  template<std::size_t I, class... Args>
  struct tuple_printer {
    static void print(std::ostream& strm, const std::tuple<Args...>& tt) {
      tuple_printer<I - 1, Args...>::print(strm, tt);
      strm << ',' << std::get<I>(tt);
    }
  };

  template<class... Args>
  struct tuple_printer<0, Args...> {
    static void print(std::ostream& strm, const std::tuple<Args...>& tt) {
      strm << std::get<0>(tt);
    }
  };

  // Helper to check if tuple has elements
  template<class... Args>
  struct tuple_has_elements : std::integral_constant<bool, (sizeof...(Args) > 0)> {};
}  // namespace detail

// std::tuple output (C++11) - only for non-empty tuples
template<class... Args>
typename std::enable_if<detail::tuple_has_elements<Args...>::value, std::ostream&>::type
operator<<(std::ostream& strm, const std::tuple<Args...>& tt)
{
  strm << '(';
  detail::tuple_printer<sizeof...(Args) - 1, Args...>::print(strm, tt);
  strm << ')';
  return strm;
}

// Empty tuple output
template<class... Args>
typename std::enable_if<!detail::tuple_has_elements<Args...>::value, std::ostream&>::type
operator<<(std::ostream& strm, const std::tuple<Args...>&)
{
  strm << "()";
  return strm;
}

#endif  // __cplusplus >= 201703L

// C++17: std::optional output
#if __cplusplus >= 201703L
template<class T>
std::ostream& operator<<(std::ostream& strm, const std::optional<T>& opt)
{
  if (opt.has_value()) {
    strm << *opt;
  } else {
    strm << "null";
  }
  return strm;
}
#endif

//
//  Print list with consistent formatting.
//
template<class T>
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


// Output operator for complex numbers
// Format: (real, imag)
template<class T>
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
