#include "../include/numlib/Aproksymacja.h"
#include <iostream>
#include <algorithm>
#include <cmath>

int main() {
    numlib::ContinuousLeastSquares approx(0.0, 1.0);

    // Dodaj bazę wielomianową stopnia 3
    auto poly_basis = numlib::StandardBasis::polynomial(3);
    for (auto& func : poly_basis) {
        approx.addBasisFunction(std::move(func));
    }

    // Dodaj funkcję do aproksymacji (np. sin(2πx))
    approx.addFunction([](double x) { return std::sin(2 * M_PI * x)/3*x + 4; }, 50);

    // Wykonaj aproksymację
    if (approx.approximate()) {
        std::cout << "Aproksymacja udana!\n";
        std::cout << "Blad: " << approx.computeError() << std::endl;

        // Testuj w punkcie x = 0.5
        std::cout << "f(0.5) = " << approx.evaluate(0.5) << std::endl;
    }

    return 0;
}