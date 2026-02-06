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

## Task 2: Input Validation in operator>>

**Priority:** High
**Effort:** ~15 minutes
**Files:** `jlt/stlio.hpp`

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
- Check `strm.good()` after each extraction
- Break out of loop on failure
- Consider leaving container in valid state on partial failure
- May need to add `operator>>` for other containers too

### Acceptance Criteria
- [ ] Input validation added to vector operator>>
- [ ] Stream state properly checked after each read
- [ ] Early exit on stream failure
- [ ] Tests added for error conditions
- [ ] All tests pass

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

## Task 6: Clean Up Field Separator #ifdefs

**Priority:** Low
**Effort:** ~30 minutes
**Files:** `jlt/stlio.hpp`

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

### Implementation Notes
- Already partially addressed with `detail::print_field_sep<T>()` helper
- Ensure all locations use the helper
- Consider if macro is still needed or can be simplified
- May need to keep macro for compile-time string vs int selection

### Acceptance Criteria
- [ ] All #ifdef JLT_FIELD_SEP_STRING blocks use helper function
- [ ] Code is cleaner and more maintainable
- [ ] No change in behavior
- [ ] All tests pass

---

## Summary

| Task | Priority | Effort | Key Benefit |
|------|----------|--------|-------------|
| 1. Add container support | Medium | 30 min | More complete STL coverage |
| 2. Input validation | High | 15 min | Robustness, error handling |
| 3. Consistent list separator | Low | 10 min | Consistency |
| 4. Refactor format_traits | Medium | 45 min | Maintainability |
| 5. Complex number support | Medium | 20 min | Completeness |
| 6. Clean up #ifdefs | Low | 30 min | Code clarity |

**Recommended order:** 2 → 3 → 1 → 5 → 4 → 6

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
