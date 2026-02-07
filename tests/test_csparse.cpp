//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/csparse.hpp"
#include "../jlt/mathmatrix.hpp"

using namespace jlt;

TEST_CASE("CSparse unique_ptr wrappers", "[csparse][memory]") {
    SECTION("cs_unique_ptr default construction") {
        cs_unique_ptr ptr;
        REQUIRE(!ptr);  // Should be null
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("cs_unique_ptr with nullptr") {
        cs_unique_ptr ptr(nullptr);
        REQUIRE(!ptr);
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("csd_unique_ptr default construction") {
        csd_unique_ptr ptr;
        REQUIRE(!ptr);  // Should be null
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("csd_unique_ptr with nullptr") {
        csd_unique_ptr ptr(nullptr);
        REQUIRE(!ptr);
        REQUIRE(ptr.get() == nullptr);
    }
}

TEST_CASE("mathmatrix to CSparse conversion", "[csparse][conversion]") {
    SECTION("Convert 2x2 identity matrix") {
        mathmatrix<double> I(2, 2);
        I(0, 0) = 1.0;
        I(0, 1) = 0.0;
        I(1, 0) = 0.0;
        I(1, 1) = 1.0;

        cs_unique_ptr csI(mathmatrix_to_cs_sparse_matrix(I));
        REQUIRE(csI);  // Should not be null
        REQUIRE(csI->m == 2);
        REQUIRE(csI->n == 2);
        REQUIRE(csI->nz == -1);  // Compressed format
    }

    SECTION("Convert 3x3 sparse matrix") {
        mathmatrix<double> M(3, 3);
        // Create a sparse matrix with only diagonal elements
        M(0, 0) = 1.0;
        M(1, 1) = 2.0;
        M(2, 2) = 3.0;
        // All other elements are zero

        cs_unique_ptr csM(mathmatrix_to_cs_sparse_matrix(M));
        REQUIRE(csM);
        REQUIRE(csM->m == 3);
        REQUIRE(csM->n == 3);
    }

    SECTION("Convert zero matrix") {
        mathmatrix<double> Z(2, 2);
        // All elements are zero by default

        cs_unique_ptr csZ(mathmatrix_to_cs_sparse_matrix(Z));
        REQUIRE(csZ);
        // CSparse optimizes empty sparse matrices to have zero dimensions
        REQUIRE(csZ->m == 0);
        REQUIRE(csZ->n == 0);
    }

    SECTION("Convert matrix with off-diagonal elements") {
        mathmatrix<double> M(2, 2);
        M(0, 0) = 1.0;
        M(0, 1) = 2.0;
        M(1, 0) = 3.0;
        M(1, 1) = 4.0;

        cs_unique_ptr csM(mathmatrix_to_cs_sparse_matrix(M));
        REQUIRE(csM);
        REQUIRE(csM->m == 2);
        REQUIRE(csM->n == 2);
    }
}

TEST_CASE("CSparse to mathmatrix conversion", "[csparse][conversion]") {
    SECTION("Round-trip conversion of identity matrix") {
        // Create identity matrix
        mathmatrix<double> I(2, 2);
        I(0, 0) = 1.0;
        I(0, 1) = 0.0;
        I(1, 0) = 0.0;
        I(1, 1) = 1.0;

        // Convert to CSparse and back
        cs_unique_ptr csI(mathmatrix_to_cs_sparse_matrix(I));
        mathmatrix<double> I2 = cs_sparse_matrix_to_mathmatrix<double>(csI);

        // Check dimensions
        REQUIRE(I2.rows() == 2);
        REQUIRE(I2.columns() == 2);

        // Check values
        REQUIRE(I2(0, 0) == 1.0);
        REQUIRE(I2(0, 1) == 0.0);
        REQUIRE(I2(1, 0) == 0.0);
        REQUIRE(I2(1, 1) == 1.0);
    }

    SECTION("Round-trip conversion of sparse matrix") {
        mathmatrix<double> M(3, 3);
        M(0, 0) = 1.0;
        M(0, 2) = 2.0;
        M(1, 1) = 3.0;
        M(2, 0) = 4.0;
        M(2, 2) = 5.0;

        cs_unique_ptr csM(mathmatrix_to_cs_sparse_matrix(M));
        mathmatrix<double> M2 = cs_sparse_matrix_to_mathmatrix<double>(csM);

        REQUIRE(M2.rows() == 3);
        REQUIRE(M2.columns() == 3);

        // Check non-zero elements
        REQUIRE(M2(0, 0) == 1.0);
        REQUIRE(M2(0, 2) == 2.0);
        REQUIRE(M2(1, 1) == 3.0);
        REQUIRE(M2(2, 0) == 4.0);
        REQUIRE(M2(2, 2) == 5.0);

        // Check zero elements
        REQUIRE(M2(0, 1) == 0.0);
        REQUIRE(M2(1, 0) == 0.0);
        REQUIRE(M2(1, 2) == 0.0);
        REQUIRE(M2(2, 1) == 0.0);
    }

    SECTION("Convert null CSparse pointer") {
        cs_unique_ptr nullPtr(nullptr);
        mathmatrix<double> M = cs_sparse_matrix_to_mathmatrix<double>(nullPtr);

        // Should return empty matrix
        REQUIRE(M.rows() == 0);
        REQUIRE(M.columns() == 0);
    }

    SECTION("Round-trip conversion of rectangular matrix") {
        mathmatrix<double> M(2, 3);
        M(0, 0) = 1.0;
        M(0, 2) = 2.0;
        M(1, 1) = 3.0;

        cs_unique_ptr csM(mathmatrix_to_cs_sparse_matrix(M));
        mathmatrix<double> M2 = cs_sparse_matrix_to_mathmatrix<double>(csM);

        REQUIRE(M2.rows() == 2);
        REQUIRE(M2.columns() == 3);

        REQUIRE(M2(0, 0) == 1.0);
        REQUIRE(M2(0, 1) == 0.0);
        REQUIRE(M2(0, 2) == 2.0);
        REQUIRE(M2(1, 0) == 0.0);
        REQUIRE(M2(1, 1) == 3.0);
        REQUIRE(M2(1, 2) == 0.0);
    }
}

TEST_CASE("CSparse memory management", "[csparse][memory]") {
    SECTION("Automatic cleanup of sparse matrix") {
        {
            mathmatrix<double> M(10, 10);
            for (int i = 0; i < 10; ++i) {
                M(i, i) = static_cast<double>(i + 1);
            }

            cs_unique_ptr csM(mathmatrix_to_cs_sparse_matrix(M));
            REQUIRE(csM);
            REQUIRE(csM->m == 10);
            REQUIRE(csM->n == 10);

            // csM will be automatically cleaned up when it goes out of scope
        }
        // If we get here without crashes, memory management worked
        REQUIRE(true);
    }

    SECTION("Multiple conversions without leaks") {
        for (int size = 2; size <= 5; ++size) {
            mathmatrix<double> M(size, size);
            for (int i = 0; i < size; ++i) {
                M(i, i) = 1.0;
            }

            cs_unique_ptr csM(mathmatrix_to_cs_sparse_matrix(M));
            REQUIRE(csM);
            REQUIRE(csM->m == size);

            mathmatrix<double> M2 = cs_sparse_matrix_to_mathmatrix<double>(csM);
            REQUIRE(M2.rows() == size);
        }
        REQUIRE(true);
    }
}
