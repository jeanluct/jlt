#include "catch.hpp"
#include "../jlt/vector.hpp"
#include <string>

using namespace jlt;

TEST_CASE("vector basic construction", "[vector]") {
    SECTION("default construction") {
        vector<int> v;
        REQUIRE(v.size() == 0);
        REQUIRE(v.empty());
    }

    SECTION("construction with size and value") {
        vector<int> v(5, 42);
        REQUIRE(v.size() == 5);
        for (size_t i = 0; i < 5; ++i) {
            REQUIRE(v[i] == 42);
        }
    }

    SECTION("construction with initializer list") {
        vector<int> v = {1, 2, 3, 4, 5};
        REQUIRE(v.size() == 5);
        REQUIRE(v[0] == 1);
        REQUIRE(v[4] == 5);
    }
}

TEST_CASE("vector element access", "[vector]") {
    vector<int> v = {10, 20, 30, 40, 50};

    SECTION("operator[] access") {
        REQUIRE(v[0] == 10);
        REQUIRE(v[2] == 30);
        REQUIRE(v[4] == 50);
    }

    SECTION("operator[] modification") {
        v[2] = 100;
        REQUIRE(v[2] == 100);
    }

    SECTION("at() access") {
        REQUIRE(v.at(0) == 10);
        REQUIRE(v.at(2) == 30);
    }

    SECTION("at() out of range throws") {
        REQUIRE_THROWS_AS(v.at(10), std::out_of_range);
    }
}

TEST_CASE("vector STL compatibility", "[vector]") {
    SECTION("push_back and emplace_back") {
        vector<std::string> v;
        v.push_back("hello");
        v.emplace_back("world");
        REQUIRE(v.size() == 2);
        REQUIRE(v[0] == "hello");
        REQUIRE(v[1] == "world");
    }

    SECTION("resize") {
        vector<double> v(3, 1.0);
        v.resize(5, 2.0);
        REQUIRE(v.size() == 5);
        REQUIRE(v[0] == 1.0);
        REQUIRE(v[4] == 2.0);
    }

    SECTION("clear") {
        vector<int> v = {1, 2, 3};
        v.clear();
        REQUIRE(v.empty());
    }

    SECTION("iterators") {
        vector<int> v = {1, 2, 3, 4, 5};
        int sum = 0;
        for (auto it = v.begin(); it != v.end(); ++it) {
            sum += *it;
        }
        REQUIRE(sum == 15);
    }

    SECTION("range-based for loop") {
        vector<int> v = {1, 2, 3, 4, 5};
        int sum = 0;
        for (const auto& x : v) {
            sum += x;
        }
        REQUIRE(sum == 15);
    }
}

TEST_CASE("vector type variations", "[vector]") {
    SECTION("double vector") {
        vector<double> v = {1.5, 2.5, 3.5};
        REQUIRE(v[0] == Approx(1.5));
        REQUIRE(v[1] == Approx(2.5));
    }

    SECTION("string vector") {
        vector<std::string> v = {"a", "bb", "ccc"};
        REQUIRE(v[0] == "a");
        REQUIRE(v[2].size() == 3);
    }

    SECTION("empty vector of custom size") {
        vector<int> v(100);
        REQUIRE(v.size() == 100);
    }
}
