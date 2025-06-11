#include "../include/numlib/Interpolacja.h"

namespace numlib {
    vector<double> DividedDiff(const vector<double>& XIN, const vector<double>& FXIN) {
        int n = XIN.size();
        vector<double> coeffs = FXIN;

        for (int j = 1; j < n; j++) {
            for (int i = n - 1; i >= j; i--) {
                coeffs[i] = (coeffs[i] - coeffs[i - 1]) / (XIN[i] - XIN[i - j]);
            }
        }
        return coeffs;
    }

    double Newton(const vector<double>& XIN, const vector<double>& coeffs, double x) {
        double result = coeffs[0];
        double term = 1.0;
        int n = XIN.size();

        for (int i = 1; i < n; i++) {
            term *= (x - XIN[i - 1]);
            result += coeffs[i] * term;

            cout << "a" << i << " = " << coeffs[i] << endl;
        }
        return result;
    }
}
