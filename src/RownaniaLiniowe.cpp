#include "../include/RownaniaLiniowe.h"

void printMatrix(vector<vector<double>>& Mat, vector<double>& B) {
    int n = B.size();
    const int width = 10;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(width) << fixed << setprecision(2) << Mat[i][j] << " ";
        }
        cout << "| " << setw(width) << fixed << setprecision(2) << B[i] << endl;
    }
    cout << "-----------------------------" << endl;
}

void Gauss(vector<vector<double>>& Mat, vector<double>& B) {
    int n = B.size();

    printMatrix(Mat, B);

    for (int i = 0; i < n - 1; i++) {
        int maxRow = i;
        for (int j = i + 1; j < n; j++) {
            if (fabs(Mat[j][i]) > fabs(Mat[maxRow][i])) {
                maxRow = j;
            }
        }

        swap(Mat[i], Mat[maxRow]);
        swap(B[i], B[maxRow]);

        for (int j = i + 1; j < n; j++) {
            double factor = Mat[j][i] / Mat[i][i];
            for (int k = i; k < n; k++) {
                Mat[j][k] -= factor * Mat[i][k];
            }
            B[j] -= factor * B[i];
        }

        printMatrix(Mat, B);
    }

    vector<double> X(n);
    for (int i = n - 1; i >= 0; i--) {
        double sum = B[i];
        for (int j = i + 1; j < n; j++) {
            sum -= Mat[i][j] * X[j];
        }
        X[i] = sum / Mat[i][i];
    }

    for (int i = 0; i < n; i++) {
        if (isnan(X[i]) || isinf(X[i])) {
            cout << "The solution contains invalid values (NaN or Inf)." << endl;
            return;
        }
    }

    cout << "Solution: " << endl;
    for (int i = 0; i < n; i++) {
        cout << "x[" << i << "] = " << fixed << setprecision(2) << X[i] << endl;
    }
}