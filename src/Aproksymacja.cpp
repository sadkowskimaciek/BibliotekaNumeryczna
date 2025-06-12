#include "../include/numlib/Aproksymacja.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <numeric>

namespace numlib {

    // Implementacja PolynomialBasis
    double PolynomialBasis::operator()(double x) const {
        return std::pow(x, degree);
    }

    std::unique_ptr<BasisFunction> PolynomialBasis::clone() const {
        return std::make_unique<PolynomialBasis>(degree);
    }

    // Implementacja SinBasis
    double SinBasis::operator()(double x) const {
        return std::sin(frequency * M_PI * x);
    }

    std::unique_ptr<BasisFunction> SinBasis::clone() const {
        return std::make_unique<SinBasis>(frequency);
    }

    // Implementacja CosBasis
    double CosBasis::operator()(double x) const {
        return std::cos(frequency * M_PI * x);
    }

    std::unique_ptr<BasisFunction> CosBasis::clone() const {
        return std::make_unique<CosBasis>(frequency);
    }

    // Implementacja ExponentialBasis
    double ExponentialBasis::operator()(double x) const {
        return std::exp(alpha * x);
    }

    std::unique_ptr<BasisFunction> ExponentialBasis::clone() const {
        return std::make_unique<ExponentialBasis>(alpha);
    }

    // Implementacja ContinuousLeastSquares
    ContinuousLeastSquares::ContinuousLeastSquares(double interval_a, double interval_b)
        : a(interval_a), b(interval_b) {
        if (a >= b) {
            throw std::invalid_argument("Nieprawidłowy przedział: a musi być mniejsze od b");
        }
    }

    void ContinuousLeastSquares::addBasisFunction(std::unique_ptr<BasisFunction> basis_func) {
        basis_functions.push_back(std::move(basis_func));
    }

    void ContinuousLeastSquares::addDataPoint(double x, double y) {
        data_points.emplace_back(x, y);
    }

    void ContinuousLeastSquares::addDataPoints(const std::vector<DataPoint>& points) {
        data_points.insert(data_points.end(), points.begin(), points.end());
    }

    void ContinuousLeastSquares::addFunction(std::function<double(double)> func, int num_points) {
        if (num_points <= 0) {
            throw std::invalid_argument("Liczba punktów musi być dodatnia");
        }

        double step = (b - a) / (num_points - 1);
        for (int i = 0; i < num_points; ++i) {
            double x = a + i * step;
            addDataPoint(x, func(x));
        }
    }

    double ContinuousLeastSquares::computeGramMatrix(int i, int j) const {
        // Całka iloczynu funkcji bazowych <φ_i, φ_j> na [a,b]
        // Używamy kwadratur numerycznych (metoda trapezów)
        const int n_quad = 1000; // liczba punktów kwadratury
        double h = (b - a) / n_quad;
        double sum = 0.0;

        for (int k = 0; k <= n_quad; ++k) {
            double x = a + k * h;
            double weight = (k == 0 || k == n_quad) ? 0.5 : 1.0;
            sum += weight * (*basis_functions[i])(x) * (*basis_functions[j])(x);
        }

        return h * sum;
    }

    double ContinuousLeastSquares::computeRightHandSide(int i) const {
        // Oblicza <f, φ_i> gdzie f jest aproksymowana przez punkty danych
        // Używamy interpolacji liniowej między punktami danych
        const int n_quad = 1000;
        double h = (b - a) / n_quad;
        double sum = 0.0;

        for (int k = 0; k <= n_quad; ++k) {
            double x = a + k * h;
            double weight = (k == 0 || k == n_quad) ? 0.5 : 1.0;

            // Interpolacja liniowa wartości funkcji w punkcie x
            double f_val = 0.0;
            if (data_points.empty()) {
                f_val = 0.0;
            } else if (data_points.size() == 1) {
                f_val = data_points[0].y;
            } else {
                // Znajdź najbliższe punkty do interpolacji
                auto it = std::lower_bound(data_points.begin(), data_points.end(),
                    DataPoint(x, 0), [](const DataPoint& a, const DataPoint& b) {
                        return a.x < b.x;
                    });

                if (it == data_points.begin()) {
                    f_val = data_points[0].y;
                } else if (it == data_points.end()) {
                    f_val = data_points.back().y;
                } else {
                    auto prev_it = it - 1;
                    double t = (x - prev_it->x) / (it->x - prev_it->x);
                    f_val = prev_it->y + t * (it->y - prev_it->y);
                }
            }

            sum += weight * f_val * (*basis_functions[i])(x);
        }

        return h * sum;
    }

    std::vector<std::vector<double>> ContinuousLeastSquares::solveLinearSystem(
        std::vector<std::vector<double>>& matrix, std::vector<double>& rhs) const {

        int n = matrix.size();
        if (n == 0 || matrix[0].size() != n || rhs.size() != n) {
            throw std::invalid_argument("Nieprawidłowe wymiary macierzy");
        }

        // Eliminacja Gaussa z wyborem elementu głównego
        for (int i = 0; i < n; ++i) {
            // Znajdź element główny
            int max_row = i;
            for (int k = i + 1; k < n; ++k) {
                if (std::abs(matrix[k][i]) > std::abs(matrix[max_row][i])) {
                    max_row = k;
                }
            }

            // Zamiana wierszy
            std::swap(matrix[i], matrix[max_row]);
            std::swap(rhs[i], rhs[max_row]);

            // Sprawdź czy macierz jest osobliwa
            if (std::abs(matrix[i][i]) < 1e-12) {
                throw std::runtime_error("Macierz jest osobliwa");
            }

            // Eliminacja
            for (int k = i + 1; k < n; ++k) {
                double factor = matrix[k][i] / matrix[i][i];
                for (int j = i; j < n; ++j) {
                    matrix[k][j] -= factor * matrix[i][j];
                }
                rhs[k] -= factor * rhs[i];
            }
        }

        // Podstawianie wsteczne
        std::vector<double> solution(n);
        for (int i = n - 1; i >= 0; --i) {
            solution[i] = rhs[i];
            for (int j = i + 1; j < n; ++j) {
                solution[i] -= matrix[i][j] * solution[j];
            }
            solution[i] /= matrix[i][i];
        }

        return {solution};
    }

    bool ContinuousLeastSquares::approximate() {
        if (basis_functions.empty()) {
            throw std::runtime_error("Brak funkcji bazowych");
        }

        if (data_points.empty()) {
            throw std::runtime_error("Brak punktów danych");
        }

        // Sortuj punkty danych według x
        std::sort(data_points.begin(), data_points.end(),
            [](const DataPoint& a, const DataPoint& b) {
                return a.x < b.x;
            });

        int n = basis_functions.size();

        try {
            // Tworzenie macierzy Grama
            std::vector<std::vector<double>> gram_matrix(n, std::vector<double>(n));
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    gram_matrix[i][j] = computeGramMatrix(i, j);
                }
            }

            // Tworzenie prawej strony
            std::vector<double> rhs(n);
            for (int i = 0; i < n; ++i) {
                rhs[i] = computeRightHandSide(i);
            }

            // Rozwiązanie układu równań
            auto solution = solveLinearSystem(gram_matrix, rhs);
            coefficients = solution[0];

            return true;

        } catch (const std::exception& e) {
            coefficients.clear();
            return false;
        }
    }

    double ContinuousLeastSquares::evaluate(double x) const {
        if (coefficients.empty() || coefficients.size() != basis_functions.size()) {
            throw std::runtime_error("Aproksymacja nie została jeszcze wykonana");
        }

        double result = 0.0;
        for (size_t i = 0; i < basis_functions.size(); ++i) {
            result += coefficients[i] * (*basis_functions[i])(x);
        }

        return result;
    }

    double ContinuousLeastSquares::computeError() const {
        if (coefficients.empty() || data_points.empty()) {
            return 0.0;
        }

        double sum_squared_error = 0.0;
        for (const auto& point : data_points) {
            double approx_val = evaluate(point.x);
            double error = point.y - approx_val;
            sum_squared_error += error * error;
        }

        return std::sqrt(sum_squared_error / data_points.size());
    }

    void ContinuousLeastSquares::clear() {
        basis_functions.clear();
        coefficients.clear();
        data_points.clear();
    }

    void ContinuousLeastSquares::setInterval(double interval_a, double interval_b) {
        if (interval_a >= interval_b) {
            throw std::invalid_argument("Nieprawidłowy przedział: a musi być mniejsze od b");
        }
        a = interval_a;
        b = interval_b;
    }

    // Implementacja StandardBasis
    namespace StandardBasis {

        std::vector<std::unique_ptr<BasisFunction>> polynomial(int degree) {
            std::vector<std::unique_ptr<BasisFunction>> basis;
            for (int i = 0; i <= degree; ++i) {
                basis.push_back(std::make_unique<PolynomialBasis>(i));
            }
            return basis;
        }

        std::vector<std::unique_ptr<BasisFunction>> trigonometric(int max_frequency) {
            std::vector<std::unique_ptr<BasisFunction>> basis;

            // Dodaj funkcję stałą (cos(0))
            basis.push_back(std::make_unique<CosBasis>(0));

            // Dodaj pary sin i cos dla każdej częstotliwości
            for (int freq = 1; freq <= max_frequency; ++freq) {
                basis.push_back(std::make_unique<CosBasis>(freq));
                basis.push_back(std::make_unique<SinBasis>(freq));
            }

            return basis;
        }

        std::vector<std::unique_ptr<BasisFunction>> exponential(const std::vector<double>& alphas) {
            std::vector<std::unique_ptr<BasisFunction>> basis;
            for (double alpha : alphas) {
                basis.push_back(std::make_unique<ExponentialBasis>(alpha));
            }
            return basis;
        }
    }

} // namespace numlib