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
ctest -E lapack

# Run only LAPACK tests (requires LAPACK installed)
ctest -R lapack
```

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

### Core Tests (No External Dependencies)
All tests passing - 465 assertions total:

- [x] **vector.hpp** - 32 assertions covering construction, element access, STL compatibility, and type variations
- [x] **matrix.hpp** - 83 assertions covering construction, element access, assignment, iterators, row extraction, and move semantics
- [x] **mathvector.hpp** - 81 assertions covering mathematical operations, dot/cross products, magnitudes, and complex numbers
- [x] **mathmatrix.hpp** - 90+ assertions covering matrix operations, multiplication, inverse, determinant, trace, and identity operations
- [x] **polynomial.hpp** - 88 assertions covering construction, coefficient access, arithmetic, evaluation, differentiation, and I/O
- [x] **math.hpp** - 53 assertions covering Mod function (modulo with sign preservation) and Sign function
- [x] **matrixutil.hpp** - 52 assertions covering LU decomposition, QR decomposition, matrix inverse, Gram-Schmidt orthonormalization, and exception safety with RAII
- [x] **exceptions.hpp** - 43 assertions covering custom exception classes, throwing, catching, inheritance, and macros

### LAPACK-Dependent Tests (Optional)
These tests are only built if LAPACK is found on your system:

- [x] **eigensystem.hpp** - 20 assertions covering symmetric matrix eigensystem, real and complex eigenvalues
  - Tag: `[lapack][eigensystem]`
  - Requires: LAPACK/BLAS libraries
  
- [x] **svdecomp.hpp** - 40 assertions covering SVD decomposition (full and singular values only)
  - Tag: `[lapack][svd]`
  - Requires: LAPACK/BLAS libraries

**Note:** If LAPACK is not installed, these tests are automatically skipped during the CMake configuration phase.

### Test Results Summary
```
test_vector:        32 assertions - ALL PASSED
test_matrix:        83 assertions - ALL PASSED
test_mathvector:    81 assertions - ALL PASSED
test_mathmatrix:    90+ assertions - ALL PASSED
test_polynomial:    88 assertions - ALL PASSED
test_matrixutil:    52 assertions - ALL PASSED
test_exceptions:    43 assertions - ALL PASSED
test_math:          53 assertions - ALL PASSED
--------------------------------------------------
Core Total:        465 assertions - ALL PASSED

LAPACK Tests (if available):
test_eigensystem:   20 assertions - ALL PASSED
test_svdecomp:      40 assertions - ALL PASSED
--------------------------------------------------
Grand Total:       583+ assertions - ALL PASSED
```

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
5. If LAPACK-dependent, wrap in `if(LAPACK_FOUND)` block

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
