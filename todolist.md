# JLT Library Improvement Tasks

## Completed ✓

### Critical Bugs
- [x] Fix matrix.hpp row() method bug (line 251: +m → +n)
  - File: jlt/matrix.hpp
  - Date: 2026-02-03
  - Impact: row() now returns correct number of columns
  - Tests: All 64 matrix assertions passing

### Test Suite
- [x] Create comprehensive Catch2 test suite in tests/
  - [x] Set up Catch2 v2.13.10 single-header framework
  - [x] Write test_vector.cpp (32 assertions - all passing)
  - [x] Write test_matrix.cpp (64 assertions - all passing)
  - [x] Create CMakeLists.txt with ctest support
  - [x] Add README.md documenting test structure

## In Progress

### High Priority
- [ ] Write tests for mathvector.hpp
- [ ] Write tests for mathmatrix.hpp
- [ ] Write tests for polynomial.hpp
- [ ] Fix exception safety in assignment operators
- [ ] Replace manual memory management with RAII in matrix.hpp
- [ ] Replace manual memory management with RAII in mathmatrix.hpp

### Medium Priority
- [ ] Fix static variable thread-safety issues
- [ ] Add move semantics for performance
- [ ] Implement proper bounds checking throughout
- [ ] Write tests for eigensystem.hpp (requires LAPACK)
- [ ] Write tests for svdecomp.hpp (requires LAPACK)
- [ ] Write tests for matrixutil.hpp (LU/QR decomposition)
- [ ] Refactor code duplication in mathvector.hpp cross product
- [ ] Optimize matrix multiplication algorithms

### Low Priority
- [ ] Update to C++17 features (remove C++98 compatibility code)
- [ ] Add comprehensive documentation
- [ ] Consider header-only implementation
- [ ] Set up CI/CD pipeline for automated testing

## Known Issues (Documented)

### Memory Safety
- matrix.hpp: Raw pointer management without RAII
- mathmatrix.hpp: Manual array allocations (row_index, col arrays)
- matrixutil.hpp: Manual memory management in LU decomposition

### Exception Safety
- matrix.hpp: Assignment operator not exception-safe
- mathmatrix.hpp: LAPACK calls without exception protection
- matrixutil.hpp: Complex algorithm without proper cleanup

### Thread Safety
- matrix.hpp: Static variables in operator[] methods (not thread-safe)
- matrix.hpp: Mutable state without synchronization

### Performance
- mathmatrix.hpp: Naïve O(n³) matrix multiplication
- mathmatrix.hpp: Temporary matrix creation in arithmetic operations
- matrixutil.hpp: Inefficient classical Gram-Schmidt

## Future Improvements

### Code Quality
- Use `constexpr` and `noexcept` where appropriate
- Replace C-style casts with `static_cast`
- Add `[[nodiscard]]` attributes
- Remove pre-C++11 compatibility code

### Testing
- Increase code coverage to >90%
- Add edge case tests (empty matrices, single element, etc.)
- Add performance benchmarks
- Add fuzzing tests for numerical stability

### Documentation
- Add API documentation with examples
- Create migration guide from other libraries
- Document LAPACK/BLAS dependencies clearly
