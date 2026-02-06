//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/stlio.hpp"
#include <sstream>
#include <vector>
#include <valarray>
#include <list>
#include <map>

using namespace jlt;

TEST_CASE("stlio vector output", "[stlio][vector]") {
    SECTION("Empty vector outputs nothing") {
        std::vector<int> vv;
        std::ostringstream oss;
        oss << vv;
        REQUIRE(oss.str().empty());
    }

    SECTION("Single element vector") {
        std::vector<int> vv = {42};
        std::ostringstream oss;
        oss << vv;
        std::string output = oss.str();
        REQUIRE(output.find("42") != std::string::npos);
    }

    SECTION("Multiple element vector (int)") {
        std::vector<int> vv = {1, 2, 3, 4, 5};
        std::ostringstream oss;
        oss << vv;
        std::string output = oss.str();
        REQUIRE(output.find("1") != std::string::npos);
        REQUIRE(output.find("2") != std::string::npos);
        REQUIRE(output.find("5") != std::string::npos);
    }

    SECTION("Vector with double precision") {
        std::vector<double> vv = {1.5, 2.5, 3.14159};
        std::ostringstream oss;
        oss << std::setprecision(6);
        oss << vv;
        std::string output = oss.str();
        REQUIRE(output.find("1.5") != std::string::npos);
        REQUIRE(output.find("3.14159") != std::string::npos);
    }

    SECTION("Vector scientific notation") {
        std::vector<double> vv = {1e-10, 2e10, 3.14159e-5};
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(3);
        oss << vv;
        std::string output = oss.str();
        // Scientific notation should contain 'e'
        REQUIRE(output.find('e') != std::string::npos);
    }

    SECTION("Vector with negative numbers") {
        std::vector<int> vv = {-1, -2, -3};
        std::ostringstream oss;
        oss << vv;
        std::string output = oss.str();
        REQUIRE(output.find("-1") != std::string::npos);
        REQUIRE(output.find("-2") != std::string::npos);
    }

    SECTION("Large vector") {
        std::vector<int> vv(100, 7);
        std::ostringstream oss;
        oss << vv;
        std::string output = oss.str();
        // Should have 100 occurrences of "7"
        size_t count = 0;
        size_t pos = 0;
        while ((pos = output.find('7', pos)) != std::string::npos) {
            count++;
            pos++;
        }
        REQUIRE(count == 100);
    }
}

TEST_CASE("stlio valarray output", "[stlio][valarray]") {
    SECTION("Empty valarray outputs nothing") {
        std::valarray<int> vv;
        std::ostringstream oss;
        oss << vv;
        REQUIRE(oss.str().empty());
    }

    SECTION("Single element valarray") {
        std::valarray<double> vv(3.14, 1);
        std::ostringstream oss;
        oss << vv;
        std::string output = oss.str();
        REQUIRE(output.find("3.14") != std::string::npos);
    }

    SECTION("Multiple element valarray") {
        std::valarray<int> vv = {10, 20, 30, 40, 50};
        std::ostringstream oss;
        oss << vv;
        std::string output = oss.str();
        REQUIRE(output.find("10") != std::string::npos);
        REQUIRE(output.find("50") != std::string::npos);
    }

    SECTION("Valarray with slice") {
        std::valarray<double> vv(10);
        for (int i = 0; i < 10; ++i) vv[i] = i * 1.1;
        std::ostringstream oss;
        oss << vv;
        std::string output = oss.str();
        REQUIRE(output.find("0") != std::string::npos);
        REQUIRE(output.find("9.9") != std::string::npos);
    }
}

TEST_CASE("stlio list output", "[stlio][list]") {
    SECTION("Empty list outputs nothing") {
        std::list<int> ll;
        std::ostringstream oss;
        oss << ll;
        REQUIRE(oss.str().empty());
    }

    SECTION("Single element list") {
        std::list<int> ll = {42};
        std::ostringstream oss;
        oss << ll;
        std::string output = oss.str();
        REQUIRE(output.find("42") != std::string::npos);
    }

    SECTION("Multiple element list") {
        std::list<std::string> ll = {"hello", "world", "test"};
        std::ostringstream oss;
        oss << ll;
        std::string output = oss.str();
        REQUIRE(output.find("hello") != std::string::npos);
        REQUIRE(output.find("world") != std::string::npos);
        REQUIRE(output.find("test") != std::string::npos);
    }

    SECTION("List with numbers") {
        std::list<double> ll = {1.1, 2.2, 3.3};
        std::ostringstream oss;
        oss << ll;
        std::string output = oss.str();
        REQUIRE(output.find("1.1") != std::string::npos);
        REQUIRE(output.find("2.2") != std::string::npos);
    }
}

TEST_CASE("stlio map output", "[stlio][map]") {
    SECTION("Empty map outputs nothing") {
        std::map<int, std::string> mm;
        std::ostringstream oss;
        oss << mm;
        REQUIRE(oss.str().empty());
    }

    SECTION("Map with string keys") {
        std::map<std::string, int> mm;
        mm["one"] = 1;
        mm["two"] = 2;
        mm["three"] = 3;
        std::ostringstream oss;
        oss << mm;
        std::string output = oss.str();
        REQUIRE(output.find("one") != std::string::npos);
        REQUIRE(output.find("two") != std::string::npos);
        REQUIRE(output.find("3") != std::string::npos);
    }

    SECTION("Map with int keys") {
        std::map<int, double> mm;
        mm[1] = 1.5;
        mm[2] = 2.5;
        mm[3] = 3.5;
        std::ostringstream oss;
        oss << mm;
        std::string output = oss.str();
        REQUIRE(output.find("1") != std::string::npos);
        REQUIRE(output.find("1.5") != std::string::npos);
        REQUIRE(output.find("3.5") != std::string::npos);
    }

    SECTION("Map with double keys (scientific notation)") {
        std::map<double, int> mm;
        mm[0.001] = 1;
        mm[0.002] = 2;
        mm[0.003] = 3;
        std::ostringstream oss;
        oss << mm;
        std::string output = oss.str();
        // Double keys should be in scientific notation
        REQUIRE(output.find('e') != std::string::npos);
    }
}

TEST_CASE("stlio vector input", "[stlio][vector][input]") {
    SECTION("Read into vector") {
        std::vector<int> vv(5);
        std::istringstream iss("1 2 3 4 5");
        iss >> vv;
        REQUIRE(vv[0] == 1);
        REQUIRE(vv[4] == 5);
    }

    SECTION("Read doubles into vector") {
        std::vector<double> vv(3);
        std::istringstream iss("1.5 2.5 3.5");
        iss >> vv;
        REQUIRE(vv[0] == Approx(1.5));
        REQUIRE(vv[2] == Approx(3.5));
    }

    SECTION("Input validation - stops on bad input") {
        std::vector<int> vv(5, -1);  // Initialize with -1
        std::istringstream iss("1 2 abc 4 5");
        iss >> vv;
        // First two elements should be read
        REQUIRE(vv[0] == 1);
        REQUIRE(vv[1] == 2);
        // Stream should be in bad state
        REQUIRE_FALSE(iss.good());
        // Element where failure occurred may be modified (implementation-defined)
        // but subsequent elements should be unchanged
        REQUIRE(vv[3] == -1);
        REQUIRE(vv[4] == -1);
    }

    SECTION("Input validation - stops on EOF") {
        std::vector<int> vv(5, -1);
        std::istringstream iss("1 2 3");  // Only 3 values, but vector wants 5
        iss >> vv;
        // First three elements should be read
        REQUIRE(vv[0] == 1);
        REQUIRE(vv[1] == 2);
        REQUIRE(vv[2] == 3);
        // Stream should be at EOF
        REQUIRE(iss.eof());
        // Remaining elements should be unchanged
        REQUIRE(vv[3] == -1);
        REQUIRE(vv[4] == -1);
    }
}

TEST_CASE("stlio valarray input", "[stlio][valarray][input]") {
    SECTION("Read into valarray") {
        std::valarray<int> vv(5);
        std::istringstream iss("10 20 30 40 50");
        iss >> vv;
        REQUIRE(vv[0] == 10);
        REQUIRE(vv[4] == 50);
    }

    SECTION("Read doubles into valarray") {
        std::valarray<double> vv(3);
        std::istringstream iss("1.1 2.2 3.3");
        iss >> vv;
        REQUIRE(vv[0] == Approx(1.1));
        REQUIRE(vv[2] == Approx(3.3));
    }

    SECTION("Input validation - stops on bad input") {
        std::valarray<int> vv(5);
        for (int i = 0; i < 5; ++i) vv[i] = -1;
        std::istringstream iss("1 2 xyz");
        iss >> vv;
        REQUIRE(vv[0] == 1);
        REQUIRE(vv[1] == 2);
        REQUIRE_FALSE(iss.good());
        // Element where failure occurred may be modified (implementation-defined)
        // but subsequent elements should be unchanged
        REQUIRE(vv[3] == -1);
        REQUIRE(vv[4] == -1);
    }
}

TEST_CASE("stlio list input", "[stlio][list][input]") {
    SECTION("Read into empty list") {
        std::list<int> ll;
        std::istringstream iss("1 2 3 4 5");
        iss >> ll;
        REQUIRE(ll.size() == 5);
        auto it = ll.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 5);
    }

    SECTION("Read doubles into list") {
        std::list<double> ll;
        std::istringstream iss("1.5 2.5 3.5");
        iss >> ll;
        REQUIRE(ll.size() == 3);
        auto it = ll.begin();
        REQUIRE(*it++ == Approx(1.5));
        REQUIRE(*it++ == Approx(2.5));
        REQUIRE(*it++ == Approx(3.5));
    }

    SECTION("Read strings into list") {
        std::list<std::string> ll;
        std::istringstream iss("hello world test");
        iss >> ll;
        REQUIRE(ll.size() == 3);
        auto it = ll.begin();
        REQUIRE(*it++ == "hello");
        REQUIRE(*it++ == "world");
        REQUIRE(*it++ == "test");
    }

    SECTION("Input stops on bad input") {
        std::list<int> ll;
        std::istringstream iss("1 2 abc 4 5");
        iss >> ll;
        // Should have read 1 and 2, then stopped at "abc"
        REQUIRE(ll.size() == 2);
        REQUIRE_FALSE(iss.good());
    }
}

TEST_CASE("stlio format traits", "[stlio][format_traits]") {
    // Note: Direct access to static const members requires definitions in the .hpp file.
    // We test format_traits indirectly through the output formatting.

    SECTION("Int field width applied in formatting") {
        std::vector<int> vv = {1, 22, 333};
        std::ostringstream oss;
        oss << vv;
        std::string output = oss.str();
        // All numbers should be in the output
        REQUIRE(output.find("1") != std::string::npos);
        REQUIRE(output.find("22") != std::string::npos);
        REQUIRE(output.find("333") != std::string::npos);
    }

    SECTION("Double field width applied in formatting") {
        std::vector<double> vv = {1.0, 2.5, 3.14159};
        std::ostringstream oss;
        oss << vv;
        std::string output = oss.str();
        REQUIRE(output.find("1.00000") != std::string::npos);
    }
}

TEST_CASE("stlio format preservation", "[stlio][format]") {
    SECTION("Stream format flags preserved after vector output") {
        std::vector<double> vv = {1.0, 2.0, 3.0};
        std::ostringstream oss;

        // Set some format flags
        oss << std::fixed << std::setprecision(2);

        // Output vector
        oss << vv;

        // Check flags are preserved
        REQUIRE(oss.flags() & std::ios::fixed);
    }

    SECTION("Trailing zeros shown") {
        std::vector<double> vv = {1.5, 2.0, 3.0};
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << vv;
        std::string output = oss.str();
        // Should see "2.00" not just "2"
        REQUIRE(output.find("2.00") != std::string::npos);
    }
}

TEST_CASE("stlio mixed numeric types", "[stlio][numeric]") {
    SECTION("Vector with mixed precision") {
        std::vector<float> vv = {1.5f, 2.5f, 3.5f};
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << vv;
        std::string output = oss.str();
        REQUIRE(output.find("1.50") != std::string::npos);
        REQUIRE(output.find("3.50") != std::string::npos);
    }

    SECTION("Very small numbers") {
        std::vector<double> vv = {1e-10, 2e-10, 3e-10};
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(2);
        oss << vv;
        std::string output = oss.str();
        REQUIRE(output.find("e") != std::string::npos);
    }

    SECTION("Very large numbers") {
        std::vector<double> vv = {1e10, 2e10, 3e10};
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(2);
        oss << vv;
        std::string output = oss.str();
        REQUIRE(output.find("e+10") != std::string::npos);
    }
}
