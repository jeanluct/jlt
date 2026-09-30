# jlt lib

*jlt lib* is a C++ and Matlab library of various utility functions
that I found useful over the years, and which are used in many of my
other projects.

## Overview

**jlt lib** provides efficient implementations of common numerical operations including:
- Mathematical vectors and matrices with BLAS/LAPACK integration
- Sparse matrix support via CSparse
- Polynomial operations and finite difference methods
- I/O utilities (Matlab format, STL containers, version control info)
- Timing utilities with detailed performance metrics

## Quick Start

```cpp
#include "jlt/mathvector.hpp"
#include "jlt/mathmatrix.hpp"

jlt::mathvector<double, double> v(3);
v[0] = 1.0; v[1] = 2.0; v[2] = 3.0;

jlt::mathmatrix<double, double> A(3, 3);
// ... perform linear algebra operations
```

### Building

Header-only library - just add the parent directory to your include path:

```bash
g++ -std=c++11 -O3 -I/path/to/jlt/parent myprog.cpp -o myprog
```

For LAPACK functionality, add `-lblas -llapack`. See `AGENTS.md` for
detailed build instructions.

## Testing

Comprehensive test suite of **23 test suites** using Catch2 (ctest and
each Catch2 executable report the current pass/assertion counts):

```bash
cd tests && mkdir -p build && cd build
cmake .. && make
ctest                    # Run all tests
ctest -LE lapack         # Skip LAPACK tests (if not installed)
ctest -L lapack          # Run only LAPACK tests
```

Tests with optional dependencies carry CTest labels: `lapack`
(`test_lapack`, `test_eigensystem`, `test_svdecomp`), `matlab`
(`test_matlab_lib`), `csparse` (`test_csparse`), `boost` (`test_tictoc`)
and `bounds` (`test_bounds_checking`).  Use `ctest -L <label>` /
`ctest -LE <label>`; note that `-R`/`-E` match test *names*, not labels.

Test coverage includes:
- **Core library**: vector, matrix, mathvector, mathmatrix, polynomial,
  finite differences, Matlab text output, bounds checking, etc.
- **LAPACK integration**: LAPACK wrappers, eigensystems, SVD
  decomposition for real and complex matrices
- **Optional libraries**: Matlab MAT-file I/O, CSparse (built in-tree
  from `extern/CSparse/`), Boost timer

See `tests/README.md` for detailed testing documentation.

## Key Components

### Mathematical Types

#### `jlt::vector` - Enhanced std::vector
- Derived from `std::vector` with additional features
- Optional compile-time bounds checking (`-DJLT_VECTOR_CHECK_BOUNDS`)
- Matlab and Mathematica output via the free functions
  `jlt::printMatlabForm` (`jlt/matlab.hpp`) and
  `jlt::printMathematicaForm` (`jlt/mathematica.hpp`)

#### `jlt::matrix` - 2D Matrix Class
- Efficient row-major storage using `std::vector` (RAII)
- Move semantics (C++11)
- Optional bounds checking (`-DJLT_MATRIX_CHECK_BOUNDS`)
- Matlab and Mathematica output via `jlt::printMatlabForm` and
  `jlt::printMathematicaForm` (free functions, not members)
- Example: `examples/matlab_mathematica_test.cpp`

#### `jlt::mathvector` and `jlt::mathmatrix` - Mathematical Operations
- Full arithmetic operations (+, -, *, /, dot product, cross product)
- Integration with LAPACK for advanced operations
- Specialized support for eigenvalues/eigenvectors (`jlt/eigensystem.hpp`)
- LU and QR decomposition (`jlt/matrixutil.hpp`)
- Singular Value Decomposition for real and complex matrices (`jlt/svdecomp.hpp`)
- **Requires**: `-lblas -llapack` for linear algebra operations
- Examples: `examples/mathvector_test.cpp`, `examples/eigensystem_test.cpp`

### Sparse Matrices

#### `jlt/csparse.hpp` - CSparse Integration
- Wrappers for Timothy A. Davis's [CSparse](http://www.suitesparse.com) library
- Conversion to/from `jlt::mathmatrix`
- Type-safe `cs_unique_ptr` with automatic memory management
- CSparse v4.3.2 included in `extern/CSparse/` (the CMake test build
  compiles it in-tree automatically)
- **Requires**: `-lcsparse` or use included version
- Example: `examples/csparse_test.cpp`

### Polynomials

#### `jlt::polynomial` - General Polynomials
- Arithmetic operations, evaluation, derivatives
- Root finding and polynomial division
- Example: `examples/polynomial_test.cpp`

#### `jlt::reciprocal_polynomial` - Monic Reciprocal Polynomials
- Specialized class for polynomials satisfying P(x) = x^n * P(1/x)
- Efficient storage (only n/2 + 1 coefficients needed)
- Applications in dynamical systems and number theory

### Numerical Methods

#### `jlt/finitediff.hpp` - Finite Difference Calculations
- 1st through 4th order accurate schemes
- Forward, backward, and central differences
- Equal and unequal spacing support
- Error estimation for 2nd and 4th order methods

### I/O and Utilities

#### `jlt/matlab.hpp` - Matlab Integration
- `jlt::MatlabFile`: RAII file handle; `MatlabFile mf("data")` writes
  `data.mat` (binary) if `JLT_MATLAB_LIB_SUPPORT` is defined, otherwise
  `data.m` (text)
- `jlt::printMatlabForm(out, obj, name, description)`, where `out` is a
  `std::ostream` or a `MatlabFile`
- Works with scalars, vectors, matrices, and strings
- **Binary mode requires**: Matlab libraries (`-lmat -lmx -leng`) and
  `JLT_MATLAB_LIB_SUPPORT` defined
- **Text mode**: No external dependencies
- Example: `examples/matlab_mathematica_test.cpp`

#### `jlt/mathematica.hpp` - Mathematica Output
- `jlt::printMathematicaForm(strm, obj, name)` for vectors and matrices
- Example: `examples/matlab_mathematica_test.cpp`

#### `jlt/stlio.hpp` - STL Container I/O
- Simple iostream operators for STL containers
- Supports: vector, list, set, map, valarray, array, deque, tuple, optional
- Customizable formatting (field width, separators)
- Complex number support

#### `jlt::tictoc` - Performance Timing
- MATLAB-style `tic()`/`toc()` interface
- Reports wall-clock, user CPU, and system CPU times
- Output format: `0.5w 0.5u 0s  (1.2w 1.2u 0s)` (elapsed and cumulative)
- Programmatic API via `get_timing()` for accessing timing data
- **Requires**: Boost timer (`-lboost_timer -lboost_chrono`)
- Example: `examples/tictoc_test.cpp`

#### `jlt/vcs.hpp` - Version Control System Info
- Extract commit information from Git, Mercurial, or Subversion
- Detects uncommitted changes (appends '+' to revision)
- Useful for reproducible research and embedding build info
- Example: `examples/vcs_test.cpp`

#### `jlt/display_task.hpp` - Progress Display
- RAII-based task progress display with log levels
- Automatic timing and status reporting
- Mutable mode for quiet operation

### Error Handling

#### `jlt/exceptions.hpp` - Numerical Exceptions
- Specialized exceptions: `stepsize_too_small`, `failed_to_converge`, `too_many_steps`
- `JLT_THROW` macro for conditional exception compilation
- Used throughout library for robust error handling

### Other Components

- **`jlt/prompt.hpp`**: Terminal prompts for interactive programs
- **`jlt/command.hpp`**: Execute Unix commands and capture output
- **`jlt/math.hpp`**: Mathematical utilities (Mod, Sign functions)
- **`jlt/lapack.hpp`**: C++ wrappers for LAPACK Fortran functions
  (Fortran declarations in `jlt/internal/lapack_fortran.hpp`)
- **`jlt/freeword.hpp`**: Reduced words in a free group
- **`jlt/freeauto.hpp`**: Automorphisms acting on a free group

## Matlab Functions

The `matlab/` directory contains complementary Matlab functions, including:
- `fourdif.m`: Fourier differentiation matrix (by S. C. Reddy and J. A. C. Weideman)

## Documentation

See `AGENTS.md` for:
- Detailed build instructions and compiler flags
- Code style guidelines and conventions
- Test suite documentation
- Example compilation commands

## Project History

- **2004**: Imported from CVS (later moved to Subversion)
- **2012**: Migration from Subversion to Mercurial
- **2014**: Migration to Git
- **2020s**: Modernization (C++11, RAII, comprehensive test suite)
- **2026**: Catch2 test suite, complex matrix SVD, bounds checking, improved error handling

See `CHANGELOG.md` for detailed version history.

## Requirements

- **C++11 or later**
- **Optional dependencies**:
  - BLAS/LAPACK: For linear algebra operations
  - Boost timer: For `jlt::tictoc` timing utilities
  - Matlab libraries: For binary MAT-file I/O
  - CSparse: For sparse matrix operations (v4.3.2 included)

## Contributors

**jlt lib** was written and is maintained by [Jean-Luc
Thiffeault](http://www.math.wisc.edu/~jeanluc/).

## License

Released under the [MIT License](LICENSE).

Special acknowledgments:
- `fourdif.m` by S. C. Reddy and J. A. C. Weideman
- CSparse by Timothy A. Davis (included under GNU LGPL)

## Status

This library is actively maintained and used in multiple research
projects. While originally developed for personal use, it is stable
and well-tested. Contributions and bug reports are welcome.
