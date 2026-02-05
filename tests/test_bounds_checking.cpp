//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/vector.hpp"
#include "../jlt/matrix.hpp"

using namespace jlt;

#ifdef JLT_VECTOR_CHECK_BOUNDS
TEST_CASE("Vector bounds checking", "[bounds]") {
    SECTION("in-bounds access works normally") {
        vector<int> v = {10, 20, 30, 40, 50};
        REQUIRE(v[0] == 10);
        REQUIRE(v[2] == 30);
        REQUIRE(v[4] == 50);

        // Test modification
        v[2] = 100;
        REQUIRE(v[2] == 100);
    }

    SECTION("out of range access throws") {
        vector<int> v = {1, 2, 3};
        REQUIRE_THROWS_AS(v[3], std::out_of_range);
        REQUIRE_THROWS_AS(v[10], std::out_of_range);
        REQUIRE_THROWS_AS(v[100], std::out_of_range);
    }

    SECTION("empty vector out of range throws") {
        vector<int> v;
        REQUIRE_THROWS_AS(v[0], std::out_of_range);
    }

    SECTION("const vector out of range throws") {
        const vector<int> v = {1, 2, 3};
        REQUIRE(v[0] == 1);
        REQUIRE_THROWS_AS(v[3], std::out_of_range);
    }
}
#endif

#ifdef JLT_MATRIX_CHECK_BOUNDS
TEST_CASE("Matrix bounds checking", "[bounds]") {
    SECTION("in-bounds access works normally") {
        matrix<int> M(3, 4, {
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12
        });

        // Test operator() access
        REQUIRE(M(0, 0) == 1);
        REQUIRE(M(1, 2) == 7);
        REQUIRE(M(2, 3) == 12);

        // Test operator() modification
        M(1, 1) = 100;
        REQUIRE(M(1, 1) == 100);

        // Test operator[] access
        REQUIRE(M[0][0] == 1);
        REQUIRE(M[1][2] == 7);
        REQUIRE(M[2][3] == 12);

        // Test at() access
        REQUIRE(M.at(0, 0) == 1);
        REQUIRE(M.at(1, 2) == 7);
        REQUIRE(M.at(2, 3) == 12);
    }

    SECTION("operator() out of range throws") {
        matrix<int> M(2, 3, {1, 2, 3, 4, 5, 6});

        // Row out of range
        REQUIRE_THROWS_AS(M(2, 0), std::out_of_range);
        REQUIRE_THROWS_AS(M(10, 0), std::out_of_range);

        // Column out of range
        REQUIRE_THROWS_AS(M(0, 3), std::out_of_range);
        REQUIRE_THROWS_AS(M(0, 10), std::out_of_range);

        // Both out of range
        REQUIRE_THROWS_AS(M(2, 3), std::out_of_range);
    }

    SECTION("operator[] row index out of range throws") {
        matrix<int> M(2, 3, {1, 2, 3, 4, 5, 6});

        REQUIRE_THROWS_AS(M[2], std::out_of_range);
        REQUIRE_THROWS_AS(M[10], std::out_of_range);
    }

    SECTION("at() out of range throws") {
        matrix<int> M(2, 3, {1, 2, 3, 4, 5, 6});

        // Row out of range
        REQUIRE_THROWS_AS(M.at(2, 0), std::out_of_range);
        REQUIRE_THROWS_AS(M.at(10, 0), std::out_of_range);

        // Column out of range
        REQUIRE_THROWS_AS(M.at(0, 3), std::out_of_range);
        REQUIRE_THROWS_AS(M.at(0, 10), std::out_of_range);

        // Both out of range
        REQUIRE_THROWS_AS(M.at(2, 3), std::out_of_range);
    }

    SECTION("const matrix out of range throws") {
        const matrix<int> M(2, 3, {1, 2, 3, 4, 5, 6});

        REQUIRE(M(0, 0) == 1);
        REQUIRE_THROWS_AS(M(2, 0), std::out_of_range);
        REQUIRE_THROWS_AS(M(0, 3), std::out_of_range);
    }

    SECTION("empty matrix access throws") {
        matrix<int> M;
        REQUIRE_THROWS_AS(M(0, 0), std::out_of_range);
        REQUIRE_THROWS_AS(M[0], std::out_of_range);
    }

    SECTION("row() out of range throws") {
        matrix<int> M(2, 3, {1, 2, 3, 4, 5, 6});

        REQUIRE_THROWS_AS(M.row(2), std::out_of_range);
        REQUIRE_THROWS_AS(M.row(10), std::out_of_range);
    }
}
#endif
