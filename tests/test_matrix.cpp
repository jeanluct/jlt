#include "catch.hpp"
#include "../jlt/matrix.hpp"

using namespace jlt;

TEST_CASE("matrix basic construction", "[matrix]") {
    SECTION("default construction") {
        matrix<int> M;
        REQUIRE(M.size() == 0);
        REQUIRE(M.rows() == 0);
        REQUIRE(M.columns() == 0);
        REQUIRE(M.empty());
    }

    SECTION("construction with size and value") {
        matrix<int> M(3, 4, 7);
        REQUIRE(M.rows() == 3);
        REQUIRE(M.columns() == 4);
        REQUIRE(M.size() == 12);
        
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                REQUIRE(M(i, j) == 7);
            }
        }
    }

    SECTION("construction with initializer list") {
        matrix<int> M(2, 3, {1, 2, 3, 4, 5, 6});
        REQUIRE(M.rows() == 2);
        REQUIRE(M.columns() == 3);
        REQUIRE(M(0, 0) == 1);
        REQUIRE(M(0, 1) == 2);
        REQUIRE(M(0, 2) == 3);
        REQUIRE(M(1, 0) == 4);
        REQUIRE(M(1, 1) == 5);
        REQUIRE(M(1, 2) == 6);
    }

    SECTION("copy constructor") {
        matrix<int> M1(2, 2, {1, 2, 3, 4});
        matrix<int> M2(M1);
        REQUIRE(M2.rows() == 2);
        REQUIRE(M2.columns() == 2);
        REQUIRE(M2(0, 0) == 1);
        REQUIRE(M2(1, 1) == 4);
    }
}

TEST_CASE("matrix element access", "[matrix]") {
    matrix<int> M(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});

    SECTION("operator() access") {
        REQUIRE(M(0, 0) == 1);
        REQUIRE(M(0, 1) == 2);
        REQUIRE(M(1, 0) == 4);
        REQUIRE(M(2, 2) == 9);
    }

    SECTION("operator() modification") {
        M(1, 1) = 100;
        REQUIRE(M(1, 1) == 100);
    }

    SECTION("operator[] access") {
        REQUIRE(M[0][0] == 1);
        REQUIRE(M[0][1] == 2);
        REQUIRE(M[1][0] == 4);
    }

    SECTION("at() access") {
        REQUIRE(M.at(0, 0) == 1);
        REQUIRE(M.at(1, 1) == 5);
    }

    SECTION("at() out of range throws") {
        REQUIRE_THROWS_AS(M.at(3, 0), std::out_of_range);
        REQUIRE_THROWS_AS(M.at(0, 3), std::out_of_range);
    }
}

TEST_CASE("matrix properties", "[matrix]") {
    SECTION("isSquare") {
        matrix<int> M1(3, 3);
        matrix<int> M2(2, 3);
        
        REQUIRE(M1.isSquare());
        REQUIRE_FALSE(M2.isSquare());
    }

    SECTION("dim() for square matrix") {
        matrix<int> M(4, 4);
        REQUIRE(M.dim() == 4);
    }

    SECTION("empty matrix") {
        matrix<int> M;
        REQUIRE(M.empty());
        
        matrix<int> M2(1, 1);
        REQUIRE_FALSE(M2.empty());
    }
}

TEST_CASE("matrix assignment", "[matrix]") {
    SECTION("copy assignment") {
        matrix<int> M1(2, 2, {1, 2, 3, 4});
        matrix<int> M2;
        M2 = M1;
        
        REQUIRE(M2.rows() == 2);
        REQUIRE(M2.columns() == 2);
        REQUIRE(M2(0, 0) == 1);
        REQUIRE(M2(1, 1) == 4);
    }

    SECTION("self assignment") {
        matrix<int> M(2, 2, {1, 2, 3, 4});
        M = M;
        REQUIRE(M(0, 0) == 1);
        REQUIRE(M(1, 1) == 4);
    }
}

TEST_CASE("matrix iterators", "[matrix]") {
    matrix<int> M(2, 3, {1, 2, 3, 4, 5, 6});

    SECTION("begin/end iteration") {
        int sum = 0;
        for (auto it = M.begin(); it != M.end(); ++it) {
            sum += *it;
        }
        REQUIRE(sum == 21);  // 1+2+3+4+5+6
    }

    SECTION("cbegin/cend iteration") {
        int sum = 0;
        for (auto it = M.cbegin(); it != M.cend(); ++it) {
            sum += *it;
        }
        REQUIRE(sum == 21);
    }
}

TEST_CASE("matrix row access", "[matrix]") {
    matrix<int> M(3, 4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12
    });

    SECTION("row extraction") {
        auto row0 = M.row(0);
        REQUIRE(row0.size() == 4);
        REQUIRE(row0[0] == 1);
        REQUIRE(row0[3] == 4);

        auto row1 = M.row(1);
        REQUIRE(row1[0] == 5);
        REQUIRE(row1[3] == 8);
    }
}

TEST_CASE("matrix data pointer", "[matrix]") {
    matrix<double> M(2, 2, {1.0, 2.0, 3.0, 4.0});
    
    double* ptr = M.data();
    REQUIRE(ptr[0] == 1.0);
    REQUIRE(ptr[3] == 4.0);
    
    const double* cptr = static_cast<const matrix<double>&>(M).data();
    REQUIRE(cptr[0] == 1.0);
}
