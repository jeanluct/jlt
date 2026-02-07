# Changelog

All notable changes to the jlt library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [Unreleased]

### Fixed
- **stlio.hpp**: Fixed formatting bug where library was overriding
  caller's stream format flags (fixed, scientific, precision) - now
  correctly preserves caller's formatting preferences (2026-02-06)

### Changed
- **stlio.hpp**: Removed deprecated `jlt::write_to` API and simplified
  printing helpers (2026-02-06)
- **stlio.hpp**: Replaced `JLT_FIELD_SEP_STRING` macro with
  version-guarded `field_sep` for C++17 compatibility

## [2026.1] - 2026-01-XX

### Added
- **stlio.hpp**: Added comprehensive tuple and optional support for C++11/14/17
- **stlio.hpp**: Added format_traits specializations for std::complex<T>
- **stlio.hpp**: Added input validation and input operators for vector, valarray, list
- **stlio.hpp**: Added support for std::array, std::deque, std::set, std::unordered_set
- **mathmatrix.hpp**: Added `frobenius_norm()` method
- **Tests**: Added comprehensive test suite using Catch2 v2.13.10 (446+ assertions)
  - test_stlio.cpp - STL container I/O (108 assertions)
  - test_reciprocal_polynomial.cpp - Reciprocal polynomials
  - test_bounds_checking.cpp - Bounds checking macros
  - test_display_task.cpp - Display utilities
  - test_vcs.cpp - Version control functions
  - test_command.cpp - Unix command execution
  - test_finitediff.cpp - Finite difference functions
  - test_matlab.cpp - Matlab I/O (text and binary)
- **Tests**: Added optional LAPACK-dependent tests (test_eigensystem, test_svdecomp)
- **Tests**: Added optional Matlab binary format tests (requires Matlab libraries)
- **Bounds checking**: Added compile-time bounds checking with
  `JLT_VECTOR_CHECK_BOUNDS` and `JLT_MATRIX_CHECK_BOUNDS` macros

### Changed
- **stlio.hpp**: Refactored format_traits to reduce code duplication using inheritance
- **stlio.hpp**: Refactored to use DRY principles and RAII pattern (stream_flags_saver)
- **stlio.hpp**: Moved implementation details to `detail` namespace
- **matlab.hpp**: Moved `printMatlabForm_nodefaults` to detail namespace
- **Naming**: Renamed `isSquare`, `isZero`, `isReducible` to snake_case for consistency
- **Modernization**: Added `constexpr` and `noexcept` qualifiers where appropriate
- **Modernization**: Replaced C-style casts with C++ static_cast/reinterpret_cast
- **mathvector.hpp**: Refactored cross product to single generic template

### Fixed
- **stlio.hpp**: Removed `[[nodiscard]]` from I/O operators to fix compiler warnings
- **mathmatrix.hpp**: Fixed dimension check bug in `invert()` method
- **mathmatrix.hpp**: Fixed S default value for complex numbers
- **math.hpp**: Fixed `mag2` for complex numbers and added proper type traits
- **csparse.hpp**: Fixed destructor bugs and removed thread-unsafe static variables
- **matrix.hpp**: Removed thread-unsafe static variables from operator[]
- **reciprocal_polynomial.hpp**: Fixed equality operator bug
- **CMakeLists.txt**: Fixed MATLAB/LAPACK variable name confusion

### Removed
- **stlio.hpp**: Removed deprecated `jlt::write_to` generic API
- Removed pre-C++11 conditional compilation blocks (library now requires C++11 minimum)
- Removed pre-C++11 support from csparse.hpp
- Removed Testing/ directory from git

## [Historical] - 2004-2025

The project was originally maintained in CVS (2004), migrated to
Subversion, then to Mercurial (2012), and finally to Git (2014). Over
250+ commits spanning 20+ years document the evolution of this
numerical library.

### 2020-2024: Modernization Era

#### 2023
- **Modernization**: Restored and updated `tictoc.hpp` to use modern Boost timer (issue #12)

#### 2022
- **Modernization**: Ran clang-tidy modernization passes
- **C++11**: Updated to modern C++ syntax (auto keyword, `>>` template closing)

#### 2020
- **Maintenance**: Improved exception handling
- **Deprecation**: Temporarily removed `tictoc.hpp` due to Boost
  compatibility issues (later restored in 2023)

### 2014-2019: Git Era and C++11 Adoption

#### 2016
- **Bug fix**: Fixed stlio.hpp field_sep linking issues (issue #11) -
  converted from string to numeric constant

#### 2015
- **Cleanup**: Migrated issue tracking to Bitbucket
- **Bug fixes**: Fixed `std::abs` usage in eigensystem

#### 2014 (December): Major Modernization Push
- **C++11 adoption**:
  - Added initializer list support for vector, matrix, mathvector, mathmatrix
  - Replaced `auto_ptr` with `unique_ptr`
  - Added move semantics and argument forwarding
- **Licensing**: Added MIT license and license headers to all files
- **Documentation**: Created README.md and comprehensive documentation
- **Repository**: Migrated from Mercurial to Git (tag: `hg-to-git`)
- **New utilities**:
  - `vcs.hpp` - Version control system integration (Git, Mercurial, SVN)
  - `command.hpp` - Capture output of system commands
  - `tictoc.hpp` - Timing utilities (renamed from time_mark)
- **Testing**: Added SCons build scripts for test suite
- **Cleanup**:
  - Removed obsolete `fixmatrix` and `fixvector`
  - Phased out obsolete math.hpp overloads
  - Removed old compiler workarounds (PGCC, PGI, KCC)
  - Standardized naming conventions (_test suffix)
- **Exception handling**: Converted from cerr/exit to proper exception throwing (issue #6)
  - Renamed `_THROW` macros to `JLT_THROW`

### 2012-2013: Mercurial Era and Matlab Improvements

#### 2013
- **Matlab**:
  - Added MAT-file format output support to matrix.hpp and vector.hpp
    (requires `JLT_MATLAB_SUPPORT`)
  - Updated MEX files to largeArrayDims API
  - Added `fft2udotgrad_helper.c` MEX improvements
  - Sparse matrix handling improvements
- **Maintenance**:
  - Added `empty()` method to matrix
  - Fixed memory leaks
  - Bug fixes for even N dimensions in FFT operations

#### 2012
- **Repository**: Migrated from Subversion to Mercurial (2012-12-31)

### 2009-2011: Polynomial Classes and Matlab Expansion

#### 2011
- **Compatibility**: GCC 4.5.2 compatibility fixes (explicit cstdio include)
- **Utilities**: Added `subversion.hpp` for SVN keyword extraction
- **Bug fixes**: Fixed ndims() never returning 1

#### 2010
- **Matlab**: Major improvements to `refine/refine2` functions
  - Downsampling support
  - Split into Fourier and real parts (refinek, refinek2)
  - Bug fixes for high modes in even dimensions
- **New features**: Added `interpline` for curve interpolation via point insertion

#### 2009
- **Polynomial classes**:
  - Added `reciprocal_polynomial.hpp` (from systole/ttauto project)
  - Added characteristic polynomial (`charpoly`) to mathmatrix
  - Made polynomial degree a runtime parameter instead of template parameter
  - Added odd degree support for reciprocal polynomials
  - Renamed `derive()` to `derivative()`
- **Utilities**:
  - Added `prompt.hpp` for user input
  - Added `printMatlabForm` for various types
- **Math**: Renamed `mag` to `mag2` to avoid confusion
- **Compatibility**: GCC 4.3.3 compatibility fixes
- **Exceptions**: Added `failed_to_converge` exception

### 2007-2008: LAPACK and Matrix Improvements

#### 2008
- **SVD**: Fixed serious bug where `dgesdd` returned negative singular values
  - Switched to `dgesvd` LAPACK routine
  - Restored optimal workspace query (worksize=-1)
- **Matlab**: Added extensive Matlab routines collection
- **Compatibility**: GCC 4.2.3 fixes (missing cassert include,
  std::vector::data() compatibility)
- **Bug fixes**: Fixed transpose type bug (leftover "double")

#### 2007
- **Major features**:
  - Added **CSparse integration** (`csparse.hpp`) for sparse matrix operations
  - Added `auto_ptr` wrapper for CSparse pointers (predecessor to unique_ptr)
  - Added `printMathematicaForm` output for matrix and vector
- **Matrix improvements**:
  - Renamed `dim1/dim2` to `rows/columns`
  - Renamed `bcvector` to `vector`
  - Added `data()` method to return base pointer
  - Added `spectral_radius()` function
  - Added `isReducible()` to detect reducible matrices
  - Added `operator==` comparison
  - Added `ones_and_zeros` utilities
- **Bug fixes**: Fixed memory leak, isReducible bug, const_iterator issues

### 2005-2006: LAPACK Integration

#### 2006
- **Cleanup**: Removed antiquated Numerical Recipes (nr.h, nrutil.h) includes

#### 2005
- **Major refactoring**:
  - Renamed "include" subdirectory to "jlt"
  - Removed all Numerical Recipes in C (NRC) dependencies
  - Pure LAPACK implementation
- **LAPACK wrappers**:
  - Added `matrix_eigenvalues` for real and complex matrices
  - Added `lapack.h` to encapsulate Fortran LAPACK declarations
  - Eigenvalues returned in descending order (consistent with SVD)
- **Utilities**: Added `time_mark` for execution timing marks

### 2004: Project Origins

#### 2004-10-09: **Initial CVS Import**
- **Core library established** (imported from CVS repository)
- **Initial components**:
  - vector and matrix classes (originally bcvector)
  - mathvector and mathmatrix with mathematical operations
  - Basic I/O support
- **Important early decision**: Removed all `using namespace`
  declarations from headers to avoid global scope pollution

### Version Control Timeline

```
2004-10-09: CVS (initial)
     ↓
2004-2012:  Subversion
     ↓
2012-12-31: Mercurial (tag: svn-to-hg)
     ↓
2014-12-12: Git (tag: hg-to-git)
     ↓
2004-2026:  254+ commits
```

### Major Architectural Decisions

1. **Header-only design** (with optional LAPACK/Matlab linking)
2. **Namespace discipline**: All code in `jlt::` namespace, never
   `using namespace` in headers
3. **LAPACK backend**: Moved from Numerical Recipes to pure LAPACK/BLAS (2005)
4. **C++11 requirement**: Dropped pre-C++11 support (2014-2026)
5. **MIT License**: Adopted in 2014
6. **Exception-based error handling**: Moved from cerr/exit to exceptions (2014)
7. **Smart pointers**: Evolved from raw pointers → auto_ptr → unique_ptr

### Historical Bug Fixes & Notable Issues

- **Issue #3**: `mag2` specialization for complex numbers
- **Issue #6**: Exception handling standardization (JLT_THROW macros)
- **Issue #7**: Phase out obsolete math.hpp overloads
- **Issue #11**: stlio field_sep linking problems (solved with numeric constant)
- **Issue #12**: tictoc/Boost timer compatibility issues (resolved in 2023)
- **SVD negative values bug** (2008): Critical fix switching from dgesdd to dgesvd
- **Thread safety**: Removed static variables from matrix operator[] and csparse

### Long-Term Collaborations & External Dependencies

- **LAPACK/BLAS**: Core dependency since 2005
- **CSparse** (Timothy A. Davis): Integration added 2007
- **Matlab**: I/O support added 2009-2013
- **Boost**: Timer library (tictoc.hpp)
- **Catch2**: Test framework adopted 2026

---

## Development

This library is a C++11 header-only library (with optional
LAPACK/Matlab dependencies) maintained by Jean-Luc Thiffeault.

For testing instructions, see `tests/README.md`.

For contribution guidelines, see `AGENTS.md`.
