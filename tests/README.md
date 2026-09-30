# JLT Library Tests

This directory contains the comprehensive test suite for the jlt library using Catch2 v2.13.10.

## Structure

- `catch.hpp` - Catch2 v2.13.10 single-header test framework
- `CMakeLists.txt` - CMake build configuration
- `test_*.cpp` - Test files for each component
- `catch_main.cpp` - Main entry point for Catch2

## Running Tests

### Using CMake (recommended):

```bash
# Build all tests
cd tests && mkdir -p build && cd build && cmake .. && make

# Run all tests
ctest

# Run only non-LAPACK tests (works on any system)
ctest -LE lapack

# Run only LAPACK tests (requires LAPACK installed)
ctest -L lapack

# Build with maximal warnings, optionally as errors (both OFF by default)
cmake .. -DJLT_TESTS_MAX_WARNINGS=ON -DJLT_TESTS_WARNINGS_AS_ERRORS=ON
```

### CTest labels

`ctest -R`/`-E` match test *names*; to select tests by dependency use
the labels with `ctest -L <label>` / `ctest -LE <label>`:

| Label     | Tests                                              |
|-----------|----------------------------------------------------|
| `lapack`  | `test_lapack`, `test_eigensystem`, `test_svdecomp` |
| `matlab`  | `test_matlab_lib`                                  |
| `csparse` | `test_csparse`                                     |
| `boost`   | `test_tictoc`                                      |
| `bounds`  | `test_bounds_checking`                             |

### CMake options

- `JLT_TESTS_MAX_WARNINGS` (default `OFF`) - add `-Wextra`, `-Wpedantic`,
  `-Wconversion`, `-Wshadow` and many more (GCC/Clang).  `-Wall` is
  always on.
- `JLT_TESTS_WARNINGS_AS_ERRORS` (default `OFF`) - add `-Werror`.

### Running Individual Test Executables:

```bash
# Run a specific test executable
./test_vector

# Run a single test case by tag
./test_vector "[vector]"

# Run LAPACK tests by tag
./test_eigensystem "[lapack]"
./test_svdecomp "[lapack]"

# Run with verbose output
./test_vector -s
```

### Using g++ directly (no LAPACK):

```bash
g++ -std=c++11 -I.. test_vector.cpp catch_main.cpp -o test_vector
./test_vector
```

## Test Coverage

Assertion counts change as tests are added, so they are not listed here;
`ctest` reports pass/fail per suite, and each executable prints its own
Catch2 summary (e.g. `./test_vector` ends with
`All tests passed (N assertions in M test cases)`).

### Core Tests (No External Dependencies)

- [x] **vector.hpp** (`test_vector`) - construction, element access, STL compatibility, and type variations
- [x] **matrix.hpp** (`test_matrix`) - construction, element access, assignment, iterators, row extraction, and move semantics
- [x] **matrix.hpp** (`test_matrix_transpose`) - transpose of square and non-square matrices
- [x] **mathvector.hpp** (`test_mathvector`) - mathematical operations, dot/cross products, magnitudes, and complex numbers
- [x] **mathmatrix.hpp** (`test_mathmatrix`) - matrix operations, multiplication, inverse, determinant, trace, and identity operations
- [x] **polynomial.hpp** (`test_polynomial`) - construction, coefficient access, arithmetic, evaluation, differentiation, and I/O
- [x] **reciprocal_polynomial.hpp** (`test_reciprocal_polynomial`) - construction, coefficient access, evaluation, derivative, and symmetry properties
- [x] **stlio.hpp** (`test_stlio`) - STL container output formatting and input
- [x] **display_task.hpp** (`test_display_task`) - task display with begin/end, log levels, scoped tasks, and output formatting
- [x] **vcs.hpp** (`test_vcs`) - Git/Mercurial detection, SVN keyword extraction, VCS revision/date extraction
- [x] **command.hpp** (`test_command`) - Unix command execution and output capture
- [x] **math.hpp** (`test_math`) - Mod function (modulo with sign preservation) and Sign function
- [x] **matrixutil.hpp** (`test_matrixutil`) - LU decomposition, QR decomposition, matrix inverse, Gram-Schmidt orthonormalization, and exception safety with RAII
- [x] **exceptions.hpp** (`test_exceptions`) - custom exception classes, throwing, catching, inheritance, and macros
- [x] **finitediff.hpp** (`test_finitediff`) - finite difference schemes
- [x] **prompt.hpp** (`test_prompt`) - `read_number` reads exactly one line per prompt, including when only one value is valid
- [x] **matlab.hpp** (`test_matlab`) - `printMatlabForm` and `MatlabFile` in text (`.m`) mode
- [x] **vector.hpp/matrix.hpp** (`test_bounds_checking`) - out-of-range accesses throw when `JLT_VECTOR_CHECK_BOUNDS`/`JLT_MATRIX_CHECK_BOUNDS` are defined (CMake defines them for this executable only)
  - Label: `bounds`

**Note:** `test_vcs` expects to run inside a Git work tree, so it fails
if the build directory is outside the repository (use `tests/build/`).

### Optional-Dependency Tests
These tests are only built if the dependency is found during CMake
configuration; otherwise they are skipped automatically.

- [x] **lapack.hpp** (`test_lapack`) - LAPACK wrapper overload resolution and basic functionality
  - Label: `lapack`; requires LAPACK/BLAS libraries
- [x] **eigensystem.hpp** (`test_eigensystem`) - symmetric matrix eigensystem, real and complex eigenvalues
  - Tag: `[lapack][eigensystem]`; label: `lapack`
- [x] **svdecomp.hpp** (`test_svdecomp`) - SVD decomposition of real and complex matrices (full and singular values only)
  - Tag: `[lapack][svd]`; label: `lapack`
- [x] **matlab.hpp** (`test_matlab_lib`) - binary MAT-file output with `JLT_MATLAB_LIB_SUPPORT`
  - Label: `matlab`; requires Matlab `mat`/`mx` libraries under `/usr/local/MATLAB` or `/opt/MATLAB`
- [x] **csparse.hpp** (`test_csparse`) - CSparse wrappers
  - Label: `csparse`; CSparse is always built in-tree from `extern/CSparse/`
- [x] **tictoc.hpp** (`test_tictoc`) - timing utilities
  - Label: `boost`; requires Boost timer (and chrono)

`freeword.hpp` and `freeauto.hpp` have no tests yet.

## Installing LAPACK (Optional)

### Ubuntu/Debian:
```bash
sudo apt-get install liblapack-dev libblas-dev
```

### macOS:
```bash
brew install lapack
```

### Verify LAPACK is installed:
```bash
ldconfig -p | grep lapack
```

## Test Organization

Tests are organized by component and use Catch2's BDD-style syntax:

- **TEST_CASE**: Groups related test scenarios (e.g., "matrix basic construction")
- **SECTION**: Specific test scenarios within a case (e.g., "default construction")
- **Tags**: Used to categorize tests (e.g., `[vector]`, `[matrix]`, `[lapack]`)

### Common Tags:
- `[vector]` - Vector container tests
- `[matrix]` - Matrix container tests
- `[math]` - Mathematical operations
- `[lapack]` - LAPACK-dependent numerical routines
- `[eigensystem]` - Eigenvalue/eigenvector tests
- `[svd]` - Singular value decomposition tests

## Adding New Tests

To add tests for a new component:

1. Create `test_<component>.cpp`
2. Include `"catch.hpp"` and the component header
3. Add `TEST_CASE` blocks with descriptive names and tags
4. Update `CMakeLists.txt` to add the new test executable
5. If LAPACK-dependent, wrap in `if(LAPACK_FOUND)` block and give it
   the `lapack` label (`set_tests_properties(... PROPERTIES LABELS "lapack")`)
6. Link against `jlt_test_warnings` so the warning options apply

Example:
```cpp
#include "catch.hpp"
#include "../jlt/mycomponent.hpp"

using namespace jlt;

TEST_CASE("mycomponent functionality", "[mycomponent]") {
    SECTION("basic operation") {
        mycomponent<double> c;
        REQUIRE(c.size() == 0);
    }
}
```
