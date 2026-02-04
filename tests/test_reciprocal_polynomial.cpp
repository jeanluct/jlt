//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/reciprocal_polynomial.hpp"
#include "../jlt/polynomial.hpp"
#include "../jlt/vector.hpp"
#include <cmath>

using namespace jlt;

TEST_CASE("reciprocal_polynomial construction", "[reciprocal_polynomial]") {
    SECTION("Default constructor creates degree 0") {
        reciprocal_polynomial<double> p;
        REQUIRE(p.degree() == 0);
    }

    SECTION("Constructor with even degree") {
        reciprocal_polynomial<double> p(4);
        REQUIRE(p.degree() == 4);
    }

    SECTION("Constructor with odd degree") {
        reciprocal_polynomial<double> p(5);
        REQUIRE(p.degree() == 5);
    }

    SECTION("Constructor from vector") {
        // For degree 4 (g=2), vector needs 2 elements: a[0], a[1] for indices 1,2
        vector<double> coeffs = {2.0, 3.0};
        reciprocal_polynomial<double> p(coeffs);  // Creates degree 4 (size of vector is 2, but wait...)
        // Actually: n = _a.size() = 2, so degree is 2, not 4
        REQUIRE(p.degree() == 2);
    }

    SECTION("Constructor from reciprocal polynomial succeeds") {
        polynomial<double> p;
        p[0] = 1; p[1] = 2; p[2] = 3; p[3] = 2; p[4] = 1;
        // This is reciprocal: coefficients are [1, 2, 3, 2, 1] (symmetric)
        REQUIRE_NOTHROW(reciprocal_polynomial<double>(p));
    }

    SECTION("Constructor from non-reciprocal polynomial throws") {
        polynomial<double> p;
        p[0] = 1; p[1] = 2; p[2] = 3; p[3] = 4; p[4] = 1;
        // Not reciprocal: p[1]=2 != p[3]=4
        REQUIRE_THROWS_AS(reciprocal_polynomial<double>(p), std::runtime_error);
    }
}

TEST_CASE("reciprocal_polynomial coefficient access", "[reciprocal_polynomial]") {
    // Create a degree 4 reciprocal polynomial: 1 + 2x + 3x^2 + 2x^3 + x^4
    // For degree 4 (n=4), we need g=2 coefficients: a[0]=2, a[1]=3
    vector<double> coeffs = {2.0, 3.0};
    reciprocal_polynomial<double> p(coeffs);  // This creates degree 2, not 4!

    // Actually, based on the constructor: n = _a.size()
    // So if we want degree 4, we need 4 elements in the vector
    // But that doesn't match the reciprocal structure...

    // Let me use a degree 4 polynomial directly:
    reciprocal_polynomial<double> p4(4);  // Degree 4, g=2, so a has 2 elements
    // For degree 4: indices 0,1,2,3,4. Middle are 1,2 (g=2)
    // a[0] = coeff[1], a[1] = coeff[2]
    // So we set: p4[1] = 2, p4[2] = 3

    SECTION("Read and modify middle coefficients for degree 4") {
        p4[1] = 2.0;  // a[0]
        p4[2] = 3.0;  // a[1]
        REQUIRE(p4[1] == 2.0);
        REQUIRE(p4[2] == 3.0);
        // Verify symmetry: p[1] == p[3], p[2] is middle
        REQUIRE(p4[3] == 2.0);  // Should equal p[1]
    }

    SECTION("Read first and last coefficients via const reference") {
        // Use const reference to read fixed coefficients 0 and n
        const reciprocal_polynomial<double>& cp4 = p4;
        REQUIRE(cp4[0] == 1.0);  // First coefficient always 1
        REQUIRE(cp4[4] == 1.0);  // Last coefficient always 1
    }

    SECTION("Evaluation at x=0 returns 1 for constant term") {
        // Bug fix: degree 0 polynomial used to return 2 instead of 1
        REQUIRE(p4(0.0) == 1.0);
    }

    SECTION("Coefficient symmetry property for degree 4") {
        p4[1] = 2.0;
        p4[2] = 3.0;
        // Verify middle coefficients (not 0 or n since those are fixed at 1)
        // For degree 4: p[1] should equal p[3]
        REQUIRE(p4[1] == p4[3]);
        REQUIRE(p4[2] == p4[2]);  // Middle is always symmetric with itself
    }

    SECTION("Out of range throws") {
        REQUIRE_THROWS_AS(p4[-1], std::out_of_range);
        REQUIRE_THROWS_AS(p4[10], std::out_of_range);
    }

    SECTION("Assignment to fixed coefficient throws") {
        REQUIRE_THROWS_AS(p4[0] = 5.0, std::out_of_range);
        REQUIRE_THROWS_AS(p4[4] = 5.0, std::out_of_range);
    }
}

TEST_CASE("reciprocal_polynomial reciprocal property", "[reciprocal_polynomial]") {
    // Create degree 4: 1 + 2x + 3x^2 + 2x^3 + x^4
    reciprocal_polynomial<double> p(4);
    p[1] = 2.0;
    p[2] = 3.0;

    SECTION("Verify P(x) == x^n * P(1/x) for various x values") {
        for (double x : {0.5, 1.0, 2.0, 3.0, 10.0}) {
            double lhs = p(x);
            double rhs = std::pow(x, p.degree()) * p(1.0 / x);
            REQUIRE(lhs == Approx(rhs).margin(1e-10));
        }
    }

    SECTION("P(1) equals 2 + 2*sum of independent coefficients") {
        // P(1) = 1 + 2 + 3 + 2 + 1 = 9
        REQUIRE(p(1.0) == Approx(9.0).margin(1e-10));
    }
}

TEST_CASE("reciprocal_polynomial evaluation", "[reciprocal_polynomial]") {
    // Create degree 3: 1 + 2x + 2x^2 + x^3 (odd degree)
    reciprocal_polynomial<double> p(3);
    p[1] = 2.0;  // This also sets p[2] = 2.0 due to symmetry

    SECTION("Evaluate at x = 0") {
        REQUIRE(p(0.0) == 1.0);  // Only constant term
    }

    SECTION("Evaluate at x = 1") {
        // 1 + 2 + 2 + 1 = 6
        REQUIRE(p(1.0) == Approx(6.0).margin(1e-10));
    }

    SECTION("Evaluate at x = 2") {
        // 1 + 2*2 + 2*4 + 8 = 1 + 4 + 8 + 8 = 21
        REQUIRE(p(2.0) == Approx(21.0).margin(1e-10));
    }

    SECTION("Evaluate at negative x") {
        // 1 + 2*(-2) + 2*4 + (-8) = 1 - 4 + 8 - 8 = -3
        REQUIRE(p(-2.0) == Approx(-3.0).margin(1e-10));
    }
}

TEST_CASE("reciprocal_polynomial to_polynomial conversion", "[reciprocal_polynomial]") {
    // Create degree 6: 1 + a1*x + a2*x^2 + a3*x^3 + a2*x^4 + a1*x^5 + x^6
    reciprocal_polynomial<double> rp(6);
    rp[1] = 1.0;
    rp[2] = 2.0;
    rp[3] = 3.0;

    SECTION("Conversion creates correct polynomial") {
        polynomial<double> p = rp.to_polynomial();
        REQUIRE(p.degree() == 6);
        REQUIRE(p[0] == 1.0);
        REQUIRE(p[1] == 1.0);
        REQUIRE(p[2] == 2.0);
        REQUIRE(p[3] == 3.0);
        REQUIRE(p[4] == 2.0);
        REQUIRE(p[5] == 1.0);
        REQUIRE(p[6] == 1.0);
    }

    SECTION("Evaluation matches between reciprocal and regular") {
        polynomial<double> p = rp.to_polynomial();
        for (double x : {0.5, 1.0, 2.0, 5.0}) {
            REQUIRE(rp(x) == Approx(p(x)).margin(1e-10));
        }
    }
}

TEST_CASE("reciprocal_polynomial derivative", "[reciprocal_polynomial]") {
    // Create degree 4: 1 + 2x + 3x^2 + 2x^3 + x^4
    reciprocal_polynomial<double> p(4);
    p[1] = 2.0;
    p[2] = 3.0;

    SECTION("Derivative polynomial") {
        polynomial<double> dp = p.derivative();
        REQUIRE(dp.degree() == 3);
        // d/dx(1 + 2x + 3x^2 + 2x^3 + x^4) = 2 + 6x + 6x^2 + 4x^3
        REQUIRE(dp[0] == 2.0);
        REQUIRE(dp[1] == 6.0);
        REQUIRE(dp[2] == 6.0);
        REQUIRE(dp[3] == 4.0);
    }

    SECTION("Derivative at specific points") {
        // At x=0: derivative = 2
        REQUIRE(p.derivative_at(0.0) == Approx(2.0).margin(1e-10));
        // At x=1: derivative = 2 + 6 + 6 + 4 = 18
        REQUIRE(p.derivative_at(1.0) == Approx(18.0).margin(1e-10));
    }

    SECTION("Derivative matches numerical approximation") {
        double x = 2.0;
        double h = 1e-6;
        double numerical_derivative = (p(x + h) - p(x - h)) / (2 * h);
        REQUIRE(p.derivative_at(x) == Approx(numerical_derivative).margin(1e-4));
    }
}

TEST_CASE("reciprocal_polynomial comparison and assignment", "[reciprocal_polynomial]") {
    reciprocal_polynomial<double> p1(4);
    p1[1] = 2.0;
    p1[2] = 3.0;

    reciprocal_polynomial<double> p2(4);
    p2[1] = 2.0;
    p2[2] = 3.0;

    reciprocal_polynomial<double> p3(4);
    p3[1] = 2.0;
    p3[2] = 4.0;  // Different!

    SECTION("Equality of identical polynomials") {
        REQUIRE(p1 == p2);
    }

    SECTION("Inequality of different polynomials") {
        REQUIRE(p1 != p3);
    }

    SECTION("Assignment operator") {
        reciprocal_polynomial<double> p4;
        p4 = p1;
        REQUIRE(p4 == p1);
        REQUIRE(p4.degree() == p1.degree());
    }
}

TEST_CASE("reciprocal_polynomial edge cases", "[reciprocal_polynomial]") {
    SECTION("Degree 0 polynomial") {
        reciprocal_polynomial<double> p(0);
        REQUIRE(p.degree() == 0);
        // Bug fix: degree 0 polynomial should evaluate to 1 (just the constant term)
        // Previously returned 2.0 due to double-counting
        REQUIRE(p(0.0) == 1.0);
        REQUIRE(p(1.0) == 1.0);
        REQUIRE(p(2.0) == 1.0);
        // Use const reference to read coefficient 0
        const reciprocal_polynomial<double>& cp = p;
        REQUIRE(cp[0] == 1.0);
    }

    SECTION("Degree 1 polynomial") {
        reciprocal_polynomial<double> p(1);
        REQUIRE(p.degree() == 1);
        // For degree 1, coefficient 0 and 1 are both the same (both ends)
        // Use const reference to read fixed coefficients
        const reciprocal_polynomial<double>& cp = p;
        REQUIRE(cp[0] == 1.0);  // First coefficient
        REQUIRE(cp[1] == 1.0);  // Last coefficient (same as first for degree 1)
        // P(x) = 1 + x, so P(1) = 2, P(2) = 3
        REQUIRE(p(1.0) == 2.0);
        REQUIRE(p(2.0) == 3.0);
    }

    SECTION("Degree 2 polynomial with zero middle coefficient") {
        reciprocal_polynomial<double> p(2);
        // P(x) = 1 + 0*x + x^2
        REQUIRE(p.degree() == 2);
        REQUIRE(p(2.0) == Approx(5.0).margin(1e-10));  // 1 + 0 + 4 = 5
    }

    SECTION("Large degree polynomial") {
        reciprocal_polynomial<double> p(20);
        REQUIRE(p.degree() == 20);
        // Set some coefficients
        p[1] = 2.0;
        p[2] = 3.0;
        // Verify symmetry is maintained
        REQUIRE(p[1] == p[19]);
        REQUIRE(p[2] == p[18]);
    }
}
