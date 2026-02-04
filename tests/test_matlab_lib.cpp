//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//
// This test requires Matlab libraries and JLT_MATLAB_LIB_SUPPORT to be defined.
//

#include "catch.hpp"

// Only compile this test if Matlab support is enabled
#ifdef JLT_MATLAB_LIB_SUPPORT

#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include "mat.h"
#include "../jlt/matlab.hpp"
#include "../jlt/matrix.hpp"

using namespace jlt;

// Helper function to read MAT file and return variable
template<typename T>
bool readMatVariable(const std::string& filename, const std::string& varname,
                     T* data, size_t expected_size) {
    MATFile* pmat = matOpen(filename.c_str(), "r");
    if (!pmat) return false;

    mxArray* arr = matGetVariable(pmat, varname.c_str());
    if (!arr) {
        matClose(pmat);
        return false;
    }

    double* pr = mxGetPr(arr);
    size_t num_elements = mxGetNumberOfElements(arr);

    bool success = (num_elements == expected_size);
    if (success && data) {
        for (size_t i = 0; i < num_elements; ++i) {
            data[i] = static_cast<T>(pr[i]);
        }
    }

    mxDestroyArray(arr);
    matClose(pmat);
    return success;
}

TEST_CASE("Matlab binary output - double scalar", "[matlab_lib]") {
    const char* testfile = "/tmp/test_matlab_scalar.mat";

    // Remove file if it exists
    std::remove(testfile);

    // Create MAT file
    MATFile* pmat = matOpen(testfile, "w");
    REQUIRE(pmat != nullptr);

    SECTION("Write and read back scalar") {
        double value = 3.14159;
        printMatlabForm(pmat, value, "pi", "The value of pi");

        // Close file
        matClose(pmat);

        // Read back and verify
        MATFile* pmat_read = matOpen(testfile, "r");
        REQUIRE(pmat_read != nullptr);

        mxArray* arr = matGetVariable(pmat_read, "pi");
        REQUIRE(arr != nullptr);
        REQUIRE(mxGetNumberOfElements(arr) == 1);

        double* pr = mxGetPr(arr);
        REQUIRE(pr[0] == Approx(value));

        mxDestroyArray(arr);

        // Check description variable exists
        mxArray* desc_arr = matGetVariable(pmat_read, "pi_descr");
        REQUIRE(desc_arr != nullptr);
        REQUIRE(mxIsChar(desc_arr));

        char desc_buffer[256];
        mxGetString(desc_arr, desc_buffer, 256);
        REQUIRE(std::string(desc_buffer) == "The value of pi");

        mxDestroyArray(desc_arr);
        matClose(pmat_read);
    }

    // Cleanup
    std::remove(testfile);
}

TEST_CASE("Matlab binary output - std::vector", "[matlab_lib]") {
    const char* testfile = "/tmp/test_matlab_vector.mat";
    std::remove(testfile);

    MATFile* pmat = matOpen(testfile, "w");
    REQUIRE(pmat != nullptr);

    SECTION("Column vector (default)") {
        std::vector<double> v = {1.0, 2.0, 3.0, 4.0, 5.0};
        printMatlabForm(pmat, v, "col_vec", "A column vector");
        matClose(pmat);

        // Read back
        MATFile* pmat_read = matOpen(testfile, "r");
        REQUIRE(pmat_read != nullptr);

        mxArray* arr = matGetVariable(pmat_read, "col_vec");
        REQUIRE(arr != nullptr);
        REQUIRE(mxGetM(arr) == 5);  // 5 rows
        REQUIRE(mxGetN(arr) == 1);  // 1 column

        double* pr = mxGetPr(arr);
        for (size_t i = 0; i < 5; ++i) {
            REQUIRE(pr[i] == Approx(v[i]));
        }

        mxDestroyArray(arr);
        matClose(pmat_read);
    }

    // Cleanup
    std::remove(testfile);
}

TEST_CASE("Matlab binary output - matrix", "[matlab_lib]") {
    const char* testfile = "/tmp/test_matlab_matrix.mat";
    std::remove(testfile);

    MATFile* pmat = matOpen(testfile, "w");
    REQUIRE(pmat != nullptr);

    SECTION("2x3 matrix") {
        matrix<double> m(2, 3);
        m(0, 0) = 1.0; m(0, 1) = 2.0; m(0, 2) = 3.0;
        m(1, 0) = 4.0; m(1, 1) = 5.0; m(1, 2) = 6.0;

        printMatlabForm(pmat, m, "m23", "A 2x3 matrix");
        matClose(pmat);

        // Read back
        MATFile* pmat_read = matOpen(testfile, "r");
        REQUIRE(pmat_read != nullptr);

        mxArray* arr = matGetVariable(pmat_read, "m23");
        REQUIRE(arr != nullptr);
        REQUIRE(mxGetM(arr) == 2);  // 2 rows
        REQUIRE(mxGetN(arr) == 3);  // 3 columns

        double* pr = mxGetPr(arr);
        // Matlab uses column-major order
        REQUIRE(pr[0] == Approx(1.0));  // m(0,0)
        REQUIRE(pr[1] == Approx(4.0));  // m(1,0)
        REQUIRE(pr[2] == Approx(2.0));  // m(0,1)
        REQUIRE(pr[3] == Approx(5.0));  // m(1,1)
        REQUIRE(pr[4] == Approx(3.0));  // m(0,2)
        REQUIRE(pr[5] == Approx(6.0));  // m(1,2)

        mxDestroyArray(arr);
        matClose(pmat_read);
    }

    SECTION("Empty matrix") {
        matrix<double> m(0, 0);
        printMatlabForm(pmat, m, "empty_mat");
        matClose(pmat);

        MATFile* pmat_read = matOpen(testfile, "r");
        REQUIRE(pmat_read != nullptr);

        mxArray* arr = matGetVariable(pmat_read, "empty_mat");
        REQUIRE(arr != nullptr);
        REQUIRE(mxIsEmpty(arr));

        mxDestroyArray(arr);
        matClose(pmat_read);
    }

    // Cleanup
    std::remove(testfile);
}

TEST_CASE("Matlab binary output - string", "[matlab_lib]") {
    const char* testfile = "/tmp/test_matlab_string.mat";
    std::remove(testfile);

    MATFile* pmat = matOpen(testfile, "w");
    REQUIRE(pmat != nullptr);

    SECTION("Write and read back string") {
        std::string test_str = "Hello Matlab!";
        printMatlabForm(pmat, test_str, "message", "A greeting");
        matClose(pmat);

        // Read back
        MATFile* pmat_read = matOpen(testfile, "r");
        REQUIRE(pmat_read != nullptr);

        mxArray* arr = matGetVariable(pmat_read, "message");
        REQUIRE(arr != nullptr);
        REQUIRE(mxIsChar(arr));

        char str_buffer[256];
        mxGetString(arr, str_buffer, 256);
        REQUIRE(std::string(str_buffer) == test_str);

        mxDestroyArray(arr);
        matClose(pmat_read);
    }

    // Cleanup
    std::remove(testfile);
}

TEST_CASE("Matlab binary output - multiple variables", "[matlab_lib]") {
    const char* testfile = "/tmp/test_matlab_multi.mat";
    std::remove(testfile);

    MATFile* pmat = matOpen(testfile, "w");
    REQUIRE(pmat != nullptr);

    SECTION("Write multiple types") {
        // Scalar
        printMatlabForm(pmat, 42.0, "answer");

        // Vector
        std::vector<double> v = {1.0, 2.0, 3.0};
        printMatlabForm(pmat, v, "vec");

        // Matrix
        matrix<double> m(2, 2);
        m(0, 0) = 1.0; m(0, 1) = 0.0;
        m(1, 0) = 0.0; m(1, 1) = 1.0;
        printMatlabForm(pmat, m, "identity");

        // String
        printMatlabForm(pmat, "test complete", "status");

        matClose(pmat);

        // Read back and verify all exist
        MATFile* pmat_read = matOpen(testfile, "r");
        REQUIRE(pmat_read != nullptr);

        // Check scalar
        mxArray* arr = matGetVariable(pmat_read, "answer");
        REQUIRE(arr != nullptr);
        REQUIRE(mxGetNumberOfElements(arr) == 1);
        REQUIRE(mxGetPr(arr)[0] == Approx(42.0));
        mxDestroyArray(arr);

        // Check vector
        arr = matGetVariable(pmat_read, "vec");
        REQUIRE(arr != nullptr);
        REQUIRE(mxGetNumberOfElements(arr) == 3);
        mxDestroyArray(arr);

        // Check matrix
        arr = matGetVariable(pmat_read, "identity");
        REQUIRE(arr != nullptr);
        REQUIRE(mxGetM(arr) == 2);
        REQUIRE(mxGetN(arr) == 2);
        mxDestroyArray(arr);

        // Check string
        arr = matGetVariable(pmat_read, "status");
        REQUIRE(arr != nullptr);
        REQUIRE(mxIsChar(arr));
        mxDestroyArray(arr);

        matClose(pmat_read);
    }

    // Cleanup
    std::remove(testfile);
}

TEST_CASE("Matlab binary output - vector<vector>", "[matlab_lib]") {
    const char* testfile = "/tmp/test_matlab_vv.mat";
    std::remove(testfile);

    MATFile* pmat = matOpen(testfile, "w");
    REQUIRE(pmat != nullptr);

    SECTION("2D vector as matrix") {
        std::vector<std::vector<double>> vv = {
            {1.0, 2.0, 3.0},
            {4.0, 5.0, 6.0}
        };
        printMatlabForm(pmat, vv, "vv23");
        matClose(pmat);

        MATFile* pmat_read = matOpen(testfile, "r");
        REQUIRE(pmat_read != nullptr);

        mxArray* arr = matGetVariable(pmat_read, "vv23");
        REQUIRE(arr != nullptr);
        REQUIRE(mxGetM(arr) == 2);  // 2 rows
        REQUIRE(mxGetN(arr) == 3);  // 3 columns

        double* pr = mxGetPr(arr);
        // Column-major order
        REQUIRE(pr[0] == Approx(1.0));  // vv[0][0]
        REQUIRE(pr[1] == Approx(4.0));  // vv[1][0]
        REQUIRE(pr[2] == Approx(2.0));  // vv[0][1]
        REQUIRE(pr[3] == Approx(5.0));  // vv[1][1]
        REQUIRE(pr[4] == Approx(3.0));  // vv[0][2]
        REQUIRE(pr[5] == Approx(6.0));  // vv[1][2]

        mxDestroyArray(arr);
        matClose(pmat_read);
    }

    // Cleanup
    std::remove(testfile);
}

#endif // JLT_MATLAB_LIB_SUPPORT
