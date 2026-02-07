//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_MATHEMATICA_HPP
#define JLT_MATHEMATICA_HPP

#include <iostream>
#include <string>
#include <vector>
#include <jlt/matrix.hpp>

namespace jlt {

//
// Standalone functions for printing data structures in Mathematica format
//
// These functions output data in a format compatible with Mathematica/Wolfram Language.
// Note: Newlines are not appended automatically, as it's common in Mathematica to
// write multiple expressions on the same line.
//

// Forward declarations
template<typename T> class matrix;

//
// Print vector in Mathematica format: {elem1, elem2, elem3, ...}
//
template<typename T>
std::ostream& printMathematicaForm(std::ostream& strm,
                                    const std::vector<T>& v,
                                    const std::string name = "",
                                    const std::string description = "")
{
  if (v.empty()) return strm;

  // Print description as comment if specified.
  if (!description.empty())
    strm << "(* " << description << " *)" << std::endl;

  // Only print = if variable name is specified.
  if (!name.empty()) strm << name << " = ";

  strm << "{";
  for (auto i = v.cbegin(); i != v.cend() - 1; ++i)
  {
    strm << *i << ",";
  }
  // Don't append newline, since in Mathematica it is common to
  // write on same line.
  strm << v.back() << "}";

  return strm;
}

//
// Print matrix in Mathematica format: {{row1}, {row2}, ...}
//
template<typename T>
std::ostream& printMathematicaForm(std::ostream& strm,
                                    const matrix<T>& A,
                                    const std::string name = "",
                                    const std::string description = "")
{
  if (A.empty()) return strm;

  // Print description as comment if specified.
  if (!description.empty())
    strm << "(* " << description << " *)" << std::endl;

  // Only print = if variable name is specified.
  if (!name.empty()) strm << name << " = ";

  strm << "{";
  for (typename matrix<T>::size_type i = 0; i < A.rows(); ++i)
  {
    strm << "{";
    for (typename matrix<T>::size_type j = 0; j < A.columns(); ++j)
    {
      strm << A(i, j);
      if (j < A.columns() - 1) strm << ",";
    }
    strm << "}";
    if (i < A.rows() - 1) strm << ",";
  }
  // Don't append newline, since in Mathematica it is common to
  // write on same line.
  strm << "}";

  return strm;
}

} // namespace jlt

#endif // JLT_MATHEMATICA_HPP
