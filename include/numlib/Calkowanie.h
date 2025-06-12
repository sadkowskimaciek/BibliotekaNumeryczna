#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include <iomanip>

using namespace std;

namespace numlib {
    double horner(const vector<double>& coefficients, double x);
    double rectangle_method(const vector<double>& coefficients, double a, double b, int n);
    double trapezoid_method(const vector<double>& coefficients, double a, double b, int n);
    double simpson_method(const vector<double>& coefficients, double a, double b, int n);
    double analytical_integral(const vector<double>& coefficients, double a, double b);
}