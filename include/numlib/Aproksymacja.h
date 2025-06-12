#ifndef NUMLIB_H
#define NUMLIB_H

#include <vector>
#include <functional>
#include <memory>

namespace numlib {

    /**
     * @brief Klasa bazowa dla funkcji bazowych
     */
    class BasisFunction {
    public:
        virtual ~BasisFunction() = default;
        virtual double operator()(double x) const = 0;
        virtual std::unique_ptr<BasisFunction> clone() const = 0;
    };

    /**
     * @brief Funkcja bazowa - wielomian
     */
    class PolynomialBasis : public BasisFunction {
    private:
        int degree;
    public:
        PolynomialBasis(int deg) : degree(deg) {}
        double operator()(double x) const override;
        std::unique_ptr<BasisFunction> clone() const override;
    };

    /**
     * @brief Funkcja bazowa - funkcja trygonometryczna (sinus)
     */
    class SinBasis : public BasisFunction {
    private:
        int frequency;
    public:
        SinBasis(int freq) : frequency(freq) {}
        double operator()(double x) const override;
        std::unique_ptr<BasisFunction> clone() const override;
    };

    /**
     * @brief Funkcja bazowa - funkcja trygonometryczna (cosinus)
     */
    class CosBasis : public BasisFunction {
    private:
        int frequency;
    public:
        CosBasis(int freq) : frequency(freq) {}
        double operator()(double x) const override;
        std::unique_ptr<BasisFunction> clone() const override;
    };

    /**
     * @brief Funkcja bazowa - funkcja wykładnicza
     */
    class ExponentialBasis : public BasisFunction {
    private:
        double alpha;
    public:
        ExponentialBasis(double a) : alpha(a) {}
        double operator()(double x) const override;
        std::unique_ptr<BasisFunction> clone() const override;
    };

    /**
     * @brief Punkt danych dla aproksymacji
     */
    struct DataPoint {
        double x;
        double y;
        DataPoint(double x_val, double y_val) : x(x_val), y(y_val) {}
    };

    /**
     * @brief Klasa do aproksymacji średniokwadratowej ciągłej
     */
    class ContinuousLeastSquares {
    private:
        std::vector<std::unique_ptr<BasisFunction>> basis_functions;
        std::vector<double> coefficients;
        std::vector<DataPoint> data_points;
        double a, b; // przedział całkowania
        
        // Metody pomocnicze
        double computeGramMatrix(int i, int j) const;
        double computeRightHandSide(int i) const;
        std::vector<std::vector<double>> solveLinearSystem(
            std::vector<std::vector<double>>& matrix, 
            std::vector<double>& rhs) const;
        
    public:
        /**
         * @brief Konstruktor
         * @param interval_a początek przedziału
         * @param interval_b koniec przedziału
         */
        ContinuousLeastSquares(double interval_a = 0.0, double interval_b = 1.0);
        
        /**
         * @brief Dodaje funkcję bazową
         * @param basis_func unikalna funkcja bazowa
         */
        void addBasisFunction(std::unique_ptr<BasisFunction> basis_func);
        
        /**
         * @brief Dodaje punkt danych
         * @param x współrzędna x
         * @param y współrzędna y
         */
        void addDataPoint(double x, double y);
        
        /**
         * @brief Dodaje wiele punktów danych
         * @param points wektor punktów danych
         */
        void addDataPoints(const std::vector<DataPoint>& points);
        
        /**
         * @brief Dodaje funkcję do aproksymacji
         * @param func funkcja do aproksymacji
         * @param num_points liczba punktów do próbkowania
         */
        void addFunction(std::function<double(double)> func, int num_points = 100);
        
        /**
         * @brief Przeprowadza aproksymację średniokwadratową
         * @return true jeśli sukces, false w przeciwnym razie
         */
        bool approximate();
        
        /**
         * @brief Oblicza wartość aproksymacji w punkcie x
         * @param x punkt do obliczenia
         * @return wartość aproksymacji
         */
        double evaluate(double x) const;
        
        /**
         * @brief Zwraca współczynniki aproksymacji
         * @return wektor współczynników
         */
        const std::vector<double>& getCoefficients() const { return coefficients; }
        
        /**
         * @brief Oblicza błąd średniokwadratowy
         * @return wartość błędu
         */
        double computeError() const;
        
        /**
         * @brief Czyści dane
         */
        void clear();
        
        /**
         * @brief Ustawia nowy przedział całkowania
         * @param interval_a początek
         * @param interval_b koniec
         */
        void setInterval(double interval_a, double interval_b);
    };

    /**
     * @brief Funkcje pomocnicze do tworzenia standardowych baz
     */
    namespace StandardBasis {
        /**
         * @brief Tworzy bazę wielomianową
         * @param degree stopień wielomianu
         * @return wektor funkcji bazowych
         */
        std::vector<std::unique_ptr<BasisFunction>> polynomial(int degree);
        
        /**
         * @brief Tworzy bazę trygonometryczną
         * @param max_frequency maksymalna częstotliwość
         * @return wektor funkcji bazowych
         */
        std::vector<std::unique_ptr<BasisFunction>> trigonometric(int max_frequency);
        
        /**
         * @brief Tworzy bazę wykładniczą
         * @param alphas wektor parametrów wykładniczych
         * @return wektor funkcji bazowych
         */
        std::vector<std::unique_ptr<BasisFunction>> exponential(const std::vector<double>& alphas);
    }

} // namespace numlib

#endif // NUMLIB_H