//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_MATLAB_HPP
#define JLT_MATLAB_HPP

#include <string>


namespace jlt {

// TODO: add the iostream versions (no need to have
// JLT_MATLAB_LIB_SUPPORT defined).

// TODO: more objects.  Move matrix.hpp and vector.hpp output to here.

#ifdef JLT_MATLAB_LIB_SUPPORT

#include "mat.h"

void printMatlabForm(MATFile *pmat,
		     const double var,
		     const std::string name,
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

#else // JLT_MATLAB_LIB_SUPPORT

#include <iostream>

// iostream versions (no need to have JLT_MATLAB_LIB_SUPPORT defined).

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
  if (!name.empty()) strm << name << " = ";
  strm << str << ";\n";

  return strm;
}

#endif // JLT_MATLAB_LIB_SUPPORT

} // namespace jlt

#endif // JLT_MATLAB_HPP
