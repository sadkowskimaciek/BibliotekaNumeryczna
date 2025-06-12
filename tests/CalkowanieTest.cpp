#include "../include/numlib/Calkowanie.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

int main(){
 int degree;
    vector<double> coefficients;
    double a, b;

    cout << "Podaj stopien wielomianu: ";
    cin >> degree;

    cout << "Podaj wspolczynniki wielomianu (od a0 do a" << degree << "): ";
    coefficients.resize(degree + 1);
    for (int i = 0; i <= degree; i++) {
        cin >> coefficients[i];
    }

    cout << "Podaj przedzial calkowania [a, b]: ";
    cin >> a >> b;

    cout << "\nWielomian stopnia: " << degree << endl;
    cout << "Współczynniki: ";
    for (const auto& c : coefficients) {
        cout << c << " ";
    }
    cout << endl;
    cout << "Przedzial calkowania: [" << a << ", " << b << "]" << endl;

    // Calculate exact integral value
    double exact_value = numlib::analytical_integral(coefficients, a, b);
    cout << "Dokladna wartosc calki: " << exact_value << endl;

    // Calculations for different number of subdivisions
    vector<int> n_values = {10, 50, 100, 500, 1000, 5000, 10000};

    cout << "\nWyniki dla roznych metod calkowania:" << endl;
    cout << setw(10) << "n" << setw(20) << "Prostokaty" << setw(20) << "Trapezy"
         << setw(20) << "Simpson" << setw(20) << "Blad prostokaty"
         << setw(20) << "Blad trapezy" << setw(20) << "Blad Simpson" << endl;

    for (int n : n_values) {
        double rect_result = numlib::rectangle_method(coefficients, a, b, n);
        double trap_result = numlib::trapezoid_method(coefficients, a, b, n);
        double simp_result = numlib::simpson_method(coefficients, a, b, n);

        // Calculate errors
        double rect_error = abs(rect_result - exact_value);
        double trap_error = abs(trap_result - exact_value);
        double simp_error = abs(simp_result - exact_value);

        cout << setw(10) << n
             << setw(20) << fixed << setprecision(10) << rect_result
             << setw(20) << trap_result
             << setw(20) << simp_result
             << setw(20) << rect_error
             << setw(20) << trap_error
             << setw(20) << simp_error
             << endl;
    }

    return 0;
}
