#pragma once
#include <vector>
#include <functional>
#include <iostream>

using namespace std;

namespace numlib {
	pair<vector<double>, vector<double>> RungegKutty(function<double(double, double)> f, double x0, double y0, double h, int n);
}

