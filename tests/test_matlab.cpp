//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include <sstream>
#include "../jlt/matlab.hpp"
#include "../jlt/matrix.hpp"
#include "../jlt/vector.hpp"

using namespace jlt;

TEST_CASE("printMatlabForm for double - basic output", "[matlab]") {
  std::ostringstream oss;

  SECTION("Double without name") {
    printMatlabForm(oss, 3.14159, "");
    REQUIRE(oss.str() == "3.14159;\n");
  }

  SECTION("Double with name") {
    printMatlabForm(oss, 3.14159, "pi");
    REQUIRE(oss.str() == "pi = 3.14159;\n");
  }

  SECTION("Double with name and description") {
    printMatlabForm(oss, 3.14159, "pi", "The ratio of a circle's circumference to its diameter");
    std::string expected = "pi_descr = 'The ratio of a circle\'s circumference to its diameter';\n"
                           "pi = 3.14159;\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Double without name but with description") {
    printMatlabForm(oss, 3.14159, "", "This is a value");
    std::string expected = "% This is a value\n"
                           "3.14159;\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Integer value") {
    printMatlabForm(oss, 42.0, "answer");
    REQUIRE(oss.str() == "answer = 42;\n");
  }

  SECTION("Negative value") {
    printMatlabForm(oss, -5.5, "neg");
    REQUIRE(oss.str() == "neg = -5.5;\n");
  }

  SECTION("Zero value") {
    printMatlabForm(oss, 0.0, "zero");
    REQUIRE(oss.str() == "zero = 0;\n");
  }
}

TEST_CASE("printMatlabForm for double - overload format", "[matlab]") {
  std::ostringstream oss;

  SECTION("Old format with name first") {
    // Overload: printMatlabForm(strm, name, var, description)
    printMatlabForm(oss, "pi", 3.14159);
    REQUIRE(oss.str() == "pi = 3.14159;\n");
  }

  SECTION("Old format with description") {
    printMatlabForm(oss, "e", 2.71828, "Euler's number");
    std::string expected = "e_descr = 'Euler\'s number';\n"
                           "e = 2.71828;\n";
    REQUIRE(oss.str() == expected);
  }
}

TEST_CASE("printMatlabForm for string", "[matlab]") {
  std::ostringstream oss;

  SECTION("String without name") {
    printMatlabForm(oss, "hello world", "");
    // Note: when name is empty, opening quote is not printed (only closing quote)
    REQUIRE(oss.str() == "hello world';\n");
  }

  SECTION("String with name") {
    printMatlabForm(oss, "hello world", "greeting");
    REQUIRE(oss.str() == "greeting = 'hello world';\n");
  }

  SECTION("String with name and description") {
    printMatlabForm(oss, "hello world", "greeting", "A friendly greeting");
    std::string expected = "greeting_descr = 'A friendly greeting';\n"
                           "greeting = 'hello world';\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("String with description but no name") {
    printMatlabForm(oss, "test", "", "This is a test string");
    // Note: when name is empty, opening quote is not printed
    std::string expected = "% This is a test string\n"
                           "test';\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Empty string") {
    printMatlabForm(oss, "", "empty_str");
    REQUIRE(oss.str() == "empty_str = '';\n");
  }
}

TEST_CASE("printMatlabForm for std::vector", "[matlab]") {
  std::ostringstream oss;

  SECTION("Empty vector") {
    std::vector<double> v;
    printMatlabForm(oss, v, "empty_vec");
    // Note: empty vector should print nothing currently
    REQUIRE(oss.str().empty());
  }

  SECTION("Vector without name") {
    std::vector<double> v = {1.0, 2.0, 3.0, 4.0, 5.0};
    printMatlabForm(oss, v, "");
    std::string expected = "[\n 1\n 2\n 3\n 4\n 5\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Vector with name") {
    std::vector<double> v = {1.0, 2.0, 3.0};
    printMatlabForm(oss, v, "x");
    std::string expected = "x = [\n 1\n 2\n 3\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Vector with name and description") {
    std::vector<double> v = {1.0, 2.0, 3.0};
    printMatlabForm(oss, v, "x", "A simple vector");
    std::string expected = "x_descr = 'A simple vector';\n"
                           "x = [\n 1\n 2\n 3\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Vector with description but no name") {
    std::vector<double> v = {1.0, 2.0};
    printMatlabForm(oss, v, "", "Two values");
    std::string expected = "% Two values\n"
                           "[\n 1\n 2\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Single element vector") {
    std::vector<double> v = {42.0};
    printMatlabForm(oss, v, "single");
    std::string expected = "single = [\n 42\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Vector with negative values") {
    std::vector<double> v = {-1.5, 0.0, 2.5};
    printMatlabForm(oss, v, "mixed");
    std::string expected = "mixed = [\n -1.5\n 0\n 2.5\n];\n";
    REQUIRE(oss.str() == expected);
  }
}

TEST_CASE("printMatlabForm for matrix", "[matlab]") {
  std::ostringstream oss;

  SECTION("Empty matrix") {
    matrix<double> m(0, 0);
    printMatlabForm(oss, m, "empty_mat");
    REQUIRE(oss.str() == "empty_mat = [];\n");
  }

  SECTION("1x1 matrix") {
    matrix<double> m(1, 1);
    m(0, 0) = 5.0;
    printMatlabForm(oss, m, "scalar");
    std::string expected = "scalar = [\n5\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("2x2 matrix") {
    matrix<double> m(2, 2);
    m(0, 0) = 1.0; m(0, 1) = 2.0;
    m(1, 0) = 3.0; m(1, 1) = 4.0;
    printMatlabForm(oss, m, "m22");
    std::string expected = "m22 = [\n1 2\n3 4\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("3x2 matrix") {
    matrix<double> m(3, 2);
    m(0, 0) = 1.0; m(0, 1) = 2.0;
    m(1, 0) = 3.0; m(1, 1) = 4.0;
    m(2, 0) = 5.0; m(2, 1) = 6.0;
    printMatlabForm(oss, m, "m32");
    std::string expected = "m32 = [\n1 2\n3 4\n5 6\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Matrix without name") {
    matrix<double> m(2, 2);
    m(0, 0) = 1.0; m(0, 1) = 0.0;
    m(1, 0) = 0.0; m(1, 1) = 1.0;
    printMatlabForm(oss, m, "");
    std::string expected = "[\n1 0\n0 1\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Matrix with name and description") {
    matrix<double> m(2, 2);
    m(0, 0) = 1.0; m(0, 1) = 2.0;
    m(1, 0) = 3.0; m(1, 1) = 4.0;
    printMatlabForm(oss, m, "A", "A 2x2 matrix");
    std::string expected = "A_descr = 'A 2x2 matrix';\n"
                           "A = [\n1 2\n3 4\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Matrix with description but no name") {
    matrix<double> m(1, 2);
    m(0, 0) = 1.0; m(0, 1) = 2.0;
    printMatlabForm(oss, m, "", "Row vector");
    std::string expected = "% Row vector\n"
                           "[\n1 2\n];\n";
    REQUIRE(oss.str() == expected);
  }
}

TEST_CASE("printMatlabForm for vector<vector>", "[matlab]") {
  std::ostringstream oss;

  SECTION("2D vector 2x3") {
    std::vector<std::vector<double>> vv = {
      {1.0, 2.0, 3.0},
      {4.0, 5.0, 6.0}
    };
    printMatlabForm(oss, vv, "vv23");
    std::string expected = "vv23 = [\n1 2 3\n4 5 6\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("2D vector with name and description") {
    std::vector<std::vector<double>> vv = {
      {1.0, 2.0},
      {3.0, 4.0},
      {5.0, 6.0}
    };
    printMatlabForm(oss, vv, "data", "Some data points");
    std::string expected = "data_descr = 'Some data points';\n"
                           "data = [\n1 2\n3 4\n5 6\n];\n";
    REQUIRE(oss.str() == expected);
  }
}

TEST_CASE("printMatlabForm_nodefaults for matrix", "[matlab]") {
  std::ostringstream oss;

  SECTION("Matrix with nodefaults - requires explicit arguments") {
    matrix<double> m(2, 2);
    m(0, 0) = 1.0; m(0, 1) = 2.0;
    m(1, 0) = 3.0; m(1, 1) = 4.0;
    printMatlabForm_nodefaults<double>(oss, m, "B", "");
    std::string expected = "B = [\n1 2\n3 4\n];\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Matrix with nodefaults and description") {
    matrix<double> m(2, 2);
    m(0, 0) = 5.0; m(0, 1) = 6.0;
    m(1, 0) = 7.0; m(1, 1) = 8.0;
    printMatlabForm_nodefaults<double>(oss, m, "C", "Another matrix");
    std::string expected = "C_descr = 'Another matrix';\n"
                           "C = [\n5 6\n7 8\n];\n";
    REQUIRE(oss.str() == expected);
  }
}

TEST_CASE("Chained printMatlabForm calls", "[matlab]") {
  std::ostringstream oss;

  SECTION("Multiple variables in same stream") {
    printMatlabForm(oss, 3.14159, "pi");
    printMatlabForm(oss, 2.71828, "e");
    printMatlabForm(oss, "hello", "msg");

    std::string expected = "pi = 3.14159;\n"
                           "e = 2.71828;\n"
                           "msg = 'hello';\n";
    REQUIRE(oss.str() == expected);
  }

  SECTION("Mixed types in same stream") {
    printMatlabForm(oss, 42.0, "scalar");
    std::vector<double> v = {1.0, 2.0, 3.0};
    printMatlabForm(oss, v, "vec");
    matrix<double> m(2, 2);
    m(0, 0) = 1.0; m(0, 1) = 0.0;
    m(1, 0) = 0.0; m(1, 1) = 1.0;
    printMatlabForm(oss, m, "identity");

    std::string expected = "scalar = 42;\n"
                           "vec = [\n 1\n 2\n 3\n];\n"
                           "identity = [\n1 0\n0 1\n];\n";
    REQUIRE(oss.str() == expected);
  }
}

TEST_CASE("Edge cases and special values", "[matlab]") {
  std::ostringstream oss;

  SECTION("Very small number") {
    printMatlabForm(oss, 1e-10, "tiny");
    REQUIRE(oss.str().find("tiny = ") != std::string::npos);
    REQUIRE(oss.str().find("e-10") != std::string::npos);
  }

  SECTION("Very large number") {
    printMatlabForm(oss, 1e10, "huge");
    REQUIRE(oss.str().find("huge = ") != std::string::npos);
    // Large numbers use scientific notation (1e+10)
    REQUIRE(oss.str().find("1e+10") != std::string::npos);
  }

  SECTION("Scientific notation") {
    printMatlabForm(oss, 6.022e23, "avogadro");
    REQUIRE(oss.str().find("avogadro = ") != std::string::npos);
  }

  SECTION("Special characters in string") {
    printMatlabForm(oss, "line1\nline2", "multiline");
    std::string result = oss.str();
    REQUIRE(result.find("multiline = '") != std::string::npos);
    // The newline should be preserved in the string literal
    REQUIRE(result.find("\n") != std::string::npos);
  }

  SECTION("Special characters in description") {
    printMatlabForm(oss, 1.0, "x", "Special chars: \t \n \" ' ");
    std::string result = oss.str();
    // Description should be in the output
    REQUIRE(result.find("x_descr") != std::string::npos);
  }
}
