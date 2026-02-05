#include "catch.hpp"
#include "../jlt/mathvector.hpp"
#include <complex>

using namespace jlt;

TEST_CASE("mathvector basic operations", "[mathvector]") {
    SECTION("construction and initialization") {
        mathvector<double> v1;
        REQUIRE(v1.size() == 0);
        REQUIRE(v1.is_zero());

        mathvector<double> v2(5, 3.0);
        REQUIRE(v2.size() == 5);
        REQUIRE(v2[0] == 3.0);
        REQUIRE(v2[4] == 3.0);

        mathvector<double> v3 = {1.0, 2.0, 3.0, 4.0, 5.0};
        REQUIRE(v3.size() == 5);
        REQUIRE(v3[0] == 1.0);
        REQUIRE(v3[4] == 5.0);
    }

    SECTION("sum method") {
        mathvector<double> v = {1.0, 2.0, 3.0, 4.0, 5.0};
        REQUIRE(v.sum() == 15.0);

        mathvector<double> empty;
        REQUIRE(empty.sum() == 0.0);

        mathvector<double> single = {42.0};
        REQUIRE(single.sum() == 42.0);
    }

    SECTION("is_zero method") {
        mathvector<double> v1(3, 0.0);
        REQUIRE(v1.is_zero());

        mathvector<double> v2 = {0.0, 0.0, 1.0};
        REQUIRE_FALSE(v2.is_zero());

        mathvector<double> v3;
        REQUIRE(v3.is_zero());  // Empty vector is "zero"
    }
}

TEST_CASE("mathvector compound assignment operators", "[mathvector]") {
    mathvector<double> v1 = {1.0, 2.0, 3.0};
    mathvector<double> v2 = {4.0, 5.0, 6.0};

    SECTION("operator+=") {
        v1 += v2;
        REQUIRE(v1[0] == 5.0);
        REQUIRE(v1[1] == 7.0);
        REQUIRE(v1[2] == 9.0);
    }

    SECTION("operator-=") {
        v1 -= v2;
        REQUIRE(v1[0] == -3.0);
        REQUIRE(v1[1] == -3.0);
        REQUIRE(v1[2] == -3.0);
    }

    SECTION("operator*=") {
        v1 *= 2.0;
        REQUIRE(v1[0] == 2.0);
        REQUIRE(v1[1] == 4.0);
        REQUIRE(v1[2] == 6.0);
    }

    SECTION("operator/=") {
        v1 /= 2.0;
        REQUIRE(v1[0] == 0.5);
        REQUIRE(v1[1] == 1.0);
        REQUIRE(v1[2] == 1.5);
    }

    SECTION("operator/= component-wise") {
        mathvector<double> v3 = {2.0, 4.0, 6.0};
        v1 /= v3;
        REQUIRE(v1[0] == 0.5);
        REQUIRE(v1[1] == 0.5);
        REQUIRE(v1[2] == 0.5);
    }
}

TEST_CASE("mathvector arithmetic operators", "[mathvector]") {
    mathvector<double> v1 = {1.0, 2.0, 3.0};
    mathvector<double> v2 = {4.0, 5.0, 6.0};

    SECTION("unary plus") {
        auto result = +v1;
        REQUIRE(result[0] == 1.0);
        REQUIRE(result[1] == 2.0);
        REQUIRE(result[2] == 3.0);
    }

    SECTION("unary minus") {
        auto result = -v1;
        REQUIRE(result[0] == -1.0);
        REQUIRE(result[1] == -2.0);
        REQUIRE(result[2] == -3.0);
    }

    SECTION("addition") {
        auto result = v1 + v2;
        REQUIRE(result[0] == 5.0);
        REQUIRE(result[1] == 7.0);
        REQUIRE(result[2] == 9.0);
    }

    SECTION("subtraction") {
        auto result = v1 - v2;
        REQUIRE(result[0] == -3.0);
        REQUIRE(result[1] == -3.0);
        REQUIRE(result[2] == -3.0);
    }

    SECTION("scalar multiplication (left)") {
        auto result = 2.0 * v1;
        REQUIRE(result[0] == 2.0);
        REQUIRE(result[1] == 4.0);
        REQUIRE(result[2] == 6.0);
    }

    SECTION("scalar multiplication (right)") {
        auto result = v1 * 3.0;
        REQUIRE(result[0] == 3.0);
        REQUIRE(result[1] == 6.0);
        REQUIRE(result[2] == 9.0);
    }

    SECTION("scalar division") {
        auto result = v1 / 2.0;
        REQUIRE(result[0] == 0.5);
        REQUIRE(result[1] == 1.0);
        REQUIRE(result[2] == 1.5);
    }

    SECTION("component-wise division") {
        auto result = v2 / v1;
        REQUIRE(result[0] == 4.0);
        REQUIRE(result[1] == 2.5);
        REQUIRE(result[2] == 2.0);
    }
}

TEST_CASE("mathvector dot product", "[mathvector]") {
    mathvector<double> v1 = {1.0, 2.0, 3.0};
    mathvector<double> v2 = {4.0, 5.0, 6.0};

    SECTION("dot function") {
        double result = dot(v1, v2);
        REQUIRE(result == 32.0);  // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    }

    SECTION("operator* for dot product") {
        double result = v1 * v2;
        REQUIRE(result == 32.0);
    }

    SECTION("dot product with zero vector") {
        mathvector<double> zero(3, 0.0);
        double result = dot(v1, zero);
        REQUIRE(result == 0.0);
    }
}

TEST_CASE("mathvector cross product", "[mathvector]") {
    SECTION("float cross product") {
        mathvector<float> v1 = {1.0f, 0.0f, 0.0f};
        mathvector<float> v2 = {0.0f, 1.0f, 0.0f};
        auto result = cross(v1, v2);

        REQUIRE(result[0] == Approx(0.0f));
        REQUIRE(result[1] == Approx(0.0f));
        REQUIRE(result[2] == Approx(1.0f));
    }

    SECTION("double cross product") {
        mathvector<double> v1 = {1.0, 2.0, 3.0};
        mathvector<double> v2 = {4.0, 5.0, 6.0};
        auto result = cross(v1, v2);

        // v × w = (v2*w3 - v3*w2, v3*w1 - v1*w3, v1*w2 - v2*w1)
        // = (2*6 - 3*5, 3*4 - 1*6, 1*5 - 2*4)
        // = (12 - 15, 12 - 6, 5 - 8)
        // = (-3, 6, -3)
        REQUIRE(result[0] == Approx(-3.0));
        REQUIRE(result[1] == Approx(6.0));
        REQUIRE(result[2] == Approx(-3.0));
    }

    SECTION("cross product anti-commutative") {
        mathvector<double> v1 = {1.0, 0.0, 0.0};
        mathvector<double> v2 = {0.0, 1.0, 0.0};
        auto result1 = cross(v1, v2);
        auto result2 = cross(v2, v1);

        REQUIRE(result1[0] == Approx(-result2[0]));
        REQUIRE(result1[1] == Approx(-result2[1]));
        REQUIRE(result1[2] == Approx(-result2[2]));
    }
}

TEST_CASE("mathvector magnitude", "[mathvector]") {
    mathvector<double> v = {3.0, 4.0};

    SECTION("mag2 (squared magnitude)") {
        double result = mag2(v);
        REQUIRE(result == 25.0);  // 3² + 4² = 9 + 16 = 25
    }

    SECTION("abs (magnitude)") {
        double result = abs(v);
        REQUIRE(result == Approx(5.0));  // √(3² + 4²) = 5
    }

    SECTION("magnitude of unit vectors") {
        mathvector<double> unit_x = {1.0, 0.0, 0.0};
        REQUIRE(abs(unit_x) == Approx(1.0));
    }

    SECTION("magnitude of zero vector") {
        mathvector<double> zero(3, 0.0);
        REQUIRE(abs(zero) == Approx(0.0));
        REQUIRE(mag2(zero) == 0.0);
    }
}

TEST_CASE("mathvector with complex numbers", "[mathvector]") {
    using cd = std::complex<double>;

    SECTION("complex vector construction") {
        mathvector<cd> v = {cd(1.0, 2.0), cd(3.0, 4.0)};
        REQUIRE(v.size() == 2);
        REQUIRE(v[0] == cd(1.0, 2.0));
    }

    SECTION("complex vector default scalar type") {
        // mathvector<std::complex<double>> should default to double as scalar type
        mathvector<cd> v = {cd(3.0, 4.0)};
        // Check that scalar_type is double (via mag2 return type)
        double result = mag2(v);
        REQUIRE(result == 25.0);  // |3+4i|² = 3² + 4² = 25
    }

    SECTION("complex mag2 with default scalar type") {
        mathvector<cd> v = {cd(3.0, 4.0), cd(1.0, 1.0)};
        // |3+4i|² + |1+i|² = 25 + 2 = 27
        double result = mag2(v);
        REQUIRE(result == 27.0);
    }

    SECTION("complex mag2 with explicit scalar type") {
        mathvector<cd, double> v = {cd(3.0, 4.0)};  // |3+4i|² = 3² + 4² = 25
        double result = mag2(v);
        REQUIRE(result == 25.0);
    }

    SECTION("complex abs with default scalar type") {
        mathvector<cd> v = {cd(3.0, 4.0)};
        double result = abs(v);
        REQUIRE(result == Approx(5.0));  // |3+4i| = 5
    }

    SECTION("complex abs with explicit scalar type") {
        mathvector<cd, double> v = {cd(3.0, 4.0)};
        double result = abs(v);
        REQUIRE(result == Approx(5.0));
    }

    SECTION("complex vector with multiple elements") {
        mathvector<cd> v = {cd(1.0, 0.0), cd(0.0, 1.0), cd(1.0, 1.0)};
        // |1|² + |i|² + |1+i|² = 1 + 1 + 2 = 4
        REQUIRE(mag2(v) == 4.0);
        REQUIRE(abs(v) == Approx(2.0));
    }
}

TEST_CASE("mathvector edge cases", "[mathvector]") {
    SECTION("single element vector") {
        mathvector<double> v = {42.0};
        REQUIRE(v.sum() == 42.0);
        REQUIRE(mag2(v) == 1764.0);
        REQUIRE(abs(v) == Approx(42.0));
    }

    SECTION("large vector") {
        mathvector<double> v(1000, 1.0);
        REQUIRE(v.sum() == 1000.0);
        REQUIRE(mag2(v) == 1000.0);
    }

    SECTION("negative values") {
        mathvector<double> v = {-1.0, -2.0, -3.0};
        REQUIRE(v.sum() == -6.0);
        REQUIRE(mag2(v) == 14.0);  // (-1)² + (-2)² + (-3)² = 1 + 4 + 9 = 14
    }
}
