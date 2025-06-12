#include "../include/numlib/RownaniaRozniczkowe.h"

/*
	Przykład zastosowania rozwiązania równania różniczkowego.

	Ruch spadającej piłki z uwzględnieniem oporu powietrza
	dv/dt = g - (k/m)v²
	
	gdzie:
	v = prędkość
	g = przyspieszenie ziemskie
	k = współczynnik oporu
	m = masa obiektu
*/

int main() {
	
	//wartości fizyczne
	double g = 9.8;
	double k = 0.47;
	double m = 1.0;
	
	//wartości początkowe i numeryczne
	double v0 = 0.0;
	double t0 = 0.0;
	double h = 0.1;
	int n = 10;

	//funkcja różniczkowa
	auto f = [g, k, m](double v, double t) -> double {
		return g - (k / m) * v * v;
		};

	//wywołanie metody Rungego-Kutty
	auto result = numlib::RungegKutty(f, v0, t0, h, n);
	auto t = result.first;
	auto v = result.second;

	//wyświetlenie wyników
	for (size_t i = 0; i < t.size(); i++) {
		cout << "t = " << t[i] << ", v = " << v[i] << endl;
	}

	return 0;
}