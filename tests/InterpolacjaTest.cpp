#include "../include/numlib/Interpolacja.h"
#include <string>

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

void InterpolationTest1() {
    vector<double> x = { -1, 0, 1 };
    vector<double> y = { 1, 0, 1 }; 

    auto coeffs = numlib::DividedDiff(x, y);

    for (int i = 0; i < x.size(); i++) {
        double result = numlib::Newton(x, coeffs, x[i]);
        assertTest(abs(result - y[i]) < 1e-10,
            "B³¹d interpolacji w punkcie x = " + to_string(x[i]));
    }

    double result = numlib::Newton(x, coeffs, 0.5);
    assertTest(abs(result - 0.25) < 1e-10,
        "B³¹d interpolacji w punkcie x = 0.5");
}

void InterpolationTest2() {
    vector<double> x;
    vector<double> y;
    int n = 15; 

    for (int i = 0; i < n; i++) {
        double xi = -1.0 + 2.0 * i / (n - 1);
        x.push_back(xi);
        y.push_back(1.0 / (1.0 + 25.0 * xi * xi));
    }

    auto coeffs = numlib::DividedDiff(x, y);

    double x_test = 0.75;
    double y_exact = 1.0 / (1.0 + 25.0 * x_test * x_test); 
    double y_interp = numlib::Newton(x, coeffs, x_test);   

    cout << "Wartoœæ dok³adna w x = " << x_test << ": " << y_exact << endl;
    cout << "Wartoœæ interpolowana: " << y_interp << endl;
    cout << "B³¹d bezwzglêdny: " << abs(y_exact - y_interp) << endl;

    assertTest(abs(y_exact - y_interp) < 0.1,
        "Interpolacja funkcji Rungego powinna dawaæ znacz¹cy b³¹d");
}

int main() {
    int passedTests = 0;
    int totalTests = 2;

    if(runTest("Test interpolacji wielomianem kwadratowym", InterpolationTest1)) {
        passedTests++;
    }
    if(runTest("Test interpolacji funkcji Rungego", InterpolationTest2)) {
        passedTests++;
    }
    
    cout << "\n=== Podsumowanie testów ===" << endl;
    cout << "Przesz³o: " << passedTests << "/" << totalTests << endl;

    return passedTests != totalTests;
}