//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_MATLAB_HPP
#define JLT_MATLAB_HPP

#include <iostream>
#include <string>
#include <vector>
#include <jlt/matrix.hpp>
#include <jlt/exceptions.hpp>
#ifdef JLT_MATLAB_LIB_SUPPORT
#  include "mat.h"
#endif


namespace jlt {


  // Forward declarations.
template<typename T> class matrix;

template<typename T> std::ostream&
printMatlabForm_nodefaults(std::ostream&, const matrix<T>&,
			   const std::string, const std::string);


#ifdef JLT_MATLAB_LIB_SUPPORT

//
// MATfile versions (requires linking to Matlab libraries)
//

void printMatlabForm(MATFile *pmat,
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
void printMatlabForm(MATFile *pmat,
		     const std::string name,
		     const double var,
		     const std::string description = "")
  {
    printMatlabForm(pmat,var,name,description);
  }

// Overload for string output.  Too much code duplication for now.
void printMatlabForm(MATFile *pmat,
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
	for (int i = 0; i < (int)v.size(); ++i) Ap[i] = v[i];
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
    printMatlabForm_nodefaults<T>(pmat,A,name,description);
  }

template<typename T>
void printMatlabForm_nodefaults(MATFile *pmat,
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
	for (int i = 0; i < (int)A.rows(); ++i)
	  {
	    for (int j = 0; j < (int)A.columns(); ++j)
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
    jlt::matrix<T> A(Avv.size(),Avv[0].size());

    /* TODO: this should be a constructor; check all rows have same size. */
    for (int i = 0; i < Avv.size(); ++i)
      {
	MATRIX_ASSERT(Avv[i].size() == Avv[0].size());
	for (int j = 0; j < Avv[i].size(); ++j)
	  {
	    A(i,j) = Avv[i][j];
	  }
      }
    printMatlabForm(pmat,A,name,description);
  }
#endif // JLT_MATLAB_LIB_SUPPORT


//
// iostream versions
//

std::ostream& printMatlabForm(std::ostream& strm,
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
std::ostream& printMatlabForm(std::ostream& strm,
			      const std::string name,
			      const double var,
			      const std::string description = "")
  {
    return printMatlabForm(strm,var,name,description);
  }

// Overload for string output.  Too much code duplication for now.
std::ostream& printMatlabForm(std::ostream& strm,
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
    return printMatlabForm_nodefaults<T>(strm,A,name,description);
  }

template<typename T>
std::ostream& printMatlabForm_nodefaults(std::ostream& strm,
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
    for (int i = 0; i < (int)A.rows(); ++i) {
      for (int j = 0; j < (int)A.columns()-1; ++j)
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
    jlt::matrix<T> A(Avv.size(),Avv[0].size());

    /* TODO: this should be a constructor; check all rows have same size. */
    for (int i = 0; i < Avv.size(); ++i)
      {
	MATRIX_ASSERT(Avv[i].size() == Avv[0].size());
	for (int j = 0; j < Avv[i].size(); ++j)
	  {
	    A(i,j) = Avv[i][j];
	  }
      }
    return printMatlabForm(strm,A,name,description);
  }

} // namespace jlt

#endif // JLT_MATLAB_HPP
