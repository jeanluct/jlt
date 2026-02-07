//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include <string>
#include <jlt/matlab.hpp>
#include <jlt/mathematica.hpp>
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

  // Write matrix using standalone function (Matlab format).
  // "M" will be the Matlab workspace name.
  cout << "Text form readable by Matlab:\n";
  jlt::printMatlabForm(cout, M, "M", "Optional description for M");
  jlt::printMatlabForm(cout, v, "v", "Optional description for v");

  cout << endl;

  // Write matrix using standalone function (Mathematica format).
  cout << "Text form readable by Mathematica:\n";
  jlt::printMathematicaForm(cout, M, "M", "Optional description for M")
    << endl;  // for Mathematica form, newline is not included
  jlt::printMathematicaForm(cout, v, "v", "Optional description for v")
    << endl;  // for Mathematica form, newline is not included

  // Write to Matlab file using the unified MatlabFile interface.
  // No #ifdef needed - automatically creates .mat or .m based on compilation.
  cout << "\nWrite to Matlab file using MatlabFile:\n";

  {
    jlt::MatlabFile mf("matlabtest");
    cout << "Created: " << mf.getFilename() << endl;

    // Set high precision for text output (no-op in binary mode)
    mf.setHighPrecision();

    // Write using standalone printMatlabForm functions
    jlt::printMatlabForm(mf, M, "M");
    jlt::printMatlabForm(mf, v, "v");

    // Write with a description string.
    jlt::printMatlabForm(mf, M, "M2", "a matrix");
    jlt::printMatlabForm(mf, v, "v2", "a vector");

    // Write a number
    double b = 1.23;
    jlt::printMatlabForm(mf, b, "b");
    jlt::printMatlabForm(mf, b, "b2", "a constant");

    // File automatically closed by destructor
  }

  cout << "done (file auto-closed)\n";
}
