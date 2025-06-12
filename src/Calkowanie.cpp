#include "../include/numlib/Calkowanie.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;
namespace numlib {
    // Function to calculate polynomial value at point x using Horner's scheme
    double horner(const vector<double>& coefficients, double x) {
        double result = 0.0;
        for (int i = coefficients.size() - 1; i >= 0; i--) {
            result = result * x + coefficients[i];
        }
        return result;
    }

    // Rectangle method
    double rectangle_method(const vector<double>& coefficients, double a, double b, int n) {
        double dx = (b - a) / n;
        double integral = 0.0;
        for (int i = 0; i < n; i++) {
            double x_i = a + i * dx;  // left end of interval
            integral += horner(coefficients, x_i) * dx;
        }
        return integral;
    }

    // Trapezoidal method
    double trapezoid_method(const vector<double>& coefficients, double a, double b, int n) {
        double dx = (b - a) / n;
        double integral = 0.0;
        for (int i = 0; i < n; i++) {
            double x_i = a + i * dx;
            double x_i_plus_1 = a + (i + 1) * dx;
            integral += 0.5 * (horner(coefficients, x_i) + horner(coefficients, x_i_plus_1)) * dx;
        }
        return integral;
    }

    // Simpson's method
    double simpson_method(const vector<double>& coefficients, double a, double b, int n) {
        if (n % 2 != 0) {
            n += 1;  // ensure n is even
        }

        double dx = (b - a) / n;
        double integral = horner(coefficients, a) + horner(coefficients, b);

        for (int i = 1; i < n; i++) {
            double x_i = a + i * dx;
            double coef = (i % 2 == 1) ? 4.0 : 2.0;
            integral += coef * horner(coefficients, x_i);
        }

        integral *= dx / 3.0;
        return integral;
    }

    // Calculate integral analytically for polynomial
    double analytical_integral(const vector<double>& coefficients, double a, double b) {
        double result = 0.0;
        for (size_t i = 0; i < coefficients.size(); i++) {
            result += coefficients[i] * (pow(b, i + 1) - pow(a, i + 1)) / (i + 1);
        }
        return result;
    }
}