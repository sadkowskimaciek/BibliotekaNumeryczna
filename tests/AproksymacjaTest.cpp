#include "../include/numlib/Aproksymacja.h"
#include <iostream>
#include <cmath>

int main() {
    numlib::ContinuousLeastSquares approx(0.0, 1.0);

    // Dodaj bazę wielomianową stopnia 3
    auto poly_basis = numlib::StandardBasis::polynomial(3);
    for (auto& func : poly_basis) {
        approx.addBasisFunction(std::move(func));
    }

    // Dodaj funkcję do aproksymacji (np. sin(2πx))
    approx.addFunction([](double x) { return std::sin(2 * M_PI * x); }, 50);

    // Wykonaj aproksymację
    if (approx.approximate()) {
        for (double i=0;i<=2;i+=0.5) {
            std::cout << "f( "<<i<<") = " << approx.evaluate(i) << std::endl;
            std::cout << "Aproksymacja udana!\n";

        }
    }

    return 0;
}