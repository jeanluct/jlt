#include "catch.hpp"
#include "../jlt/exceptions.hpp"
#include <stdexcept>

using namespace jlt;

TEST_CASE("too_many_steps exception", "[exceptions]") {
    SECTION("construction with description and steps") {
        too_many_steps exc("Iteration failed", 1000);
        
        REQUIRE(exc.what() == std::string("Iteration failed"));
        REQUIRE(exc.how_many() == 1000);
    }
    
    SECTION("construction with empty description") {
        too_many_steps exc("", 5);
        
        REQUIRE(exc.what() == std::string(""));
        REQUIRE(exc.how_many() == 5);
    }
    
    SECTION("large number of steps") {
        too_many_steps exc("Exceeded maximum steps", 1000000UL);
        
        REQUIRE(exc.how_many() == 1000000UL);
    }
    
    SECTION("throw and catch too_many_steps") {
        bool caught = false;
        unsigned long int steps = 0;
        
        try {
            throw too_many_steps("Test message", 42);
        } catch (const too_many_steps& e) {
            caught = true;
            steps = e.how_many();
            REQUIRE(e.what() == std::string("Test message"));
        }
        
        REQUIRE(caught);
        REQUIRE(steps == 42);
    }
    
    SECTION("catch as std::runtime_error") {
        bool caught = false;
        
        try {
            throw too_many_steps("Test", 10);
        } catch (const std::runtime_error& e) {
            caught = true;
            // Verify it can be caught as base class
            REQUIRE(e.what() == std::string("Test"));
        }
        
        REQUIRE(caught);
    }
}

TEST_CASE("stepsize_too_small exception (double)", "[exceptions]") {
    SECTION("construction with description and stepsize") {
        stepsize_too_small<double> exc("Step size too small", 1e-10);
        
        REQUIRE(exc.what() == std::string("Step size too small"));
        REQUIRE(exc.how_small() == Approx(1e-10));
    }
    
    SECTION("construction with very small stepsize") {
        stepsize_too_small<double> exc("Tiny step", 1e-100);
        
        REQUIRE(exc.how_small() == Approx(1e-100));
    }
    
    SECTION("throw and catch stepsize_too_small") {
        bool caught = false;
        double stepsize = 0.0;
        
        try {
            throw stepsize_too_small<double>("Convergence failed", 0.001);
        } catch (const stepsize_too_small<double>& e) {
            caught = true;
            stepsize = e.how_small();
            REQUIRE(e.what() == std::string("Convergence failed"));
        }
        
        REQUIRE(caught);
        REQUIRE(stepsize == Approx(0.001));
    }
    
    SECTION("catch as std::runtime_error") {
        bool caught = false;
        
        try {
            throw stepsize_too_small<double>("Test", 0.01);
        } catch (const std::runtime_error& e) {
            caught = true;
            REQUIRE(e.what() == std::string("Test"));
        }
        
        REQUIRE(caught);
    }
}

TEST_CASE("stepsize_too_small exception (float)", "[exceptions]") {
    SECTION("construction with float stepsize") {
        stepsize_too_small<float> exc("Float step small", 0.001f);
        
        REQUIRE(exc.how_small() == Approx(0.001f));
    }
    
    SECTION("throw and catch float stepsize") {
        bool caught = false;
        float stepsize = 0.0f;
        
        try {
            throw stepsize_too_small<float>("Float test", 0.01f);
        } catch (const stepsize_too_small<float>& e) {
            caught = true;
            stepsize = e.how_small();
            REQUIRE(e.what() == std::string("Float test"));
        }
        
        REQUIRE(caught);
        REQUIRE(stepsize == Approx(0.01f));
    }
}

TEST_CASE("failed_to_converge exception (double)", "[exceptions]") {
    SECTION("construction with description and residual") {
        failed_to_converge<double> exc("Newton method failed", 1e-6);
        
        REQUIRE(exc.what() == std::string("Newton method failed"));
        REQUIRE(exc.residual() == Approx(1e-6));
    }
    
    SECTION("construction with large residual") {
        failed_to_converge<double> exc("Large residual", 100.0);
        
        REQUIRE(exc.residual() == Approx(100.0));
    }
    
    SECTION("throw and catch failed_to_converge") {
        bool caught = false;
        double residual = 0.0;
        
        try {
            throw failed_to_converge<double>("Solver failed", 0.001);
        } catch (const failed_to_converge<double>& e) {
            caught = true;
            residual = e.residual();
            REQUIRE(e.what() == std::string("Solver failed"));
        }
        
        REQUIRE(caught);
        REQUIRE(residual == Approx(0.001));
    }
    
    SECTION("catch as std::runtime_error") {
        bool caught = false;
        
        try {
            throw failed_to_converge<double>("Test", 0.5);
        } catch (const std::runtime_error& e) {
            caught = true;
            REQUIRE(e.what() == std::string("Test"));
        }
        
        REQUIRE(caught);
    }
}

TEST_CASE("failed_to_converge exception (float)", "[exceptions]") {
    SECTION("construction with float residual") {
        failed_to_converge<float> exc("Float residual", 0.01f);
        
        REQUIRE(exc.residual() == Approx(0.01f));
    }
    
    SECTION("throw and catch float residual") {
        bool caught = false;
        float residual = 0.0f;
        
        try {
            throw failed_to_converge<float>("Float conv failed", 0.001f);
        } catch (const failed_to_converge<float>& e) {
            caught = true;
            residual = e.residual();
            REQUIRE(e.what() == std::string("Float conv failed"));
        }
        
        REQUIRE(caught);
        REQUIRE(residual == Approx(0.001f));
    }
}

TEST_CASE("throwing and rethrowing", "[exceptions]") {
    SECTION("throw and rethrow") {
        bool caught = false;
        
        try {
            try {
                throw std::runtime_error("Inner exception");
            } catch (...) {
                throw;  // rethrow
            }
        } catch (const std::runtime_error&) {
            caught = true;
        }
        
        REQUIRE(caught);
    }
    
    SECTION("throw and rethrow custom exception") {
        bool caught = false;
        int steps = 0;
        
        try {
            try {
                throw too_many_steps("First throw", 10);
            } catch (...) {
                throw;  // rethrow
            }
        } catch (const too_many_steps& e) {
            caught = true;
            steps = e.how_many();
        }
        
        REQUIRE(caught);
        REQUIRE(steps == 10);
    }
}

TEST_CASE("multiple exception types in same catch", "[exceptions]") {
    SECTION("throwing too_many_steps") {
        bool caught_too_many = false;
        bool caught_too_small = false;
        bool caught_failed_conv = false;
        
        try {
            throw too_many_steps("Test", 5);
        } catch (const too_many_steps&) {
            caught_too_many = true;
        } catch (const stepsize_too_small<double>&) {
            caught_too_small = true;
        } catch (const failed_to_converge<double>&) {
            caught_failed_conv = true;
        }
        
        REQUIRE(caught_too_many);
        REQUIRE_FALSE(caught_too_small);
        REQUIRE_FALSE(caught_failed_conv);
    }
}

TEST_CASE("exception inheritance", "[exceptions]") {
    SECTION("all exceptions inherit from std::runtime_error") {
        bool runtime_caught_1 = false;
        bool runtime_caught_2 = false;
        bool runtime_caught_3 = false;
        
        try {
            throw too_many_steps("Test", 10);
        } catch (const std::runtime_error&) {
            runtime_caught_1 = true;
        }
        
        try {
            throw stepsize_too_small<double>("Test", 0.1);
        } catch (const std::runtime_error&) {
            runtime_caught_2 = true;
        }
        
        try {
            throw failed_to_converge<double>("Test", 0.5);
        } catch (const std::runtime_error&) {
            runtime_caught_3 = true;
        }
        
        REQUIRE(runtime_caught_1);
        REQUIRE(runtime_caught_2);
        REQUIRE(runtime_caught_3);
    }
}
