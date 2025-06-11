#include "../include/numlib/RownaniaRozniczkowe.h"
#include <string>
#include <cmath>

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

void RungeKuttaTest1() {
    auto f = [](double x, double y) { return x + y; };
    double x0 = 0.0;
    double y0 = 1.0;
    double h = 0.1;
    int n = 10;

    auto result = numlib::RungegKutty(f, x0, y0, h, n);
    vector<double>& x_values = result.first;
    vector<double>& y_values = result.second;

    assertTest(x_values.size() == n + 1, 
        "Nieprawid³owa liczba punktów w wyniku");

    assertTest(abs(y_values[0] - y0) < 1e-10,
        "Nieprawid³owy warunek pocz¹tkowy");

    for (int i = 1; i < x_values.size(); i++) {
        assertTest(abs(x_values[i] - x_values[i-1] - h) < 1e-10,
            "Nieprawid³owy krok ca³kowania");
    }

    double y_analytical = -2.0 + 2.0 * exp(1.0);  
    assertTest(abs(y_values[n] - y_analytical) < 0.1,
        "Zbyt du¿y b³¹d rozwi¹zania w x = 1.0");
}

void RungeKuttaTest2() {
    auto f_error = [](double x, double y) {
        return y / x;
        };

    double x0 = 0.0;
    double y0 = 1.0;

    auto result = numlib::RungegKutty(f_error, x0, y0, 0.1, 10);

    for (const auto& y : result.second) {
        if (!isfinite(y)) {
            throw TestFailedException("Metoda zwróci³a nieskoñczonoœæ lub NaN");
        }
    }
}

int main() {
    int passedTests = 0;
    int totalTests = 2;

    if(runTest("Test1", RungeKuttaTest1)) {
        passedTests++;
    }
    if(runTest("Test2", RungeKuttaTest2)) {
        passedTests++;
    }
    
    cout << "\n=== Podsumowanie testów ===" << endl;
    cout << "Przesz³o: " << passedTests << "/" << totalTests << endl;

    return passedTests != totalTests;
}