# JLT Library Tests

This directory contains the comprehensive test suite for the jlt library using Catch2.

## Structure

- `catch.hpp` - Catch2 v2.13.10 single-header test framework
- `CMakeLists.txt` - CMake build configuration
- `test_*.cpp` - Test files for each component

## Running Tests

### Using CMake (recommended):
```bash
mkdir -p build && cd build
cmake ..
make
ctest
```

### Using g++ directly:
```bash
g++ -std=c++11 -I.. test_vector.cpp -o test_vector
g++ -std=c++11 -I.. test_matrix.cpp -o test_matrix
```

## Test Coverage

### Complete (All Tests Passing)
- [x] **vector.hpp** - 32 assertions covering construction, element access, STL compatibility, and type variations
- [x] **matrix.hpp** - 64 assertions covering construction, element access, assignment, iterators, and row extraction

### Planned (TODO)
- [ ] mathvector.hpp
- [ ] mathmatrix.hpp  
- [ ] polynomial.hpp
- [ ] eigensystem.hpp
- [ ] svdecomp.hpp
- [ ] matrixutil.hpp
- [ ] exceptions.hpp

### Test Results
```
test_vector:  32 assertions - ALL PASSED
test_matrix:  64 assertions - ALL PASSED
```
