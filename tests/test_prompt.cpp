//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/prompt.hpp"
#include <iostream>
#include <sstream>
#include <string>

using namespace jlt;

namespace {

// read_number writes its prompt to std::cout; swallow it so the tests
// stay readable, and hand back what was written for inspection.
class capture_cout
{
public:
  capture_cout() : old(std::cout.rdbuf(buf.rdbuf())) {}
  ~capture_cout() { std::cout.rdbuf(old); }
  std::string str() const { return buf.str(); }
private:
  std::ostringstream buf;
  std::streambuf* old;
};

} // namespace


TEST_CASE("read_number consumes exactly one line", "[prompt]") {
  SECTION("ordinary range") {
    std::istringstream in("2\nnext\n");
    capture_cout guard;
    REQUIRE(read_number("pick",1,5,1,in) == 2);
    std::string rest;
    std::getline(in,rest);
    REQUIRE(rest == "next");
  }

  // A prompt with a single valid answer must behave like any other, or
  // a script that answers each prompt in turn has its next answer
  // swallowed (ttauto issue #15).
  SECTION("single-choice range still consumes its line") {
    std::istringstream in("1\nnext\n");
    capture_cout guard;
    REQUIRE(read_number("pick",1,1,1,in) == 1);
    std::string rest;
    std::getline(in,rest);
    REQUIRE(rest == "next");
  }

  SECTION("single-choice range is still prompted for") {
    std::istringstream in("1\n");
    capture_cout guard;
    read_number("pick",1,1,1,in);
    REQUIRE(guard.str().find("pick") != std::string::npos);
  }

  SECTION("two single-choice prompts in a row stay in step") {
    std::istringstream in("1\n1\n7\n");
    capture_cout guard;
    REQUIRE(read_number("first",1,1,1,in) == 1);
    REQUIRE(read_number("second",1,1,1,in) == 1);
    REQUIRE(read_number("length",0,100,5,in) == 7);
  }
}

TEST_CASE("read_number defaults and retries", "[prompt]") {
  SECTION("an empty line takes the default") {
    std::istringstream in("\nnext\n");
    capture_cout guard;
    REQUIRE(read_number("pick",1,5,3,in) == 3);
    std::string rest;
    std::getline(in,rest);
    REQUIRE(rest == "next");
  }

  SECTION("an empty line takes the default in a single-choice range") {
    std::istringstream in("\nnext\n");
    capture_cout guard;
    REQUIRE(read_number("pick",4,4,4,in) == 4);
    std::string rest;
    std::getline(in,rest);
    REQUIRE(rest == "next");
  }

  SECTION("an out-of-range answer is rejected and re-read") {
    std::istringstream in("9\n2\n");
    capture_cout guard;
    REQUIRE(read_number("pick",1,5,1,in) == 2);
  }

  SECTION("exhausted input falls back to the default") {
    std::istringstream in("");
    capture_cout guard;
    REQUIRE(read_number("pick",1,5,4,in) == 4);
  }
}
