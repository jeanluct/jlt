# jlt lib

*jlt lib* is a C++ and Matlab library of various utility functions that I found useful over the years, and which are used in many of my other projects.

### contributors

*jlt lib* was written and is maintained by [Jean-Luc Thiffeault][1].

### license

*jlt lib* is released under the [MIT License][2].

### documentation

There isn't any.  In fact this library should really not be used by anyone, and I've mostly uploaded it so I can easily include it in other projects.  If you feel brave, many functions have comments documenting their use, and there are examples in the [testsuite][3] folder.

Here is a list of some of the more useful C++ functions and classes, in the folder [jlt][4].  They can be included as `#include "jlt/<filename>.hpp"`, so the folder containing `jlt` must be in the include files search path.  The programs in [testsuite][3] are readily compiled with the [SCONS][5] build tool.

* `jlt::vector` is derived from `std::vector`.  Bounds-checking can be turned on or off at compile time, and the vectors have a `printMatlabForm` member function to output to Matlab format (text or MAT file), and a `printMathematicaForm` to output in Mathematica text format.  See also `matlab.hpp` below.
* `jlt::matrix` is a matrix class for 2D data.
* `jlt::mathvector` and `jlt::mathmatrix` implement vectors and matrices with mathematical operations.  Many operations can then be performed, such as eigenvalues and eigenvectors (in `eigensystem.hpp`), LU and QR decomposition (`matrixutil.hpp`), and SVD (`svdecomp.hpp`).  Many of these use LAPACK behind the scences, so must be linked with `-lblas -llapack`.  See the testuite programs `mathvector.cpp`, `eigensystem.cpp`, `qrtest.cpp`, and `svdtest.cpp`.
* `csparse.hpp` provides wrappers for Timothy A. Davis's [CSparse][6] library, in particular conversion to and from `jlt::mathmatrix`, wrapping CSparse functions in a namespace, and a type `jlt::cs_auto_ptr` derived from `std::auto_ptr` that deallocates pointers automatically.  Link with `-lcsparse`.  See the testsuite program ``csparsetest.cpp`.
* `lapack.h` and `lapack.hpp` are wrappers for the Fortran [LAPACK][7] libraries.  Link with `-lblas -llapack`.
* `matlab.hpp` provides `printMatlabForm` for exporting variables in Matlab MAT-file format.  Some of this functionality is provided in-class by `jlt::matrix` and `jlt::vector` as well, and is compiled in if `matlab.hpp` is included.  See the testsuite program `matlabtest.cpp`, which write a `mathmatrix` to a MAT file.
* `stlio.hpp` defines printing for some STL containers.
* `jlt::polynomial` is a polynomial class.  See the testsuite program `polytest.cpp`.
* `jlt::reciprocal_polynomial` is specialized for monic reciprocal polynomials.
* `prompt.hpp` is helpful for quick-and-dirty terminal prompts.
* `jlt::time_mark` works a bit like Matlab's `tic` and `toc` to time programs.  It uses `boost/timer.hpp`.  See the testsuite program `time_mark.cpp`.
* `exceptions.hpp` provides some exceptions tailored to numerical problems, such as `stepsize_too_small`, `failed_to_converge`, `too_many_steps`.  It also defines macros such as `JLT_THROW`, etc, which can be selectively redefined to compile out exception testing.  (In the old days exceptions were a much bigger performance hit than they are now, I suspect, so it made sense to test a code and then remove exceptions completely.)
* `vcs.hpp` provides functions for extracting commit info from Version Control Systems, using Subversion [keyword strings][8] or dynamically in the case of Mercurial and Git.  See `vcs_banner.cpp` for an example.  Note that this is a bit fragile: if you run a command from outside the repo then the version information won't be detected.  In that case `printVCSBanner` simply does nothing.
* Other `.hpp` files you might find in there are a bit dodgy and maybe haven't been tested in a while.

The folder [matlab][9] contains Matlab functions.

[1]: http://www.math.wisc.edu/~jeanluc/
[2]: https://github.com/jeanluct/jlt/raw/master/LICENSE
[3]: https://github.com/jeanluct/jlt/tree/master/testsuite
[4]: https://github.com/jeanluct/jlt/tree/master/jlt
[5]: http://www.scons.org
[6]: http://www.suitesparse.com
[7]: http://www.netlib.org/lapack/
[8]: http://svnbook.red-bean.com/en/1.4/svn.advanced.props.special.keywords.html
[9]: https://github.com/jeanluct/jlt/tree/master/matlab
