# STLIO Improvements Task List

## Overview
Remaining improvements for `jlt/stlio.hpp` - STL container I/O operations.

---

## Task 1: Add Missing Container Support

**Priority:** Medium
**Effort:** ~30 minutes
**Files:** `jlt/stlio.hpp`, `tests/test_stlio.cpp`

### Description
Add `operator<<` support for additional STL containers:
- `std::array<T, N>` - Fixed-size arrays (C++11)
- `std::deque<T>` - Double-ended queue
- `std::set<T>` / `std::unordered_set<T>` - Sets
- `std::tuple<T...>` / `std::pair<T1, T2>` - Tuples and pairs
- `std::optional<T>` (if C++17 available) - Optional values

### Implementation Notes
- Follow existing patterns in stlio.hpp
- Use `detail::print_sequence` for sequence containers
- Add appropriate `[[nodiscard]]` attributes
- Add corresponding test cases

### Acceptance Criteria
- [ ] `std::array` output operator implemented and tested
- [ ] `std::deque` output operator implemented and tested
- [ ] `std::set` output operator implemented and tested
- [ ] `std::pair` output operator implemented and tested
- [ ] All tests pass

---

## Task 2: Input Validation in operator>> ✅ COMPLETED

**Priority:** High
**Effort:** ~15 minutes
**Files:** `jlt/stlio.hpp`, `tests/test_stlio.cpp`
**Completed:** 2026-02-05

### Description
The current `operator>>` for `std::vector` doesn't validate stream state during reads:

```cpp
template<class T>
std::istream& operator>>(std::istream& strm, std::vector<T>& vv)
{
  for (auto i = vv.begin(); i != vv.end(); ++i)
  {
    strm >> *i;  // What if this fails?
  }
  return strm;
}
```

### Implementation Notes
- ✅ Check `strm.good()` after each extraction
- ✅ Break out of loop on failure
- ✅ Consider leaving container in valid state on partial failure
- ✅ May need to add `operator>>` for other containers too

### Changes Made
- Added stream state validation to `vector` operator>> - stops on failure
- Added `operator>>` for `std::valarray` with validation
- Added `operator>>` for `std::list` (reads until EOF or failure)
- Added comprehensive tests for input validation
- Added tests for valarray and list input operators
- Test error handling for bad input and EOF conditions

### Acceptance Criteria
- [x] Input validation added to vector operator>>
- [x] Stream state properly checked after each read
- [x] Early exit on stream failure
- [x] Tests added for error conditions
- [x] All tests pass

---

## Task 3: Consistent Separator for std::list

**Priority:** Low
**Effort:** ~10 minutes
**Files:** `jlt/stlio.hpp`

### Description
Line 241: `std::list` uses `"\t"` (tab) separator, but other containers use `format_traits<T>::field_sep` (2 spaces by default).

**Current code:**
```cpp
template<class T>
std::ostream& operator<<(std::ostream& strm, const std::list<T>& ll)
{
  if (ll.empty()) return strm;
  std::copy(ll.begin(), ll.end(), std::ostream_iterator<T>(strm, "\t"));
  return strm;
}
```

### Implementation Notes
- Make `std::list` use `detail::print_field_sep<T>()` for consistency
- Or use `format_traits<T>::field_sep` directly
- Update tests if output format changes

### Acceptance Criteria
- [ ] std::list uses same separator as other containers
- [ ] Tests updated if needed
- [ ] All tests pass

---

## Task 4: Refactor format_traits to Reduce Duplication

**Priority:** Medium
**Effort:** ~45 minutes
**Files:** `jlt/stlio.hpp`

### Description
The `format_traits` specializations for `int`, `float`, `long double` (lines 56-120) have lots of duplicated code. Each specialization repeats the same `field_sep` handling.

**Current pattern:**
```cpp
template<>
struct format_traits<int> {
  #ifdef JLT_FIELD_SEP_STRING
    static const char field_sep[];
  #else
    static const int field_sep = 2;
  #endif
  static const int extra_width_scientific = 0;
  static const int field_width = 6;
};
// Repeated for float, long double...
```

### Implementation Notes
- Create a base `format_traits_base` template with common defaults
- Use inheritance or composition to reduce duplication
- Keep specializations only for values that differ
- Maintain backward compatibility

### Acceptance Criteria
- [ ] Base template created with common defaults
- [ ] Specializations simplified
- [ ] No change in behavior (backward compatible)
- [ ] All tests pass

---

## Task 5: Add format_traits for std::complex<T>

**Priority:** Medium
**Effort:** ~20 minutes
**Files:** `jlt/stlio.hpp`, `tests/test_stlio.cpp`

### Description
No `format_traits` specialization exists for complex numbers. This would be useful for consistent formatting of complex values in containers.

### Implementation Notes
- Add `format_traits<std::complex<T>>` specialization
- Field width should account for both real and imaginary parts
- Consider how to format: `(real, imag)` or `real+imagi`
- May need to specialize output operator for complex

### Acceptance Criteria
- [ ] format_traits<std::complex<T>> defined
- [ ] Appropriate field_width for complex numbers
- [ ] Output format defined and documented
- [ ] Tests added for complex container output
- [ ] All tests pass

---

## Task 6: Clean Up Field Separator #ifdefs ✅ COMPLETED

**Priority:** Low
**Effort:** ~30 minutes
**Files:** `jlt/stlio.hpp`
**Completed:** 2026-02-05

### Description
The `JLT_FIELD_SEP_STRING` macro appears 8+ times throughout the file, making the code verbose and harder to read.

**Current pattern:**
```cpp
#ifdef JLT_FIELD_SEP_STRING
  << format_traits<T>::field_sep;
#else
  << std::string(format_traits<T>::field_sep,' ');
#endif
```

### Changes Made
- ✅ Created `detail::print_field_sep<T>()` helper function
- ✅ Updated `std::map` output operators to use the helper
- ✅ Updated `std::list` output operator to use consistent separator (was using `\t`)
- ✅ All output operators now use consistent field separator logic
- Note: `#ifdef` blocks remain in `format_traits` definitions (required for type system)

### Benefits
- Eliminated 4 redundant `#ifdef` blocks from output operators
- Consistent separator behavior across all container types
- Easier to maintain and modify separator logic in one place
- `std::list` now uses same formatting as other containers

### Acceptance Criteria
- [x] All output operators use helper function
- [x] Code is cleaner and more maintainable
- [x] No change in behavior (except list uses consistent separator)
- [x] All tests pass

---

## Summary

| Task | Priority | Status | Effort | Key Benefit |
|------|----------|--------|--------|-------------|
| 1. Add container support | Medium | ⏳ Pending | 30 min | More complete STL coverage |
| 2. Input validation | High | ✅ Done | 15 min | Robustness, error handling |
| 3. Consistent list separator | Low | ✅ Done* | 10 min | Consistency |
| 4. Refactor format_traits | Medium | ⏳ Pending | 45 min | Maintainability |
| 5. Complex number support | Medium | ⏳ Pending | 20 min | Completeness |
| 6. Clean up #ifdefs | Low | ✅ Done | 30 min | Code clarity |

\* Task 3 (consistent list separator) completed as part of Task 6

**Completed:** Tasks 2, 3, 6
**Pending:** Tasks 1, 4, 5

**Recommended order:** 2 ✅ → 6 ✅ → 1 → 5 → 4

---

## Related Files
- `jlt/stlio.hpp` - Main header to modify
- `tests/test_stlio.cpp` - Test file to update
- `jlt/mathvector.hpp` - Uses stlio for vector output
- `jlt/matrix.hpp` - Uses stlio for matrix output

## Context
These improvements follow the recent refactoring (commit a0d8493) which:
- Extracted `detail::print_sequence` helper
- Added `detail::stream_flags_saver` RAII class
- Added `[[nodiscard]]` to all operators
- Fixed stream flag restoration bugs

The remaining tasks build on this foundation to make stlio.hpp more complete, robust, and maintainable.
