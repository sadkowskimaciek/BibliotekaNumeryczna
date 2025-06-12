#include "../include/numlib/RownaniaLiniowe.h"
#include <cassert>
#include <stdexcept>

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

/**
 * Test 1: Rozwi¹zywanie uk³adu 2x2
 * Uk³ad równañ:
 * 2x + y = 5
 * x + 3y = 6
 * Oczekiwane rozwi¹zanie: x = 1.8, y = 1.4
 */
void LinearEquationTest1() {
    // Definicja uk³adu równañ w postaci macierzowej
    vector<vector<double>> A = {
        {2, 1},   
        {1, 3}     
    };
    vector<double> b = { 5, 6 };  // Wektor wyrazów wolnych

    // Rozwi¹zanie uk³adu metod¹ Gaussa
    auto x = numlib::Gauss(A, b);

    // Weryfikacja otrzymanych wyników
    assertTest(abs(x[0] - 1.8) < 1e-6, "x[0] powinno byæ 1.8");
    assertTest(abs(x[1] - 1.4) < 1e-6, "x[1] powinno byæ 1.4");

    cout << "x[0] = " << x[0] << ", x[1] = " << x[1] << endl;
}

/**
 * Test 2: Rozwi¹zywanie uk³adu 3x3 z macierz¹ trójdiagonaln¹
 * Uk³ad równañ:
 * 2x - y = 1
 * -x + 2y - z = 0
 * -y + 2z = 1
 */
void LinearEquationTest2() {
    // Definicja macierzy trójdiagonalnej
    vector<vector<double>> A = {
        {2, -1, 0},    
        {-1, 2, -1},   
        {0, -1, 2}     
    };
    vector<double> b = { 1, 0, 1 };  // Wektor wyrazów wolnych

    // Rozwi¹zanie uk³adu metod¹ Gaussa
    auto x = numlib::Gauss(A, b);

    // Sprawdzenie czy rozwi¹zanie spe³nia równania
    double check1 = 2 * x[0] - x[1];             // Pierwsze równanie
    double check2 = -x[0] + 2 * x[1] - x[2];     // Drugie równanie
    double check3 = -x[1] + 2 * x[2];            // Trzecie równanie

    // Weryfikacja czy równania s¹ spe³nione
    assertTest(abs(check1 - 1) < 1e-6, "Równanie 1 nie jest spe³nione");
    assertTest(abs(check2 - 0) < 1e-6, "Równanie 2 nie jest spe³nione");
    assertTest(abs(check3 - 1) < 1e-6, "Równanie 3 nie jest spe³nione");

    cout << "check1 = " << check1 << ", check2 = " << check2 << ", check3 = " << check3 << endl;
}

/**
 * Test 3: Rozwi¹zywanie uk³adu 3x3 z dok³adnymi wartoœciami ca³kowitymi
 * Uk³ad równañ:
 * 2x + y - z = 8
 * -3x - y + 2z = -11
 * -2x + y + 2z = -3
 * Oczekiwane rozwi¹zanie: x = 2, y = 3, z = -1
 */
void LinearEquationTest3() {
    // Definicja uk³adu równañ
    vector<vector<double>> A = {
        {2, 1, -1},    
        {-3, -1, 2}, 
        {-2, 1, 2}     
    };
    vector<double> b = { 8, -11, -3 };  // Wektor wyrazów wolnych

    // Rozwi¹zanie uk³adu metod¹ Gaussa
    auto x = numlib::Gauss(A, b);

    // Weryfikacja dok³adnych wartoœci rozwi¹zania
    assertTest(abs(x[0] - 2) < 1e-6, "x[0] powinno byæ 2");
    assertTest(abs(x[1] - 3) < 1e-6, "x[1] powinno byæ 3");
    assertTest(abs(x[2] - (-1)) < 1e-6, "x[2] powinno byæ -1");

    cout << "x[0] = " << x[0] << ", x[1] = " << x[1] << ", x[2] = " << x[2] << endl;
}

int main() {
    int passedTests = 0;
    int totalTests = 3;

    // Wykonanie wszystkich testów uk³adów równañ liniowych
    if (runTest("Test1", LinearEquationTest1)) passedTests++;
    if (runTest("Test2", LinearEquationTest2)) passedTests++;
    if (runTest("Test3", LinearEquationTest3)) passedTests++;

    // Wyœwietlenie podsumowania wyników testów
    cout << "\n=== Podsumowanie testów ===" << endl;
    cout << "Przesz³o: " << passedTests << "/" << totalTests << endl;
    
    // Zwrócenie kodu b³êdu (0 jeœli wszystkie testy przesz³y)
    return passedTests != totalTests;
}