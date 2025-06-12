#include "../include/numlib/Interpolacja.h"

namespace numlib {
    vector<double> DividedDiff(const vector<double>& XIN, const vector<double>& FXIN) {
        int n = XIN.size();
        vector<double> coeffs = FXIN;  // Inicjalizacja wspó³czynników wartoœciami funkcji

        // Obliczanie ilorazów ró¿nicowych kolejnych rzêdów
        for (int j = 1; j < n; j++) {
            // Obliczanie ilorazów ró¿nicowych j-tego rzêdu
            for (int i = n - 1; i >= j; i--) {
                coeffs[i] = (coeffs[i] - coeffs[i - 1]) / (XIN[i] - XIN[i - j]);
            }
        }
        return coeffs;
    }

    double Newton(const vector<double>& XIN, const vector<double>& coeffs, double x) {
        double result = coeffs[0];  // Pierwszy wspó³czynnik
        double term = 1.0;          // Iloczyn (x-x_0)(x-x_1)...(x-x_{i-1})
        int n = XIN.size();

        // Obliczanie kolejnych wyrazów wielomianu
        for (int i = 1; i < n; i++) {
            term *= (x - XIN[i - 1]);  // Aktualizacja iloczynu
            result += coeffs[i] * term; // Dodanie kolejnego wyrazu

            // Diagnostyczny wydruk wspó³czynników
            cout << "a" << i << " = " << coeffs[i] << endl;
        }
        return result;
    }
}
