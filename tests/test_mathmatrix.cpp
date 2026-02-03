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
        
        // transpose() prints warning to stderr for non-square matrices
        // but doesn't throw - dimensions remain unchanged
        // This test just verifies it doesn't crash
        M.transpose();
        REQUIRE(M.rows() == 2);
        REQUIRE(M.columns() == 3);
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
