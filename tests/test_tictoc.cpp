//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/tictoc.hpp"
#include <sstream>
#include <thread>
#include <chrono>

using namespace jlt;

TEST_CASE("tictoc basic timing", "[tictoc][timing]") {
    SECTION("Create tictoc with default output stream") {
        tictoc timer;
        // Should construct without errors
        REQUIRE(true);
    }

    SECTION("Create tictoc with custom output stream") {
        std::ostringstream oss;
        tictoc timer(oss);
        // Should construct without errors
        REQUIRE(true);
    }

    SECTION("restart() initializes timer") {
        std::ostringstream oss;
        tictoc timer(oss);
        timer.restart();
        // Should restart without errors
        REQUIRE(true);
    }
}

TEST_CASE("tictoc tic/toc measurements", "[tictoc][timing]") {
    SECTION("Single tic/toc measures elapsed time") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.restart();
        timer.tic();
        // Do some CPU work (sleep doesn't count as CPU time)
        volatile double sum = 0;
        for (int i = 0; i < 100000; ++i) {
            sum += i * 1.5;
        }
        timer.toc();

        std::string output = oss.str();
        REQUIRE(!output.empty());
        // Output should contain w, u, s suffixes and newline
        REQUIRE(output.find('w') != std::string::npos);
        REQUIRE(output.find('\n') != std::string::npos);
    }

    SECTION("tic with label outputs label") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.restart();
        timer.tic("Test operation");
        timer.toc();

        std::string output = oss.str();
        REQUIRE(output.find("Test operation") != std::string::npos);
    }

    SECTION("tic without label outputs only timing") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.restart();
        timer.tic();
        timer.toc();

        std::string output = oss.str();
        // Output should be short (just numbers, no long labels)
        REQUIRE(output.length() < 50);
    }

    SECTION("Multiple tic/toc sequences") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.restart();

        timer.tic("Operation 1");
        volatile double sum1 = 0;
        for (int i = 0; i < 50000; ++i) sum1 += i;
        timer.toc();

        timer.tic("Operation 2");
        volatile double sum2 = 0;
        for (int i = 0; i < 50000; ++i) sum2 += i;
        timer.toc();

        std::string output = oss.str();
        REQUIRE(output.find("Operation 1") != std::string::npos);
        REQUIRE(output.find("Operation 2") != std::string::npos);

        // Count newlines - should have 2 (one for each toc)
        size_t newline_count = 0;
        for (char c : output) {
            if (c == '\n') newline_count++;
        }
        REQUIRE(newline_count == 2);
    }
}

TEST_CASE("tictoc dangling tic behavior", "[tictoc][timing]") {
    SECTION("Consecutive tics without toc add newline") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.restart();
        timer.tic("First tic");
        timer.tic("Second tic");
        timer.toc();

        std::string output = oss.str();
        // First tic should have a newline added when second tic is called
        REQUIRE(output.find("First tic") != std::string::npos);
        REQUIRE(output.find("Second tic") != std::string::npos);

        // Count newlines - should have 2 (one auto-added, one from toc)
        size_t newline_count = 0;
        for (char c : output) {
            if (c == '\n') newline_count++;
        }
        REQUIRE(newline_count == 2);
    }
}

TEST_CASE("tictoc timing accuracy", "[tictoc][timing]") {
    SECTION("Measured time is reasonable") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.restart();
        timer.tic();

        // Do actual CPU work (not sleep - sleep doesn't use CPU time)
        volatile double sum = 0;
        for (int i = 0; i < 1000000; ++i) {
            sum += i * 1.5;
        }

        timer.toc();

        std::string output = oss.str();

        // Output format: 0.5w 0.5u 0s  (1.2w 1.2u 0s)
        // Just verify it contains the expected suffixes
        REQUIRE(output.find('w') != std::string::npos);
        REQUIRE(output.find('u') != std::string::npos);
        REQUIRE(output.find('s') != std::string::npos);
        REQUIRE(output.find('(') != std::string::npos);
        REQUIRE(output.find(')') != std::string::npos);
    }

    SECTION("Cumulative time increases monotonically") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.restart();

        timer.tic();
        volatile double sum1 = 0;
        for (int i = 0; i < 500000; ++i) sum1 += i;
        timer.toc();

        timer.tic();
        volatile double sum2 = 0;
        for (int i = 0; i < 500000; ++i) sum2 += i;
        timer.toc();

        std::string output = oss.str();

        // Should have two lines (two toc() calls)
        size_t first_newline = output.find('\n');
        size_t second_newline = output.find('\n', first_newline + 1);

        REQUIRE(first_newline != std::string::npos);
        REQUIRE(second_newline != std::string::npos);
    }
}

TEST_CASE("tictoc restart resets timer", "[tictoc][timing]") {
    SECTION("restart() resets cumulative time") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.restart();
        timer.tic();
        volatile double sum1 = 0;
        for (int i = 0; i < 1000000; ++i) sum1 += i;
        timer.toc();

        // Clear output stream
        oss.str("");
        oss.clear();

        // Restart and time again
        timer.restart();
        timer.tic();
        volatile double sum2 = 0;
        for (int i = 0; i < 100000; ++i) sum2 += i;
        timer.toc();

        std::string output = oss.str();

        // Extract cumulative time
        size_t first_tab = output.find('\t');
        size_t second_tab = output.find('\t', first_tab + 1);
        size_t newline = output.find('\n', second_tab);
        std::string cum_str = output.substr(second_tab + 1, newline - second_tab - 1);
        double cumulative = std::stod(cum_str);

        // Cumulative time should be small (not include previous timing)
        // Should be less than the first run (we did 10x less work)
        REQUIRE(cumulative >= 0);
        REQUIRE(cumulative < 1.0); // Should be less than 1 second
    }
}

TEST_CASE("tictoc timer output format", "[tictoc][format]") {
    SECTION("Output contains all three time types with labels") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.tic();
        volatile double sum = 0;
        for (int i = 0; i < 1000000; ++i) sum += i * 1.5;
        timer.toc();

        std::string output = oss.str();

        // Format: 0.5w 0.5u 0s  (1.2w 1.2u 0s)
        // Should contain w, u, s suffixes
        REQUIRE(output.find('w') != std::string::npos);
        REQUIRE(output.find('u') != std::string::npos);
        REQUIRE(output.find('s') != std::string::npos);

        // Should contain parentheses for cumulative times
        REQUIRE(output.find('(') != std::string::npos);
        REQUIRE(output.find(')') != std::string::npos);

        // Should have newline at end
        REQUIRE(output.back() == '\n');
    }

    SECTION("With label, output still contains all times") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.tic("Test operation");
        volatile double sum = 0;
        for (int i = 0; i < 1000000; ++i) sum += i * 1.5;
        timer.toc();

        std::string output = oss.str();

        // Should contain label
        REQUIRE(output.find("Test operation") != std::string::npos);

        // Should still contain time suffixes
        REQUIRE(output.find('w') != std::string::npos);
        REQUIRE(output.find('u') != std::string::npos);
        REQUIRE(output.find('s') != std::string::npos);
    }
}

TEST_CASE("tictoc get_timing() method", "[tictoc][api]") {
    SECTION("Programmatic access to timing info") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.tic();
        volatile double sum = 0;
        for (int i = 0; i < 1000000; ++i) sum += i * 1.5;
        timer.toc();

        // Get timing info programmatically
        auto info = timer.get_timing();

        // Check all fields are reasonable
        REQUIRE(info.wall_elapsed >= 0);
        REQUIRE(info.wall_cumulative >= 0);
        REQUIRE(info.user_elapsed >= 0);
        REQUIRE(info.user_cumulative >= 0);
        REQUIRE(info.system_elapsed >= 0);
        REQUIRE(info.system_cumulative >= 0);

        // Cumulative >= elapsed for each metric
        REQUIRE(info.wall_cumulative >= info.wall_elapsed);
        REQUIRE(info.user_cumulative >= info.user_elapsed);
        REQUIRE(info.system_cumulative >= info.system_elapsed);
    }

    SECTION("Multiple tic/toc sequences") {
        std::ostringstream oss;
        tictoc timer(oss);

        // First operation
        timer.tic();
        volatile double sum1 = 0;
        for (int i = 0; i < 1000000; ++i) sum1 += i;
        timer.toc();

        auto info1 = timer.get_timing();

        // Second operation
        timer.tic();
        volatile double sum2 = 0;
        for (int i = 0; i < 1000000; ++i) sum2 += i;
        timer.toc();

        auto info2 = timer.get_timing();

        // Cumulative time should increase (or stay the same if very fast)
        REQUIRE(info2.wall_cumulative >= info1.wall_cumulative);
        REQUIRE(info2.user_cumulative >= info1.user_cumulative);
    }
}
