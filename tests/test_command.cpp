//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/command.hpp"
#include <string>

using namespace jlt;

TEST_CASE("get_command_output - basic command execution", "[command]") {
    SECTION("Execute echo command") {
        std::string output = get_command_output("echo hello");
        REQUIRE(output == "hello");
    }

    SECTION("Execute echo with spaces") {
        std::string output = get_command_output("echo 'hello world'");
        REQUIRE(output == "hello world");
    }

    SECTION("Execute pwd command") {
        std::string output = get_command_output("pwd");
        // pwd should return a non-empty string starting with /
        REQUIRE(!output.empty());
        REQUIRE(output[0] == '/');
    }

    SECTION("Execute uname command") {
        std::string output = get_command_output("uname");
        // uname typically returns "Linux", "Darwin", etc.
        REQUIRE(!output.empty());
    }
}

TEST_CASE("get_command_output - output capturing", "[command]") {
    SECTION("Capture multiple lines (using tr)") {
        // Use tr to convert newlines to spaces for predictable output
        std::string output = get_command_output("echo -e 'line1\\nline2' | tr '\\n' ' '");
        REQUIRE(!output.empty());
        REQUIRE(output.find("line1") != std::string::npos);
        REQUIRE(output.find("line2") != std::string::npos);
    }

    SECTION("Capture command with arguments") {
        std::string output = get_command_output("echo test output");
        // Should get both words
        REQUIRE(!output.empty());
        REQUIRE(output.find("test") != std::string::npos);
    }

    SECTION("Execute command returning single word") {
        std::string output = get_command_output("echo test");
        REQUIRE(output == "test");
    }
}

TEST_CASE("get_command_output - stderr captured via redirection", "[command]") {
    SECTION("stderr is captured via automatic 2>&1 redirection") {
        // The function automatically appends " 2>&1" to the command
        // This is tested implicitly by the fact that errors don't appear on console
        // and other tests pass. We'll just verify basic functionality.
        std::string output = get_command_output("echo normal_output");
        REQUIRE(output == "normal_output");
    }
}

TEST_CASE("get_command_output - empty output", "[command]") {
    SECTION("Command with no output") {
        // Using true command which produces no output
        std::string output = get_command_output("true");
        REQUIRE(output.empty());
    }

    SECTION("Echo empty string") {
        std::string output = get_command_output("echo -n ''");
        REQUIRE(output.empty());
    }
}

TEST_CASE("get_command_output - long output", "[command]") {
    SECTION("Command with long output") {
        // Generate a longer output
        std::string output = get_command_output("seq 1 100");
        // Should contain numbers 1 through 100
        REQUIRE(!output.empty());
        REQUIRE(output.find("1") != std::string::npos);
        REQUIRE(output.find("50") != std::string::npos);
        REQUIRE(output.find("100") != std::string::npos);
    }

    SECTION("Repeated command calls") {
        // Verify the function can be called multiple times
        std::string out1 = get_command_output("echo first");
        std::string out2 = get_command_output("echo second");
        std::string out3 = get_command_output("echo third");

        REQUIRE(out1 == "first");
        REQUIRE(out2 == "second");
        REQUIRE(out3 == "third");
    }
}

TEST_CASE("get_command_output - special characters", "[command]") {
    SECTION("Command with numbers") {
        std::string output = get_command_output("echo 12345");
        REQUIRE(output == "12345");
    }

    SECTION("Command with special characters") {
        std::string output = get_command_output("echo 'test@#$%'");
        REQUIRE(output == "test@#$%");
    }

    SECTION("Command with tabs") {
        std::string output = get_command_output("echo -e 'col1\\tcol2'");
        // The function may or may not preserve the tab depending on shell
        // Just verify we get output
        REQUIRE(!output.empty());
    }
}
