#include "../include/numlib/RownaniaRozniczkowe.h"
#include <string>
#include <cmath>

// Klasa wyj�tku u�ywana do sygnalizowania niepowodzenia testu
class TestFailedException : public runtime_error {
public:
    TestFailedException(const string& message) : runtime_error(message) {}
};

// Funkcja pomocnicza do sprawdzania warunk�w testowych
void assertTest(bool condition, const string& message) {
    if (!condition) {
        throw TestFailedException(message);
    }
}


// Obs�uguje wyj�tki i wy�wietla wyniki test�w
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
        cout << testName << " FAILED z nieoczekiwanym wyj�tkiem: " << e.what() << endl;
        return false;
    }
}

// Test 1: Weryfikacja metody Rungego-Kutty dla r�wnania dy/dx = x + y
// Sprawdza podstawowe w�a�ciwo�ci rozwi�zania oraz por�wnuje z rozwi�zaniem analitycznym
void RungeKuttaTest1() {
    // Definicja r�wnania r�niczkowego: dy/dx = x + y
    auto f = [](double x, double y) { return x + y; };
    double x0 = 0.0;    // Punkt pocz�tkowy x
    double y0 = 1.0;    // Warunek pocz�tkowy y(0)
    double h = 0.1;     // Krok ca�kowania
    int n = 10;         // Liczba krok�w

    // Obliczenie rozwi�zania numerycznego
    auto result = numlib::RungegKutty(f, x0, y0, h, n);
    vector<double>& x_values = result.first;
    vector<double>& y_values = result.second;

    // Sprawdzenie poprawno�ci liczby punkt�w wynikowych
    assertTest(x_values.size() == n + 1, 
        "Nieprawid�owa liczba punkt�w w wyniku");

    // Weryfikacja warunku pocz�tkowego
    assertTest(abs(y_values[0] - y0) < 1e-10,
        "Nieprawid�owy warunek pocz�tkowy");

    // Sprawdzenie r�wnomierno�ci kroku ca�kowania
    for (int i = 1; i < x_values.size(); i++) {
        assertTest(abs(x_values[i] - x_values[i-1] - h) < 1e-10,
            "Nieprawid�owy krok ca�kowania");
    }

    // Por�wnanie z rozwi�zaniem analitycznym w punkcie x = 1.0
    double y_analytical = -2.0 + 2.0 * exp(1.0);  
    assertTest(abs(y_values[n] - y_analytical) < 0.1,
        "Zbyt du�y b��d rozwi�zania w x = 1.0");
}

// Test 2: Sprawdzenie zachowania metody dla r�wnania osobliwego dy/dx = y/x
// Weryfikuje obs�ug� przypadk�w szczeg�lnych (dzielenie przez zero)
void RungeKuttaTest2() {
    // Definicja r�wnania r�niczkowego z osobliwo�ci�: dy/dx = y/x
    auto f_error = [](double x, double y) {
        return y / x;
        };

    double x0 = 0.0;    // Punkt pocz�tkowy
    double y0 = 1.0;    // Warunek pocz�tkowy

    // Obliczenie rozwi�zania i sprawdzenie czy nie zawiera warto�ci NaN lub Inf
    auto result = numlib::RungegKutty(f_error, x0, y0, 0.1, 10);

    for (const auto& y : result.second) {
        if (!isfinite(y)) {
            throw TestFailedException("Metoda zwr�ci�a niesko�czono�� lub NaN");
        }
    }
}

int main() {
    int passedTests = 0;
    int totalTests = 2;

    // Wykonanie wszystkich test�w i zliczenie wynik�w
    if(runTest("Test1", RungeKuttaTest1)) {
        passedTests++;
    }
    if(runTest("Test2", RungeKuttaTest2)) {
        passedTests++;
    }
    
    // Wy�wietlenie podsumowania test�w
    cout << "\n=== Podsumowanie test�w ===" << endl;
    cout << "Przesz�o: " << passedTests << "/" << totalTests << endl;

    // Zwr�cenie 0 je�li wszystkie testy przesz�y, w przeciwnym razie warto�� niezerowa
    return passedTests != totalTests;
}