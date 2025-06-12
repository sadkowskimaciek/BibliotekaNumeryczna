#include "../include/numlib/RownaniaNieLiniowe.h"
#include <iostream>
#include <cmath>
#include <cassert>
#include <iomanip>

// Funkcje testowe
double f1(double x) {
    if (x >= 1) return INFINITY;
    return log(1 - x) + x / (x*x + 3);
}

double f1_derivative(double x) {
    if (x >= 1) return INFINITY;
    return -1.0/(1-x) + (3 - x*x)/pow(x*x + 3, 2);
}

double f2(double x) {
    if (abs(x) < 1e-10) return INFINITY;
    return x*x*x + 30 * sin(x) - 12.0/x - 28;
}

double f2_derivative(double x) {
    if (abs(x) < 1e-10) return INFINITY;
    return 3*x*x + 30*cos(x) + 12.0/(x*x);
}

double simple_poly(double x) {
    return x*x - 4; // Roots: -2, 2
}

double simple_poly_derivative(double x) {
    return 2*x;
}

void testBisection() {
    std::cout << "Testing Bisection Method..." << std::endl;

    // Test simple polynomial
    Result result = numlib::bisection(simple_poly, 1, 3);
    assert(result.converged);
    assert(abs(result.root - 2.0) < 1e-6);
    std::cout << "  Simple poly: root = " << std::fixed << std::setprecision(8) << result.root << ", iterations = " << result.iterations << std::endl;

    // Test f1
    result = numlib::bisection(f1, -2, 0.5);
    if (result.converged) {
        std::cout << "  f1: root = " << result.root << ", f(root) = " << std::scientific << f1(result.root) << std::endl;
    }
}

void testNewton() {
    std::cout << "Testing Newton Method..." << std::endl;

    // Test simple polynomial
    Result result = numlib::newton(simple_poly, simple_poly_derivative, 1.5);
    assert(result.converged);
    assert(abs(result.root - 2.0) < 1e-6);
    std::cout << "  Simple poly: root = " << std::fixed << std::setprecision(8) << result.root << ", iterations = " << result.iterations << std::endl;

    // Test f1
    result = numlib::newton(f1, f1_derivative, -1.0);
    if (result.converged) {
        std::cout << "  f1: root = " << result.root << ", f(root) = " << std::scientific << f1(result.root) << std::endl;
    }
}

void testSecant() {
    std::cout << "Testing Secant Method..." << std::endl;

    // Test simple polynomial
    Result result = numlib::secant(simple_poly, 1, 3);
    assert(result.converged);
    assert(abs(result.root - 2.0) < 1e-6);
    std::cout << "  Simple poly: root = " << std::fixed << std::setprecision(8) << result.root << ", iterations = " << result.iterations << std::endl;

    // Test f2
    result = numlib::secant(f2, 1, 2);
    if (result.converged) {
        std::cout << "  f2: root = " << result.root << ", f(root) = " << std::scientific << f2(result.root) << std::endl;
    }
}

void testFindAllRoots() {
    std::cout << "Testing findAllRoots..." << std::endl;

    // Test simple polynomial - should find 2 roots
    auto roots = numlib::findAllRoots(simple_poly, simple_poly_derivative, -3, 3, "bisection");
    std::cout << "  Simple poly bisection found " << roots.size() << " roots: ";
    for (double root : roots) {
        std::cout << std::fixed << std::setprecision(6) << root << " ";
    }
    std::cout << std::endl;

    // Test f1
    roots = numlib::findAllRoots(f1, f1_derivative, -3, 0.9, "newton");
    std::cout << "  f1 newton found " << roots.size() << " roots: ";
    for (double root : roots) {
        std::cout << std::fixed << std::setprecision(6) << root << " ";
    }
    std::cout << std::endl;
}

void testConvergence() {
    std::cout << "Testing convergence properties..." << std::endl;

    // Test with different tolerances
    for (double tol : {1e-4, 1e-6, 1e-8}) {
        Result result = numlib::bisection(simple_poly, 1, 3, tol);
        std::cout << "  Tolerance " << std::scientific << tol << ": error = " << result.error << ", iterations = " << result.iterations << std::endl;
    }
}

int main() {
    std::cout << "=== NUMERICAL SOLVER TESTS ===" << std::endl;

    testBisection();
    std::cout << std::endl;

    testNewton();
    std::cout << std::endl;

    testSecant();
    std::cout << std::endl;

    testFindAllRoots();
    std::cout << std::endl;

    testConvergence();

    std::cout << "\n=== ALL TESTS COMPLETED ===" << std::endl;

    return 0;
}