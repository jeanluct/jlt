#include "catch.hpp"
#include "../jlt/exceptions.hpp"
#include "../jlt/matrixutil.hpp"
#include "../jlt/mathmatrix.hpp"
#include <cmath>

using namespace jlt;

TEST_CASE("LU decomposition", "[matrixutil]") {
    SECTION("LUdecomp on 2x2 matrix") {
        mathmatrix<double> A(2, 2, {4.0, 3.0, 6.0, 3.0});
        int row_index[2];
        int perm;
        
        LUdecomp<double, mathmatrix<double>>(A, row_index, &perm);
        
        // After LU decomposition, A contains L and U
        // L has 1s on diagonal, U is upper triangular
        REQUIRE(std::abs(A(0,0)) > 0.0);
        REQUIRE(std::abs(A(1,1)) > 0.0);
    }
    
    SECTION("LUdecomp on identity matrix") {
        mathmatrix<double> A(3, 3, {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0
        });
        int row_index[3];
        int perm;
        
        LUdecomp<double, mathmatrix<double>>(A, row_index, &perm);
        
        // For identity, perm should remain 1 (no row swaps needed)
        REQUIRE(std::abs(perm) == 1);
    }
    
    SECTION("LUdecomp on zero matrix") {
        mathmatrix<double> A(2, 2, {0.0, 0.0, 0.0, 0.0});
        int row_index[2];
        int perm;
        
        REQUIRE_THROWS(LUdecomp<double, mathmatrix<double>>(A, row_index, &perm));
    }
}

TEST_CASE("LU back substitution", "[matrixutil]") {
    SECTION("Solve simple 2x2 system") {
        mathmatrix<double> A(2, 2, {2.0, 1.0, 1.0, 3.0});
        int row_index[2];
        int perm;
        
        LUdecomp<double, mathmatrix<double>>(A, row_index, &perm);
        
        double b[2] = {5.0, 8.0};
        LUbacksub<double, mathmatrix<double>>(A, row_index, b);
        
        REQUIRE(b[0] == Approx(1.4).margin(1e-10));
        REQUIRE(b[1] == Approx(2.2).margin(1e-10));
    }
    
    SECTION("Solve 3x3 diagonal system") {
        mathmatrix<double> A(3, 3, {
            2.0, 0.0, 0.0,
            0.0, 3.0, 0.0,
            0.0, 0.0, 4.0
        });
        int row_index[3];
        int perm;
        
        LUdecomp<double, mathmatrix<double>>(A, row_index, &perm);
        
        double b[3] = {4.0, 9.0, 16.0};
        LUbacksub<double, mathmatrix<double>>(A, row_index, b);
        
        REQUIRE(b[0] == Approx(2.0).margin(1e-10));
        REQUIRE(b[1] == Approx(3.0).margin(1e-10));
        REQUIRE(b[2] == Approx(4.0).margin(1e-10));
    }
}

TEST_CASE("Matrix inverse via LU", "[matrixutil]") {
    SECTION("Inverse of 2x2 matrix") {
        mathmatrix<double> A(2, 2, {4.0, 7.0, 2.0, 6.0});
        mathmatrix<double> A_original = A;
        mathmatrix<double> Ainv = inverse<double, mathmatrix<double>>(A);
        
        // Verify: A_original * Ainv = I (A was modified by inverse())
        mathmatrix<double> I = A_original * Ainv;
        
        REQUIRE(I(0, 0) == Approx(1.0).margin(1e-5));
        REQUIRE(I(0, 1) == Approx(0.0).margin(1e-5));
        REQUIRE(I(1, 0) == Approx(0.0).margin(1e-5));
        REQUIRE(I(1, 1) == Approx(1.0).margin(1e-5));
    }
    
    SECTION("Inverse of identity") {
        mathmatrix<double> I(3, 3);
        I.identity();
        
        mathmatrix<double> Iinv = inverse<double, mathmatrix<double>>(I);
        
        REQUIRE(Iinv(0, 0) == Approx(1.0).margin(1e-10));
        REQUIRE(Iinv(1, 1) == Approx(1.0).margin(1e-10));
        REQUIRE(Iinv(2, 2) == Approx(1.0).margin(1e-10));
    }
    
    SECTION("Inverse of diagonal matrix") {
        mathmatrix<double> D(3, 3, {
            2.0, 0.0, 0.0,
            0.0, 3.0, 0.0,
            0.0, 0.0, 4.0
        });
        
        mathmatrix<double> Dinv = inverse<double, mathmatrix<double>>(D);
        
        REQUIRE(Dinv(0, 0) == Approx(0.5).margin(1e-10));
        REQUIRE(Dinv(1, 1) == Approx(1.0/3.0).margin(1e-10));
        REQUIRE(Dinv(2, 2) == Approx(0.25).margin(1e-10));
    }
}

TEST_CASE("QR decomposition", "[matrixutil]") {
    SECTION("QRdecomp on 2x2 matrix") {
        mathmatrix<double> A(2, 2, {4.0, 3.0, 6.0, 3.0});
        mathvector<double> c(2);
        mathvector<double> d(2);
        
        bool singular = QRdecomp<double, mathmatrix<double>, mathvector<double>>(A, c, d);
        
        REQUIRE_FALSE(singular);
    }
    
    SECTION("QRdecomp on singular matrix") {
        mathmatrix<double> A(2, 2, {0.0, 1.0, 0.0, 2.0});
        mathvector<double> c(2);
        mathvector<double> d(2);
        
        bool singular = QRdecomp<double, mathmatrix<double>, mathvector<double>>(A, c, d);
        
        // QR decomposition detects singularity
        REQUIRE(singular);
    }
    
    SECTION("Full QR decomposition - verify Q is orthogonal") {
        mathmatrix<double> A(3, 3, {
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0,
            7.0, 8.0, 10.0
        });
        
        mathmatrix<double> Q(3, 3);
        mathmatrix<double> R(3, 3);
        
        bool singular = QRdecomp<double, mathmatrix<double>, mathvector<double>>(A, Q, R);
        
        REQUIRE_FALSE(singular);
        
        // Verify Q is orthogonal (Q^T * Q = I)
        mathmatrix<double> QT = Q;
        QT.transpose();
        mathmatrix<double> I_approx = QT * Q;
        
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (i == j) {
                    REQUIRE(I_approx(i, j) == Approx(1.0).margin(1e-10));
                } else {
                    REQUIRE(I_approx(i, j) == Approx(0.0).margin(1e-10));
                }
            }
        }
    }
    
    SECTION("QR on identity matrix") {
        mathmatrix<double> I(3, 3);
        I.identity();
        
        mathmatrix<double> Q(3, 3);
        mathmatrix<double> R(3, 3);
        
        bool singular = QRdecomp<double, mathmatrix<double>, mathvector<double>>(I, Q, R);
        
        REQUIRE_FALSE(singular);
        
        REQUIRE(Q(0, 0) == Approx(1.0).margin(1e-10));
        REQUIRE(R(0, 0) == Approx(1.0).margin(1e-10));
    }
}

TEST_CASE("Gram-Schmidt orthonormalization", "[matrixutil]") {
    SECTION("Orthonormalize 2x2 matrix rows") {
        mathmatrix<double> A(2, 2, {3.0, 4.0, 1.0, 0.0});
        
        GramSchmidtOrthonorm<mathmatrix<double>>(A);
        
        double norm1 = std::sqrt(A(0,0)*A(0,0) + A(0,1)*A(0,1));
        REQUIRE(norm1 == Approx(1.0).margin(1e-10));
    }
    
    SECTION("Orthonormalize identity (should remain identity)") {
        mathmatrix<double> I(3, 3);
        I.identity();
        
        GramSchmidtOrthonorm<mathmatrix<double>>(I);
        
        REQUIRE(I(0, 0) == Approx(1.0).margin(1e-10));
        REQUIRE(I(1, 1) == Approx(1.0).margin(1e-10));
        REQUIRE(I(2, 2) == Approx(1.0).margin(1e-10));
    }
    
    SECTION("Orthonormalize simple matrix") {
        mathmatrix<double> A(2, 2, {1.0, 0.0, 0.0, 2.0});
        
        GramSchmidtOrthonorm<mathmatrix<double>>(A);
        
        REQUIRE(A(0, 0) == Approx(1.0).margin(1e-10));
        REQUIRE(A(0, 1) == Approx(0.0).margin(1e-10));
        REQUIRE(A(1, 0) == Approx(0.0).margin(1e-10));
        REQUIRE(A(1, 1) == Approx(1.0).margin(1e-10));
    }
}

TEST_CASE("Matrix utility edge cases", "[matrixutil]") {
    SECTION("LU on 1x1 matrix") {
        mathmatrix<double> A(1, 1, {5.0});
        int row_index[1];
        int perm;
        
        LUdecomp<double, mathmatrix<double>>(A, row_index, &perm);
        
        double b[1] = {10.0};
        LUbacksub<double, mathmatrix<double>>(A, row_index, b);
        
        REQUIRE(b[0] == Approx(2.0).margin(1e-10));
    }
    
    SECTION("Inverse of 1x1 matrix") {
        mathmatrix<double> A(1, 1, {5.0});
        
        mathmatrix<double> Ainv = inverse<double, mathmatrix<double>>(A);
        
        REQUIRE(Ainv(0, 0) == Approx(0.2).margin(1e-10));
    }
    
    SECTION("QR on 2x2 identity matrix") {
        mathmatrix<double> I(2, 2);
        I.identity();
        
        mathmatrix<double> Q(2, 2);
        mathmatrix<double> R(2, 2);
        
        bool singular = QRdecomp<double, mathmatrix<double>, mathvector<double>>(I, Q, R);
        
        REQUIRE_FALSE(singular);
        
        REQUIRE(Q(0, 0) == Approx(1.0).margin(1e-10));
        REQUIRE(R(0, 0) == Approx(1.0).margin(1e-10));
    }
}
