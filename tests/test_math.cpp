//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/math.hpp"
#include <cmath>

using namespace jlt;

TEST_CASE("Mod function - basic operations", "[math][mod]") {
    SECTION("Mod with positive dividend and divisor") {
        REQUIRE(Mod(5.0, 3.0) == Approx(2.0));
        REQUIRE(Mod(5.0f, 3.0f) == Approx(2.0f));
    }

    SECTION("Mod with zero dividend") {
        REQUIRE(Mod(0.0, 5.0) == Approx(0.0));
        REQUIRE(Mod(0.0f, 5.0f) == Approx(0.0f));
    }

    SECTION("Mod where dividend equals divisor") {
        REQUIRE(Mod(5.0, 5.0) == Approx(0.0));
        REQUIRE(Mod(7.5f, 7.5f) == Approx(0.0f));
    }

    SECTION("Mod where dividend is less than divisor") {
        REQUIRE(Mod(3.0, 5.0) == Approx(3.0));
        REQUIRE(Mod(2.5f, 10.0f) == Approx(2.5f));
    }

    SECTION("Mod with large numbers") {
        REQUIRE(Mod(1000.0, 7.0) == Approx(1000.0 - 142 * 7.0));  // 1000 % 7 = 6
        REQUIRE(Mod(10000.0f, 100.0f) == Approx(0.0f));  // 10000 % 100 = 0
    }

    SECTION("Mod with floating point values") {
        REQUIRE(Mod(5.5, 2.0) == Approx(1.5));
        REQUIRE(Mod(3.3f, 1.5f) == Approx(0.3f).margin(1e-6f));
    }
}

TEST_CASE("Mod function - negative values", "[math][mod]") {
    SECTION("Mod with negative dividend (key feature of jlt::Mod)") {
        // Standard fmod(-1, 3) = -1, but Mod(-1, 3) = 2 (same sign as divisor)
        REQUIRE(Mod(-1.0, 3.0) == Approx(2.0));
        REQUIRE(Mod(-4.0, 3.0) == Approx(2.0));
        REQUIRE(Mod(-7.0, 3.0) == Approx(2.0));
    }

    SECTION("Mod with negative dividend - float") {
        REQUIRE(Mod(-1.0f, 3.0f) == Approx(2.0f));
        REQUIRE(Mod(-5.5f, 2.0f) == Approx(0.5f));
    }

    SECTION("Mod with negative divisor") {
        // Result should have same sign as divisor (negative)
        REQUIRE(Mod(5.0, -3.0) == Approx(-1.0));
        REQUIRE(Mod(7.0, -4.0) == Approx(-1.0));
    }

    SECTION("Mod with both negative") {
        // Both negative, result should be negative
        REQUIRE(Mod(-5.0, -3.0) == Approx(-2.0));
        REQUIRE(Mod(-1.0, -3.0) == Approx(-1.0));
    }

    SECTION("Mod preserves sign of divisor - positive case") {
        // This is the key property: result has same sign as b
        double result = Mod(-1.0/3.0, 1.0);
        REQUIRE(result > 0);  // Should be positive like the divisor
        REQUIRE(result == Approx(2.0/3.0).margin(1e-10));
    }
}

TEST_CASE("Mod function - different precisions", "[math][mod]") {
    SECTION("Mod with float") {
        float a = 7.5f, b = 2.5f;
        float result = Mod(a, b);
        REQUIRE(result == Approx(0.0f).margin(1e-6f));
    }

    SECTION("Mod with double") {
        double a = 10.5, b = 3.2;
        double result = Mod(a, b);
        // 10.5 / 3.2 = 3.28125, so remainder = 10.5 - 3*3.2 = 0.9
        REQUIRE(result == Approx(0.9).margin(1e-10));
    }

    SECTION("Mod with long double") {
        long double a = 15.0L, b = 4.0L;
        long double result = Mod(a, b);
        REQUIRE(result == Approx(3.0L).margin(1e-15L));
    }
}

TEST_CASE("Sign function", "[math][sign]") {
    SECTION("Sign of positive numbers") {
        REQUIRE(Sign(5.0) == 1);
        REQUIRE(Sign(1.0f) == 1);
        REQUIRE(Sign(0.001) == 1);
        REQUIRE(Sign(1000000.0L) == 1);
    }

    SECTION("Sign of negative numbers") {
        REQUIRE(Sign(-5.0) == -1);
        REQUIRE(Sign(-1.0f) == -1);
        REQUIRE(Sign(-0.001) == -1);
        REQUIRE(Sign(-1000000.0L) == -1);
    }

    SECTION("Sign of zero (returns 1, not 0)") {
        // Documentation says "This function never returns 0"
        REQUIRE(Sign(0.0) == 1);
        REQUIRE(Sign(0.0f) == 1);
        REQUIRE(Sign(0.0L) == 1);
        REQUIRE(Sign(-0.0) == 1);  // Negative zero is still >= 0
    }

    SECTION("Sign with integers") {
        REQUIRE(Sign(42) == 1);
        REQUIRE(Sign(-42) == -1);
        REQUIRE(Sign(0) == 1);
    }
}

TEST_CASE("Sign function - edge cases", "[math][sign]") {
    SECTION("Sign of very small positive") {
        REQUIRE(Sign(1e-300) == 1);
        REQUIRE(Sign(1e-30f) == 1);
    }

    SECTION("Sign of very small negative") {
        REQUIRE(Sign(-1e-300) == -1);
        REQUIRE(Sign(-1e-30f) == -1);
    }

    SECTION("Sign of very large positive") {
        REQUIRE(Sign(1e300) == 1);
        REQUIRE(Sign(1e30f) == 1);
    }

    SECTION("Sign of very large negative") {
        REQUIRE(Sign(-1e300) == -1);
        REQUIRE(Sign(-1e30f) == -1);
    }
}

TEST_CASE("Mod and Sign combined operations", "[math][combined]") {
    SECTION("Using Mod result with Sign") {
        // Mod always returns [0, b) for positive b, so Sign should always be 1
        double result = Mod(-10.0, 3.0);
        REQUIRE(result == Approx(2.0));
        REQUIRE(Sign(result) == 1);
    }

    SECTION("Sign with Mod result for negative divisor") {
        // Mod returns (-b, 0] for negative b, so Sign should be -1 or 1
        double result = Mod(5.0, -3.0);
        REQUIRE(result == Approx(-1.0));
        REQUIRE(Sign(result) == -1);
    }
}
