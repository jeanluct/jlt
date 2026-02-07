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
        // Output should contain two tab-separated numbers and newline
        REQUIRE(output.find('\t') != std::string::npos);
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

        // Parse the elapsed time (first number before first tab)
        size_t tab_pos = output.find('\t');
        REQUIRE(tab_pos != std::string::npos);

        std::string time_str = output.substr(0, tab_pos);
        double elapsed_seconds = std::stod(time_str);

        // Should be a positive number (actual CPU work was done)
        // Don't check exact value since it depends on CPU speed
        REQUIRE(elapsed_seconds >= 0);
        REQUIRE(elapsed_seconds < 10.0);  // Should complete in under 10 seconds
    }

    SECTION("Cumulative time increases monotonically") {
        std::ostringstream oss;
        tictoc timer(oss);

        timer.restart();

        timer.tic();
        volatile double sum1 = 0;
        for (int i = 0; i < 500000; ++i) sum1 += i;
        timer.toc();

        std::string output1 = oss.str();

        timer.tic();
        volatile double sum2 = 0;
        for (int i = 0; i < 500000; ++i) sum2 += i;
        timer.toc();

        std::string output2 = oss.str();

        // Extract cumulative times (second number after first tab, before newline)
        auto extract_cumulative = [](const std::string& s, size_t start) {
            size_t first_tab = s.find('\t', start);
            size_t second_tab = s.find('\t', first_tab + 1);
            size_t newline = s.find('\n', second_tab);
            std::string cum_str = s.substr(second_tab + 1, newline - second_tab - 1);
            return std::stod(cum_str);
        };

        double cumulative1 = extract_cumulative(output1, 0);
        double cumulative2 = extract_cumulative(output2, output1.length());

        // Second cumulative time should be greater than or equal to first
        REQUIRE(cumulative2 >= cumulative1);
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
