#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <functional>
#include <set>
#include <algorithm>

struct Result {
    double root;
    int iterations;
    double error;
    bool converged;
    std::vector<double> approximations;
};

namespace numlib {
    Result bisection(std::function<double(double)> f, double a, double b, double tolerance = 1e-8, int maxIterations = 1000);
    Result newton(std::function<double(double)> f, std::function<double(double)> df, double x0, double tolerance = 1e-8, int maxIterations = 1000);
    Result secant(std::function<double(double)> f, double x0, double x1, double tolerance = 1e-8, int maxIterations = 1000);


    std::vector<std::pair<double, double>> findSignChangeIntervals(std::function<double(double)> f, double start, double end, double step = 0.01);
    std::vector<double> removeDuplicates(std::vector<double> roots, double tolerance = 1e-6);
    std::vector<double> findAllRoots(std::function<double(double)> f, std::function<double(double)> df, double start, double end, const std::string& method, double step = 0.01);
}
