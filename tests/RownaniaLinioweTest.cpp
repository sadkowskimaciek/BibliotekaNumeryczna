#include "../include/numlib/RownaniaLiniowe.h"
#include <cassert>
#include <stdexcept>

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

/**
 * Test 1: Rozwi�zywanie uk�adu 2x2
 * Uk�ad r�wna�:
 * 2x + y = 5
 * x + 3y = 6
 * Oczekiwane rozwi�zanie: x = 1.8, y = 1.4
 */
void LinearEquationTest1() {
    // Definicja uk�adu r�wna� w postaci macierzowej
    vector<vector<double>> A = {
        {2, 1},   
        {1, 3}     
    };
    vector<double> b = { 5, 6 };  // Wektor wyraz�w wolnych

    // Rozwi�zanie uk�adu metod� Gaussa
    auto x = numlib::Gauss(A, b);

    // Weryfikacja otrzymanych wynik�w
    assertTest(abs(x[0] - 1.8) < 1e-6, "x[0] powinno by� 1.8");
    assertTest(abs(x[1] - 1.4) < 1e-6, "x[1] powinno by� 1.4");

    cout << "x[0] = " << x[0] << ", x[1] = " << x[1] << endl;
}

/**
 * Test 2: Rozwi�zywanie uk�adu 3x3 z macierz� tr�jdiagonaln�
 * Uk�ad r�wna�:
 * 2x - y = 1
 * -x + 2y - z = 0
 * -y + 2z = 1
 */
void LinearEquationTest2() {
    // Definicja macierzy tr�jdiagonalnej
    vector<vector<double>> A = {
        {2, -1, 0},    
        {-1, 2, -1},   
        {0, -1, 2}     
    };
    vector<double> b = { 1, 0, 1 };  // Wektor wyraz�w wolnych

    // Rozwi�zanie uk�adu metod� Gaussa
    auto x = numlib::Gauss(A, b);

    // Sprawdzenie czy rozwi�zanie spe�nia r�wnania
    double check1 = 2 * x[0] - x[1];             // Pierwsze r�wnanie
    double check2 = -x[0] + 2 * x[1] - x[2];     // Drugie r�wnanie
    double check3 = -x[1] + 2 * x[2];            // Trzecie r�wnanie

    // Weryfikacja czy r�wnania s� spe�nione
    assertTest(abs(check1 - 1) < 1e-6, "R�wnanie 1 nie jest spe�nione");
    assertTest(abs(check2 - 0) < 1e-6, "R�wnanie 2 nie jest spe�nione");
    assertTest(abs(check3 - 1) < 1e-6, "R�wnanie 3 nie jest spe�nione");

    cout << "check1 = " << check1 << ", check2 = " << check2 << ", check3 = " << check3 << endl;
}

/**
 * Test 3: Rozwi�zywanie uk�adu 3x3 z dok�adnymi warto�ciami ca�kowitymi
 * Uk�ad r�wna�:
 * 2x + y - z = 8
 * -3x - y + 2z = -11
 * -2x + y + 2z = -3
 * Oczekiwane rozwi�zanie: x = 2, y = 3, z = -1
 */
void LinearEquationTest3() {
    // Definicja uk�adu r�wna�
    vector<vector<double>> A = {
        {2, 1, -1},    
        {-3, -1, 2}, 
        {-2, 1, 2}     
    };
    vector<double> b = { 8, -11, -3 };  // Wektor wyraz�w wolnych

    // Rozwi�zanie uk�adu metod� Gaussa
    auto x = numlib::Gauss(A, b);

    // Weryfikacja dok�adnych warto�ci rozwi�zania
    assertTest(abs(x[0] - 2) < 1e-6, "x[0] powinno by� 2");
    assertTest(abs(x[1] - 3) < 1e-6, "x[1] powinno by� 3");
    assertTest(abs(x[2] - (-1)) < 1e-6, "x[2] powinno by� -1");

    cout << "x[0] = " << x[0] << ", x[1] = " << x[1] << ", x[2] = " << x[2] << endl;
}

int main() {
    int passedTests = 0;
    int totalTests = 3;

    // Wykonanie wszystkich test�w uk�ad�w r�wna� liniowych
    if (runTest("Test1", LinearEquationTest1)) passedTests++;
    if (runTest("Test2", LinearEquationTest2)) passedTests++;
    if (runTest("Test3", LinearEquationTest3)) passedTests++;

    // Wy�wietlenie podsumowania wynik�w test�w
    cout << "\n=== Podsumowanie test�w ===" << endl;
    cout << "Przesz�o: " << passedTests << "/" << totalTests << endl;
    
    // Zwr�cenie kodu b��du (0 je�li wszystkie testy przesz�y)
    return passedTests != totalTests;
}