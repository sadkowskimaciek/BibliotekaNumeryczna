#include "../include/numlib/RownaniaRozniczkowe.h"

namespace numlib {
    pair<vector<double>, vector<double>> RungegKutty(function<double(double, double)> f, double x0, double y0, double h, int n) {
        if (n <= 0) {
            throw invalid_argument("Number of steps must be positive");
        }
        if (h <= 0) {
            throw invalid_argument("Step must be positive");
        }

        // Inicjalizacja wektor�w wynikowych
        vector<double> x_values, y_values;
        x_values.reserve(n + 1);
        y_values.reserve(n + 1);

        // Zapisanie warunk�w pocz�tkowych
        double x = x0;
        double y = y0;
        x_values.push_back(x);
        y_values.push_back(y);

        // G��wna p�tla metody RK4
        for (int i = 0; i < n; i++) {
            // Obliczenie wsp�czynnik�w metody RK4
            double k1 = h * f(x, y);
            
            double k2 = h * f(x + h / 2.0, y + k1 / 2.0);
            
            double k3 = h * f(x + h / 2.0, y + k2 / 2.0);
            
            double k4 = h * f(x + h, y + k3);

            // Obliczenie nast�pnej warto�ci y u�ywaj�c wa�onej �redniej wsp�czynnik�w:
            y = y + (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
            x = x + h;

            // Zapisanie wynik�w
            x_values.push_back(x);
            y_values.push_back(y);
        }

        return { x_values, y_values };
    }
}