#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {
    int n;
    cout << "================ ELIMINACION GAUSSIANA ================\n";
    cout << "Ingrese la dimension del sistema (N): ";
    cin >> n;

    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);

    cout << "\n--- Ingrese la Matriz A (" << n << "x" << n << ") fila por fila ---\n";
    for (int i = 0; i < n; i++) {
        cout << "Fila " << i + 1 << " (" << n << " valores separados por espacio): ";
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }

    cout << "\n--- Ingrese el Vector b (" << n << " valores) ---\n";
    for (int i = 0; i < n; i++) {
        cout << "b[" << i + 1 << "]: ";
        cin >> b[i];
    }

    // --- FASE 1: ELIMINACION HACIA ADELANTE CON PIVOTEO AUTOMATICO ---
    for (int k = 0; k < n - 1; k++) {
        // Busca el mayor valor absoluto en la columna k desde la fila k en adelante
        int max_fila = k;
        for (int i = k + 1; i < n; i++) {
            if (fabs(A[i][k]) > fabs(A[max_fila][k])) {
                max_fila = i;
            }
        }

        // Si el elemento mayor no esta en la diagonal, pivotea automaticamente
        if (max_fila != k) {
            swap(A[k], A[max_fila]);
            swap(b[k], b[max_fila]);
            cout << "--> Pivoteo automatico: Intercambio de Fila " << k + 1 << " con Fila " << max_fila + 1 << "\n";
        }

        // Si aun despues de pivotear el pivote es 0, el sistema no tiene solucion unica
        if (fabs(A[k][k]) < 1e-12) {
            cout << "\nERROR: Pivote nulo en A[" << k + 1 << "][" << k + 1 << "]. El sistema no tiene solucion unica.\n";
            return 1;
        }

        // Operacion de eliminacion
        for (int i = k + 1; i < n; i++) {
            double m = A[i][k] / A[k][k];
            for (int j = k; j < n; j++) {
                A[i][j] -= m * A[k][j];
            }
            b[i] -= m * b[k];
        }
    }

    // --- FASE 2: RETROSUSTITUCION ---
    vector<double> x(n);
    for (int i = n - 1; i >= 0; i--) {
        double suma = 0.0;
        for (int j = i + 1; j < n; j++) {
            suma += A[i][j] * x[j];
        }
        x[i] = (b[i] - suma) / A[i][i];
    }

    // --- PRESENTACION DE RESULTADOS ---
    cout << "\n================ SOLUCION EXACTA OBTENIDA ================\n";
    for (int i = 0; i < n; i++) {
        cout << "x[" << i + 1 << "] = " << fixed << setprecision(3) << x[i] << "\n";
    }

    return 0;
}