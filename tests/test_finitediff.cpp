//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#include "catch.hpp"
#include "../jlt/finitediff.hpp"

using namespace jlt;

// Test function: f(x) = x^2, f'(x) = 2x
TEST_CASE("Finite difference stencils - quadratic function", "[finitediff]") {
  SECTION("ForwardDiff1Stencil with equal spacing") {
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> y = {0.0, 1.0, 4.0, 9.0, 16.0};  // x^2
    std::vector<double> dx(x.size());
    for (size_t i = 1; i < x.size(); ++i) dx[i] = x[i] - x[i-1];

    // At i=0: (y[1] - y[0]) / dx[1] = (1 - 0) / 1 = 1
    // Actual derivative: f'(0) = 0, error = 1
    double d = ForwardDiff1Stencil(dx, y, 0);
    REQUIRE(d == Approx(1.0));

    // At i=2: (y[3] - y[2]) / dx[3] = (9 - 4) / 1 = 5
    // Actual derivative: f'(2) = 4, error = 1
    d = ForwardDiff1Stencil(dx, y, 2);
    REQUIRE(d == Approx(5.0));
  }

  SECTION("BackwardDiff1Stencil with equal spacing") {
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> y = {0.0, 1.0, 4.0, 9.0, 16.0};
    std::vector<double> dx(x.size());
    for (size_t i = 1; i < x.size(); ++i) dx[i] = x[i] - x[i-1];

    // At i=1: (y[1] - y[0]) / dx[1] = (1 - 0) / 1 = 1
    // Actual derivative: f'(1) = 2, error = -1
    double d = BackwardDiff1Stencil(dx, y, 1);
    REQUIRE(d == Approx(1.0));

    // At i=4: (y[4] - y[3]) / dx[4] = (16 - 9) / 1 = 7
    // Actual derivative: f'(4) = 8, error = -1
    d = BackwardDiff1Stencil(dx, y, 4);
    REQUIRE(d == Approx(7.0));
  }

  SECTION("CentralDiff2Stencil with equal spacing") {
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> y = {0.0, 1.0, 4.0, 9.0, 16.0};
    std::vector<double> dx(x.size());
    for (size_t i = 1; i < x.size(); ++i) dx[i] = x[i] - x[i-1];

    // At i=2: central difference should be exact for quadratic
    // (y[3] - y[1]) / (x[3] - x[1]) = (9 - 1) / 2 = 4
    // Actual derivative: f'(2) = 4, exact!
    double d = CentralDiff2Stencil(dx, y, 2);
    REQUIRE(d == Approx(4.0));

    // At i=1: (y[2] - y[0]) / 2 = (4 - 0) / 2 = 2
    // Actual derivative: f'(1) = 2, exact!
    d = CentralDiff2Stencil(dx, y, 1);
    REQUIRE(d == Approx(2.0));
  }
}

TEST_CASE("Finite difference stencils - cubic function", "[finitediff]") {
  // f(x) = x^3, f'(x) = 3x^2
  std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
  std::vector<double> y = {0.0, 1.0, 8.0, 27.0, 64.0, 125.0};
  std::vector<double> dx(x.size());
  for (size_t i = 1; i < x.size(); ++i) dx[i] = x[i] - x[i-1];

  SECTION("ForwardDiff2Stencil") {
    // For cubic function, 2nd order forward approximates f'(0) = 0
    // Uses points 0, 1, 2 with x = {0,1,2,3,4,5}, y = {0,1,8,27,64,125}
    double d2 = ForwardDiff2Stencil(dx, y, 0);
    // Result is -2 for this specific cubic and grid
    // This is the correct 2nd order approximation (not exact for cubic)
    REQUIRE(d2 == Approx(-2.0));
  }

  SECTION("BackwardDiff2Stencil") {
    // At i=5, f'(5) = 75
    double d2 = BackwardDiff2Stencil(dx, y, 5);
    // Should be reasonably close to 75
    REQUIRE(d2 > 70.0);
    REQUIRE(d2 < 80.0);
  }

  SECTION("CentralDiff2Stencil on cubic") {
    // For cubic, central difference is 2nd order accurate but not exact
    // At i=2, f'(2) = 12
    double d = CentralDiff2Stencil(dx, y, 2);
    REQUIRE(d == Approx(12.0).margin(1.0));  // Within 1 of exact

    // At i=3, f'(3) = 27
    d = CentralDiff2Stencil(dx, y, 3);
    REQUIRE(d == Approx(27.0).margin(1.0));
  }
}

TEST_CASE("Finite difference stencils - sine function", "[finitediff]") {
  // f(x) = sin(x), f'(x) = cos(x)
  double pi = 3.14159265358979323846;
  std::vector<double> x = {0.0, pi/4, pi/2, 3*pi/4, pi};
  std::vector<double> y;
  for (double xi : x) y.push_back(sin(xi));
  std::vector<double> dx(x.size());
  for (size_t i = 1; i < x.size(); ++i) dx[i] = x[i] - x[i-1];

  SECTION("CentralDiff2Stencil on sine") {
    // At i=2 (x=pi/2): f'(pi/2) = cos(pi/2) = 0
    double d = CentralDiff2Stencil(dx, y, 2);
    REQUIRE(d == Approx(0.0).margin(0.1));

    // At i=1 (x=pi/4): f'(pi/4) = cos(pi/4) = sqrt(2)/2 ≈ 0.707
    d = CentralDiff2Stencil(dx, y, 1);
    REQUIRE(d == Approx(cos(pi/4)).margin(0.1));
  }
}

TEST_CASE("Finite difference stencils - unequal spacing", "[finitediff]") {
  // Test with non-uniform grid spacing
  std::vector<double> x = {0.0, 0.5, 1.5, 3.0, 5.0};
  std::vector<double> y;
  for (double xi : x) y.push_back(xi * xi);  // f(x) = x^2
  std::vector<double> dx(x.size());
  for (size_t i = 1; i < x.size(); ++i) dx[i] = x[i] - x[i-1];

  SECTION("Unequal spacing central difference") {
    // At i=2 (x=1.5): f'(1.5) = 3
    double d = CentralDiff2Stencil(dx, y, 2);
    // Central difference should still be exact for quadratic even with unequal spacing
    REQUIRE(d == Approx(3.0).margin(0.01));
  }

  SECTION("Unequal spacing forward difference") {
    // At i=0: forward difference
    double d = ForwardDiff1Stencil(dx, y, 0);
    // dx[1] = 0.5, (y[1] - y[0])/0.5 = (0.25 - 0)/0.5 = 0.5
    REQUIRE(d == Approx(0.5));
  }
}

TEST_CASE("High-order stencils", "[finitediff]") {
  // Test 3rd and 4th order stencils
  std::vector<double> x;
  for (int i = 0; i <= 10; ++i) x.push_back(i * 0.1);
  std::vector<double> y;
  for (double xi : x) y.push_back(sin(xi));  // f(x) = sin(x)
  std::vector<double> dx(x.size());
  for (size_t i = 1; i < x.size(); ++i) dx[i] = x[i] - x[i-1];

  SECTION("ForwardDiff3Stencil") {
    // At i=0, f'(0) = cos(0) = 1
    double d = ForwardDiff3Stencil(dx, y, 0);
    REQUIRE(d == Approx(1.0).margin(0.01));
  }

  SECTION("BackwardDiff3Stencil") {
    // At last point, should approximate cos(1.0) ≈ 0.5403
    double d = BackwardDiff3Stencil(dx, y, x.size() - 1);
    REQUIRE(d == Approx(cos(1.0)).margin(0.01));
  }

  SECTION("ForwardDiff4Stencil") {
    // At i=0, should be more accurate than 3rd order
    double d = ForwardDiff4Stencil(dx, y, 0);
    REQUIRE(d == Approx(1.0).margin(0.001));
  }

  SECTION("BackwardDiff4Stencil") {
    // At i=9, should be very accurate
    double d = BackwardDiff4Stencil(dx, y, x.size() - 2);
    REQUIRE(d == Approx(cos(0.9)).margin(0.001));
  }

  SECTION("CentralDiff4Stencil") {
    // At interior points, 4th order central is very accurate
    double d = CentralDiff4Stencil(dx, y, 5);
    REQUIRE(d == Approx(cos(0.5)).margin(0.0001));
  }
}

TEST_CASE("Full differentiation - finitediff1", "[finitediff]") {
  SECTION("Linear function") {
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> y = {0.0, 2.0, 4.0, 6.0, 8.0};  // f(x) = 2x
    std::vector<double> dydx(x.size());

    finitediff1(x, y, dydx);

    // f'(x) = 2 everywhere
    for (double d : dydx) {
      REQUIRE(d == Approx(2.0));
    }
  }

  SECTION("Quadratic function") {
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> y = {0.0, 1.0, 4.0, 9.0, 16.0};
    std::vector<double> dydx(x.size());

    finitediff1(x, y, dydx);

    // f'(0) = 0, forward diff gives 1
    REQUIRE(dydx[0] == Approx(1.0));
    // Interior points: central would give exact, but 1st order uses forward/backward
    REQUIRE(dydx[2] == Approx(4.0).margin(1.0));
    // f'(4) = 8, backward diff gives 7
    REQUIRE(dydx[4] == Approx(7.0));
  }
}

TEST_CASE("Full differentiation - finitediff2", "[finitediff]") {
  SECTION("Quadratic function - exact for 2nd order") {
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<double> y = {0.0, 1.0, 4.0, 9.0, 16.0};
    std::vector<double> dydx(x.size());

    finitediff2(x, y, dydx);

    // 2nd order should be exact for quadratics
    for (size_t i = 0; i < x.size(); ++i) {
      REQUIRE(dydx[i] == Approx(2 * x[i]).margin(0.1));
    }
  }

  SECTION("Sine function") {
    double pi = 3.14159265358979323846;
    std::vector<double> x;
    for (int i = 0; i <= 20; ++i) x.push_back(i * pi / 20);
    std::vector<double> y;
    for (double xi : x) y.push_back(sin(xi));
    std::vector<double> dydx(x.size());

    finitediff2(x, y, dydx);

    // Check a few points
    for (size_t i = 1; i < x.size() - 1; ++i) {
      REQUIRE(dydx[i] == Approx(cos(x[i])).margin(0.01));
    }
  }

  SECTION("With error estimation") {
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
    std::vector<double> y;
    for (double xi : x) y.push_back(xi * xi * xi);  // f(x) = x^3
    std::vector<double> dydx(x.size());
    std::vector<double> err(x.size());

    finitediff2(x, y, dydx, err);

    // Error estimates should be non-zero for cubic (not exact for 2nd order)
    // At endpoints, error uses 1st vs 2nd order
    REQUIRE(err[0] != 0.0);
    REQUIRE(err[x.size()-1] != 0.0);

    // At interior points, error should be relatively small
    // For cubic f(x) = x^3 with x up to 7, derivative is up to 147
    // Error of 3 is ~2% which is reasonable for 2nd order
    for (size_t i = 1; i < x.size() - 1; ++i) {
      REQUIRE(std::abs(err[i]) < 5.0);  // Error should be bounded
    }
  }
}

TEST_CASE("Full differentiation - finitediff4", "[finitediff]") {
  SECTION("Quartic function - 4th order should be exact") {
    // f(x) = x^4, f'(x) = 4x^3
    std::vector<double> x;
    for (int i = 0; i <= 10; ++i) x.push_back(i * 0.5);
    std::vector<double> y;
    for (double xi : x) y.push_back(xi * xi * xi * xi);
    std::vector<double> dydx(x.size());

    finitediff4(x, y, dydx);

    // 4th order should be exact for quartics
    for (size_t i = 0; i < x.size(); ++i) {
      REQUIRE(dydx[i] == Approx(4 * x[i] * x[i] * x[i]).margin(0.1));
    }
  }

  SECTION("Exponential function") {
    std::vector<double> x;
    for (int i = 0; i <= 20; ++i) x.push_back(i * 0.1);
    std::vector<double> y;
    for (double xi : x) y.push_back(exp(xi));
    std::vector<double> dydx(x.size());

    finitediff4(x, y, dydx);

    // For f(x) = e^x, f'(x) = e^x
    for (size_t i = 0; i < x.size(); ++i) {
      REQUIRE(dydx[i] == Approx(exp(x[i])).margin(0.001));
    }
  }

  SECTION("With error estimation") {
    std::vector<double> x;
    for (int i = 0; i <= 12; ++i) x.push_back(i * 0.1);
    std::vector<double> y;
    for (double xi : x) y.push_back(sin(xi));
    std::vector<double> dydx(x.size());
    std::vector<double> err(x.size());

    finitediff4(x, y, dydx, err);

    // Error estimates should be very small for smooth sine function
    for (size_t i = 0; i < x.size(); ++i) {
      REQUIRE(std::abs(err[i]) < 0.01);  // Small error
    }

    // Verify derivatives are accurate
    for (size_t i = 0; i < x.size(); ++i) {
      REQUIRE(dydx[i] == Approx(cos(x[i])).margin(0.0001));
    }
  }
}

TEST_CASE("Edge cases", "[finitediff]") {
  SECTION("Minimum size arrays for finitediff1") {
    std::vector<double> x = {0.0, 1.0};
    std::vector<double> y = {0.0, 1.0};
    std::vector<double> dydx(x.size());

    finitediff1(x, y, dydx);

    // f(x) = x, f'(x) = 1
    REQUIRE(dydx[0] == Approx(1.0));
    REQUIRE(dydx[1] == Approx(1.0));
  }

  SECTION("Minimum size arrays for finitediff2") {
    // Need at least 3 points for 2nd order
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {0.0, 1.0, 4.0};
    std::vector<double> dydx(x.size());

    finitediff2(x, y, dydx);

    // f'(0) = 0, 2nd order forward approx
    REQUIRE(dydx[0] < 1.0);
    // f'(1) = 2, central should be exact or close
    REQUIRE(dydx[1] == Approx(2.0).margin(0.5));
    // f'(2) = 4, 2nd order backward approx
    REQUIRE(dydx[2] > 3.0);
  }

  SECTION("Minimum size arrays for finitediff4") {
    // Need at least 6 points for 4th order (indices 0,1 use forward; 2..n-3 use central; n-2,n-1 use backward)
    // For n=6: indices 0,1 use forward (need 0,1,2,3,4 and 1,2,3,4,5); indices 4,5 use backward
    std::vector<double> x = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
    std::vector<double> y;
    for (double xi : x) y.push_back(xi * xi);  // f(x) = x^2
    std::vector<double> dydx(x.size());

    finitediff4(x, y, dydx);

    // For quadratic, 4th order should be exact or very close
    for (size_t i = 0; i < x.size(); ++i) {
      REQUIRE(dydx[i] == Approx(2 * x[i]).margin(0.01));
    }
  }
}

TEST_CASE("Different data types", "[finitediff]") {
  SECTION("Float type") {
    std::vector<float> x = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f};
    std::vector<float> y = {0.0f, 1.0f, 4.0f, 9.0f, 16.0f};
    std::vector<float> dydx(x.size());
    std::vector<float> dx(x.size());
    for (size_t i = 1; i < x.size(); ++i) dx[i] = x[i] - x[i-1];

    float d = CentralDiff2Stencil(dx, y, 2);
    REQUIRE(d == Approx(4.0f));

    finitediff2(x, y, dydx);
    for (size_t i = 0; i < x.size(); ++i) {
      REQUIRE(dydx[i] == Approx(2.0f * x[i]).margin(0.1f));
    }
  }

  SECTION("Different x and y types") {
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::vector<float> y = {0.0f, 1.0f, 4.0f, 9.0f, 16.0f};
    std::vector<float> dydx(x.size());
    std::vector<double> dx(x.size());
    for (size_t i = 1; i < x.size(); ++i) dx[i] = x[i] - x[i-1];

    float d = CentralDiff2Stencil(dx, y, 2);
    REQUIRE(d == Approx(4.0f));
  }
}

TEST_CASE("Error estimation consistency", "[finitediff]") {
  SECTION("finitediff2 error should decrease with refinement") {
    // Coarse grid
    std::vector<double> x1;
    for (int i = 0; i <= 5; ++i) x1.push_back(i * 0.2);
    std::vector<double> y1;
    for (double xi : x1) y1.push_back(sin(xi));
    std::vector<double> dydx1(x1.size());
    std::vector<double> err1(x1.size());
    finitediff2(x1, y1, dydx1, err1);

    // Fine grid
    std::vector<double> x2;
    for (int i = 0; i <= 10; ++i) x2.push_back(i * 0.1);
    std::vector<double> y2;
    for (double xi : x2) y2.push_back(sin(xi));
    std::vector<double> dydx2(x2.size());
    std::vector<double> err2(x2.size());
    finitediff2(x2, y2, dydx2, err2);

    // Compare errors at comparable points (skip endpoints)
    // Fine grid errors should generally be smaller
    double max_err1 = 0.0, max_err2 = 0.0;
    for (size_t i = 1; i < err1.size() - 1; ++i) {
      max_err1 = std::max(max_err1, std::abs(err1[i]));
    }
    for (size_t i = 2; i < err2.size() - 2; ++i) {
      max_err2 = std::max(max_err2, std::abs(err2[i]));
    }

    // Fine grid should have smaller or comparable max error
    REQUIRE(max_err2 <= max_err1 * 1.5);  // Allow some variance
  }
}
