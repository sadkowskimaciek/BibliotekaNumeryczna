#include "../include/numlib/RownaniaLiniowe.h"
#include <cassert>

void LinearEquationTest1() {
    vector<vector<double>> A = {
        {2, 1},
        {1, 3}
    };
    vector<double> b = { 5, 6 };

    auto x = numlib::Gauss(A, b);

    assert(abs(x[0] - 1.8) < 1e-6);
    assert(abs(x[1] - 1.4) < 1e-6);

    cout << "Test1 Passed\n";
}

void LinearEquationTest2() {
    vector<vector<double>> A = {
        {2, -1, 0},
        {-1, 2, -1},
        {0, -1, 2}
    };
    vector<double> b = { 1, 0, 1 };

    auto x = numlib::Gauss(A, b);

    double check1 = 2 * x[0] - x[1];
    double check2 = -x[0] + 2 * x[1] - x[2];
    double check3 = -x[1] + 2 * x[2];

    assert(abs(check1 - 1) < 1e-6);
    assert(abs(check2 - 0) < 1e-6);
    assert(abs(check3 - 1) < 1e-6);

    cout << "Test2 PASSED\n";
}

int main() {

    LinearEquationTest1();
    LinearEquationTest2();

    return 0;
}