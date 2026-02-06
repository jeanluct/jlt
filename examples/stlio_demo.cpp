//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

//
// stlio_demo.cpp
//
// Demonstration of stlio.hpp formatting capabilities.
// Shows how different types are formatted with field widths and separators.
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <valarray>
#include <list>
#include <map>
#include <array>
#include <deque>
#include <set>
#include <complex>
#include <tuple>
#include "../jlt/stlio.hpp"

using namespace jlt;

// Helper to print a section header
void print_section(const char* title) {
  std::cout << "\n" << std::string(70, '=') << "\n";
  std::cout << "  " << title << "\n";
  std::cout << std::string(70, '=') << "\n";
}

// Helper to print subsection
void print_subsection(const char* title) {
  std::cout << "\n--- " << title << " ---\n";
}

int main()
{
  std::cout << "STLIO Formatting Demonstration\n";
  std::cout << "==============================\n";
  std::cout << "Shows how different types are formatted with field widths and separators.\n";
  std::cout << "Default separator is two spaces between elements.\n";

  // Integer types
  print_section("Integer Types");

  print_subsection("int (field_width = 6)");
  std::vector<int> vi = { 99999, -99999, 12345, -12345, 54321 };
  std::cout << "Values: " << vi << std::endl;

  print_subsection("long long (field_width = 20)");
  std::vector<long long> vll = { 9223372036854775807LL, -9223372036854775807LL, 1234567890123456789LL };
  std::cout << "Values: " << vll << std::endl;

  print_subsection("unsigned int (field_width = 10)");
  std::vector<unsigned int> vui = { 4294967295u, 1234567890u, 987654321u };
  std::cout << "Values: " << vui << std::endl;

  // Floating point types
  print_section("Floating Point Types");

  print_subsection("float (field_width = 13)");
  std::vector<float> vf = { 1234567.0f, -1234567.0f, 3.14159f, 1e6f };
  std::cout << std::setprecision(6);
  std::cout << "Values: " << vf << std::endl;

  print_subsection("double (field_width = 15) - default");
  std::vector<double> vd = { 123456789012345.0, -123456789012345.0, 3.14159265358979, 1e15 };
  std::cout << "Values: " << vd << std::endl;

  print_subsection("long double (field_width = 22)");
  std::vector<long double> vld = { 1.2345678901234567e19L, -1.2345678901234567e19L };
  std::cout << "Values: " << vld << std::endl;

  // Scientific notation
  print_subsection("Scientific notation (extra_width_scientific = 7)");
  std::vector<double> vsci = {1.5e-10, 2.3e15, -5.6e20};
  std::cout << std::scientific << std::setprecision(3);
  std::cout << "Values: " << vsci << std::endl;
  std::cout << std::fixed;  // Reset

  // Boolean
  print_section("Boolean Type");

  print_subsection("bool (field_width = 1; textual width = 5 when std::boolalpha)");
  std::vector<bool> vb = {true, false, true, true};
  std::cout << "Values: " << vb << std::endl;

  // Demonstrate textual booleans with std::boolalpha
  print_subsection("bool (with std::boolalpha)");
  std::cout << std::boolalpha;
  std::cout << "Values: " << vb << std::endl;
  std::cout << std::noboolalpha; // restore numeric form

  // Complex numbers
  print_section("Complex Numbers");

  print_subsection("complex<double> (field_width = 2*15 + 3 = 33)");
  std::vector<std::complex<double>> vc = {
    {123456789012345.0, 2.0},
    {-3.5, -123456789012345.0},
    {0.0, 1.0}
  };
  std::cout << std::setprecision(4);
  std::cout << "Values: " << vc << std::endl;

  // Different container types
  print_section("Different Container Types");

  print_subsection("std::vector<int>");
  std::cout << vi << std::endl;

  print_subsection("std::valarray<double>");
  std::valarray<double> va(3);
  va[0] = 1.1; va[1] = 2.2; va[2] = 3.3;
  std::cout << va << std::endl;

  print_subsection("std::list<std::string>");
  std::list<std::string> ls = {"hello", "world", "test"};
  std::cout << ls << std::endl;

  print_subsection("std::array<int, 5>");
  std::array<int, 5> arr = {1, 2, 3, 4, 5};
  std::cout << arr << std::endl;

  print_subsection("std::deque<double>");
  std::deque<double> deq = {1.1, 2.2, 3.3, 4.4};
  std::cout << deq << std::endl;

  print_subsection("std::set<int> (sorted)");
  std::set<int> si = {3, 1, 4, 1, 5, 9, 2, 6};
  std::cout << si << std::endl;

  // Pairs and tuples
  print_section("Pairs and Tuples");

  print_subsection("std::pair<int, double>");
  std::vector<std::pair<int, double>> vp = {{99999, 123456789012345.0}, {-99999, -1.2345e10}};
  std::cout << vp << std::endl;

  print_subsection("std::tuple<int, double, std::string>");
  std::tuple<int, double, std::string> t = std::make_tuple(42, 3.14, "hello");
  std::cout << t << std::endl;

  // Maps
  print_section("Maps");

  print_subsection("std::map<int, std::string>");
  std::map<int, std::string> mis;
  mis[1] = "one";
  mis[2] = "two";
  mis[10] = "ten";
  std::cout << mis << std::endl;

  print_subsection("std::map<double, int> (scientific notation for keys)");
  std::map<double, int> mdi;
  mdi[1.5e-10] = 1;
  mdi[2.3e5] = 2;
  std::cout << mdi << std::endl;

  // Custom separator demonstration via specialization
  print_section("Custom Separator via Template Specialization");
  std::cout << "Users can customize separators by specializing format_traits<T>.\n";
  std::cout << "See the source code for how to define custom format_traits.\n";

  std::cout << "\n" << std::string(70, '=') << "\n";
  std::cout << "End of demonstration.\n";

  return 0;
}
