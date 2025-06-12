## Autorzy
- **Maciej Sadkowski**
- **Kamil Siarkowicz**

## Opis

Biblioteka Numeryczna implementująca wybrane metody obliczeniowe numeryczne w języku C++.

## Funkcjonalności

### Rozwiązywanie układów równań liniowych
- **Metoda eliminacji Gaussa** z wyborem elementu głównego
- Wizualizacja kroków eliminacji
- Weryfikacja rozwiązania

### Interpolacja
- **Interpolacja Newtona** z ilorazami różnicowymi
- Obliczanie współczynników wielomianu interpolacyjnego
- Ewaluacja wielomianu w dowolnym punkcie

### Aproksymacja (Metoda najmniejszych kwadratów)
- **Aproksymacja ciągła** z funkcjami bazowymi
- Funkcje bazowe wielomianowe (dowolny stopień)
- Funkcje bazowe trygonometryczne (sin, cos)
- Funkcje bazowe wykładnicze
- Automatyczne rozwiązywanie układu równań normalnych
- Obliczanie błędu aproksymacji

### Rozwiązywanie równań różniczkowych
- **Metoda Rungego-Kutty 4. rzędu (RK4)**
- Rozwiązywanie równań różniczkowych pierwszego rzędu
- Zwracanie tablic wartości x i y

## Instalacja

-Skopiuj pliki do folderu
-Stwórz folder "build"
-Przejdź do folder "build" i wpisz "make .." w terminalu
-Następnie wpisz "cmake --build . --config Debug"
-Projekt powinien zostać zbudowany

## Przykłady użycia

### Aproksymacja metodą najmniejszych kwadratów

```cpp
#include "../include/numlib/Aproksymacja.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

int main() {
    // Utworzenie obiektu aproksymacji dla przedziału [0, 1]
    numlib::ContinuousLeastSquares approx(0.0, 1.0);
    
    // Dodanie bazy wielomianowej stopnia 3
    auto poly_basis = numlib::StandardBasis::polynomial(3);
    for (auto& func : poly_basis) {
        approx.addBasisFunction(std::move(func));
    }
    
    // Dodanie funkcji do aproksymacji (funkcja złożona: sin(2πx)/3*x + 4)
    approx.addFunction([](double x) { 
        return std::sin(2 * M_PI * x) / 3 * x + 4; 
    }, 50);
    
    // Wykonanie aproksymacji
    if (approx.approximate()) {
        cout << "Aproksymacja udana!" << endl;
        cout << "Błąd: " << approx.computeError() << endl;
        
        // Test w punkcie x = 0.5
        cout << "f(0.5) = " << approx.evaluate(0.5) << endl;
    }
    
    return 0;
}
```

### Rozwiązywanie równań różniczkowych - Ruch spadającej piłki

```cpp
#include "../include/numlib/RownaniaRozniczkowe.h"
#include <iostream>

using namespace std;

/*
    Przykład zastosowania rozwiązania równania różniczkowego.
    Ruch spadającej piłki z uwzględnieniem oporu powietrza:
    dv/dt = g - (k/m)v²
    
    gdzie:
    v = prędkość
    g = przyspieszenie ziemskie (9.8 m/s²)
    k = współczynnik oporu (0.47)
    m = masa obiektu (1.0 kg)
*/
int main() {
    // Wartości fizyczne
    double g = 9.8;  // przyspieszenie ziemskie
    double k = 0.47; // współczynnik oporu powietrza
    double m = 1.0;  // masa piłki w kg
    
    // Parametry numeryczne
    double v0 = 0.0; // prędkość początkowa
    double t0 = 0.0; // czas początkowy
    double h = 0.1;  // krok czasowy
    int n = 10;      // liczba kroków
    
    // Definicja równania różniczkowego
    auto f = [g, k, m](double t, double v) -> double {
        return g - (k / m) * v * v;
    };
    
    // Wywołanie metody Rungego-Kutty
    auto result = numlib::RungegKutty(f, t0, v0, h, n);
    auto t = result.first;
    auto v = result.second;
    
    // Wyświetlenie wyników
    cout << "Symulacja ruchu spadającej piłki:" << endl;
    cout << "Czas [s]\tPrędkość [m/s]" << endl;
    for (size_t i = 0; i < t.size(); i++) {
        cout << "t = " << t[i] << "\tv = " << v[i] << endl;
    }
    
    return 0;
}
```

## Dokumentacja funkcji

### Rozwiązywanie układów równań liniowych

#### `std::vector<double> Gauss(std::vector<std::vector<double>>& Mat, std::vector<double>& B)`
**Opis**: Rozwiązuje układ równań liniowych Ax = b metodą eliminacji Gaussa z wyborem elementu głównego.

**Argumenty**:
- `Mat` - macierz współczynników (n×n)
- `B` - wektor prawych stron (n×1)

**Zwraca**: Wektor rozwiązań x

**Funkcje pomocnicze**:
- `printMatrix()` - wyświetla macierz z wektorem prawych stron podczas eliminacji

### Interpolacja

#### `std::vector<double> DividedDiff(const std::vector<double>& XIN, const std::vector<double>& FXIN)`
**Opis**: Oblicza ilorazy różnicowe dla interpolacji metodą Newtona.

**Argumenty**:
- `XIN` - wektor punktów x
- `FXIN` - wektor wartości funkcji f(x)

**Zwraca**: Wektor współczynników ilorazów różnicowych

#### `double Newton(const std::vector<double>& XIN, const std::vector<double>& coeffs, double x)`
**Opis**: Ewaluuje wielomian interpolacyjny Newtona w zadanym punkcie.

**Argumenty**:
- `XIN` - wektor punktów x
- `coeffs` - współczynniki ilorazów różnicowych
- `x` - punkt, w którym obliczamy wartość wielomianu

**Zwraca**: Wartość wielomianu w punkcie x

### Aproksymacja

#### Klasa `ContinuousLeastSquares`
**Opis**: Implementuje aproksymację metodą najmniejszych kwadratów z funkcjami bazowymi.

**Główne metody**:
- `addBasisFunction(std::unique_ptr<BasisFunction> basis_func)` - dodaje funkcję bazową
- `addDataPoint(double x, double y)` - dodaje punkt danych
- `addFunction(std::function<double(double)> func, int num_points)` - dodaje funkcję przez próbkowanie
- `approximate()` - wykonuje aproksymację (zwraca true przy sukcesie)
- `evaluate(double x)` - oblicza wartość funkcji aproksymującej
- `computeError()` - oblicza błąd RMSE

**Dostępne funkcje bazowe**:
- `PolynomialBasis(int degree)` - funkcje wielomianowe x^n
- `SinBasis(double frequency)` - funkcje sin(frequency * π * x)
- `CosBasis(double frequency)` - funkcje cos(frequency * π * x)
- `ExponentialBasis(double alpha)` - funkcje exp(alpha * x)

### Rozwiązywanie równań różniczkowych

#### `std::pair<std::vector<double>, std::vector<double>> RungegKutty(std::function<double(double, double)> f, double x0, double y0, double h, int n)`
**Opis**: Rozwiązuje równanie różniczkowe pierwszego rzędu dy/dx = f(x,y) metodą Rungego-Kutty 4. rzędu.

**Argumenty**:
- `f` - funkcja prawej strony równania różniczkowego f(x,y)
- `x0` - punkt początkowy
- `y0` - wartość początkowa y(x0)
- `h` - krok całkowania
- `n` - liczba kroków

**Zwraca**: Parę wektorów (x_values, y_values) zawierających rozwiązanie

## Testowanie

### Uruchomienie testów

-ctest -C Debug

### Kompilacja przykładów

./Debug/nazwa_przykladu
