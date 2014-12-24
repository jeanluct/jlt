# jlt lib

*jlt lib* is a C++ and Matlab library of various utility functions that I found useful over the years, and which are used in many of my other projects.

### contributors

*jlt lib* was written and is maintained by [Jean-Luc Thiffeault][1].

### license

*jlt lib* is released under the [MIT License][2].

### documentation

There isn't any.  In fact this library should really not be used by anyone, and I've mostly uploaded it so I can easily include it in other projects.  If you feel brave, many functions have comments documenting their use, and there are examples in the [testsuite][3] folder.

Here are is a list of some of the more useful C++ functions and classes, in the folder [jlt][4].  They are included as `#include "jlt/matrix.hpp"`, so the folder containing `jlt` must be in the include files search path.

* `jlt::vector` is a version of `std::vector` where bounds-checking can be turned on or off at compile time.
* `jlt::matrix` is a matrix class for 2D data.
* `jlt::mathvector` and `jlt::mathmatrix` implement vectors and matrices with mathematical operations.  Many operations can then be performed, such as eigenvalues and eigenvectors (in `eigensystem.hpp`), LU and QR decomposition (`matrixutil.hpp`), and SVD (`svdecomp.hpp`).  Many of these use LAPACK behind the scences, so must be linked with `-lblas -llapack`.
* `csparse.hpp` provides wrappers for Timothy A. Davis's [CSparse][5] library, in particular conversion to and from `jlt::mathmatrix` and wrapping in a namespace.  Link with `-lcsparse`.
* `lapack.h` and `lapack.hpp` are wrappers for the Fortran [LAPACK][6] libraries.  Link with `-lblas -llapack`.
* `matlab.hpp` provides `printMatlabForm` for exporting variables in Matlab mat-file format.  Some of this functionality is provided in-class by `jlt::matrix` and `jlt::vector` as well, and is compiled in if `matlab.hpp` is included.
* `stlio.hpp` defines printing for some STL containers.
* `jlt::polynomial` is a polynomial class.
* `jlt::reciprocal_polynomial` is specialized for monic reciprocal polynomials.
* `prompt.hpp` is helpful for quick-and-dirty terminal prompts.
* `jlt::time_mark` works a bit like Matlab's `tic` and `toc` to time programs.  It uses `boost/timer.hpp`.
* `exceptions.hpp` provides some exceptions tailored to numerical problems, such as `stepsize_too_small`, `failed_to_converge`, `too_many_steps`.  It also defines macros such as `JLT_THROW`, etc, which can be selectively redefined to compile out exception testing.  (In the old days exceptions were a much bigger performance hit than they are now, I suspect, so it made sense to test a code and then remove exceptions completely.)
* `subversion.hpp` provides functions for extracting data from Subversion [keyword strings][7].  I don't use subversion anymore, so I won't be updating this.
* Other `.hpp` files you might find in there are a bit dodgy and maybe haven't been tested in a while.

The folder [matlab][8] contains Matlab functions.

[1]: http://www.math.wisc.edu/~jeanluc/
[2]: https://github.com/jeanluct/jlt/raw/master/LICENSE
[3]: https://github.com/jeanluct/jlt/tree/master/testsuite
[4]: https://github.com/jeanluct/jlt/tree/master/jlt
[5]: http://www.suitesparse.com
[6]: http://www.netlib.org/lapack/
[7]: http://svnbook.red-bean.com/en/1.4/svn.advanced.props.special.keywords.html
[8]: https://github.com/jeanluct/jlt/tree/master/matlab
