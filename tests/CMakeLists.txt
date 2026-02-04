cmake_minimum_required(VERSION 3.10)
project(jlt_tests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the jlt library headers
include_directories(..)

# Create a library from catch.hpp (to speed up compilation)
add_library(catch2 STATIC catch_main.cpp)
target_include_directories(catch2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Find LAPACK (optional - tests will only be built if LAPACK is found)
find_package(LAPACK)
if(LAPACK_FOUND)
    message(STATUS "LAPACK found - building eigensystem and SVD tests")
else()
    message(STATUS "LAPACK not found - skipping eigensystem and SVD tests")
endif()

# Core container tests
add_executable(test_vector test_vector.cpp)
target_link_libraries(test_vector catch2)
add_test(NAME test_vector COMMAND test_vector)

add_executable(test_matrix test_matrix.cpp)
target_link_libraries(test_matrix catch2)
add_test(NAME test_matrix COMMAND test_matrix)

# Mathematical vector tests
add_executable(test_mathvector test_mathvector.cpp)
target_link_libraries(test_mathvector catch2)
add_test(NAME test_mathvector COMMAND test_mathvector)

# Mathematical matrix tests
add_executable(test_mathmatrix test_mathmatrix.cpp)
target_link_libraries(test_mathmatrix catch2)
add_test(NAME test_mathmatrix COMMAND test_mathmatrix)

# Polynomial tests
add_executable(test_polynomial test_polynomial.cpp)
target_link_libraries(test_polynomial catch2)
add_test(NAME test_polynomial COMMAND test_polynomial)

# Matrix utility tests (LU, QR, inverse - no LAPACK required)
add_executable(test_matrixutil test_matrixutil.cpp)
target_link_libraries(test_matrixutil catch2)
add_test(NAME test_matrixutil COMMAND test_matrixutil)

# Exception tests
add_executable(test_exceptions test_exceptions.cpp)
target_link_libraries(test_exceptions catch2)
add_test(NAME test_exceptions COMMAND test_exceptions)

# Math utility tests (Mod, Sign functions)
add_executable(test_math test_math.cpp)
target_link_libraries(test_math catch2)
add_test(NAME test_math COMMAND test_math)

# Command execution tests
add_executable(test_command test_command.cpp)
target_link_libraries(test_command catch2)
add_test(NAME test_command COMMAND test_command)

# VCS (Version Control System) tests
add_executable(test_vcs test_vcs.cpp)
target_link_libraries(test_vcs catch2)
add_test(NAME test_vcs COMMAND test_vcs)

# Display task tests
add_executable(test_display_task test_display_task.cpp)
target_link_libraries(test_display_task catch2)
add_test(NAME test_display_task COMMAND test_display_task)

# STL I/O tests (container output formatting)
add_executable(test_stlio test_stlio.cpp)
target_link_libraries(test_stlio catch2)
add_test(NAME test_stlio COMMAND test_stlio)

# Reciprocal polynomial tests
add_executable(test_reciprocal_polynomial test_reciprocal_polynomial.cpp)
target_link_libraries(test_reciprocal_polynomial catch2)
add_test(NAME test_reciprocal_polynomial COMMAND test_reciprocal_polynomial)

# Finite difference tests
add_executable(test_finitediff test_finitediff.cpp)
target_link_libraries(test_finitediff catch2)
add_test(NAME test_finitediff COMMAND test_finitediff)

# Matlab output format tests (text mode, no Matlab libraries required)
add_executable(test_matlab test_matlab.cpp)
target_link_libraries(test_matlab catch2)
add_test(NAME test_matlab COMMAND test_matlab)

# LAPACK-dependent tests (only if LAPACK is found)
if(LAPACK_FOUND)
    # Eigensystem tests (eigenvalue/eigenvector decomposition)
    add_executable(test_eigensystem test_eigensystem.cpp)
    target_link_libraries(test_eigensystem catch2 ${LAPACK_LIBRARIES})
    add_test(NAME test_eigensystem COMMAND test_eigensystem)
    set_tests_properties(test_eigensystem PROPERTIES LABELS "lapack")

    # SVD tests (singular value decomposition)
    add_executable(test_svdecomp test_svdecomp.cpp)
    target_link_libraries(test_svdecomp catch2 ${LAPACK_LIBRARIES})
    add_test(NAME test_svdecomp COMMAND test_svdecomp)
    set_tests_properties(test_svdecomp PROPERTIES LABELS "lapack")
endif()

# Find Matlab (optional - tests will only be built if Matlab is found)
# Search for Matlab in versioned directories under /usr/local/MATLAB or /opt/MATLAB

# First, try to find any installed Matlab version by looking for version directories
file(GLOB MATLAB_VERSION_DIRS
    "/usr/local/MATLAB/R*"
    "/opt/MATLAB/R*"
)

# Sort to get the latest version (e.g., R2025a, R2024b, etc.)
list(SORT MATLAB_VERSION_DIRS)
list(REVERSE MATLAB_VERSION_DIRS)

# Set default MATLAB_ROOT to the first (latest) found version, or empty if none found
if(MATLAB_VERSION_DIRS)
    list(GET MATLAB_VERSION_DIRS 0 MATLAB_ROOT_DEFAULT)
else()
    set(MATLAB_ROOT_DEFAULT "")
endif()

set(MATLAB_ROOT ${MATLAB_ROOT_DEFAULT} CACHE PATH "Matlab installation directory")

# If MATLAB_ROOT is set (either from cache or auto-detected), use it
if(MATLAB_ROOT)
    find_path(MATLAB_INCLUDE_DIR mat.h
        PATHS ${MATLAB_ROOT}/extern/include
        DOC "Matlab include directory"
        NO_DEFAULT_PATH
    )
    find_library(MATLAB_MAT_LIBRARY mat
        PATHS ${MATLAB_ROOT}/bin/glnxa64
        DOC "Matlab MAT library"
        NO_DEFAULT_PATH
    )
    find_library(MATLAB_MX_LIBRARY mx
        PATHS ${MATLAB_ROOT}/bin/glnxa64
        DOC "Matlab MX library"
        NO_DEFAULT_PATH
    )
    find_library(MATLAB_ENG_LIBRARY eng
        PATHS ${MATLAB_ROOT}/bin/glnxa64
        DOC "Matlab ENG library"
        NO_DEFAULT_PATH
    )
endif()

# If not found with specific path, try wildcards as fallback
if(NOT MATLAB_INCLUDE_DIR)
    find_path(MATLAB_INCLUDE_DIR mat.h
        PATHS /usr/local/MATLAB/*/extern/include
              /opt/MATLAB/*/extern/include
        DOC "Matlab include directory"
    )
endif()

if(NOT MATLAB_MAT_LIBRARY)
    find_library(MATLAB_MAT_LIBRARY mat
        PATHS /usr/local/MATLAB/*/bin/glnxa64
              /opt/MATLAB/*/bin/glnxa64
        DOC "Matlab MAT library"
    )
endif()

if(NOT MATLAB_MX_LIBRARY)
    find_library(MATLAB_MX_LIBRARY mx
        PATHS /usr/local/MATLAB/*/bin/glnxa64
              /opt/MATLAB/*/bin/glnxa64
        DOC "Matlab MX library"
    )
endif()

if(NOT MATLAB_ENG_LIBRARY)
    find_library(MATLAB_ENG_LIBRARY eng
        PATHS /usr/local/MATLAB/*/bin/glnxa64
              /opt/MATLAB/*/bin/glnxa64
        DOC "Matlab ENG library"
    )
endif()

if(MATLAB_INCLUDE_DIR AND MATLAB_MAT_LIBRARY AND MATLAB_MX_LIBRARY)
    set(MATLAB_FOUND TRUE)
    set(MATLAB_LIBRARIES ${MATLAB_MAT_LIBRARY} ${MATLAB_MX_LIBRARY} ${MATLAB_ENG_LIBRARY})
    message(STATUS "Matlab found - building Matlab binary format tests")
    message(STATUS "  Include: ${MATLAB_INCLUDE_DIR}")
    message(STATUS "  Libraries: ${MATLAB_LIBRARIES}")
else()
    set(MATLAB_FOUND FALSE)
    message(STATUS "Matlab not found - skipping Matlab binary format tests")
endif()

# Matlab binary format tests (only if Matlab libraries are found)
if(MATLAB_FOUND)
    add_executable(test_matlab_lib test_matlab_lib.cpp)
    target_include_directories(test_matlab_lib PRIVATE ${MATLAB_INCLUDE_DIR})
    target_link_libraries(test_matlab_lib catch2 ${MATLAB_LIBRARIES})
    target_compile_definitions(test_matlab_lib PRIVATE JLT_MATLAB_LIB_SUPPORT)
    add_test(NAME test_matlab_lib COMMAND test_matlab_lib)
    set_tests_properties(test_matlab_lib PROPERTIES LABELS "matlab")
endif()

# Enable CTest
enable_testing()

# Print summary
message(STATUS "")
message(STATUS "Test Summary:")
message(STATUS "  Core tests: vector, matrix, mathvector, mathmatrix, polynomial, matrixutil, exceptions, math, command, vcs, display_task, stlio, reciprocal_polynomial, finitediff, matlab")
if(LAPACK_FOUND)
    message(STATUS "  LAPACK tests: eigensystem, svdecomp")
else()
    message(STATUS "  LAPACK tests: SKIPPED (LAPACK not found)")
endif()
if(MATLAB_FOUND)
    message(STATUS "  Matlab tests: test_matlab_lib")
else()
    message(STATUS "  Matlab tests: SKIPPED (Matlab not found)")
endif()
message(STATUS "")
