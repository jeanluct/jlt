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

- [x] vector.hpp
- [ ] matrix.hpp
- [ ] mathvector.hpp
- [ ] mathmatrix.hpp
- [ ] polynomial.hpp
- [ ] eigensystem.hpp
- [ ] svdecomp.hpp
- [ ] matrixutil.hpp
- [ ] exceptions.hpp
