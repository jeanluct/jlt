//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <string>
#include <jlt/matlab.hpp>	// include first
#include <jlt/mathmatrix.hpp>

// Write a mathmatrix and a mathvector in Matlab and Mathematica form.
// Write them and a scalar to a Matlab MAT file.

int main()
{
  using std::cout;
  using std::endl;

  jlt::mathmatrix<double> M(2,2);
  M(0,0) = 1; M(0,1) = -1;
  M(1,0) = 2; M(1,1) = 3;
  jlt::mathvector<double> v(2);
  v[0] = 0; v[1] = -1;

  // Write matrix using member function (Matlab format).
  // "M" will be the Matlab workspace name.
  cout << "Text form readable by Matlab:\n";
  M.printMatlabForm(cout,"M","Optional description for M");
  v.printMatlabForm(cout,"v","Optional description for v");

  cout << endl;

  // Write matrix using member function (Mathematica format).
  cout << "Text form readable by Mathematica:\n";
  M.printMathematicaForm(cout,"M","Optional description for M")
    << endl;  // for Mathematica form, newline is not included
  v.printMathematicaForm(cout,"v","Optional description for v")
    << endl;  // for Mathematica form, newline is not included

  // Open the Matlab file for writing.
  std::string matfile = "matlabtest.mat";	// MAT file name
  MATFile *pmat = 0;
  pmat = matOpen(matfile.c_str(), "wz");

  cout << "\nWrite to MAT file " << matfile << ":";

  // Write matrix using member function.

  // "M" will be the Matlab workspace name.
  M.printMatlabForm(pmat,"M");
  // "v" will be the Matlab workspace name.
  v.printMatlabForm(pmat,"v");
  // Output as a row vector.
  v.printMatlabForm(pmat,"v_row","","row");

  // Write with a description string.
  M.printMatlabForm(pmat,"M2","a matrix");
  v.printMatlabForm(pmat,"v2","a vector");

  // Write a number using function.
  // "b" will be the Matlab workspace name.
  double b = 1.23;
  jlt::printMatlabForm(pmat,b,"b");
  jlt::printMatlabForm(pmat,b,"b2","a constant");

  matClose(pmat);

  cout << " done\n";
}
