#pragma once
#include <vector>
#include <iostream>

using namespace std;

double NewtonInterpolation(const vector<double>& XIN, const vector<double>& coeffs, double x);

double Horner(int n, double x, vector<double>& A);

vector<double> DividedDiff(const vector<double>& XIN, const vector<double>& FXIN);