# JLT Library Improvement Tasks

## Completed ✓

### Documentation
- [x] Create AGENTS.md for AI coding agents
  - File: /home/jeanluc/C/jlt/AGENTS.md
  - Date: 2026-02-03
  - Content: Build/lint/test commands, code style guidelines, project structure
  - Lines: ~150
- [x] Create CHANGELOG.md documenting project history
  - File: /home/jeanluc/C/jlt/CHANGELOG.md
  - Date: 2026-02-06
  - Content: Comprehensive 22-year history (2004-2026), version control migrations, architectural decisions
  - Lines: 279
  - Covers: Recent improvements (2026.1), historical changes by era, bug fixes, external dependencies

### Critical Bugs
- [x] Fix matrix.hpp row() method bug (line 251: +m → +n)
  - File: jlt/matrix.hpp
  - Date: 2026-02-03
  - Impact: row() now returns correct number of columns
  - Tests: All 64 matrix assertions passing

### Code Quality Improvements
- [x] Refactor matrix.hpp to use RAII (std::vector) instead of raw pointers
  - Changed: `pointer start, finish` → `std::vector<T> storage`
  - Eliminated: Manual `new[]`/`delete[]` calls
  - Fixed: Assignment operator now uses copy-and-swap idiom (exception-safe)
  - Added: Move constructor and move assignment operator (C++11)
  - Fixed: Member initialization order bug (was using m/n before they were initialized)
  - Impact: Eliminates memory leaks, improves exception safety, enables move semantics
  - Tests: All 64 matrix assertions still passing
  - Date: 2026-02-03

- [x] Refactor mathmatrix.hpp to use RAII (std::vector) instead of raw pointers
  - Changed: `int* row_index = new int[n]` → `std::vector<int> row_index(n)`
  - Changed: `T* col = new T[n]` → `std::vector<T> col(n)`
  - Fixed: Memory leak in `det()` function (was missing `delete[] row_index`)
  - Eliminated: All manual `new[]`/`delete[]` calls (5 occurrences)
  - Functions updated: `invert()`, `invert(Ainv)`, `inverse()`, `inverse(Ainv)`, `det()`
  - Impact: Eliminates memory leaks, exception-safe, no manual cleanup needed
  - Tests: All 131 mathmatrix assertions still passing
  - Date: 2026-02-03

- [x] Refactor matrixutil.hpp to fix exception safety issues
  - Fixed: `LUdecomp()` - `T* vv = new T[n]` → `std::vector<T> vv(n)` (was leaking if singular matrix exception thrown)
  - Fixed: `inverse()` - `int* row_index` and `T* col` → `std::vector` (exception-safe)
  - Impact: Exception-safe, automatic cleanup even if exceptions thrown during LU decomposition
  - Tests: All 47 matrixutil assertions still passing
  - Date: 2026-02-03

- [x] Remove redundant at() implementation from vector.hpp
  - Removed: 18 lines of custom at() that duplicated std::vector::at()
  - Changed: operator[] now directly calls std::vector<T>::at() when JLT_VECTOR_CHECK_BOUNDS enabled
  - Benefits: Better error messages (includes indices and size), less code duplication, more maintainable
  - Example error: "vector::_M_range_check: __n (which is 5) >= this->size() (which is 3)"
  - Tests: All 32 vector assertions and 39 bounds checking assertions passing
  - Date: 2026-02-06

- [x] Fix mag2_traits for nested mathvectors
  - Added: Specialization for mathvector<T,S> that recursively calls mag2()
  - Fixed: Compile error in mathvector_test.cpp when using nested vectors
  - Impact: Allows mag2() to work with arbitrarily nested mathvectors
  - Test case: mathvector<mathvector<double>, double> now compiles and works correctly
  - Date: 2026-02-06

- [x] Fix stlio.hpp formatting bug
  - Fixed: Library was overriding caller's stream format flags (fixed, scientific, precision)
  - Removed: strm.setf() calls in print_sequence() that modified caller's flags
  - Added: Comprehensive flag mirroring (flags, precision, fill, locale) in print_element()
  - Impact: Caller's formatting preferences now preserved correctly
  - Tests: All 108 stlio assertions passing
  - Date: 2026-02-06

- [x] Add uncommitted changes indicator to vcs.hpp
  - Added: hasVCSChanges() function to detect uncommitted changes in Git/Hg repos
  - Changed: getVCSRevision() now appends "+" when there are uncommitted changes
  - Benefits: Users can immediately see if their build includes uncommitted changes
  - Example: "rev 53f13c3+ (2026-02-06)" indicates uncommitted changes present
  - Tests: Updated test_vcs.cpp to handle 7 or 8 character revisions (32 assertions passing)
  - Date: 2026-02-06

### Test Suite
- [x] Create comprehensive Catch2 test suite in tests/
  - [x] Set up Catch2 v2.13.10 single-header framework
  - [x] Write test_vector.cpp (32 assertions - all passing)
  - [x] Write test_matrix.cpp (83 assertions - all passing)
    - Added: Move semantics tests (move constructor, move assignment, self-move)
    - Tests verify C++11 move operations work correctly with std::vector storage
  - [x] Write test_mathvector.cpp (124 assertions - all passing)
    - Added: Division by zero behavior tests (inf/NaN handling)
    - Added: Size mismatch documentation tests
    - Added: Vector normalization and unit vector tests
  - [x] Write test_mathmatrix.cpp (206 assertions - all passing)
    - Added: Matrix-vector multiplication tests (identity, zero, diagonal matrices)
    - Added: Invalid operation documentation tests (size mismatch)
    - Added: Mathematical identity tests (transpose, distributive, associative properties)
  - [x] Write test_polynomial.cpp (88 assertions - all passing)
  - [x] Write test_matrixutil.cpp (52 assertions - all passing)
  - Added: Exception safety tests for RAII improvements (5 new assertions)
  - Tests verify that std::vector properly cleans up when exceptions thrown in LUdecomp and inverse
- [x] Write test_math.cpp (53 assertions - all passing)
  - Tests Mod function: basic ops, negative values, different precisions (float/double/long double)
  - Tests Sign function: positive, negative, zero values, edge cases
  - Tests combined operations
- [x] Write test_command.cpp (24 assertions - all passing)
  - Tests Unix command execution via get_command_output()
  - Tests output capture, empty output, long output, special characters
  - Tests stderr redirection (2>&1)
- [x] Write test_vcs.cpp (29 assertions - all passing)
  - Tests SVN keyword extraction (revision and date)
  - Tests Git repository detection
  - Tests VCS revision/date extraction and banner output
- [x] Write test_display_task.cpp (43 assertions - all passing)
  - Tests display_task construction, configuration, begin/end
  - Tests log levels (error, warn, info, debug)
  - Tests scoped_task RAII guard
  - Tests muted behavior and custom status
- [x] Write test_stlio.cpp (46 assertions - all passing)
  - Tests STL container output (vector, valarray, list, map)
  - Tests formatted output with field width and separators
  - Tests scientific notation handling
  - Tests input operator for vectors
  - [x] Write test_reciprocal_polynomial.cpp (75 assertions - all passing)
    - Tests reciprocal polynomial construction from degree and from polynomial
    - Tests coefficient access and symmetry property
    - Tests evaluation and conversion to regular polynomial
    - Tests derivative calculation
    - Tests reciprocal property: P(x) = x^n * P(1/x)
    - Tests equality operator with various degree/coefficient combinations
  - [x] Write test_finitediff.cpp (137 assertions - all passing)
    - Tests finite difference stencils (1st-4th order, forward/backward/central)
    - Tests equal and unequal spacing
    - Tests full differentiation functions (finitediff1, finitediff2, finitediff4)
    - Tests error estimation for 2nd and 4th order methods
    - Tests edge cases and different data types
  - [x] Write test_matlab.cpp (42 assertions - all passing)
    - Tests text mode output (no Matlab libraries required)
    - Tests printMatlabForm for double, string, vector, and matrix types
    - Tests description output and edge cases
    - Tests chained output operations
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

### Matlab-dependent Tests (optional)
- [x] Write test_matlab_lib.cpp (requires Matlab libraries)
  - Tests: Binary MAT-file output for double, string, vector, matrix
  - Tests: Reading back written data and verifying correctness
  - Tests: Multiple variables in single MAT file
  - Tests: Empty matrix handling
  - File: tests/test_matlab_lib.cpp
  - Tag: [matlab]
- [x] Update CMakeLists.txt with Matlab detection
  - Searches in /usr/local/MATLAB/*/ and /opt/MATLAB/*/
  - Requires: mat.h header and libmat, libmx, libeng libraries
  - Defines JLT_MATLAB_LIB_SUPPORT when building
  - Tests tagged with "matlab" label for filtering

**Test Coverage: 1510 assertions across 22 test suites - ALL PASSING**
- 16 core test suites: 1280 assertions (no external dependencies)
- 2 LAPACK test suites: 60 assertions (built conditionally if LAPACK found)
- 1 Matlab test suite: 61 assertions (built conditionally if Matlab found)
- 1 CSparse test suite: 65 assertions (built conditionally if CSparse found)
- 1 Boost test suite: 44 assertions (built conditionally if Boost timer found)

## In Progress

### High Priority
- [ ] (No high priority code fixes remaining - see Future Improvements)

### Medium Priority
- [x] Fix static variable thread-safety issues
  - Removed `static bool only_once` from matrix.hpp operator[] methods
  - These warned about lack of bounds checking on 2nd [] index
  - Removed because: (1) not thread-safe, (2) users often miss once-only warnings, (3) if JLT_MATRIX_CHECK_BOUNDS is enabled, user explicitly requested bounds checking
  - Added comments explaining why warnings were removed
- [x] Add move semantics for performance (matrix.hpp - DONE)
- [x] Implement proper bounds checking throughout
  - ✅ Bounds checking already implemented in vector.hpp (operator[] via at())
  - ✅ Bounds checking already implemented in matrix.hpp (operator() via at(), row())
  - ✅ Created test_bounds_checking.cpp to verify checks work when flags enabled
  - ✅ Renamed macros to use JLT_ prefix (JLT_VECTOR_CHECK_BOUNDS, JLT_MATRIX_CHECK_BOUNDS)
  - Documented in AGENTS.md how to compile with bounds checking flags
- [x] Refactor code duplication in mathvector.hpp cross product
  - Replaced three specializations (float, double, long double) with single template
  - Eliminated 48 lines of duplicate code
  - All 81 mathvector tests still pass
- [ ] Optimize matrix multiplication algorithms
- [ ] Set up automated linting/formatting (clang-format, clang-tidy)
- [x] Replace auto_ptr with unique_ptr in csparse.hpp (deprecated in C++11)
  - Removed pre-C++11 support block (auto_ptr fallback for C++98)
  - Changed auto_ptr alias to std::unique_ptr directly
  - Fixed destructor bugs: inverted logic `if (!get())` → `if (get())` in cs_cl_drop, cs_ci_drop, cs_cl_free, cs_ci_free
  - Date: 2026-02-04

### Low Priority
- [ ] Update to C++17 features
  - ✅ **COMPLETED** Removed pre-C++11 __cplusplus > 199711L checks from:
    - jlt/vector.hpp
    - jlt/mathvector.hpp
    - jlt/mathmatrix.hpp
    - jlt/matrix.hpp
- [ ] Add comprehensive documentation
- [ ] Consider header-only implementation
- [ ] Set up CI/CD pipeline for automated testing
- [ ] Implement expression templates for lazy matrix evaluation


## Known Issues (Documented)

### Memory Safety
- ✅ **FIXED** matrix.hpp: Raw pointer management without RAII → Now uses std::vector
- ✅ **FIXED** mathmatrix.hpp: Manual array allocations (row_index, col arrays) → Now uses std::vector
- ✅ **FIXED** matrixutil.hpp: Manual memory management in LU decomposition → Now uses std::vector

### Exception Safety
- ✅ **FIXED** matrix.hpp: Assignment operator not exception-safe → Now uses copy-and-swap
- ✅ **FIXED** mathmatrix.hpp: Manual allocations without exception protection → Now uses std::vector
- ✅ **FIXED** matrixutil.hpp: Complex algorithm without proper cleanup → Now uses std::vector, automatic cleanup

### Thread Safety
- ✅ **FIXED** matrix.hpp: Static variables in operator[] methods (not thread-safe) → Warnings removed, now thread-safe
- matrix.hpp: Mutable state without synchronization

### Performance
- mathmatrix.hpp: Naïve O(n³) matrix multiplication
- mathmatrix.hpp: Temporary matrix creation in arithmetic operations
- matrixutil.hpp: Inefficient classical Gram-Schmidt (should use modified algorithm)

### Bug Fixes Needed
- ✅ **FIXED** mathmatrix.hpp: Wrong matrix dimension checks in invert() (line 354 - was checking private members m/n, now uses rows()/columns() public methods)
- ✅ **FIXED** csparse.hpp: Removed deprecated auto_ptr alias, now uses std::unique_ptr directly; also fixed destructor bugs (inverted logic: if (!get()) → if (get()))
- ✅ **VERIFIED** matrix.hpp: Constructor exception safety - Already uses std::vector which provides strong exception guarantee (no manual uninitialized_copy needed)

- mathmatrix.hpp: LAPACK calls without exception protection (lines 332-360)

## Future Improvements

### Code Quality
- ✅ **COMPLETED** Use `constexpr` and `noexcept` where appropriate - added to matrix.hpp accessor functions (size(), rows(), columns(), isSquare(), empty())
- ✅ **COMPLETED** Replace C-style casts with `static_cast` - updated polynomial.hpp, svdecomp.hpp, matlab.hpp, eigensystem.hpp, mathmatrix.hpp, tictoc.hpp
- ✅ **COMPLETED** Add `[[nodiscard]]` attributes - already present on appropriate functions throughout codebase
- [x] Remove pre-C++11 compatibility code - csparse.hpp auto_ptr fallback removed (2026-02-04)
- ✅ **COMPLETED** Refactor duplicate logic in matlab.hpp
  - Extracted `vector_of_vectors_to_matrix()` helper function in `detail` namespace
  - Removed duplicate code blocks at lines 206-216 and 382-392
  - Benefits: Single location for logic, better maintainability, removed TODO comments
  - Added empty vector/row handling for robustness
  - Tests: All 42 matlab assertions still passing
  - Date: 2026-02-06

- ✅ **COMPLETED** Refactor matrix print functions in matrix.hpp
  - Extracted `detail::print_elements_with_separator()` helper function
  - Refactored printOn(), printMatrixForm(), and printMathematicaForm()
  - Benefits: DRY principle, eliminated nested loops, clearer intent with lambdas
  - Reduced code duplication: 3 methods now share common iteration logic
  - Tests: All 83 matrix assertions still passing
  - Date: 2026-02-06

### Testing
- Increase code coverage to >90% (currently: 18 test suites, 1040+ assertions)
- Add edge case tests (empty matrices, single element, etc.)
- Add performance benchmarks
- Add fuzzing tests for numerical stability
- [x] Add integration tests for LAPACK-dependent components (eigensystem, svdecomp) - COMPLETED 2026-02-03
- See "Test Improvements for Existing Components" section below for detailed audit results

### Documentation
- Add API documentation with examples
- Create migration guide from other libraries
- Document LAPACK/BLAS dependencies clearly

## Missing Test Coverage

The following components in `jlt/` still need test coverage:

### Core Components (No External Dependencies)
- [x] **reciprocal_polynomial.hpp** - Monic reciprocal polynomial operations ✅ **COMPLETED** - 75 assertions
- [x] **command.hpp** - Unix command execution wrapper ✅ **COMPLETED** - 24 assertions
- [x] **math.hpp** - Mathematical utility functions (Mod, Sign) ✅ **COMPLETED** - 53 assertions
- [x] **stlio.hpp** - STL container I/O printing ✅ **COMPLETED** - 46 assertions
- [x] **display_task.hpp** - Task display utilities ✅ **COMPLETED** - 43 assertions
- [x] **vcs.hpp** - Version control system info extraction ✅ **COMPLETED** - 32 assertions
- [x] **finitediff.hpp** - Finite difference calculations ✅ **COMPLETED** - 137 assertions

### External Dependency Tests (Optional)
These tests would only be built if the respective libraries are found:

- [x] **matlab.hpp** - Matlab output format (text mode) ✅ **COMPLETED** - 42 assertions
  - Tests text mode output (default, no external libraries required)
  - Note: Binary MAT-file mode requires Matlab libraries (`-leng -lmat -lmex -lut -lmx`)
  - Compile flag: `JLT_MATLAB_LIB_SUPPORT` enables binary mode

- [x] **matlab.hpp** - Binary MAT-file export (requires Matlab libraries) ✅ **COMPLETED** - 61 assertions
  - Requires: Matlab libraries (`-leng -lmat -lmex -lut -lmx`)
  - Compile flag: `JLT_MATLAB_LIB_SUPPORT`
  - Note: Requires Matlab installation

- [x] **csparse.hpp** - CSparse sparse matrix library interface ✅ **COMPLETED** - 65 assertions
  - Requires: CSparse library (`-lcsparse`)
  - Tests: unique_ptr wrappers, mathmatrix↔CSparse conversion, round-trip conversion, memory management
  - Note: Wrapper for Timothy A. Davis's CSparse library
  - Date: 2026-02-06

- [x] **tictoc.hpp** - Timing utilities ✅ **COMPLETED** - 44 assertions
  - Requires: Boost timer library (`-lboost_timer -lboost_chrono`)
  - Tests: Basic timing, tic/toc measurements, labels, dangling tic behavior, timing accuracy, restart functionality, output format validation, programmatic API
  - Features: Outputs all three timing metrics (wall-clock, user CPU, system CPU) with labeled format
  - Output format: `0.5w 0.5u 0s  (1.2w 1.2u 0s)` - elapsed times, then cumulative in parens; w=wall, u=user, s=system
  - API: get_timing() method for programmatic access to all timing values
  - Note: Uses `boost::timer::cpu_timer` - provides complete timing information
  - Date: 2026-02-06

### Wrapper/Interface Files (Low Priority)
- [ ] **lapack.hpp** - LAPACK wrapper functions
  - Note: Already tested indirectly via eigensystem.hpp and svdecomp.hpp tests

## Test Improvements for Existing Components

The following improvements would strengthen existing test coverage:

### High Priority Test Additions
These may require verifying/fixing jlt code behavior:

- [ ] **test_vector.hpp improvements:**
  - ✅ **COMPLETED** Test `at()` with bounds checking enabled (`JLT_VECTOR_CHECK_BOUNDS`) - covered in test_bounds_checking.cpp
  - [ ] Test size mismatch operations (should they throw?)
  - [ ] Verify exception messages in `at()` out-of-range throws

- [ ] **test_mathvector.hpp improvements:**
  - Test zero division in `operator/=` (verify behavior)
  - Test operations with vectors of different sizes (should throw exception)
  - Add normalization/unit vector tests (verify `normalize()` exists and works)

- [ ] **test_mathmatrix.hpp improvements:**
  - Test matrix-vector multiplication
  - Test invalid operations (multiplying incompatible sizes - should throw)
  - Test `trace()` with non-square matrices (verify it throws or handles gracefully)
  - Add comprehensive tests for mathematical identities (e.g., (AB)^T = B^T A^T)

- [x] **test_matrixutil.hpp improvements:**
  - ✅ **COMPLETED** Test singular matrix handling in `inverse()` - added exception safety tests
  - ✅ **COMPLETED** Verify RAII works correctly when exceptions thrown (5 new assertions)
  - [ ] Verify LU decomposition produces correct L and U matrices (reconstruct original)
  - [ ] Verify QR decomposition produces orthogonal Q matrix (check Q^T * Q = I)
  - [ ] Test Gram-Schmidt with linearly dependent vectors (verify behavior)

### Medium Priority Test Additions

- [x] **test_matrix.hpp improvements:**
  - ✅ **COMPLETED** Test move constructor and move assignment (19 new assertions added)
  - [ ] Test column access (if available in the API)
  - [ ] Test column iterators (if they exist)
  - ✅ **COMPLETED** Add more `at()` bounds checking tests for both dimensions - covered in test_bounds_checking.cpp

- [ ] **test_polynomial.hpp improvements:**
  - Test polynomial evaluation at specific points (`p(x)`)
  - Test `printFancy()` output format (verify format matches expected)
  - Test degree changes after arithmetic operations

### Low Priority Test Additions

- [ ] **test_eigensystem.hpp improvements (LAPACK):**
  - Add tests for larger matrices (5x5, 10x10)
  - Add tests for ill-conditioned matrices
  - Test repeated eigenvalues

- [ ] **test_svdecomp.hpp improvements (LAPACK):**
  - Add tests for near-singular matrices
  - Test very large condition numbers
  - Add rectangular matrix stress tests

- [ ] **Output format tests:**
  - Test `printMatlabForm()` output format for vector and matrix
  - Test `printMathematicaForm()` output format
  - Test STL container printing via `stlio.hpp`

### Code Improvements Required
Some test additions may require jlt code improvements:

- [ ] Verify exception handling for invalid operations (add throws where missing)
- [x] Implement bounds checking for matrix operations (`JLT_MATRIX_CHECK_BOUNDS`)
- [ ] Add `normalize()` function to mathvector if missing
- [ ] Improve QR decomposition to guarantee orthogonality of Q
- [ ] Add input validation to `trace()` for non-square matrices
