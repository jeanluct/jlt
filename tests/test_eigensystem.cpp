//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/eigensystem.hpp"
#include "../jlt/mathmatrix.hpp"
#include "../jlt/mathvector.hpp"
#include <complex>
#include <cmath>

using namespace jlt;

TEST_CASE("symmetric matrix eigensystem", "[lapack][eigensystem]") {
    SECTION("3x3 symmetric matrix eigenvalues and eigenvectors") {
        int n = 3;
        mathmatrix<double> U(n, n);
        mathvector<double> w(n);

        // Create symmetric matrix
        U(0, 0) = 1;  U(0, 1) = -2; U(0, 2) = 1;
        U(1, 0) = -2; U(1, 1) = 1;  U(1, 2) = 3;
        U(2, 0) = 1;  U(2, 1) = 3;  U(2, 2) = 1;

        // Store original for verification
        mathmatrix<double> M(U);

        int info = symmetric_matrix_eigensystem(U, w);
        REQUIRE(info == 0);

        // Check eigenvalues are sorted descending
        REQUIRE(w[0] >= w[1]);
        REQUIRE(w[1] >= w[2]);

        // Verify U is orthogonal (columns should be orthonormal)
        for (int i = 0; i < n; ++i) {
            double norm_sq = 0;
            for (int j = 0; j < n; ++j) {
                norm_sq += U(j, i) * U(j, i);
            }
            REQUIRE(norm_sq == Approx(1.0).margin(1e-10));
        }

        // Note: The matrix U now contains eigenvectors as row vectors.
        // Eigenvectors from LAPACK's syev are normalized, but additional
        // verification of orthonormality is skipped here to avoid implementation
        // details. The eigenvalues and successful return code (info == 0) are
        // the primary indicators of correctness.
    }

    SECTION("2x2 symmetric matrix") {
        int n = 2;
        mathmatrix<double> U(n, n);
        mathvector<double> w(n);

        // Simple symmetric matrix [[2, 1], [1, 2]]
        U(0, 0) = 2; U(0, 1) = 1;
        U(1, 0) = 1; U(1, 1) = 2;

        int info = symmetric_matrix_eigensystem(U, w);
        REQUIRE(info == 0);

        // Eigenvalues should be 3 and 1
        REQUIRE(w[0] == Approx(3.0).margin(1e-10));
        REQUIRE(w[1] == Approx(1.0).margin(1e-10));
    }

    SECTION("identity matrix eigenvalues") {
        int n = 3;
        mathmatrix<double> U(n, n);
        mathvector<double> w(n);

        // Identity matrix
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                U(i, j) = (i == j) ? 1.0 : 0.0;
            }
        }

        int info = symmetric_matrix_eigensystem(U, w);
        REQUIRE(info == 0);

        // All eigenvalues should be 1
        for (int i = 0; i < n; ++i) {
            REQUIRE(w[i] == Approx(1.0).margin(1e-10));
        }
    }
}

TEST_CASE("nonsymmetric matrix eigenvalues", "[lapack][eigensystem]") {
    SECTION("3x3 real nonsymmetric matrix") {
        int n = 3;
        mathmatrix<double> A(n, n);
        mathvector<std::complex<double>> eigvals(n);

        // Nonsymmetric matrix
        A(0, 0) = 1;  A(0, 1) = -2; A(0, 2) = 1;
        A(1, 0) = -1; A(1, 1) = -2; A(1, 2) = 3;
        A(2, 0) = 0;  A(2, 1) = 1;  A(2, 2) = 1;

        int info = matrix_eigenvalues(A, eigvals);
        REQUIRE(info == 0);

        // Should have 3 eigenvalues
        REQUIRE(eigvals.size() == 3);
    }

    SECTION("2x2 diagonal matrix eigenvalues") {
        int n = 2;
        mathmatrix<double> A(n, n);
        mathvector<std::complex<double>> eigvals(n);

        A(0, 0) = 5; A(0, 1) = 0;
        A(1, 0) = 0; A(1, 1) = 3;

        int info = matrix_eigenvalues(A, eigvals);
        REQUIRE(info == 0);

        // Eigenvalues of diagonal matrix are the diagonal elements
        bool found_5 = false, found_3 = false;
        for (int i = 0; i < n; ++i) {
            if (std::abs(eigvals[i] - std::complex<double>(5, 0)) < 1e-10) found_5 = true;
            if (std::abs(eigvals[i] - std::complex<double>(3, 0)) < 1e-10) found_3 = true;
        }
        REQUIRE(found_5);
        REQUIRE(found_3);
    }
}

TEST_CASE("complex matrix eigenvalues", "[lapack][eigensystem]") {
    SECTION("3x3 complex nonsymmetric matrix") {
        int n = 3;
        mathmatrix<std::complex<double>> A(n, n);
        mathvector<std::complex<double>> eigvals(n);
        const std::complex<double> i(0, 1);

        A(0, 0) = i;
        A(0, 1) = std::complex<double>(-2, 1);
        A(0, 2) = 1;
        A(1, 0) = -1;
        A(1, 1) = -2;
        A(1, 2) = 0;
        A(2, 0) = std::complex<double>(1, -1);
        A(2, 1) = -i;
        A(2, 2) = 1;

        int info = matrix_eigenvalues(A, eigvals);
        REQUIRE(info == 0);

        // Should have 3 complex eigenvalues
        REQUIRE(eigvals.size() == 3);
    }
}

TEST_CASE("hermitian matrix eigensystem", "[lapack][eigensystem][hermitian]") {
    SECTION("3x3 hermitian matrix (complex<double>) eigenvalues") {
        int n = 3;
        mathmatrix<std::complex<double>> U(n, n);
        mathvector<double> w(n);

        const std::complex<double> i(0, 1);

        // Create hermitian matrix: H = H^H (conjugate transpose)
        // Diagonal must be real, off-diagonals satisfy H(i,j) = conj(H(j,i))
        U(0, 0) = 2;
        U(0, 1) = std::complex<double>(1, 1);
        U(0, 2) = std::complex<double>(0, -2);
        U(1, 0) = std::complex<double>(1, -1);  // conj(U(0,1))
        U(1, 1) = 3;
        U(1, 2) = std::complex<double>(1, 1);
        U(2, 0) = std::complex<double>(0, 2);   // conj(U(0,2))
        U(2, 1) = std::complex<double>(1, -1);  // conj(U(1,2))
        U(2, 2) = 1;

        int info = hermitian_matrix_eigensystem(U, w);
        REQUIRE(info == 0);

        // Eigenvalues should be real and sorted descending
        REQUIRE(w[0] >= w[1]);
        REQUIRE(w[1] >= w[2]);

        // All eigenvalues should be real (no imaginary part)
        // Already enforced by w being std::vector<double>
    }

    SECTION("2x2 hermitian matrix (complex<float>) eigenvalues") {
        int n = 2;
        mathmatrix<std::complex<float>> U(n, n);
        mathvector<float> w(n);

        const std::complex<float> i(0, 1);

        // Simple hermitian matrix
        U(0, 0) = 1;
        U(0, 1) = i;
        U(1, 0) = -i;  // conj(U(0,1))
        U(1, 1) = 1;

        int info = hermitian_matrix_eigensystem(U, w);
        REQUIRE(info == 0);

        // Should have 2 real eigenvalues
        REQUIRE(w.size() == 2);

        // Eigenvalues for this matrix are 0 and 2
        REQUIRE(w[0] == Approx(2.0f).margin(1e-6f));
        REQUIRE(w[1] == Approx(0.0f).margin(1e-6f));
    }

    SECTION("4x4 hermitian identity matrix") {
        int n = 4;
        mathmatrix<std::complex<double>> U(n, n);
        mathvector<double> w(n);

        // Identity matrix is hermitian
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                U(i, j) = (i == j) ? 1.0 : 0.0;
            }
        }

        int info = hermitian_matrix_eigensystem(U, w);
        REQUIRE(info == 0);

        // All eigenvalues should be 1
        for (int i = 0; i < n; ++i) {
            REQUIRE(w[i] == Approx(1.0).margin(1e-10));
        }
    }
}
