# jlt lib

*jlt lib* is a C++ and Matlab library of various utility functions that I found useful over the years, and which are used in many of my other projects.

### contributors

*jlt lib* was written and is maintained by [Jean-Luc Thiffeault][1].

### documentation

There isn't any.  In fact this library should really not be used by anyone, and I've mostly uploaded it so I can easily include it in other projects.  If you feel brave, many functions have comments documenting their use, and there are examples in the [testsuite][2] folder.

Here is a list of some of the more useful C++ functions and classes, in the folder [jlt][3].  They can be included as `#include "jlt/<filename>.hpp"`, so the folder containing `jlt` must be in the include files search path.  The programs in [testsuite][2] are readily compiled with the [SCONS][4] build tool, but unless linking to external libraries is required `g++ -O <prog>.cpp -o <prog> -I..` is usually sufficient.

* `jlt::vector` is derived from `std::vector`.  Bounds-checking can be turned on or off at compile time, and the vectors have a `printMatlabForm` member function to output to Matlab format (text or MAT file), and a `printMathematicaForm` to output in Mathematica text format.  See also `jlt/matlab.hpp` below.

* `jlt::matrix` is a matrix class for 2D data.  It is fairly efficient and implements similar output functions described for `jlt::vector` above.

* `jlt::mathvector` and `jlt::mathmatrix` implement vectors and matrices with mathematical operations.  Many operations can then be performed, such as eigenvalues and eigenvectors (in `jlt/eigensystem.hpp`), LU and QR decomposition (`jlt/matrixutil.hpp`), and SVD (`jlt/svdecomp.hpp`).  Many of these functions use LAPACK behind the scenes, so must be linked with `-lblas -llapack`.  See the testsuite programs `mathvector_test.cpp`, `eigensystem_test.cpp`, `qrdecomp_test.cpp`, and `svdecomp_test.cpp`.

* `jlt/csparse.hpp` provides wrappers for Timothy A. Davis's [CSparse][5] library, in particular conversion to and from `jlt::mathmatrix`, wrapping CSparse functions in a namespace `csparse`, and a type `jlt::cs_unique_ptr` derived from `std::unique_ptr` that deallocates pointers automatically.  Link with `-lcsparse`.  See the testsuite program `csparse_test.cpp`.

* `jlt/lapack.h` and `jlt/lapack.hpp` are wrappers for selected functions in the Fortran [LAPACK][6] libraries.  Link with `-lblas -llapack`.
* `jlt/matlab.hpp` provides `printMatlabForm` for exporting variables in Matlab MAT-file format.  Some of this functionality is provided in-class by `jlt::matrix` and `jlt::vector` as well, and is compiled in if `jlt/matlab.hpp` is included.  See the testsuite program `matlab_test.cpp`, which writes a `mathmatrix` to a MAT file.

* `jlt/stlio.hpp` defines simple iostream printing for some STL containers.

* `jlt::polynomial` is a polynomial class.  See the testsuite program `polynomial_test.cpp`.

* `jlt::reciprocal_polynomial` is specialized for monic reciprocal polynomials.

* `jlt/prompt.hpp` is helpful for quick-and-dirty terminal prompts.

* `jlt/exceptions.hpp` provides some exceptions tailored to numerical problems, such as `stepsize_too_small`, `failed_to_converge`, `too_many_steps`.  It also defines macros such as `JLT_THROW`, etc, which can be selectively redefined to compile out exception testing.  (In the old days exceptions were a much bigger performance hit than they are now, I suspect, so it made sense to test a code and then remove exceptions completely.)

* `jlt::tictoc` works a bit like Matlab's `tic` and `toc` to time programs.  It uses `boost/timer/timer.hpp`.  See the testsuite program `tictoc_test.cpp`.

* `jlt/vcs.hpp` provides functions for extracting commit info from Version Control Systems, using Subversion [keyword strings][7] or dynamically in the case of Mercurial and Git.  See `vcs_test.cpp` for an example.  Note that this is a bit fragile: if you run a command from outside the repo then the version information won't be detected.  In that case `printVCSBanner` simply does nothing.

* Other `.hpp` files you might find in there are a bit dodgy and maybe haven't been tested in a while.

The folder [matlab][8] contains Matlab functions.

### license

*jlt lib* is released under the [MIT License][9].  The Fourier differentiation matrix function [fourdif.m][10] was written by S. C. Reddy
and J. A. C. Weideman.

[1]: http://www.math.wisc.edu/~jeanluc/
[2]: https://github.com/jeanluct/jlt/tree/master/testsuite
[3]: https://github.com/jeanluct/jlt/tree/master/jlt
[4]: http://www.scons.org
[5]: http://www.suitesparse.com
[6]: http://www.netlib.org/lapack/
[7]: http://svnbook.red-bean.com/en/1.4/svn.advanced.props.special.keywords.html
[8]: https://github.com/jeanluct/jlt/tree/master/matlab
[9]: https://github.com/jeanluct/jlt/raw/master/LICENSE
[10]: http://appliedmaths.sun.ac.za/~weideman/research/differ.html

[![Analytics](https://ga-beacon.appspot.com/UA-58116885-1/jlt/readme)](https://github.com/igrigorik/ga-beacon)
