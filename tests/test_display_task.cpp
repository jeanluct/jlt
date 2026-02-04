//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/display_task.hpp"
#include <sstream>

using namespace jlt;

TEST_CASE("display_task basic construction", "[display_task]") {
    SECTION("Default construction to cerr") {
        display_task disp;
        REQUIRE_FALSE(disp.is_muted());
    }

    SECTION("Construction with custom stream") {
        std::ostringstream oss;
        display_task disp(oss);
        REQUIRE_FALSE(disp.is_muted());
    }

    SECTION("Muted construction") {
        std::ostringstream oss;
        display_task disp(oss, true);  // muted = true
        REQUIRE(disp.is_muted());
    }

    SECTION("Construction with explicit color control") {
        std::ostringstream oss;
        display_task disp(oss, false, false);  // auto_detect = false
        // When auto_detect is false, color_enabled defaults to true
        // (we can then manually disable it if needed)
        REQUIRE(disp.get_color_enabled());  // Default is true

        // Can manually disable
        disp.set_color_enabled(false);
        REQUIRE_FALSE(disp.get_color_enabled());
    }
}

TEST_CASE("display_task configuration", "[display_task]") {
    std::ostringstream oss;
    display_task disp(oss, false, false);  // no auto-detect, no color

    SECTION("Set and get align length") {
        disp.set_align_length(50);
        REQUIRE(disp.get_align_length() == 50);

        disp.set_align_length(100);
        REQUIRE(disp.get_align_length() == 100);
    }

    SECTION("Set and get color enabled") {
        disp.set_color_enabled(true);
        REQUIRE(disp.get_color_enabled());

        disp.set_color_enabled(false);
        REQUIRE_FALSE(disp.get_color_enabled());
    }

    SECTION("Mute and unmute") {
        REQUIRE_FALSE(disp.is_muted());

        disp.mute();
        REQUIRE(disp.is_muted());

        disp.unmute();
        REQUIRE_FALSE(disp.is_muted());
    }

    SECTION("Set and get log level") {
        disp.set_level(LogLevel::Debug);
        REQUIRE(disp.get_level() == LogLevel::Debug);

        disp.set_level(LogLevel::Silent);
        REQUIRE(disp.get_level() == LogLevel::Silent);
    }
}

TEST_CASE("display_task begin/end", "[display_task]") {
    std::ostringstream oss;
    display_task disp(oss, false, false);  // no color, no auto-detect

    SECTION("Simple begin/end") {
        disp.begin("Test task");
        disp.end();

        std::string output = oss.str();
        REQUIRE(output.find("Test task...") != std::string::npos);
        REQUIRE(output.find("ok") != std::string::npos);
    }

    SECTION("end() without begin() throws") {
        REQUIRE_THROWS_AS(disp.end(), std::logic_error);
    }

    SECTION("Multiple begin/end cycles") {
        disp.begin("Task 1");
        disp.end();

        disp.begin("Task 2");
        disp.end();

        std::string output = oss.str();
        REQUIRE(output.find("Task 1...") != std::string::npos);
        REQUIRE(output.find("Task 2...") != std::string::npos);
    }
}

TEST_CASE("display_task log levels", "[display_task]") {
    std::ostringstream oss;
    display_task disp(oss, false, false);

    SECTION("Error message") {
        disp.error("Test error");
        std::string output = oss.str();
        REQUIRE(output.find("Error:") != std::string::npos);
        REQUIRE(output.find("Test error") != std::string::npos);
    }

    SECTION("Warning message") {
        disp.warn("Test warning");
        std::string output = oss.str();
        REQUIRE(output.find("Warning:") != std::string::npos);
        REQUIRE(output.find("Test warning") != std::string::npos);
    }

    SECTION("Info message") {
        disp.info("Test info");
        std::string output = oss.str();
        REQUIRE(output.find("Test info") != std::string::npos);
    }

    SECTION("Debug message at Debug level") {
        disp.set_level(LogLevel::Debug);
        disp.debug("Test debug");
        std::string output = oss.str();
        REQUIRE(output.find("[debug]") != std::string::npos);
        REQUIRE(output.find("Test debug") != std::string::npos);
    }

    SECTION("Debug message suppressed at Info level") {
        disp.set_level(LogLevel::Info);
        disp.debug("Should not appear");
        std::string output = oss.str();
        REQUIRE(output.find("Should not appear") == std::string::npos);
    }

    SECTION("Warning suppressed at Error level") {
        disp.set_level(LogLevel::Error);
        disp.warn("Should not appear");
        std::string output = oss.str();
        REQUIRE(output.find("Should not appear") == std::string::npos);
    }

    SECTION("All messages suppressed at Silent level") {
        disp.set_level(LogLevel::Silent);
        disp.error("Should not appear");
        disp.warn("Should not appear");
        disp.info("Should not appear");
        std::string output = oss.str();
        REQUIRE(output.empty());
    }
}

TEST_CASE("display_task custom status", "[display_task]") {
    std::ostringstream oss;
    display_task disp(oss, false, false);

    SECTION("end_with_status with custom message") {
        disp.begin("Processing");
        disp.end_with_status("completed");

        std::string output = oss.str();
        REQUIRE(output.find("completed") != std::string::npos);
    }

    SECTION("end_with_status without begin() throws") {
        REQUIRE_THROWS_AS(disp.end_with_status("status"), std::logic_error);
    }
}

TEST_CASE("display_task muted behavior", "[display_task]") {
    std::ostringstream oss;
    display_task disp(oss, false, false);

    SECTION("Muted disp produces no output") {
        disp.mute();
        disp.begin("Muted task");
        disp.end();
        disp.error("Muted error");
        disp.warn("Muted warning");

        REQUIRE(oss.str().empty());
    }

    SECTION("Unmuting restores output") {
        disp.mute();
        disp.begin("Muted");
        disp.end();
        REQUIRE(oss.str().empty());

        disp.unmute();
        disp.begin("Visible");
        disp.end();
        REQUIRE(oss.str().find("Visible") != std::string::npos);
    }
}

TEST_CASE("display_task stream output", "[display_task]") {
    std::ostringstream oss;
    display_task disp(oss, false, false);

    SECTION("Direct stream insertion") {
        disp << "Direct output" << 42 << "\n";
        std::string output = oss.str();
        REQUIRE(output.find("Direct output42") != std::string::npos);
    }

    SECTION("Stream output between begin and end") {
        disp.begin("Task");
        disp << "  intermediate output\n";
        disp.end();

        std::string output = oss.str();
        REQUIRE(output.find("intermediate output") != std::string::npos);
    }
}

TEST_CASE("scoped_task RAII guard", "[display_task][scoped_task]") {
    std::ostringstream oss;
    display_task disp(oss, false, false);

    SECTION("scoped_task calls end automatically") {
        {
            scoped_task task(disp, "Scoped task");
        }  // destructor should call end()

        std::string output = oss.str();
        REQUIRE(output.find("Scoped task...") != std::string::npos);
        REQUIRE(output.find("ok") != std::string::npos);
    }

    SECTION("scoped_task early end") {
        {
            scoped_task task(disp, "Early end task");
            task.end();  // end early
        }  // destructor should not call end() again

        std::string output = oss.str();
        // Should only have one "ok"
        size_t pos1 = output.find("ok");
        REQUIRE(pos1 != std::string::npos);
        // Check there's only one "ok"
        REQUIRE(output.find("ok", pos1 + 1) == std::string::npos);
    }

    SECTION("scoped_task end_with_status") {
        {
            scoped_task task(disp, "Custom status task");
            task.end_with_status("done");
        }

        std::string output = oss.str();
        REQUIRE(output.find("done") != std::string::npos);
    }
}

TEST_CASE("display_task multi-line messages", "[display_task]") {
    std::ostringstream oss;
    display_task disp(oss, false, false);

    SECTION("Message with embedded newlines") {
        disp.begin("Line1\nLine2");
        disp.end();

        std::string output = oss.str();
        REQUIRE(output.find("Line1") != std::string::npos);
        REQUIRE(output.find("Line2") != std::string::npos);
    }
}
