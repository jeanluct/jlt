# AGENTS.md - Guidelines for AI Coding Agents

## Project Overview

This is **jlt** - a C++ header-only library of numerical utility functions. All headers are in the `jlt/` directory and are included as `#include "jlt/<filename>.hpp"`.

## Build Commands

### Primary Testing (Catch2 + CMake)
The `tests/` directory contains the main test suite using Catch2 v2.13.10.

**IMPORTANT:** All test commands must be run from the `tests/build/` directory:

```bash
# Build all tests
cd tests && mkdir -p build && cd build && cmake .. && make

# Run all tests (from tests/build/ directory)
ctest

# Run only non-LAPACK tests (works on any system)
ctest -E lapack

# Run only LAPACK tests (requires LAPACK/BLAS)
ctest -R lapack

# Run a specific test executable (from tests/build/ directory)
./test_vector

# Run a single test case by tag
./test_vector "[vector]"

# Run LAPACK tests by tag
./test_eigensystem "[lapack]"
./test_svdecomp "[lapack]"

# Run a single test case by name
./test_vector "vector basic construction"

# Run with verbose output
./test_vector -s
```

**Note:** The test executables are located in `tests/build/` and will not be found if you run from the repository root or tests/ directory.

### Legacy Testing (SCons)
The `examples/` directory contains older tests using SCons build system.

```bash
# Build all examples programs
cd examples && scons

# Build specific program
cd examples && scons polynomial_test

# Run a test
./polynomial_test
```

### Manual Compilation
For quick compilation without build tools:

```bash
g++ -std=c++11 -O3 -Wall -I.. -o myprog myprog.cpp
```

## Lint / Format

No automated linter configured. Code must compile with:
- `-Wall` (enable all warnings)
- `-O3` (optimization level 3)
- `-ffast-math` (fast math operations)

## Whitespace Guidelines

**IMPORTANT:** Follow these whitespace rules for all files:

- **No trailing whitespace** - Remove all trailing spaces and tabs at end of lines
- **Files must end with a newline** - Last character of every file should be `\n`

To check for trailing whitespace:
```bash
git diff --check
```

To automatically remove trailing whitespace from a file:
```bash
sed -i 's/[[:space:]]*$//' filename
```

## Code Style Guidelines

### File Headers
Every file must include the standard copyright header:
```cpp
//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//
```

### Header Guards
Use `JLT_FILENAME_HPP` pattern:
```cpp
#ifndef JLT_VECTOR_HPP
#define JLT_VECTOR_HPP
// ... content ...
#endif // JLT_VECTOR_HPP
```

### Namespacing
Everything must be in the `jlt` namespace:
```cpp
namespace jlt {
// All classes, functions, templates here
} // namespace jlt
```

### Indentation & Formatting
- **Indentation**: 2 spaces (no tabs)
- **Braces**: Opening brace on its own line for functions/classes, same line for control structures
- **Line length**: ~80-100 characters preferred
- **Comments**: Use `//` for inline comments, `/* */` for multi-line documentation

### Naming Conventions
- **Classes**: `lowercase_with_underscores` (e.g., `mathmatrix`, `polynomial`)
- **Template parameters**: `T`, `S`, or descriptive names
- **Member variables**: Use trailing underscore only if necessary to avoid shadowing
- **Functions**: `lowercase_with_underscores` or `camelCase` (follow existing patterns)
- **Macros**: `JLT_ALL_CAPS` with JLT prefix
- **Constants**: `ALL_CAPS` or `constexpr` with appropriate casing

### Type Declarations
Use modern C++11+ type aliases:
```cpp
typedef typename std::vector<T>::size_type size_type;
using reference = typename std::vector<T>::reference;
```

### Imports & Includes
- Group includes: standard library first, then project headers
- Use angle brackets for standard library: `#include <iostream>`
- Use quotes for project headers: `#include "jlt/vector.hpp"`
- Include path is `..` (parent directory) to allow `#include "jlt/file.hpp"`

### Error Handling
Use the JLT exception macros:
```cpp
#include <jlt/exceptions.hpp>

// Throw exceptions using the macro
JLT_THROW(std::out_of_range("Error message"));

// Custom exception classes inherit from std::runtime_error
class my_exception : public std::runtime_error {
public:
  my_exception(const std::string& descr) : std::runtime_error(descr) {}
};
```

### Special Attributes
Use `[[nodiscard]]` for functions where the return value should not be ignored:
```cpp
[[nodiscard]] const_reference at(size_type i) const { ... }
```

### Template Classes
Use explicit template syntax with class keyword:
```cpp
template<class T>
class vector : public std::vector<T> {
public:
  using std::vector<T>::size;
  // ...
};
```

### STL Compatibility
Maintain compatibility with STL containers:
- Provide standard typedefs (`size_type`, `reference`, `const_reference`)
- Support range-based for loops via `begin()`/`end()`
- Implement standard member functions when applicable

### Testing with Catch2
Test files follow this structure:
```cpp
#include "catch.hpp"
#include "../jlt/component.hpp"

using namespace jlt;

TEST_CASE("descriptive test name", "[tag]") {
    SECTION("specific scenario") {
        REQUIRE(condition);
        REQUIRE_THROWS_AS(expression, exception_type);
    }
}
```

Use `Approx()` for floating-point comparisons:
```cpp
REQUIRE(result == Approx(expected_value));
```

## Dependencies

- **Core library**: Header-only, no dependencies
- **tests/**: Catch2 v2.13.10 (included as `catch.hpp`)
- **examples/**: May require LAPACK (`-lblas -llapack`), CSparse, or Matlab libraries
- **Optional**: Boost timer library for `tictoc.hpp`

## Compiler Requirements

- C++11 standard minimum (`-std=c++11`)
- GCC or Clang recommended
- Flags: `-Wall -O3 -ffast-math`

## Project Structure

```
jlt/
  *.hpp          # Library headers (header-only)
tests/
  catch.hpp      # Catch2 testing framework
  test_*.cpp     # Unit tests
  CMakeLists.txt # CMake configuration
examples/
  *_test.cpp     # Legacy test programs
  SConstruct     # SCons build file
  SConscript     # SCons configuration
matlab/
  *.m            # Matlab functions
```

## Important Notes

- This is a **header-only library** - no compilation step needed for the library itself
- The library is designed for **numerical/scientific computing**
- Many classes are templates supporting multiple numeric types
- Bounds checking can be enabled with compile-time flags (e.g., `VECTOR_CHECK_BOUNDS`)
- Some features require external libraries (LAPACK, CSparse, Matlab) - check individual headers
