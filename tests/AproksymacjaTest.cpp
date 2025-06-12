#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <iomanip>
#include <functional>
#include "../include/numlib/Aproksymacja.h"

// Klasa pomocnicza do testowania
class AproksymacjaTest {
private:
    int passed = 0;
    int failed = 0;

    void assert_test(bool condition, const std::string& test_name) {
        if (condition) {
            std::cout << "" << test_name << " - PASSED" << std::endl;
            passed++;
        } else {
            std::cout << "" << test_name << " - FAILED" << std::endl;
            failed++;
        }
    }

    bool is_close(double a, double b, double tolerance = 1e-3) {
        return std::abs(a - b) < tolerance;
    }

public:
    // Test 1: Konstruktor ContinuousLeastSquares
    void test_constructor() {
        try {
            numlib::ContinuousLeastSquares approx(0.0, 1.0);
            assert_test(true, "ContinuousLeastSquares constructor [0,1]");
        } catch (...) {
            assert_test(false, "ContinuousLeastSquares constructor [0,1]");
        }

        // Test nieprawidłowego przedziału
        try {
            numlib::ContinuousLeastSquares approx(1.0, 0.0); // a > b
            assert_test(false, "Invalid interval handling");
        } catch (const std::invalid_argument&) {
            assert_test(true, "Invalid interval handling");
        } catch (...) {
            assert_test(false, "Invalid interval handling");
        }
    }

    // Test 2: StandardBasis::polynomial
    void test_polynomial_basis() {
        try {
            auto basis = numlib::StandardBasis::polynomial(2);

            bool poly_test = basis.size() == 3; // stopień 2 -> 3 funkcje (0,1,2)

            if (poly_test) {
                // Test wartości: 1, x, x²
                double x = 2.0;
                poly_test = poly_test && is_close((*basis[0])(x), 1.0);      // x^0 = 1
                poly_test = poly_test && is_close((*basis[1])(x), 2.0);      // x^1 = 2
                poly_test = poly_test && is_close((*basis[2])(x), 4.0);      // x^2 = 4
            }

            assert_test(poly_test, "StandardBasis::polynomial");
        } catch (...) {
            assert_test(false, "StandardBasis::polynomial");
        }
    }

    // Test 3: StandardBasis::trigonometric
    void test_trigonometric_basis() {
        try {
            auto basis = numlib::StandardBasis::trigonometric(2);

            // Powinno być: 1 (cos(0)) + 2*2 (cos,sin dla freq 1,2) = 5 funkcji
            bool trig_test = basis.size() == 5;

            if (trig_test) {
                double x = 0.5;
                // Test pierwszej funkcji (stała)
                trig_test = trig_test && is_close((*basis[0])(x), 1.0); // cos(0) = 1
            }

            assert_test(trig_test, "StandardBasis::trigonometric");
        } catch (...) {
            assert_test(false, "StandardBasis::trigonometric");
        }
    }

    // Test 4: StandardBasis::exponential
    void test_exponential_basis() {
        try {
            std::vector<double> alphas = {0.0, 1.0, -1.0};
            auto basis = numlib::StandardBasis::exponential(alphas);

            bool exp_test = basis.size() == 3;

            if (exp_test) {
                double x = 1.0;
                exp_test = exp_test && is_close((*basis[0])(x), 1.0);           // e^(0*1) = 1
                exp_test = exp_test && is_close((*basis[1])(x), std::exp(1.0)); // e^(1*1) = e
                exp_test = exp_test && is_close((*basis[2])(x), std::exp(-1.0));// e^(-1*1) = 1/e
            }

            assert_test(exp_test, "StandardBasis::exponential");
        } catch (...) {
            assert_test(false, "StandardBasis::exponential");
        }
    }

    // Test 5: Dodawanie punktów danych
    void test_add_data_points() {
        try {
            numlib::ContinuousLeastSquares approx(0.0, 1.0);

            // Test pojedynczego punktu
            approx.addDataPoint(0.5, 2.0);

            // Test wektora punktów
            std::vector<numlib::DataPoint> points = {{0.0, 1.0}, {1.0, 3.0}};
            approx.addDataPoints(points);

            assert_test(true, "Adding data points");
        } catch (...) {
            assert_test(false, "Adding data points");
        }
    }

    // Test 6: Dodawanie funkcji
    void test_add_function() {
        try {
            numlib::ContinuousLeastSquares approx(0.0, 1.0);

            // Funkcja liniowa f(x) = 2x + 1
            auto linear_func = [](double x) { return 2.0 * x + 1.0; };
            approx.addFunction(linear_func, 10);

            assert_test(true, "Adding function with sampling");
        } catch (...) {
            assert_test(false, "Adding function with sampling");
        }

        // Test nieprawidłowej liczby punktów
        try {
            numlib::ContinuousLeastSquares approx(0.0, 1.0);
            auto func = [](double x) { return x; };
            approx.addFunction(func, 0); // nieprawidłowa liczba punktów
            assert_test(false, "Invalid number of points handling");
        } catch (const std::invalid_argument&) {
            assert_test(true, "Invalid number of points handling");
        } catch (...) {
            assert_test(false, "Invalid number of points handling");
        }
    }

    // Test 7: Aproksymacja funkcji liniowej
    void test_linear_approximation() {
        try {
            numlib::ContinuousLeastSquares approx(0.0, 1.0);

            // Funkcja f(x) = 2x + 1
            auto linear_func = [](double x) { return 2.0 * x + 1.0; };
            approx.addFunction(linear_func, 50);

            // Dodaj wielomian stopnia 1
            auto poly_basis = numlib::StandardBasis::polynomial(1);
            for (auto& basis_func : poly_basis) {
                approx.addBasisFunction(std::move(basis_func));
            }

            bool success = approx.approximate();

            if (success) {
                // Sprawdź jakość aproksymacji
                double error = approx.computeError();
                bool linear_test = error < 0.01; // Bardzo mały błąd dla dokładnej funkcji
                assert_test(linear_test, "Linear function approximation");
            } else {
                assert_test(false, "Linear function approximation");
            }
        } catch (...) {
            assert_test(false, "Linear function approximation");
        }
    }

    // Test 8: Aproksymacja funkcji kwadratowej
    void test_quadratic_approximation() {
        try {
            numlib::ContinuousLeastSquares approx(0.0, 1.0);

            // Funkcja f(x) = x² + x + 1
            auto quad_func = [](double x) { return x*x + x + 1.0; };
            approx.addFunction(quad_func, 50);

            // Dodaj wielomian stopnia 2
            auto poly_basis = numlib::StandardBasis::polynomial(2);
            for (auto& basis_func : poly_basis) {
                approx.addBasisFunction(std::move(basis_func));
            }

            bool success = approx.approximate();

            if (success) {
                double error = approx.computeError();
                bool quad_test = error < 0.01;
                assert_test(quad_test, "Quadratic function approximation");
            } else {
                assert_test(false, "Quadratic function approximation");
            }
        } catch (...) {
            assert_test(false, "Quadratic function approximation");
        }
    }

    // Test 9: Aproksymacja funkcji trygonometrycznej
    void test_trigonometric_approximation() {
        try {
            numlib::ContinuousLeastSquares approx(0.0, 1.0);

            // Funkcja f(x) = sin(2πx)
            auto sin_func = [](double x) { return std::sin(2.0 * M_PI * x); };
            approx.addFunction(sin_func, 100);

            // Dodaj bazę trygonometryczną
            auto trig_basis = numlib::StandardBasis::trigonometric(3);
            for (auto& basis_func : trig_basis) {
                approx.addBasisFunction(std::move(basis_func));
            }

            bool success = approx.approximate();

            if (success) {
                double error = approx.computeError();
                bool trig_test = error < 0.1; // Tolerancja dla funkcji oscylujących
                assert_test(trig_test, "Trigonometric function approximation");
            } else {
                assert_test(false, "Trigonometric function approximation");
            }
        } catch (...) {
            assert_test(false, "Trigonometric function approximation");
        }
    }

    // Test 10: Aproksymacja funkcji wykładniczej
    void test_exponential_approximation() {
        try {
            numlib::ContinuousLeastSquares approx(0.0, 1.0);

            // Funkcja f(x) = e^x
            auto exp_func = [](double x) { return std::exp(x); };
            approx.addFunction(exp_func, 50);

            // Dodaj bazę wykładniczą
            std::vector<double> alphas = {0.0, 0.5, 1.0, 1.5};
            auto exp_basis = numlib::StandardBasis::exponential(alphas);
            for (auto& basis_func : exp_basis) {
                approx.addBasisFunction(std::move(basis_func));
            }

            bool success = approx.approximate();

            if (success) {
                double error = approx.computeError();
                bool exp_test = error < 0.05;
                assert_test(exp_test, "Exponential function approximation");
            } else {
                assert_test(false, "Exponential function approximation");
            }
        } catch (...) {
            assert_test(false, "Exponential function approximation");
        }
    }

    // Test 11: Ewaluacja aproksymacji
    void test_evaluation() {
        try {
            numlib::ContinuousLeastSquares approx(0.0, 1.0);

            // Funkcja stała f(x) = 5
            auto const_func = [](double x) { return 5.0; };
            approx.addFunction(const_func, 10);

            // Wielomian stopnia 0 (stała)
            auto poly_basis = numlib::StandardBasis::polynomial(0);
            for (auto& basis_func : poly_basis) {
                approx.addBasisFunction(std::move(basis_func));
            }

            bool success = approx.approximate();

            if (success) {
                double val = approx.evaluate(0.5);
                bool eval_test = is_close(val, 5.0, 0.1);
                assert_test(eval_test, "Evaluation of approximation");
            } else {
                assert_test(false, "Evaluation of approximation");
            }
        } catch (...) {
            assert_test(false, "Evaluation of approximation");
        }
    }

    // Test 12: Błędy i przypadki brzegowe
    void test_error_cases() {
        try {
            numlib::ContinuousLeastSquares approx(0.0, 1.0);

            // Test bez funkcji bazowych
            bool caught_no_basis = false;
            try {
                approx.approximate();
            } catch (const std::runtime_error&) {
                caught_no_basis = true;
            }

            // Test bez danych
            auto poly_basis = numlib::StandardBasis::polynomial(1);
            for (auto& basis_func : poly_basis) {
                approx.addBasisFunction(std::move(basis_func));
            }

            bool caught_no_data = false;
            try {
                approx.approximate();
            } catch (const std::runtime_error&) {
                caught_no_data = true;
            }

            assert_test(caught_no_basis && caught_no_data, "Error handling");
        } catch (...) {
            assert_test(false, "Error handling");
        }
    }

    // Test 13: Metody pomocnicze
    void test_utility_methods() {
        try {
            numlib::ContinuousLeastSquares approx(0.0, 1.0);

            // Test clear()
            approx.addDataPoint(0.5, 1.0);
            auto poly_basis = numlib::StandardBasis::polynomial(1);
            for (auto& basis_func : poly_basis) {
                approx.addBasisFunction(std::move(basis_func));
            }

            approx.clear();

            // Test setInterval()
            approx.setInterval(-1.0, 2.0);

            // Test nieprawidłowego przedziału
            bool caught_invalid_interval = false;
            try {
                approx.setInterval(2.0, -1.0);
            } catch (const std::invalid_argument&) {
                caught_invalid_interval = true;
            }

            assert_test(caught_invalid_interval, "Utility methods");
        } catch (...) {
            assert_test(false, "Utility methods");
        }
    }

    // Test 14: Klonowanie funkcji bazowych
    void test_basis_cloning() {
        try {
            numlib::PolynomialBasis poly(2);
            auto clone = poly.clone();

            double x = 3.0;
            bool clone_test = is_close(poly(x), (*clone)(x));

            numlib::SinBasis sin_basis(1);
            auto sin_clone = sin_basis.clone();
            clone_test = clone_test && is_close(sin_basis(x), (*sin_clone)(x));

            numlib::CosBasis cos_basis(1);
            auto cos_clone = cos_basis.clone();
            clone_test = clone_test && is_close(cos_basis(x), (*cos_clone)(x));

            numlib::ExponentialBasis exp_basis(1.0);
            auto exp_clone = exp_basis.clone();
            clone_test = clone_test && is_close(exp_basis(x), (*exp_clone)(x));

            assert_test(clone_test, "Basis function cloning");
        } catch (...) {
            assert_test(false, "Basis function cloning");
        }
    }

    void run_all_tests() {
        std::cout << "=== TESTY APROKSYMACJI CIĄGŁEJ ===" << std::endl;
        std::cout << std::endl;

        test_constructor();
        test_polynomial_basis();
        test_trigonometric_basis();
        test_exponential_basis();
        test_add_data_points();
        test_add_function();
        test_linear_approximation();
        test_quadratic_approximation();
        test_trigonometric_approximation();
        test_exponential_approximation();
        test_evaluation();
        test_error_cases();
        test_utility_methods();
        test_basis_cloning();

        std::cout << std::endl;
        std::cout << "=== PODSUMOWANIE ===" << std::endl;
        std::cout << "Testy zaliczone: " << passed << std::endl;
        std::cout << "Testy niezaliczone: " << failed << std::endl;
        std::cout << "Procent powodzenia: " << std::fixed << std::setprecision(1)
                  << (100.0 * passed / (passed + failed)) << "%" << std::endl;

        if (failed == 0) {
            std::cout << std::endl << " WSZYSTKIE TESTY ZALICZONE!" << std::endl;
        } else {
            std::cout << std::endl << " Niektóre testy nie przeszły. Sprawdź implementację." << std::endl;
        }
    }
};

int main() {
    AproksymacjaTest test;
    test.run_all_tests();
    return 0;
}

/*
INSTRUKCJA UŻYCIA:

1. Zapisz jako tests/AproksymacjaTests.cpp
2. Dodaj do CMakeLists.txt:
   add_executable(AproksymacjaTests tests/AproksymacjaTests.cpp)
   target_link_libraries(AproksymacjaTests PRIVATE BibliotekaNumeryczna)
3. Kompiluj i uruchamiaj: cmake --build . && ./AproksymacjaTests

TESTOWANE FUNKCJONALNOŚCI:
✓ Konstruktory i walidacja
✓ Wszystkie typy funkcji bazowych (polynomial, trigonometric, exponential)
✓ Dodawanie danych i funkcji
✓ Aproksymacja różnych typów funkcji
✓ Ewaluacja i obliczanie błędów
✓ Obsługa błędów i przypadków brzegowych
✓ Metody pomocnicze (clear, setInterval)
✓ Klonowanie funkcji bazowych
*/