#include "catch.hpp"
#include "../jlt/matrix.hpp"

using namespace jlt;

TEST_CASE("matrix transpose for square matrices", "[matrix][transpose]") {
    SECTION("2x2 transpose") {
        matrix<double> M(2, 2, {
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

    SECTION("3x3 transpose") {
        matrix<double> M(3, 3, {
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0,
            7.0, 8.0, 9.0
        });

        M.transpose();

        REQUIRE(M.rows() == 3);
        REQUIRE(M.columns() == 3);
        REQUIRE(M(0, 0) == 1.0);
        REQUIRE(M(0, 1) == 4.0);
        REQUIRE(M(0, 2) == 7.0);
        REQUIRE(M(1, 0) == 2.0);
        REQUIRE(M(1, 1) == 5.0);
        REQUIRE(M(1, 2) == 8.0);
        REQUIRE(M(2, 0) == 3.0);
        REQUIRE(M(2, 1) == 6.0);
        REQUIRE(M(2, 2) == 9.0);
    }

    SECTION("transpose of transpose returns original") {
        matrix<double> M_orig(2, 2, {
            1.0, 2.0,
            3.0, 4.0
        });

        matrix<double> M(M_orig);
        M.transpose();
        M.transpose();

        REQUIRE(M(0, 0) == M_orig(0, 0));
        REQUIRE(M(0, 1) == M_orig(0, 1));
        REQUIRE(M(1, 0) == M_orig(1, 0));
        REQUIRE(M(1, 1) == M_orig(1, 1));
    }

    SECTION("identity matrix transpose is itself") {
        matrix<double> I(3, 3, {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0
        });

        matrix<double> I_orig(I);
        I.transpose();

        for (auto i = 0u; i < 3; ++i) {
            for (auto j = 0u; j < 3; ++j) {
                REQUIRE(I(i, j) == I_orig(i, j));
            }
        }
    }

    SECTION("symmetric matrix transpose is itself") {
        matrix<double> S(3, 3, {
            1.0, 2.0, 3.0,
            2.0, 4.0, 5.0,
            3.0, 5.0, 6.0
        });

        matrix<double> S_orig(S);
        S.transpose();

        for (auto i = 0u; i < 3; ++i) {
            for (auto j = 0u; j < 3; ++j) {
                REQUIRE(S(i, j) == S_orig(i, j));
            }
        }
    }
}

TEST_CASE("matrix transpose for non-square matrices", "[matrix][transpose]") {
    SECTION("2x3 matrix transpose becomes 3x2") {
        matrix<double> M(2, 3, {
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0
        });

        M.transpose();

        REQUIRE(M.rows() == 3);
        REQUIRE(M.columns() == 2);
        REQUIRE(M(0, 0) == 1.0);
        REQUIRE(M(0, 1) == 4.0);
        REQUIRE(M(1, 0) == 2.0);
        REQUIRE(M(1, 1) == 5.0);
        REQUIRE(M(2, 0) == 3.0);
        REQUIRE(M(2, 1) == 6.0);
    }

    SECTION("3x2 matrix transpose becomes 2x3") {
        matrix<double> M(3, 2, {
            1.0, 2.0,
            3.0, 4.0,
            5.0, 6.0
        });

        M.transpose();

        REQUIRE(M.rows() == 2);
        REQUIRE(M.columns() == 3);
        REQUIRE(M(0, 0) == 1.0);
        REQUIRE(M(0, 1) == 3.0);
        REQUIRE(M(0, 2) == 5.0);
        REQUIRE(M(1, 0) == 2.0);
        REQUIRE(M(1, 1) == 4.0);
        REQUIRE(M(1, 2) == 6.0);
    }

    SECTION("4x1 column vector becomes 1x4 row vector") {
        matrix<double> M(4, 1, {
            1.0,
            2.0,
            3.0,
            4.0
        });

        M.transpose();

        REQUIRE(M.rows() == 1);
        REQUIRE(M.columns() == 4);
        REQUIRE(M(0, 0) == 1.0);
        REQUIRE(M(0, 1) == 2.0);
        REQUIRE(M(0, 2) == 3.0);
        REQUIRE(M(0, 3) == 4.0);
    }

    SECTION("1x4 row vector becomes 4x1 column vector") {
        matrix<double> M(1, 4, {1.0, 2.0, 3.0, 4.0});

        M.transpose();

        REQUIRE(M.rows() == 4);
        REQUIRE(M.columns() == 1);
        REQUIRE(M(0, 0) == 1.0);
        REQUIRE(M(1, 0) == 2.0);
        REQUIRE(M(2, 0) == 3.0);
        REQUIRE(M(3, 0) == 4.0);
    }

    SECTION("transpose of transpose returns original (non-square)") {
        matrix<double> M_orig(2, 3, {
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0
        });

        matrix<double> M(M_orig);
        M.transpose();

        REQUIRE(M.rows() == 3);
        REQUIRE(M.columns() == 2);

        M.transpose();

        REQUIRE(M.rows() == 2);
        REQUIRE(M.columns() == 3);

        for (auto i = 0u; i < M_orig.rows(); ++i) {
            for (auto j = 0u; j < M_orig.columns(); ++j) {
                REQUIRE(M(i, j) == M_orig(i, j));
            }
        }
    }

    SECTION("large non-square matrix 5x10") {
        matrix<double> M(5, 10);

        // Fill with i*10 + j pattern
        for (auto i = 0u; i < 5; ++i) {
            for (auto j = 0u; j < 10; ++j) {
                M(i, j) = static_cast<double>(i * 10 + j);
            }
        }

        M.transpose();

        REQUIRE(M.rows() == 10);
        REQUIRE(M.columns() == 5);

        // Check transpose worked correctly
        for (auto i = 0u; i < 5; ++i) {
            for (auto j = 0u; j < 10; ++j) {
                REQUIRE(M(j, i) == static_cast<double>(i * 10 + j));
            }
        }
    }
}

TEST_CASE("matrix transpose edge cases", "[matrix][transpose]") {
    SECTION("1x1 matrix transpose is itself") {
        matrix<double> M(1, 1, {42.0});

        M.transpose();

        REQUIRE(M.rows() == 1);
        REQUIRE(M.columns() == 1);
        REQUIRE(M(0, 0) == 42.0);
    }

    SECTION("transpose preserves int type") {
        matrix<int> M(2, 3, {
            1, 2, 3,
            4, 5, 6
        });

        M.transpose();

        REQUIRE(M.rows() == 3);
        REQUIRE(M.columns() == 2);
        REQUIRE(M(0, 0) == 1);
        REQUIRE(M(1, 1) == 5);
        REQUIRE(M(2, 1) == 6);
    }

    SECTION("transpose preserves float type") {
        matrix<float> M(2, 2, {
            1.5f, 2.5f,
            3.5f, 4.5f
        });

        M.transpose();

        REQUIRE(M(0, 0) == 1.5f);
        REQUIRE(M(0, 1) == 3.5f);
        REQUIRE(M(1, 0) == 2.5f);
        REQUIRE(M(1, 1) == 4.5f);
    }

    SECTION("zero matrix transposes correctly") {
        matrix<double> M(3, 4, 0.0);

        M.transpose();

        REQUIRE(M.rows() == 4);
        REQUIRE(M.columns() == 3);

        for (auto i = 0u; i < 4; ++i) {
            for (auto j = 0u; j < 3; ++j) {
                REQUIRE(M(i, j) == 0.0);
            }
        }
    }
}
