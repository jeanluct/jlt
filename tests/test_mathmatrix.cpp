#include "catch.hpp"
#include "../jlt/mathmatrix.hpp"
#include "../jlt/mathvector.hpp"

using namespace jlt;

TEST_CASE("mathmatrix basic construction", "[mathmatrix]") {
    SECTION("default construction") {
        mathmatrix<double> M;
        REQUIRE(M.rows() == 0);
        REQUIRE(M.columns() == 0);
        REQUIRE(M.empty());
    }

    SECTION("construction with size and value") {
        mathmatrix<double> M(3, 4, 2.5);
        REQUIRE(M.rows() == 3);
        REQUIRE(M.columns() == 4);
        REQUIRE(M(0, 0) == 2.5);
        REQUIRE(M(2, 3) == 2.5);
    }

    SECTION("construction with initializer list") {
        mathmatrix<double> M(2, 3, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
        REQUIRE(M.rows() == 2);
        REQUIRE(M.columns() == 3);
        REQUIRE(M(0, 0) == 1.0);
        REQUIRE(M(0, 1) == 2.0);
        REQUIRE(M(0, 2) == 3.0);
        REQUIRE(M(1, 0) == 4.0);
        REQUIRE(M(1, 1) == 5.0);
        REQUIRE(M(1, 2) == 6.0);
    }

    SECTION("copy from matrix base") {
        matrix<double> base(2, 2, {1.0, 2.0, 3.0, 4.0});
        mathmatrix<double> M(base);
        REQUIRE(M.rows() == 2);
        REQUIRE(M.columns() == 2);
        REQUIRE(M(0, 0) == 1.0);
        REQUIRE(M(1, 1) == 4.0);
    }
}

TEST_CASE("mathmatrix compound assignment operators", "[mathmatrix]") {
    mathmatrix<double> M1(2, 2, {1.0, 2.0, 3.0, 4.0});
    mathmatrix<double> M2(2, 2, {5.0, 6.0, 7.0, 8.0});

    SECTION("operator+= with matrix") {
        M1 += M2;
        REQUIRE(M1(0, 0) == 6.0);
        REQUIRE(M1(0, 1) == 8.0);
        REQUIRE(M1(1, 0) == 10.0);
        REQUIRE(M1(1, 1) == 12.0);
    }

    SECTION("operator-= with matrix") {
        M1 -= M2;
        REQUIRE(M1(0, 0) == -4.0);
        REQUIRE(M1(0, 1) == -4.0);
        REQUIRE(M1(1, 0) == -4.0);
        REQUIRE(M1(1, 1) == -4.0);
    }

    SECTION("operator+= with scalar (adds to diagonal)") {
        mathmatrix<double> M(2, 2, {1.0, 0.0, 0.0, 1.0});
        M += 3.0;
        REQUIRE(M(0, 0) == 4.0);
        REQUIRE(M(1, 1) == 4.0);
        REQUIRE(M(0, 1) == 0.0);  // Off-diagonal unchanged
    }

    SECTION("operator-= with scalar (subtracts from diagonal)") {
        mathmatrix<double> M(2, 2, {5.0, 0.0, 0.0, 5.0});
        M -= 2.0;
        REQUIRE(M(0, 0) == 3.0);
        REQUIRE(M(1, 1) == 3.0);
    }

    SECTION("operator*= with scalar (multiplies diagonal)") {
        mathmatrix<double> M(2, 2, {2.0, 1.0, 1.0, 2.0});
        M *= 3.0;
        REQUIRE(M(0, 0) == 6.0);
        REQUIRE(M(1, 1) == 6.0);
        REQUIRE(M(0, 1) == 1.0);  // Off-diagonal unchanged
    }

    SECTION("operator/= with scalar (divides diagonal)") {
        mathmatrix<double> M(2, 2, {6.0, 1.0, 1.0, 6.0});
        M /= 2.0;
        REQUIRE(M(0, 0) == 3.0);
        REQUIRE(M(1, 1) == 3.0);
    }
}

TEST_CASE("mathmatrix comparison operators", "[mathmatrix]") {
    mathmatrix<double> M1(2, 2, {1.0, 2.0, 3.0, 4.0});
    mathmatrix<double> M2(2, 2, {1.0, 2.0, 3.0, 4.0});
    mathmatrix<double> M3(2, 2, {5.0, 6.0, 7.0, 8.0});

    SECTION("operator== equal matrices") {
        REQUIRE(M1 == M2);
    }

    SECTION("operator== different matrices") {
        REQUIRE_FALSE(M1 == M3);
    }

    SECTION("operator!= different matrices") {
        REQUIRE(M1 != M3);
    }

    SECTION("operator!= equal matrices") {
        REQUIRE_FALSE(M1 != M2);
    }
}

TEST_CASE("mathmatrix arithmetic operators", "[mathmatrix]") {
    mathmatrix<double> M1(2, 2, {1.0, 2.0, 3.0, 4.0});
    mathmatrix<double> M2(2, 2, {5.0, 6.0, 7.0, 8.0});

    SECTION("unary plus") {
        auto result = +M1;
        REQUIRE(result(0, 0) == 1.0);
        REQUIRE(result(1, 1) == 4.0);
    }

    SECTION("unary minus") {
        auto result = -M1;
        REQUIRE(result(0, 0) == -1.0);
        REQUIRE(result(0, 1) == -2.0);
        REQUIRE(result(1, 0) == -3.0);
        REQUIRE(result(1, 1) == -4.0);
    }

    SECTION("matrix addition") {
        auto result = M1 + M2;
        REQUIRE(result(0, 0) == 6.0);
        REQUIRE(result(0, 1) == 8.0);
        REQUIRE(result(1, 0) == 10.0);
        REQUIRE(result(1, 1) == 12.0);
    }

    SECTION("matrix subtraction") {
        auto result = M1 - M2;
        REQUIRE(result(0, 0) == -4.0);
        REQUIRE(result(0, 1) == -4.0);
        REQUIRE(result(1, 0) == -4.0);
        REQUIRE(result(1, 1) == -4.0);
    }

    SECTION("scalar multiplication left") {
        auto result = 2.0 * M1;
        REQUIRE(result(0, 0) == 2.0);
        REQUIRE(result(0, 1) == 4.0);
        REQUIRE(result(1, 0) == 6.0);
        REQUIRE(result(1, 1) == 8.0);
    }

    SECTION("scalar multiplication right") {
        auto result = M1 * 3.0;
        REQUIRE(result(0, 0) == 3.0);
        REQUIRE(result(0, 1) == 6.0);
        REQUIRE(result(1, 0) == 9.0);
        REQUIRE(result(1, 1) == 12.0);
    }

    SECTION("scalar division") {
        auto result = M1 / 2.0;
        REQUIRE(result(0, 0) == 0.5);
        REQUIRE(result(0, 1) == 1.0);
        REQUIRE(result(1, 0) == 1.5);
        REQUIRE(result(1, 1) == 2.0);
    }
}

TEST_CASE("mathmatrix matrix multiplication", "[mathmatrix]") {
    SECTION("2x2 matrix multiplication") {
        mathmatrix<double> A(2, 2, {1.0, 2.0, 3.0, 4.0});
        mathmatrix<double> B(2, 2, {5.0, 6.0, 7.0, 8.0});
        auto C = A * B;

        // C(0,0) = 1*5 + 2*7 = 19
        // C(0,1) = 1*6 + 2*8 = 22
        // C(1,0) = 3*5 + 4*7 = 43
        // C(1,1) = 3*6 + 4*8 = 50
        REQUIRE(C(0, 0) == 19.0);
        REQUIRE(C(0, 1) == 22.0);
        REQUIRE(C(1, 0) == 43.0);
        REQUIRE(C(1, 1) == 50.0);
    }

    SECTION("non-square matrix multiplication") {
        mathmatrix<double> A(2, 3, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
        mathmatrix<double> B(3, 2, {7.0, 8.0, 9.0, 10.0, 11.0, 12.0});
        auto C = A * B;

        REQUIRE(C.rows() == 2);
        REQUIRE(C.columns() == 2);
        // C(0,0) = 1*7 + 2*9 + 3*11 = 58
        REQUIRE(C(0, 0) == 58.0);
    }

    SECTION("matrix multiplication with identity") {
        mathmatrix<double> A(2, 2, {1.0, 2.0, 3.0, 4.0});
        mathmatrix<double> I(2, 2, {1.0, 0.0, 0.0, 1.0});
        auto C = A * I;

        REQUIRE(C == A);
    }
}

TEST_CASE("mathmatrix matrix-vector multiplication", "[mathmatrix]") {
    mathmatrix<double> A(2, 3, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    mathvector<double> v = {7.0, 8.0, 9.0};

    auto result = A * v;

    // result[0] = 1*7 + 2*8 + 3*9 = 7 + 16 + 27 = 50
    // result[1] = 4*7 + 5*8 + 6*9 = 28 + 40 + 54 = 122
    REQUIRE(result.size() == 2);
    REQUIRE(result[0] == 50.0);
    REQUIRE(result[1] == 122.0);
}

TEST_CASE("mathmatrix identity operations", "[mathmatrix]") {
    SECTION("identity method") {
        mathmatrix<double> M(3, 3);
        M.identity();

        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                if (i == j) {
                    REQUIRE(M(i, j) == 1.0);
                } else {
                    REQUIRE(M(i, j) == 0.0);
                }
            }
        }
    }

    SECTION("identity with custom value") {
        mathmatrix<double> M(2, 2);
        M.identity(5.0);

        REQUIRE(M(0, 0) == 5.0);
        REQUIRE(M(1, 1) == 5.0);
        REQUIRE(M(0, 1) == 0.0);
    }

    SECTION("identity_matrix helper") {
        auto I = identity_matrix<double>(3);
        REQUIRE(I.rows() == 3);
        REQUIRE(I.columns() == 3);
        REQUIRE(I(0, 0) == 1.0);
        REQUIRE(I(2, 2) == 1.0);
        REQUIRE(I(0, 1) == 0.0);
    }
}

TEST_CASE("mathmatrix determinant and trace", "[mathmatrix]") {
    SECTION("determinant of 2x2") {
        mathmatrix<double> M(2, 2, {4.0, 7.0, 2.0, 6.0});
        // det = 4*6 - 7*2 = 24 - 14 = 10
        REQUIRE(M.det() == 10.0);
    }

    SECTION("determinant of identity") {
        auto I = identity_matrix<double>(3);
        REQUIRE(I.det() == 1.0);
    }

    SECTION("determinant of singular matrix") {
        mathmatrix<double> M(2, 2, {1.0, 2.0, 2.0, 4.0});
        REQUIRE(M.det() == Approx(0.0).margin(1e-10));
    }

    SECTION("trace of 3x3") {
        mathmatrix<double> M(3, 3, {
            1.0, 0.0, 0.0,
            0.0, 2.0, 0.0,
            0.0, 0.0, 3.0
        });
        REQUIRE(M.trace() == 6.0);  // 1 + 2 + 3
    }

    SECTION("frobenius norm of 2x2") {
        mathmatrix<double> M(2, 2, {3.0, 4.0, 0.0, 0.0});
        // ||M||_F = sqrt(3² + 4²) = sqrt(9 + 16) = 5
        REQUIRE(M.frobenius_norm() == Approx(5.0));
    }

    SECTION("frobenius norm of identity") {
        auto I = identity_matrix<double>(3);
        // ||I||_F = sqrt(1² + 1² + 1²) = sqrt(3)
        REQUIRE(I.frobenius_norm() == Approx(std::sqrt(3.0)));
    }

    SECTION("frobenius norm of zero matrix") {
        mathmatrix<double> M(2, 2, {0.0, 0.0, 0.0, 0.0});
        REQUIRE(M.frobenius_norm() == Approx(0.0));
    }
}

TEST_CASE("mathmatrix frobenius norm with complex numbers", "[mathmatrix]") {
    using cd = std::complex<double>;

    SECTION("frobenius norm of complex matrix") {
        mathmatrix<cd> M(2, 2, {cd(3.0, 4.0), cd(0.0, 0.0), cd(0.0, 0.0), cd(0.0, 0.0)});
        // ||M||_F = sqrt(|3+4i|²) = sqrt(25) = 5
        double norm = M.frobenius_norm();
        REQUIRE(norm == Approx(5.0));
    }

    SECTION("frobenius norm of complex matrix with multiple elements") {
        mathmatrix<cd> M(2, 2, {cd(1.0, 0.0), cd(0.0, 1.0), cd(1.0, 1.0), cd(0.0, 0.0)});
        // |1|² + |i|² + |1+i|² + |0|² = 1 + 1 + 2 + 0 = 4
        // ||M||_F = sqrt(4) = 2
        double norm = M.frobenius_norm();
        REQUIRE(norm == Approx(2.0));
    }
}

TEST_CASE("mathmatrix inverse operations", "[mathmatrix]") {
    SECTION("inverse of 2x2") {
        mathmatrix<double> A(2, 2, {4.0, 7.0, 2.0, 6.0});
        auto A_inv = A.inverse();
        auto I = A * A_inv;

        // Should be approximately identity
        REQUIRE(I(0, 0) == Approx(1.0).margin(1e-10));
        REQUIRE(I(1, 1) == Approx(1.0).margin(1e-10));
        REQUIRE(I(0, 1) == Approx(0.0).margin(1e-10));
        REQUIRE(I(1, 0) == Approx(0.0).margin(1e-10));
    }

    SECTION("inverse of identity") {
        auto I = identity_matrix<double>(3);
        auto I_inv = I.inverse();
        REQUIRE(I_inv == I);
    }

    SECTION("invert in-place") {
        mathmatrix<double> A(2, 2, {4.0, 7.0, 2.0, 6.0});
        mathmatrix<double> A_original = A;
        A.invert();

        auto I = A_original * A;
        REQUIRE(I(0, 0) == Approx(1.0).margin(1e-10));
    }
}

TEST_CASE("mathmatrix transpose", "[mathmatrix]") {
    SECTION("transpose square matrix") {
        mathmatrix<double> M(2, 2, {
            1.0, 2.0,
            3.0, 4.0
        });

        M.transpose();

        REQUIRE(M.rows() == 2);
        REQUIRE(M.columns() == 2);
        REQUIRE(M(0, 0) == 1.0);
        REQUIRE(M(0, 1) == 3.0);
        REQUIRE(M(1, 0) == 2.0);
        REQUIRE(M(1, 1) == 4.0);
    }

    SECTION("transpose of non-square (not implemented - prints warning)") {
        mathmatrix<double> M(2, 3, {
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0
        });

        // transpose() now works for non-square matrices (swaps dimensions)
        M.transpose();
        REQUIRE(M.rows() == 3);
        REQUIRE(M.columns() == 2);

        // Verify the transpose is correct
        REQUIRE(M(0, 0) == 1.0);
        REQUIRE(M(0, 1) == 4.0);
        REQUIRE(M(1, 0) == 2.0);
        REQUIRE(M(1, 1) == 5.0);
        REQUIRE(M(2, 0) == 3.0);
        REQUIRE(M(2, 1) == 6.0);
    }
}

TEST_CASE("mathmatrix ones_and_zeros", "[mathmatrix]") {
    mathmatrix<double> M(2, 2, {0.0, 2.5, -3.0, 0.0});
    M.ones_and_zeros();

    REQUIRE(M(0, 0) == 0.0);
    REQUIRE(M(0, 1) == 1.0);
    REQUIRE(M(1, 0) == 1.0);
    REQUIRE(M(1, 1) == 0.0);
}

TEST_CASE("mathmatrix diagonal_matrix helper", "[mathmatrix]") {
    mathvector<double> v = {2.0, 3.0, 4.0};
    auto M = diagonal_matrix(v);

    REQUIRE(M.rows() == 3);
    REQUIRE(M.columns() == 3);
    REQUIRE(M(0, 0) == 2.0);
    REQUIRE(M(1, 1) == 3.0);
    REQUIRE(M(2, 2) == 4.0);
    REQUIRE(M(0, 1) == 0.0);
}

TEST_CASE("mathmatrix scalar assignment", "[mathmatrix]") {
    mathmatrix<double> M(2, 2);
    M = 5.0;  // Sets to 5 * Identity

    REQUIRE(M(0, 0) == 5.0);
    REQUIRE(M(1, 1) == 5.0);
    REQUIRE(M(0, 1) == 0.0);
    REQUIRE(M(1, 0) == 0.0);
}

TEST_CASE("mathmatrix edge cases", "[mathmatrix]") {
    SECTION("single element matrix") {
        mathmatrix<double> M(1, 1, {42.0});
        REQUIRE(M.det() == 42.0);
        REQUIRE(M.trace() == 42.0);
    }

    SECTION("zero matrix trace") {
        mathmatrix<double> M(2, 2, {0.0, 0.0, 0.0, 0.0});
        // Note: det() throws for singular matrices
        REQUIRE(M.trace() == 0.0);
    }

    SECTION("large matrix sum") {
        mathmatrix<double> M(10, 10, 1.0);
        REQUIRE(M.trace() == 10.0);
    }
}

TEST_CASE("mathmatrix-vector multiplication", "[mathmatrix]") {
    SECTION("basic matrix-vector multiply") {
        // 2x3 matrix times 3x1 vector = 2x1 vector
        mathmatrix<double> A(2, 3, {
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0
        });
        mathvector<double> x = {1.0, 2.0, 3.0};

        auto y = A * x;

        REQUIRE(y.size() == 2);
        REQUIRE(y[0] == Approx(14.0));  // 1*1 + 2*2 + 3*3 = 1 + 4 + 9 = 14
        REQUIRE(y[1] == Approx(32.0));  // 4*1 + 5*2 + 6*3 = 4 + 10 + 18 = 32
    }

    SECTION("identity matrix times vector") {
        mathmatrix<double> I(3, 3, {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0
        });
        mathvector<double> x = {2.0, 3.0, 4.0};

        auto y = I * x;

        REQUIRE(y[0] == Approx(2.0));
        REQUIRE(y[1] == Approx(3.0));
        REQUIRE(y[2] == Approx(4.0));
    }

    SECTION("zero matrix times vector") {
        mathmatrix<double> Z(2, 3, 0.0);
        mathvector<double> x = {1.0, 2.0, 3.0};

        auto y = Z * x;

        REQUIRE(y[0] == Approx(0.0));
        REQUIRE(y[1] == Approx(0.0));
    }

    SECTION("diagonal matrix times vector") {
        mathmatrix<double> D(3, 3, {
            2.0, 0.0, 0.0,
            0.0, 3.0, 0.0,
            0.0, 0.0, 4.0
        });
        mathvector<double> x = {1.0, 1.0, 1.0};

        auto y = D * x;

        REQUIRE(y[0] == Approx(2.0));
        REQUIRE(y[1] == Approx(3.0));
        REQUIRE(y[2] == Approx(4.0));
    }

    SECTION("single row matrix times vector") {
        mathmatrix<double> A(1, 3, {2.0, 3.0, 4.0});
        mathvector<double> x = {1.0, 2.0, 3.0};

        auto y = A * x;

        REQUIRE(y.size() == 1);
        REQUIRE(y[0] == Approx(20.0));  // 2*1 + 3*2 + 4*3 = 20
    }

    SECTION("single column matrix times single element vector") {
        mathmatrix<double> A(3, 1, {2.0, 3.0, 4.0});
        mathvector<double> x = {5.0};

        auto y = A * x;

        REQUIRE(y.size() == 3);
        REQUIRE(y[0] == Approx(10.0));
        REQUIRE(y[1] == Approx(15.0));
        REQUIRE(y[2] == Approx(20.0));
    }
}

TEST_CASE("mathmatrix invalid operations", "[mathmatrix]") {
    // Note: These operations use JLT_MATRIX_ASSERT which is assert() in debug mode
    // In release builds (NDEBUG), these checks are disabled
    // This test documents expected behavior

    SECTION("matrix-vector multiply with incompatible dimensions") {
        mathmatrix<double> A(2, 3);  // 2x3 matrix
        mathvector<double> x(2);     // 2x1 vector (should be 3x1)

        // A * x requires A.columns() == x.size()
        REQUIRE(A.columns() == 3);
        REQUIRE(x.size() == 2);
        REQUIRE(A.columns() != x.size());
        // In debug: assert fails
        // In release: undefined behavior
    }

    SECTION("matrix multiplication with incompatible dimensions") {
        mathmatrix<double> A(2, 3);  // 2x3
        mathmatrix<double> B(2, 2);  // 2x2 (should be 3xN)

        // A * B requires A.columns() == B.rows()
        REQUIRE(A.columns() == 3);
        REQUIRE(B.rows() == 2);
        REQUIRE(A.columns() != B.rows());
    }

    SECTION("addition with mismatched dimensions") {
        mathmatrix<double> A(2, 3);
        mathmatrix<double> B(3, 2);

        REQUIRE(A.rows() != B.rows());
        REQUIRE(A.columns() != B.columns());
    }
}

TEST_CASE("mathmatrix mathematical identities", "[mathmatrix]") {
    // Note: transpose() modifies the matrix in-place (for square matrices only)
    // It returns a reference to *this after transposing

    SECTION("transpose properties (square matrix)") {
        mathmatrix<double> A_orig(3, 3, {
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0,
            7.0, 8.0, 9.0
        });

        // Make a copy to transpose
        mathmatrix<double> A(A_orig);
        A.transpose();

        // Check transpose swapped rows and columns
        REQUIRE(A(0, 0) == A_orig(0, 0));
        REQUIRE(A(0, 1) == A_orig(1, 0));
        REQUIRE(A(0, 2) == A_orig(2, 0));
        REQUIRE(A(1, 0) == A_orig(0, 1));
        REQUIRE(A(2, 1) == A_orig(1, 2));
    }

    SECTION("transpose of transpose returns original (square matrix)") {
        mathmatrix<double> A_orig(2, 2, {
            1.0, 2.0,
            3.0, 4.0
        });

        mathmatrix<double> A(A_orig);
        A.transpose();  // First transpose
        A.transpose();  // Second transpose

        // Should be back to original
        for (auto i = 0u; i < A.rows(); ++i) {
            for (auto j = 0u; j < A.columns(); ++j) {
                REQUIRE(A(i, j) == Approx(A_orig(i, j)));
            }
        }
    }

    SECTION("transpose of product (AB)^T = B^T A^T (square matrices)") {
        mathmatrix<double> A(2, 2, {
            1.0, 2.0,
            3.0, 4.0
        });
        mathmatrix<double> B(2, 2, {
            5.0, 6.0,
            7.0, 8.0
        });

        // Compute (AB)^T
        auto AB = A * B;
        AB.transpose();

        // Compute B^T A^T
        mathmatrix<double> B_T(B);
        B_T.transpose();
        mathmatrix<double> A_T(A);
        A_T.transpose();
        auto B_T_A_T = B_T * A_T;

        // Check all elements match
        for (auto i = 0u; i < AB.rows(); ++i) {
            for (auto j = 0u; j < AB.columns(); ++j) {
                REQUIRE(AB(i, j) == Approx(B_T_A_T(i, j)));
            }
        }
    }

    SECTION("transpose of sum (A+B)^T = A^T + B^T (square matrices)") {
        mathmatrix<double> A(2, 2, {
            1.0, 2.0,
            3.0, 4.0
        });
        mathmatrix<double> B(2, 2, {
            5.0, 6.0,
            7.0, 8.0
        });

        // Compute (A+B)^T
        auto AplusB = A + B;
        AplusB.transpose();

        // Compute A^T + B^T
        mathmatrix<double> A_T(A);
        A_T.transpose();
        mathmatrix<double> B_T(B);
        B_T.transpose();
        auto A_T_plusB_T = A_T + B_T;

        // Check all elements match
        for (auto i = 0u; i < AplusB.rows(); ++i) {
            for (auto j = 0u; j < AplusB.columns(); ++j) {
                REQUIRE(AplusB(i, j) == Approx(A_T_plusB_T(i, j)));
            }
        }
    }

    SECTION("distributive property A(B+C) = AB + AC") {
        mathmatrix<double> A(2, 2, {
            1.0, 2.0,
            3.0, 4.0
        });
        mathmatrix<double> B(2, 2, {
            5.0, 6.0,
            7.0, 8.0
        });
        mathmatrix<double> C(2, 2, {
            9.0, 10.0,
            11.0, 12.0
        });

        auto BplusC = B + C;
        auto A_BplusC = A * BplusC;

        auto AB = A * B;
        auto AC = A * C;
        auto AB_plusAC = AB + AC;

        for (auto i = 0u; i < A_BplusC.rows(); ++i) {
            for (auto j = 0u; j < A_BplusC.columns(); ++j) {
                REQUIRE(A_BplusC(i, j) == Approx(AB_plusAC(i, j)));
            }
        }
    }

    SECTION("associative property (AB)C = A(BC)") {
        mathmatrix<double> A(2, 3, {
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0
        });
        mathmatrix<double> B(3, 2, {
            1.0, 2.0,
            3.0, 4.0,
            5.0, 6.0
        });
        mathmatrix<double> C(2, 2, {
            7.0, 8.0,
            9.0, 10.0
        });

        auto AB = A * B;
        auto AB_C = AB * C;

        auto BC = B * C;
        auto A_BC = A * BC;

        REQUIRE(AB_C.rows() == A_BC.rows());
        REQUIRE(AB_C.columns() == A_BC.columns());

        for (auto i = 0u; i < AB_C.rows(); ++i) {
            for (auto j = 0u; j < AB_C.columns(); ++j) {
                REQUIRE(AB_C(i, j) == Approx(A_BC(i, j)));
            }
        }
    }

    SECTION("identity matrix properties") {
        mathmatrix<double> I(3, 3, {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0
        });
        mathmatrix<double> A(3, 3, {
            2.0, 3.0, 4.0,
            5.0, 6.0, 7.0,
            8.0, 9.0, 10.0
        });

        // I * A = A
        auto IA = I * A;
        for (auto i = 0u; i < A.rows(); ++i) {
            for (auto j = 0u; j < A.columns(); ++j) {
                REQUIRE(IA(i, j) == Approx(A(i, j)));
            }
        }

        // A * I = A
        auto AI = A * I;
        for (auto i = 0u; i < A.rows(); ++i) {
            for (auto j = 0u; j < A.columns(); ++j) {
                REQUIRE(AI(i, j) == Approx(A(i, j)));
            }
        }
    }
}

TEST_CASE("mathmatrix adjoint/hermitian operations", "[mathmatrix][adjoint]") {
    SECTION("adjoint of real matrix is same as transpose") {
        mathmatrix<double> M(2, 2, {
            1.0, 2.0,
            3.0, 4.0
        });

        mathmatrix<double> M_transpose(M);
        M_transpose.transpose();

        mathmatrix<double> M_adjoint(M);
        M_adjoint.adjoint();

        // For real matrices, adjoint == transpose
        for (auto i = 0u; i < M.rows(); ++i) {
            for (auto j = 0u; j < M.columns(); ++j) {
                REQUIRE(M_adjoint(i, j) == M_transpose(i, j));
            }
        }
    }

    SECTION("adjoint of complex matrix") {
        const std::complex<double> i(0, 1);
        mathmatrix<std::complex<double>> M(2, 2, {
            std::complex<double>(1, 2), std::complex<double>(3, 4),
            std::complex<double>(5, 6), std::complex<double>(7, 8)
        });

        mathmatrix<std::complex<double>> M_adj(M);
        M_adj.adjoint();

        // M_adj should be conjugate transpose
        // M_adj(0,0) = conj(M(0,0)) = 1-2i
        REQUIRE(M_adj(0, 0).real() == Approx(1.0));
        REQUIRE(M_adj(0, 0).imag() == Approx(-2.0));
        // M_adj(0,1) = conj(M(1,0)) = 5-6i
        REQUIRE(M_adj(0, 1).real() == Approx(5.0));
        REQUIRE(M_adj(0, 1).imag() == Approx(-6.0));
        // M_adj(1,0) = conj(M(0,1)) = 3-4i
        REQUIRE(M_adj(1, 0).real() == Approx(3.0));
        REQUIRE(M_adj(1, 0).imag() == Approx(-4.0));
        // M_adj(1,1) = conj(M(1,1)) = 7-8i
        REQUIRE(M_adj(1, 1).real() == Approx(7.0));
        REQUIRE(M_adj(1, 1).imag() == Approx(-8.0));
    }

    SECTION("standalone adjoint function") {
        const std::complex<double> i(0, 1);
        mathmatrix<std::complex<double>> M(2, 2, {
            1.0 + 2.0*i, 3.0 + 4.0*i,
            5.0 + 6.0*i, 7.0 + 8.0*i
        });

        auto M_adj = adjoint(M);

        // Original should be unchanged
        REQUIRE(M(0, 0).real() == Approx(1.0));
        REQUIRE(M(0, 0).imag() == Approx(2.0));

        // M_adj should be conjugate transpose
        REQUIRE(M_adj(0, 0).real() == Approx(1.0));
        REQUIRE(M_adj(0, 0).imag() == Approx(-2.0));
        REQUIRE(M_adj(0, 1).real() == Approx(5.0));
        REQUIRE(M_adj(0, 1).imag() == Approx(-6.0));
    }

    SECTION("hermitian_transpose and hermitian_conjugate aliases") {
        const std::complex<double> i(0, 1);
        mathmatrix<std::complex<double>> M(2, 2, {
            1.0 + i, 2.0,
            3.0, 4.0 - i
        });

        auto M_ht = hermitian_transpose(M);
        auto M_hc = hermitian_conjugate(M);
        auto M_adj = adjoint(M);

        // All three should be identical
        for (auto r = 0u; r < M.rows(); ++r) {
            for (auto c = 0u; c < M.columns(); ++c) {
                REQUIRE(M_ht(r, c) == M_adj(r, c));
                REQUIRE(M_hc(r, c) == M_adj(r, c));
            }
        }
    }
}

TEST_CASE("mathmatrix symmetry tests", "[mathmatrix][symmetric][hermitian]") {
    SECTION("symmetric real matrix") {
        mathmatrix<double> M(3, 3, {
            1.0, 2.0, 3.0,
            2.0, 4.0, 5.0,
            3.0, 5.0, 6.0
        });

        REQUIRE(M.is_symmetric());
        REQUIRE(is_symmetric(M));

        // Hermitian check should also pass for real symmetric matrices
        REQUIRE(M.is_hermitian());
        REQUIRE(is_hermitian(M));
    }

    SECTION("non-symmetric real matrix") {
        mathmatrix<double> M(3, 3, {
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0,
            7.0, 8.0, 9.0
        });

        REQUIRE_FALSE(M.is_symmetric());
        REQUIRE_FALSE(is_symmetric(M));
        REQUIRE_FALSE(M.is_hermitian());
        REQUIRE_FALSE(is_hermitian(M));
    }

    SECTION("hermitian complex matrix") {
        const std::complex<double> i(0, 1);
        mathmatrix<std::complex<double>> M(3, 3);

        // Create hermitian matrix: diagonal real, off-diagonal conjugate pairs
        M(0, 0) = 1.0;
        M(0, 1) = 1.0 + i;
        M(0, 2) = 2.0 - i;
        M(1, 0) = 1.0 - i;  // conj(M(0,1))
        M(1, 1) = 2.0;
        M(1, 2) = 0.5 + 0.5*i;
        M(2, 0) = 2.0 + i;  // conj(M(0,2))
        M(2, 1) = 0.5 - 0.5*i;  // conj(M(1,2))
        M(2, 2) = 3.0;

        REQUIRE(M.is_hermitian());
        REQUIRE(is_hermitian(M));

        // Complex hermitian is NOT symmetric (unless all entries are real)
        REQUIRE_FALSE(M.is_symmetric());
    }

    SECTION("non-hermitian complex matrix") {
        const std::complex<double> i(0, 1);
        mathmatrix<std::complex<double>> M(2, 2, {
            1.0 + i, 2.0,
            3.0, 4.0
        });

        // Diagonal has imaginary part, so not hermitian
        REQUIRE_FALSE(M.is_hermitian());
        REQUIRE_FALSE(is_hermitian(M));
    }

    SECTION("identity is both symmetric and hermitian") {
        mathmatrix<std::complex<double>> I(3, 3);
        for (auto i = 0u; i < 3; ++i) {
            for (auto j = 0u; j < 3; ++j) {
                I(i, j) = (i == j) ? 1.0 : 0.0;
            }
        }

        REQUIRE(I.is_symmetric());
        REQUIRE(I.is_hermitian());
    }

    SECTION("tolerance parameter for is_symmetric") {
        mathmatrix<double> M(2, 2, {
            1.0, 2.0 + 1e-8,
            2.0, 3.0
        });

        // Should fail with default (tight) tolerance
        REQUIRE_FALSE(M.is_symmetric());

        // Should pass with looser tolerance
        REQUIRE(M.is_symmetric(1e-6));
    }
}

TEST_CASE("mathmatrix non-negativity test", "[mathmatrix][nonnegative]") {
    SECTION("non-negative matrices") {
        mathmatrix<double> A(2, 2, {1.0, 2.0, 3.0, 4.0});
        REQUIRE(A.is_nonnegative());

        mathmatrix<double> B(2, 2, {0.0, 1.0, 2.0, 0.0});  // with zeros
        REQUIRE(B.is_nonnegative());

        mathmatrix<int> C(2, 2, {0, 5, 10, 0});
        REQUIRE(C.is_nonnegative());
    }

    SECTION("matrices with negative elements") {
        mathmatrix<double> A(2, 2, {-1.0, 2.0, 3.0, 4.0});
        REQUIRE_FALSE(A.is_nonnegative());

        mathmatrix<double> B(2, 2, {1.0, 2.0, 3.0, -0.001});
        REQUIRE_FALSE(B.is_nonnegative());
    }

    SECTION("empty matrix is non-negative (vacuous truth)") {
        mathmatrix<double> M;
        REQUIRE(M.is_nonnegative());
    }
}

TEST_CASE("mathmatrix primitivity test", "[mathmatrix][primitive]") {
    SECTION("primitive matrices") {
        // All-ones matrix is primitive (already positive at power 1)
        mathmatrix<double> A(3, 3, {
            1.0, 1.0, 1.0,
            1.0, 1.0, 1.0,
            1.0, 1.0, 1.0
        });
        REQUIRE(A.is_primitive());

        // Nearly-full matrix (aperiodic and irreducible)
        mathmatrix<double> B(3, 3, {
            1.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 0.0, 1.0
        });
        REQUIRE(B.is_primitive());

        // 1x1 matrix with positive entry is primitive
        mathmatrix<double> C(1, 1, {1.0});
        REQUIRE(C.is_primitive());
    }

    SECTION("non-primitive matrices") {
        // Identity matrix for n > 1 is NOT primitive (has zeros off-diagonal)
        mathmatrix<double> I(2, 2, {
            1.0, 0.0,
            0.0, 1.0
        });
        REQUIRE_FALSE(I.is_primitive());

        // Alternating permutation (period 2)
        mathmatrix<double> A(2, 2, {
            0.0, 1.0,
            1.0, 0.0
        });
        REQUIRE_FALSE(A.is_primitive());

        // Cyclic permutation (period 3)
        mathmatrix<double> B(3, 3, {
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 0.0, 0.0
        });
        REQUIRE_FALSE(B.is_primitive());

        // Zero matrix is not primitive
        mathmatrix<double> Z(2, 2, {
            0.0, 0.0,
            0.0, 0.0
        });
        REQUIRE_FALSE(Z.is_primitive());
    }

    SECTION("negative matrix throws exception") {
        mathmatrix<double> A(2, 2, {-1.0, 2.0, 3.0, 4.0});
        REQUIRE_THROWS_AS(A.is_primitive(), std::domain_error);

        mathmatrix<double> B(2, 2, {1.0, 2.0, -0.5, 4.0});
        REQUIRE_THROWS_AS(B.is_primitive(), std::domain_error);
    }
}

TEST_CASE("mathmatrix reducibility test", "[mathmatrix][reducible]") {
    SECTION("reducible matrices") {
        // Identity matrix (n > 1) is reducible (block diagonal)
        mathmatrix<double> I(3, 3, {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0
        });
        REQUIRE(I.is_reducible());

        // Diagonal matrix is reducible
        mathmatrix<double> D(2, 2, {
            2.0, 0.0,
            0.0, 3.0
        });
        REQUIRE(D.is_reducible());

        // Block upper triangular
        mathmatrix<double> B(3, 3, {
            1.0, 1.0, 0.0,
            0.0, 1.0, 1.0,
            0.0, 0.0, 1.0
        });
        REQUIRE(B.is_reducible());

        // Lower triangular with positive diagonal
        mathmatrix<double> L(3, 3, {
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 1.0
        });
        REQUIRE(L.is_reducible());
    }

    SECTION("irreducible matrices") {
        // Cyclic permutation is irreducible (strongly connected)
        mathmatrix<double> C(3, 3, {
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 0.0, 0.0
        });
        REQUIRE_FALSE(C.is_reducible());

        // All-ones matrix is irreducible (fully connected)
        mathmatrix<double> A(3, 3, {
            1.0, 1.0, 1.0,
            1.0, 1.0, 1.0,
            1.0, 1.0, 1.0
        });
        REQUIRE_FALSE(A.is_reducible());

        // Alternating permutation is irreducible
        mathmatrix<double> P(2, 2, {
            0.0, 1.0,
            1.0, 0.0
        });
        REQUIRE_FALSE(P.is_reducible());

        // 1x1 matrix is considered irreducible
        mathmatrix<double> S(1, 1, {1.0});
        REQUIRE_FALSE(S.is_reducible());
    }

    SECTION("negative matrix throws exception") {
        mathmatrix<double> A(2, 2, {-1.0, 2.0, 3.0, 4.0});
        REQUIRE_THROWS_AS(A.is_reducible(), std::domain_error);

        mathmatrix<double> B(2, 2, {1.0, 2.0, -0.5, 4.0});
        REQUIRE_THROWS_AS(B.is_reducible(), std::domain_error);
    }
}
