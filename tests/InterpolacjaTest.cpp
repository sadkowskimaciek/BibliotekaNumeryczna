#include "../include/numlib/Interpolacja.h"
#include <string>

// Klasa wyj¹tku u¿ywana do sygnalizowania niepowodzenia testu
class TestFailedException : public runtime_error {
public:
    TestFailedException(const string& message) : runtime_error(message) {}
};

// Funkcja pomocnicza do weryfikacji warunków testowych
void assertTest(bool condition, const string& message) {
    if (!condition) {
        throw TestFailedException(message);
    }
}


// Obs³uguje wyj¹tki i wyœwietla wyniki testów
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

// Test 1: Sprawdzenie interpolacji wielomianu kwadratowego
// U¿ywa trzech punktów do interpolacji paraboli 
void InterpolationTest1() {
    // Definicja punktów do interpolacji
    vector<double> x = { -1, 0, 1 };     // Wêz³y interpolacji
    vector<double> y = { 1, 0, 1 };      // Wartoœci funkcji w wêz³ach

    // Obliczenie wspó³czynników interpolacji Newtona
    auto coeffs = numlib::DividedDiff(x, y);

    // Sprawdzenie dok³adnoœci interpolacji w wêz³ach
    for (int i = 0; i < x.size(); i++) {
        double result = numlib::Newton(x, coeffs, x[i]);
        assertTest(abs(result - y[i]) < 1e-10,
            "B³¹d interpolacji w punkcie x = " + to_string(x[i]));
    }

    // Sprawdzenie dok³adnoœci interpolacji w punkcie poœrednim
    double result = numlib::Newton(x, coeffs, 0.5);
    assertTest(abs(result - 0.25) < 1e-10,
        "B³¹d interpolacji w punkcie x = 0.5");
}

// Test 2: Demonstracja zjawiska Rungego
// Interpolacja funkcji Rungego f(x) = 1/(1 + 25x^2) na przedziale [-1,1]
void InterpolationTest2() {
    vector<double> x;
    vector<double> y;
    int n = 15;     // Liczba wêz³ów interpolacji

    // Generowanie równoodleg³ych wêz³ów i wartoœci funkcji Rungego
    for (int i = 0; i < n; i++) {
        double xi = -1.0 + 2.0 * i / (n - 1);  // Wêz³y równoodleg³e na [-1,1]
        x.push_back(xi);
        y.push_back(1.0 / (1.0 + 25.0 * xi * xi));  // Funkcja Rungego
    }

    // Obliczenie wspó³czynników interpolacji
    auto coeffs = numlib::DividedDiff(x, y);

    // Test w punkcie kontrolnym
    double x_test = 0.75;
    double y_exact = 1.0 / (1.0 + 25.0 * x_test * x_test);  // Wartoœæ dok³adna
    double y_interp = numlib::Newton(x, coeffs, x_test);     // Wartoœæ interpolowana

    // Wyœwietlenie wyników
    cout << "Wartoœæ dok³adna w x = " << x_test << ": " << y_exact << endl;
    cout << "Wartoœæ interpolowana: " << y_interp << endl;
    cout << "B³¹d bezwzglêdny: " << abs(y_exact - y_interp) << endl;

    // Weryfikacja wystêpowania zjawiska Rungego
    assertTest(abs(y_exact - y_interp) < 0.1,
        "Interpolacja funkcji Rungego powinna dawaæ znacz¹cy b³¹d");
}

int main() {
    int passedTests = 0;
    int totalTests = 2;

    // Wykonanie testów
    if(runTest("Test interpolacji wielomianem kwadratowym", InterpolationTest1)) {
        passedTests++;
    }
    if(runTest("Test interpolacji funkcji Rungego", InterpolationTest2)) {
        passedTests++;
    }
    
    // Wyœwietlenie podsumowania
    cout << "\n=== Podsumowanie testów ===" << endl;
    cout << "Przesz³o: " << passedTests << "/" << totalTests << endl;

    // Zwrócenie kodu b³êdu (0 jeœli wszystkie testy przesz³y)
    return passedTests != totalTests;
}