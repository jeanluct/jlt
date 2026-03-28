//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_MATLAB_HPP
#define JLT_MATLAB_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <jlt/matrix.hpp>
#include <jlt/exceptions.hpp>
#include <jlt/stlio.hpp>
#ifdef JLT_MATLAB_LIB_SUPPORT
#  include "mat.h"
#endif


namespace jlt {


// Forward declarations.
template<typename T> class matrix;

//
// Implementation details - not part of public API
// These are internal helpers that may change without notice.
//
namespace detail {

template<typename T> std::ostream&
printMatlabForm_nodefaults(std::ostream&, const matrix<T>&,
			   const std::string, const std::string);

// Convert vector<vector<T>> to matrix<T>.
// Validates that all rows have the same size.
template<typename T>
matrix<T> vector_of_vectors_to_matrix(const std::vector<std::vector<T>>& vv)
{
  if (vv.empty() || vv[0].empty())
    {
      return matrix<T>(0, 0);
    }

  matrix<T> result(vv.size(), vv[0].size());

  for (typename std::vector<std::vector<T>>::size_type i = 0; i < vv.size(); ++i)
    {
      JLT_MATRIX_ASSERT(vv[i].size() == vv[0].size());
      for (typename std::vector<T>::size_type j = 0; j < vv[i].size(); ++j)
        {
          result(i, j) = vv[i][j];
        }
    }

  return result;
}

} // namespace detail


//
// MatlabFile: Unified file handle for Matlab binary (.mat) or text (.m) files
//
// Provides automatic file extension handling and RAII-based resource management.
// Works seamlessly with both binary MAT-files (when JLT_MATLAB_LIB_SUPPORT is
// defined) and text .m files.
//
// Example usage:
//   MatlabFile mf("data");  // Opens data.mat or data.m automatically
//   printMatlabForm(mf, 3.14, "pi");
//   printMatlabForm(mf, my_matrix, "A");
//   // File automatically closed by destructor
//
class MatlabFile {
private:
#ifdef JLT_MATLAB_LIB_SUPPORT
  MATFile* mat_handle;
#else
  std::ofstream text_handle;
#endif
  std::string filename;
  bool is_open;

public:
  // Open file with automatic extension (.mat or .m)
  // mode: "w" for write (maps to "wz" for compressed MAT-file v7)
  explicit MatlabFile(const std::string& basename, const std::string& mode = "w")
    : filename(basename), is_open(false)
    {
#ifdef JLT_MATLAB_LIB_SUPPORT
      // Append .mat extension
      filename += ".mat";

      // Map mode: "w" -> "wz" (compressed MAT-file v7)
      std::string mat_mode = "wz";
      if (mode == "w") mat_mode = "wz";
      // Future: support "w4", "w6", "w7.3" modes

      mat_handle = matOpen(filename.c_str(), mat_mode.c_str());
      if (mat_handle == nullptr)
	{
	  JLT_THROW(std::runtime_error(
	    "MatlabFile: Failed to open " + filename));
	}
      is_open = true;
#else
      // Append .m extension
      filename += ".m";

      text_handle.open(filename.c_str());
      if (!text_handle.is_open())
	{
	  JLT_THROW(std::runtime_error(
	    "MatlabFile: Failed to open " + filename));
	}
      is_open = true;
#endif
    }

  // Destructor: automatically close file
  ~MatlabFile()
    {
      if (is_open)
	{
	  close();
	}
    }

  // Delete copy constructor and copy assignment (file handle semantics)
  MatlabFile(const MatlabFile&) = delete;
  MatlabFile& operator=(const MatlabFile&) = delete;

  // Move constructor
  MatlabFile(MatlabFile&& other) noexcept
    :
#ifdef JLT_MATLAB_LIB_SUPPORT
    mat_handle(other.mat_handle),
#else
    text_handle(std::move(other.text_handle)),
#endif
    filename(std::move(other.filename)),
    is_open(other.is_open)
    {
      other.is_open = false;
#ifdef JLT_MATLAB_LIB_SUPPORT
      other.mat_handle = nullptr;
#endif
    }

  // Move assignment
  MatlabFile& operator=(MatlabFile&& other) noexcept
    {
      if (this != &other)
	{
	  // Close current file if open
	  if (is_open)
	    {
	      close();
	    }

	  // Move from other
#ifdef JLT_MATLAB_LIB_SUPPORT
	  mat_handle = other.mat_handle;
	  other.mat_handle = nullptr;
#else
	  text_handle = std::move(other.text_handle);
#endif
	  filename = std::move(other.filename);
	  is_open = other.is_open;
	  other.is_open = false;
	}
      return *this;
    }

  // Explicitly close the file
  void close()
    {
      if (is_open)
	{
#ifdef JLT_MATLAB_LIB_SUPPORT
	  if (mat_handle != nullptr)
	    {
	      matClose(mat_handle);
	      mat_handle = nullptr;
	    }
#else
	  if (text_handle.is_open())
	    {
	      text_handle.close();
	    }
#endif
	  is_open = false;
	}
    }

  // Check if file is open
  bool isOpen() const
    {
      return is_open;
    }

  // Get the full filename with extension
  std::string getFilename() const
    {
      return filename;
    }

  // Stream formatting methods (text mode only - no-ops in binary mode)
#ifdef JLT_MATLAB_LIB_SUPPORT
  // Binary mode: these are no-ops for API consistency
  void setPrecision(int) { }
  void setFlags(std::ios_base::fmtflags) { }
  void setHighPrecision() { }
#else
  // Text mode: configure stream formatting
  void setPrecision(int prec)
    {
      text_handle << std::setprecision(prec);
    }

  void setFlags(std::ios_base::fmtflags flags)
    {
      text_handle.setf(flags);
    }

  // Convenience method: set high precision with fixed and scientific notation
  void setHighPrecision()
    {
      text_handle << std::setprecision(16) << std::fixed << std::scientific;
    }

  // Stream insertion operator - forward to underlying ofstream
  // Allows: out << "something" << variable << "\n";
  template<typename T>
  MatlabFile& operator<<(const T& value)
    {
      text_handle << value;
      return *this;
    }

  // Overload for stream manipulators (endl, flush, etc.)
  MatlabFile& operator<<(std::ostream& (*manip)(std::ostream&))
    {
      text_handle << manip;
      return *this;
    }
#endif

  // Accessor methods for printMatlabForm functions
#ifdef JLT_MATLAB_LIB_SUPPORT
  MATFile* getMatHandle() { return mat_handle; }
#else
  std::ostream& getStream() { return text_handle; }
#endif
};


#ifdef JLT_MATLAB_LIB_SUPPORT

//
// MATfile versions (requires linking to Matlab libraries)
//

inline void printMatlabForm(MATFile *pmat,
		     const double var,
		     const std::string name = "",
		     const std::string description = "")
  {
    mxArray *A = mxCreateDoubleMatrix(1,1,mxREAL);
    double *Ap = mxGetPr(A);
    Ap[0] = var;
    matPutVariable(pmat,name.c_str(),A);
    mxDestroyArray(A);

    if (!description.empty())
      {
	auto name_descr = name + "_descr";
	auto mxdescr = mxCreateString(description.c_str());
	matPutVariable(pmat,name_descr.c_str(),mxdescr);
	mxDestroyArray(mxdescr);
      }
  }

// Overload for "old" format.
inline void printMatlabForm(MATFile *pmat,
		     const std::string name,
		     const double var,
		     const std::string description = "")
  {
    printMatlabForm(pmat,var,name,description);
  }

// Overload for string output.  Too much code duplication for now.
inline void printMatlabForm(MATFile *pmat,
		     const std::string str,
		     const std::string name,
		     const std::string description = "")
  {
    mxArray *A = mxCreateString(str.c_str());
    matPutVariable(pmat,name.c_str(),A);
    mxDestroyArray(A);

    if (!description.empty())
      {
	auto name_descr = name + "_descr";
	auto mxdescr = mxCreateString(description.c_str());
	matPutVariable(pmat,name_descr.c_str(),mxdescr);
	mxDestroyArray(mxdescr);
      }
  }

template<typename T>
void printMatlabForm(MATFile *pmat,
		     const std::vector<T>& v,
		     const std::string name,
		     const std::string description = "",
		     const std::string orientation = "")
  {
    // description string is written to name_descr in the MAT file.
    //
    // orientation is either "row" or "column" (default).
    mxArray *A;

    if (description == "row" || description == "column")
      {
	JLT_THROW(std::invalid_argument(
          "\"row\"/\"column\" specification should be 3rd argument."));
      }

    if (v.empty())
      {
	A = mxCreateDoubleMatrix(0,0,mxREAL);
      }
    else
      {
	if (orientation.empty())
	  {
	    // Default is a column vector.
	    A = mxCreateDoubleMatrix(v.size(),1,mxREAL);
	  }
	else
	  {
	    if (orientation == "column")
	      A = mxCreateDoubleMatrix(v.size(),1,mxREAL);
	    else if (orientation == "row")
	      A = mxCreateDoubleMatrix(1,v.size(),mxREAL);
	    else
	      {
		JLT_THROW(std::invalid_argument(
		  "3rd argument should be \"row\" or \"column\"."));
	      }
	  }
	double *Ap = mxGetPr(A);
	for (int i = 0; i < static_cast<int>(v.size()); ++i) Ap[i] = v[i];
      }
    matPutVariable(pmat,name.c_str(),A);
    mxDestroyArray(A);

    if (!description.empty())
      {
	auto name_descr = name + "_descr";
	auto mxdescr = mxCreateString(description.c_str());
	matPutVariable(pmat,name_descr.c_str(),mxdescr);
	mxDestroyArray(mxdescr);
      }
  }


template<typename T>
void printMatlabForm(MATFile *pmat,
		     const matrix<T>& A,
		     const std::string name = "",
		     const std::string description = "")
  {
    // See matrix.hpp for explanation of the nodefautls version.
    detail::printMatlabForm_nodefaults<T>(pmat,A,name,description);
  }

template<typename T>
void detail::printMatlabForm_nodefaults(MATFile *pmat,
				const matrix<T>& A,
				const std::string name,
				const std::string description)
  {
    // description string is written to name_descr in the MAT file.
    mxArray *mxA;
    if (A.empty())
      {
	mxA = mxCreateDoubleMatrix(0,0,mxREAL);
      }
    else
      {
	mxA = mxCreateDoubleMatrix(A.rows(),A.columns(),mxREAL);
	double *mxAp = mxGetPr(mxA);
	for (int i = 0; i < static_cast<int>(A.rows()); ++i)
	  {
	    for (int j = 0; j < static_cast<int>(A.columns()); ++j)
	      {
		mxAp[i + A.rows()*j] = A(i,j);
	      }
	  }
      }
    matPutVariable(pmat,name.c_str(),mxA);
    mxDestroyArray(mxA);

    if (!description.empty())
      {
	auto name_descr = name + "_descr";
	auto mxdescr = mxCreateString(description.c_str());
	matPutVariable(pmat,name_descr.c_str(),mxdescr);
	mxDestroyArray(mxdescr);
      }
  }


// vector<vector> calls matrix.
// If the rows are of different size then it's not really a good
// object to export to Matlab, except perhaps as a cell array.
template<typename T>
void printMatlabForm(MATFile *pmat,
		     const std::vector<std::vector<T>>& Avv,
		     const std::string name = "",
		     const std::string description = "")
  {
    matrix<T> A = detail::vector_of_vectors_to_matrix(Avv);
    printMatlabForm(pmat, A, name, description);
  }
#endif // JLT_MATLAB_LIB_SUPPORT


//
// iostream versions
//

inline std::ostream& printMatlabForm(std::ostream& strm,
			      const double var,
			      const std::string name,
			      const std::string description = "")
  {
    if (name.empty())
      {
	// Print description as comment if specified without name.
	if (!description.empty()) strm << "% " << description << std::endl;
      }
    else
      {
	// Print description as string name_description, before variable.
	auto name_descr = name + "_descr";
	if (!description.empty())
	  strm << name_descr << " = '" << description << "';" << std::endl;
      }

    // Only print = if name is specified.
    if (!name.empty()) strm << name << " = ";
    strm << var << ";\n";

    return strm;
  }

// Overload for "old" format.
inline std::ostream& printMatlabForm(std::ostream& strm,
			      const std::string name,
			      const double var,
			      const std::string description = "")
  {
    return printMatlabForm(strm,var,name,description);
  }

// Overload for string output.  Too much code duplication for now.
inline std::ostream& printMatlabForm(std::ostream& strm,
			      const std::string str,
			      const std::string name,
			      const std::string description = "")
  {
    if (name.empty())
      {
	// Print description as comment if specified without name.
	if (!description.empty()) strm << "% " << description << std::endl;
      }
    else
      {
	// Print description as string name_description, before variable.
	auto name_descr = name + "_descr";
	if (!description.empty())
	  strm << name_descr << " = '" << description << "';" << std::endl;
      }

    // Only print = if name is specified.
    if (!name.empty()) strm << name << " = '";
    strm << str << "';\n";

    return strm;
  }

template<typename T>
std::ostream& printMatlabForm(std::ostream& strm,
			      const std::vector<T>& v,
			      const std::string name = "",
			      const std::string description = "")
  {
    if (v.empty()) return strm;

    if (name.empty())
      {
	// Print description as comment if specified without name.
	if (!description.empty()) strm << "% " << description << std::endl;
      }
    else
      {
	// Print description as string name_description, before variable.
	auto name_descr = name + "_descr";
	if (!description.empty())
	  strm << name_descr << " = '" << description << "';" << std::endl;
      }

    // Only print = if variable name is specified.
    if (!name.empty()) strm << name << " = ";

    // If the vector is empty, just print "[];"
    if (v.empty()) { strm << "[];\n"; return strm; }

    strm << "[\n";

    for (auto i = v.cbegin(); i != v.cend(); ++i)
      {
	strm << " " << *i << std::endl;
      }
    strm << "];\n";

    return strm;
  }

template<typename T>
std::ostream& printMatlabForm(std::ostream& strm,
			      const matrix<T>& A,
			      const std::string name = "",
			      const std::string description = "")
  {
    // See matrix.hpp for explanation of the nodefautls version.
    return detail::printMatlabForm_nodefaults<T>(strm,A,name,description);
  }

template<typename T>
std::ostream& detail::printMatlabForm_nodefaults(std::ostream& strm,
					 const matrix<T>& A,
					 const std::string name,
					 const std::string description)
  {
    if (name.empty())
      {
	// Print description as comment if specified without name.
	if (!description.empty()) strm << "% " << description << std::endl;
      }
    else
      {
	// Print description as string name_description, before variable.
	auto name_descr = name + "_descr";
	if (!description.empty())
	  strm << name_descr << " = '" << description << "';" << std::endl;
      }

    // Only print = if name is specified.
    if (!name.empty()) strm << name << " = ";

    // If the vector is empty, just print "[];"
    if (A.empty()) { strm << "[];\n"; return strm; }

    strm << "[\n";
    for (int i = 0; i < static_cast<int>(A.rows()); ++i) {
      for (int j = 0; j < static_cast<int>(A.columns())-1; ++j)
	{
	  strm << A(i,j) << " ";
	}
      strm << A(i,A.columns()-1) << "\n";
    }
    strm << "];\n";

    return strm;
  }


// vector<vector> calls matrix.
// If the rows are of different size then it's not really a good
// object to export to Matlab, except perhaps as a cell array.
template<typename T>
std::ostream& printMatlabForm(std::ostream& strm,
			      const std::vector<std::vector<T>>& Avv,
			      const std::string name = "",
			      const std::string description = "")
  {
    matrix<T> A = detail::vector_of_vectors_to_matrix(Avv);
    return printMatlabForm(strm, A, name, description);
  }


//
// MatlabFile convenience overloads
//

#ifdef JLT_MATLAB_LIB_SUPPORT

inline void printMatlabForm(MatlabFile& mf, const double var,
		     const std::string name,
		     const std::string description = "")
  {
    printMatlabForm(mf.getMatHandle(), var, name, description);
  }

inline void printMatlabForm(MatlabFile& mf, const std::string str,
		     const std::string name,
		     const std::string description = "")
  {
    printMatlabForm(mf.getMatHandle(), str, name, description);
  }

template<typename T>
void printMatlabForm(MatlabFile& mf, const std::vector<T>& v,
		     const std::string name = "",
		     const std::string description = "",
		     const std::string orientation = "")
  {
    printMatlabForm(mf.getMatHandle(), v, name, description, orientation);
  }

template<typename T>
void printMatlabForm(MatlabFile& mf, const matrix<T>& A,
		     const std::string name = "",
		     const std::string description = "")
  {
    printMatlabForm(mf.getMatHandle(), A, name, description);
  }

template<typename T>
void printMatlabForm(MatlabFile& mf, const std::vector<std::vector<T>>& Avv,
		     const std::string name = "",
		     const std::string description = "")
  {
    printMatlabForm(mf.getMatHandle(), Avv, name, description);
  }

#else

inline std::ostream& printMatlabForm(MatlabFile& mf, const double var,
			      const std::string name,
			      const std::string description = "")
  {
    return printMatlabForm(mf.getStream(), var, name, description);
  }

inline std::ostream& printMatlabForm(MatlabFile& mf, const std::string str,
			      const std::string name,
			      const std::string description = "")
  {
    return printMatlabForm(mf.getStream(), str, name, description);
  }

template<typename T>
std::ostream& printMatlabForm(MatlabFile& mf, const std::vector<T>& v,
			      const std::string name = "",
			      const std::string description = "")
  {
    return printMatlabForm(mf.getStream(), v, name, description);
  }

template<typename T>
std::ostream& printMatlabForm(MatlabFile& mf, const matrix<T>& A,
			      const std::string name = "",
			      const std::string description = "")
  {
    return printMatlabForm(mf.getStream(), A, name, description);
  }

template<typename T>
std::ostream& printMatlabForm(MatlabFile& mf, const std::vector<std::vector<T>>& Avv,
			      const std::string name = "",
			      const std::string description = "")
  {
    return printMatlabForm(mf.getStream(), Avv, name, description);
  }

#endif


} // namespace jlt

#endif // JLT_MATLAB_HPP
