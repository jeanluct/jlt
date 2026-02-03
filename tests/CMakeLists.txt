cmake_minimum_required(VERSION 3.10)
project(jlt_tests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the jlt library headers
include_directories(..)

# Create a library from catch.hpp (to speed up compilation)
add_library(catch2 STATIC catch_main.cpp)
target_include_directories(catch2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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

# TODO: Add more tests as they are written:
# - test_mathmatrix.cpp (mathematical matrix with LAPACK)
# - test_polynomial.cpp (polynomial class)
# - test_eigensystem.cpp (eigenvalue computation - requires LAPACK)
# - test_svdecomp.cpp (SVD - requires LAPACK)
# - test_qrdecomp.cpp (QR decomposition - requires LAPACK)

# Enable CTest
enable_testing()
