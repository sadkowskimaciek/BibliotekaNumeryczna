#include "../include/numlib/RownaniaLiniowe.h"

namespace numlib {
    void printMatrix(vector<vector<double>>& A, vector<double>& b) {
        int n = b.size();
        const int width = 10;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << setw(width) << fixed << setprecision(4) << A[i][j] << " ";
            }
            cout << "| " << setw(width) << fixed << setprecision(4) << b[i] << endl;
        }
        cout << "-----------------------------" << endl;
    }

    vector<double> Gauss(vector<vector<double>>& Mat, vector<double>& B) {
        int n = B.size();
        vector<vector<double>> A(Mat);
        vector<double> b(B);

        cout << "Macierz początkowa:" << endl;
        printMatrix(A, b);

        // Etap 1: Eliminacja współczynników (eliminacja w przód)
        for (int k = 0; k < n - 1; k++) {
            // Wybór elementu głównego (pivot)
            int pivot_row = k;
            double pivot_value = abs(A[k][k]);

            for (int i = k + 1; i < n; i++) {
                if (abs(A[i][k]) > pivot_value) {
                    pivot_value = abs(A[i][k]);
                    pivot_row = i;
                }
            }

            // Zamiana wierszy jeśli znaleziono lepszy element główny
            if (pivot_row != k) {
                for (int j = k; j < n; j++) {
                    swap(A[k][j], A[pivot_row][j]);
                }
                swap(b[k], b[pivot_row]);
            }

            // Sprawdzenie osobliwości macierzy
            if (abs(A[k][k]) < 1e-10) {
                cout << "Macierz osobliwa - brak rozwiązania" << endl;
                return vector<double>(n, 0.0);
            }

            // Eliminacja współczynników w kolumnie
            for (int i = k + 1; i < n; i++) {
                double factor = A[i][k] / A[k][k];  // Mnożnik eliminacyjny

                for (int j = k; j < n; j++) {
                    A[i][j] -= factor * A[k][j];
                }
                b[i] -= factor * b[k];
            }

            cout << "Po eliminacji krok " << k + 1 << ":" << endl;
            printMatrix(A, b);
        }

        // Etap 2: Podstawianie wsteczne
        vector<double> x(n, 0.0);

        // Sprawdzenie czy ostatni element na przekątnej nie jest zerem
        if (abs(A[n - 1][n - 1]) < 1e-10) {
            cout << "Błąd: Dzielenie przez zero podczas podstawiania wstecznego" << endl;
            return vector<double>(n, 0.0);
        }

        // Obliczenie ostatniej niewiadomej
        x[n - 1] = b[n - 1] / A[n - 1][n - 1];
        cout << "x[" << n - 1 << "] = " << x[n - 1] << endl;

        // Obliczenie pozostałych niewiadomych
        for (int i = n - 2; i >= 0; i--) {
            double sum = 0.0;
            for (int j = i + 1; j < n; j++) {
                sum += A[i][j] * x[j];
            }
            x[i] = (b[i] - sum) / A[i][i];
            cout << "x[" << i << "] = " << x[i] << endl;
        }

        // Etap 3: Weryfikacja rozwiązania
        cout << "\nSprawdzenie rozwiązania:" << endl;
        for (int i = 0; i < n; i++) {
            double check = 0.0;
            for (int j = 0; j < n; j++) {
                check += Mat[i][j] * x[j];
            }
            cout << "Równanie " << i << ": " << check << " = " << B[i] << endl;
        }

        return x;
    }
}