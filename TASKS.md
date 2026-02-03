# JLT Library Improvement Tasks

## Completed ✓

### Documentation
- [x] Create AGENTS.md for AI coding agents
  - File: /home/jeanluc/C/jlt/AGENTS.md
  - Date: 2026-02-03
  - Content: Build/lint/test commands, code style guidelines, project structure
  - Lines: ~150

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
  - [x] Write test_mathvector.cpp (81 assertions - all passing)
  - [x] Write test_mathmatrix.cpp (90+ assertions - all passing)
  - [x] Write test_polynomial.cpp (88 assertions - all passing)
  - [x] Write test_matrixutil.cpp (47 assertions - all passing)
  - [x] Write test_exceptions.cpp (43 assertions - all passing)
  - [x] Create CMakeLists.txt with ctest support
  - [x] Add README.md documenting test structure

### LAPACK-dependent Tests (optional)
- [x] Write test_eigensystem.cpp (requires LAPACK)
  - Tests: symmetric_matrix_eigensystem, matrix_eigenvalues (real & complex)
  - File: tests/test_eigensystem.cpp
  - Tag: [lapack][eigensystem]
- [x] Write test_svdecomp.cpp (requires LAPACK)
  - Tests: SVdecomp (full and singular values only)
  - File: tests/test_svdecomp.cpp
  - Tag: [lapack][svd]
- [x] Update CMakeLists.txt with LAPACK detection
  - Uses `find_package(LAPACK)` - tests only built if LAPACK found
  - Tests tagged with "lapack" label for filtering

**Test Coverage: 506+ assertions across 9 test suites - ALL PASSING**
- 7 core test suites: 446 assertions (no external dependencies)
- 2 LAPACK test suites: 60 assertions (built conditionally if LAPACK found)

## In Progress

### High Priority
- [ ] Fix exception safety in assignment operators
- [ ] Replace manual memory management with RAII in matrix.hpp
- [ ] Replace manual memory management with RAII in mathmatrix.hpp

### Medium Priority
- [ ] Fix static variable thread-safety issues
- [ ] Add move semantics for performance
- [ ] Implement proper bounds checking throughout
- [ ] Refactor code duplication in mathvector.hpp cross product
- [ ] Optimize matrix multiplication algorithms
- [ ] Set up automated linting/formatting (clang-format, clang-tidy)

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
- Increase code coverage to >90% (currently: 7 test suites, 445+ assertions)
- Add edge case tests (empty matrices, single element, etc.)
- Add performance benchmarks
- Add fuzzing tests for numerical stability
- [x] Add integration tests for LAPACK-dependent components (eigensystem, svdecomp) - COMPLETED 2026-02-03

### Documentation
- Add API documentation with examples
- Create migration guide from other libraries
- Document LAPACK/BLAS dependencies clearly
