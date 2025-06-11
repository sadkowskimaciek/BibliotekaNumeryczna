#pragma once
#include <vector>
#include <iostream>

using namespace std;

namespace numlib {
	double Newton(const vector<double>& XIN, const vector<double>& coeffs, double x);

	vector<double> DividedDiff(const vector<double>& XIN, const vector<double>& FXIN);
}