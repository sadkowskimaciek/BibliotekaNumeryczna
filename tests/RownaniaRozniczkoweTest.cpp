#include "../include/numlib/RownaniaRozniczkowe.h"
#include <string>
#include <cmath>

// Klasa wyj¹tku u¿ywana do sygnalizowania niepowodzenia testu
class TestFailedException : public runtime_error {
public:
    TestFailedException(const string& message) : runtime_error(message) {}
};

// Funkcja pomocnicza do sprawdzania warunków testowych
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

// Test 1: Weryfikacja metody Rungego-Kutty dla równania dy/dx = x + y
// Sprawdza podstawowe w³aœciwoœci rozwi¹zania oraz porównuje z rozwi¹zaniem analitycznym
void RungeKuttaTest1() {
    // Definicja równania ró¿niczkowego: dy/dx = x + y
    auto f = [](double x, double y) { return x + y; };
    double x0 = 0.0;    // Punkt pocz¹tkowy x
    double y0 = 1.0;    // Warunek pocz¹tkowy y(0)
    double h = 0.1;     // Krok ca³kowania
    int n = 10;         // Liczba kroków

    // Obliczenie rozwi¹zania numerycznego
    auto result = numlib::RungegKutty(f, x0, y0, h, n);
    vector<double>& x_values = result.first;
    vector<double>& y_values = result.second;

    // Sprawdzenie poprawnoœci liczby punktów wynikowych
    assertTest(x_values.size() == n + 1, 
        "Nieprawid³owa liczba punktów w wyniku");

    // Weryfikacja warunku pocz¹tkowego
    assertTest(abs(y_values[0] - y0) < 1e-10,
        "Nieprawid³owy warunek pocz¹tkowy");

    // Sprawdzenie równomiernoœci kroku ca³kowania
    for (int i = 1; i < x_values.size(); i++) {
        assertTest(abs(x_values[i] - x_values[i-1] - h) < 1e-10,
            "Nieprawid³owy krok ca³kowania");
    }

    // Porównanie z rozwi¹zaniem analitycznym w punkcie x = 1.0
    double y_analytical = -2.0 + 2.0 * exp(1.0);  
    assertTest(abs(y_values[n] - y_analytical) < 0.1,
        "Zbyt du¿y b³¹d rozwi¹zania w x = 1.0");
}

// Test 2: Sprawdzenie zachowania metody dla równania osobliwego dy/dx = y/x
// Weryfikuje obs³ugê przypadków szczególnych (dzielenie przez zero)
void RungeKuttaTest2() {
    // Definicja równania ró¿niczkowego z osobliwoœci¹: dy/dx = y/x
    auto f_error = [](double x, double y) {
        return y / x;
        };

    double x0 = 0.0;    // Punkt pocz¹tkowy
    double y0 = 1.0;    // Warunek pocz¹tkowy

    // Obliczenie rozwi¹zania i sprawdzenie czy nie zawiera wartoœci NaN lub Inf
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

    // Wykonanie wszystkich testów i zliczenie wyników
    if(runTest("Test1", RungeKuttaTest1)) {
        passedTests++;
    }
    if(runTest("Test2", RungeKuttaTest2)) {
        passedTests++;
    }
    
    // Wyœwietlenie podsumowania testów
    cout << "\n=== Podsumowanie testów ===" << endl;
    cout << "Przesz³o: " << passedTests << "/" << totalTests << endl;

    // Zwrócenie 0 jeœli wszystkie testy przesz³y, w przeciwnym razie wartoœæ niezerowa
    return passedTests != totalTests;
}