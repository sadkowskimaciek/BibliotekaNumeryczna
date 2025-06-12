#include "../include/numlib/Interpolacja.h"
#include <string>

// Klasa wyj�tku u�ywana do sygnalizowania niepowodzenia testu
class TestFailedException : public runtime_error {
public:
    TestFailedException(const string& message) : runtime_error(message) {}
};

// Funkcja pomocnicza do weryfikacji warunk�w testowych
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

// Test 1: Sprawdzenie interpolacji wielomianu kwadratowego
// U�ywa trzech punkt�w do interpolacji paraboli 
void InterpolationTest1() {
    // Definicja punkt�w do interpolacji
    vector<double> x = { -1, 0, 1 };     // W�z�y interpolacji
    vector<double> y = { 1, 0, 1 };      // Warto�ci funkcji w w�z�ach

    // Obliczenie wsp�czynnik�w interpolacji Newtona
    auto coeffs = numlib::DividedDiff(x, y);

    // Sprawdzenie dok�adno�ci interpolacji w w�z�ach
    for (int i = 0; i < x.size(); i++) {
        double result = numlib::Newton(x, coeffs, x[i]);
        assertTest(abs(result - y[i]) < 1e-10,
            "B��d interpolacji w punkcie x = " + to_string(x[i]));
    }

    // Sprawdzenie dok�adno�ci interpolacji w punkcie po�rednim
    double result = numlib::Newton(x, coeffs, 0.5);
    assertTest(abs(result - 0.25) < 1e-10,
        "B��d interpolacji w punkcie x = 0.5");
}

// Test 2: Demonstracja zjawiska Rungego
// Interpolacja funkcji Rungego f(x) = 1/(1 + 25x^2) na przedziale [-1,1]
void InterpolationTest2() {
    vector<double> x;
    vector<double> y;
    int n = 15;     // Liczba w�z��w interpolacji

    // Generowanie r�wnoodleg�ych w�z��w i warto�ci funkcji Rungego
    for (int i = 0; i < n; i++) {
        double xi = -1.0 + 2.0 * i / (n - 1);  // W�z�y r�wnoodleg�e na [-1,1]
        x.push_back(xi);
        y.push_back(1.0 / (1.0 + 25.0 * xi * xi));  // Funkcja Rungego
    }

    // Obliczenie wsp�czynnik�w interpolacji
    auto coeffs = numlib::DividedDiff(x, y);

    // Test w punkcie kontrolnym
    double x_test = 0.75;
    double y_exact = 1.0 / (1.0 + 25.0 * x_test * x_test);  // Warto�� dok�adna
    double y_interp = numlib::Newton(x, coeffs, x_test);     // Warto�� interpolowana

    // Wy�wietlenie wynik�w
    cout << "Warto�� dok�adna w x = " << x_test << ": " << y_exact << endl;
    cout << "Warto�� interpolowana: " << y_interp << endl;
    cout << "B��d bezwzgl�dny: " << abs(y_exact - y_interp) << endl;

    // Weryfikacja wyst�powania zjawiska Rungego
    assertTest(abs(y_exact - y_interp) < 0.1,
        "Interpolacja funkcji Rungego powinna dawa� znacz�cy b��d");
}

int main() {
    int passedTests = 0;
    int totalTests = 2;

    // Wykonanie test�w
    if(runTest("Test interpolacji wielomianem kwadratowym", InterpolationTest1)) {
        passedTests++;
    }
    if(runTest("Test interpolacji funkcji Rungego", InterpolationTest2)) {
        passedTests++;
    }
    
    // Wy�wietlenie podsumowania
    cout << "\n=== Podsumowanie test�w ===" << endl;
    cout << "Przesz�o: " << passedTests << "/" << totalTests << endl;

    // Zwr�cenie kodu b��du (0 je�li wszystkie testy przesz�y)
    return passedTests != totalTests;
}