#include <string>
#include <jlt/matlab.hpp>	// include first
#include <jlt/mathmatrix.hpp>

// Write a mathmatrix and a scalar to a .mat file.

int main()
{
  using std::string;
  string matfile = "matlabtest.mat";	// MAT file name

  jlt::mathmatrix<double> M(2,2);
  M(0,0) = 1; M(0,1) = -1;
  M(1,0) = 2; M(1,2) = 3;

  // Open the Matlab file for writing.
  MATFile *pmat = 0;
  pmat = matOpen(matfile.c_str(), "wz");

  // Write matrix using member function.
  // "M" will be the Matlab workspace name.
  M.printMatlabForm(pmat,"M");

  // Write an number using function.
  // "a" will be the Matlab workspace name.
  double a = 1.23;
  jlt::printMatlabForm(pmat,"a",a);

  matClose(pmat);
}
