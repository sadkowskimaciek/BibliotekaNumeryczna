#include "../include/numlib/RownaniaLiniowe.h"
#include <cassert>
#include <stdexcept>

class TestFailedException : public std::runtime_error {
public:
    TestFailedException(const string& message) : std::runtime_error(message) {}
};

void assertTest(bool condition, const string& message) {
    if (!condition) {
        throw TestFailedException(message);
    }
}

bool runTest(const string& testName, void (*testFunction)()) {
    cout << "\nUruchamiam " << testName << "..." << endl;
    try {
        testFunction();
        cout << testName << " PASSED" << endl;
        return true;
    }
    catch (const TestFailedException& e) {
        cout << testName << " FAILED: " << e.what() << endl;
        return false;
    }
    catch (const std::exception& e) {
        cout << testName << " FAILED z nieoczekiwanym wyj¹tkiem: " << e.what() << endl;
        return false;
    }
}

void LinearEquationTest1() {
    vector<vector<double>> A = {
        {2, 1},
        {1, 3}
    };
    vector<double> b = { 5, 6 };

    auto x = numlib::Gauss(A, b);

    assertTest(abs(x[0] - 1.8) < 1e-6, "x[0] powinno byæ 1.8");
    assertTest(abs(x[1] - 1.4) < 1e-6, "x[1] powinno byæ 1.4");

    cout << "x[0] = " << x[0] << ", x[1] = " << x[1] << endl;
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

    assertTest(abs(check1 - 1) < 1e-6, "Równanie 1 nie jest spe³nione");
    assertTest(abs(check2 - 0) < 1e-6, "Równanie 2 nie jest spe³nione");
    assertTest(abs(check3 - 1) < 1e-6, "Równanie 3 nie jest spe³nione");

    cout << "check1 = " << check1 << ", check2 = " << check2 << ", check3 = " << check3 << endl;
}

void LinearEquationTest3() {
    vector<vector<double>> A = {
        {2, 1, -1},
        {-3, -1, 2},
        {-2, 1, 2}
    };
    vector<double> b = { 8, -11, -3 };

    auto x = numlib::Gauss(A, b);

    assertTest(abs(x[0] - 2) < 1e-6, "x[0] powinno byæ 2");
    assertTest(abs(x[1] - 3) < 1e-6, "x[1] powinno byæ 3");
    assertTest(abs(x[2] - (-1)) < 1e-6, "x[2] powinno byæ -1");

    cout << "x[0] = " << x[0] << ", x[1] = " << x[1] << ", x[2] = " << x[2] << endl;
}

int main() {
    int passedTests = 0;
    int totalTests = 3;

    if (runTest("Test1", LinearEquationTest1)) passedTests++;
    if (runTest("Test2", LinearEquationTest2)) passedTests++;
    if (runTest("Test3", LinearEquationTest3)) passedTests++;

    cout << "\n=== Podsumowanie testów ===" << endl;
    cout << "Przesz³o: " << passedTests << "/" << totalTests << endl;
    
    return passedTests != totalTests;
}