#pragma once

#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

namespace numlib {
	void printMatrix(vector<vector<double>>&Mat, vector<double>&B);

	vector<double> Gauss(vector<vector<double>>&Mat, vector<double>&B);
}