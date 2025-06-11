#include "../include/numlib/RownaniaRozniczkowe.h"

namespace numlib {
    pair<vector<double>, vector<double>> RungegKutty(function<double(double, double)> f, double x0, double y0, double h, int n) {
        if (n <= 0) {
            throw invalid_argument("Number of steps must be positive");
        }
        if (h <= 0) {
            throw invalid_argument("Step must be positive");
        }

        vector<double> x_values, y_values;
        x_values.reserve(n + 1);
        y_values.reserve(n + 1);

        double x = x0;
        double y = y0;

        x_values.push_back(x);
        y_values.push_back(y);

        for (int i = 0; i < n; i++) {
            double k1 = h * f(x, y);
            double k2 = h * f(x + h / 2.0, y + k1 / 2.0);
            double k3 = h * f(x + h / 2.0, y + k2 / 2.0);
            double k4 = h * f(x + h, y + k3 / 2.0);

            y = y + (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
            x = x + h;

            x_values.push_back(x);
            y_values.push_back(y);
        }

        return { x_values, y_values };
    }
}