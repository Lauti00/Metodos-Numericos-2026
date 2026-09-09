#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cstdio>
#include <string>

using namespace std;

// Verifica la condición suficiente de convergencia: |a_ii| > suma(|a_ij|) para i != j
bool esEDD(const vector<vector<double>>& A) {
    int n = A.size();
    for (int i = 0; i < n; i++) {
        double suma = 0.0;
        for (int j = 0; j < n; j++) {
            if (i != j) suma += fabs(A[i][j]);
        }
        // Si el elemento diagonal no es estrictamente mayor a la suma de la fila, no es EDD
        if (fabs(A[i][i]) <= suma) return false;
    }
    return true;
}

// Reordena filas de A y b para forzar que el elemento dominante de cada columna quede en la diagonal
bool forzarEDD(vector<vector<double>>& A, vector<double>& b) {
    int n = A.size();
    vector<vector<double>> A_nueva = A;
    vector<double> b_nueva = b;
    vector<bool> fila_usada(n, false);

    for (int col = 0; col < n; col++) {
        int mejor_fila = -1;
        for (int i = 0; i < n; i++) {
            if (!fila_usada[i]) {
                double suma = 0.0;
                for (int j = 0; j < n; j++) {
                    if (j != col) suma += fabs(A[i][j]);
                }
                // Comprueba si esta fila puede ser el pivote dominante para la columna actual
                if (fabs(A[i][col]) > suma) {
                    mejor_fila = i;
                    break;
                }
            }
        }
        if (mejor_fila != -1) {
            A_nueva[col] = A[mejor_fila];
            b_nueva[col] = b[mejor_fila];
            fila_usada[mejor_fila] = true;
        } else {
            return false; // No se encontró una fila que cumpla la dominancia diagonal
        }
    }
    A = A_nueva;
    b = b_nueva;
    return true;
}

int main() {
    int n;
    
    // === 1. LECTURA DE DATOS DESDE ARCHIVO ===
    FILE *fp = fopen("matriz.txt", "r");

    if (!fp) {
        cout << "Error: No se pudo abrir 'matriz.txt'. Asegurate de que el archivo exista.\n";
        return 1;
    }

    if (fscanf(fp, "%d", &n) != 1) {
        cout << "Error al leer la dimension N.\n";
        fclose(fp);
        return 1;
    }

    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(fp, "%lf", &A[i][j]);
        }
        fscanf(fp, "%lf", &b[i]);
    }
    fclose(fp);

    // Muestra en consola el sistema ingresado
    cout << "================ SISTEMA INGRESADO [A|b] ================\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) cout << setw(8) << setprecision(2) << fixed << A[i][j] << " ";
        cout << " | " << setw(8) << setprecision(2) << fixed << b[i] << "\n";
    }

    // === 2. VERIFICACIÓN Y ACONDICIONAMIENTO DE CONVERGENCIA ===
    cout << "\n================ ANALISIS DE CONVERGENCIA (EDD) ================\n";
    if (esEDD(A)) {
        cout << "El sistema ES Estrictamente Diagonal Dominante (EDD).\n";
    } else {
        cout << "El sistema NO es EDD originalmente. Reordenando filas...\n";
        if (forzarEDD(A, b)) {
            cout << "EXITO: Filas reordenadas. El sistema ahora ES EDD:\n\n";
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) cout << setw(8) << setprecision(2) << fixed << A[i][j] << " ";
                cout << " | " << setw(8) << setprecision(2) << fixed << b[i] << "\n";
            }
        } else {
            cout << "ADVERTENCIA: No se pudo forzar EDD. El metodo podria no converger.\n";
        }
    }

    // === 3. CONFIGURACIÓN INICIAL DEL MÉTODO ITERATIVO ===
    vector<double> x(n, 0.0);      // Vector de estimación inicial x^(0) = (0, 0, ..., 0)
    vector<double> x_old(n, 0.0);  // Reserva de memoria para guardar la iteración anterior
    double tol = 1e-4;             // Criterio de parada (tolerancia de error)
    int max_iter = 100;            // Límite de seguridad contra bucles infinitos

    cout << "\n================ TABLA DE ITERACIONES GAUSS-SEIDEL ================\n";
    cout << setw(6) << "Iter";
    for (int i = 0; i < n; i++) cout << setw(18) << ("x[" + to_string(i + 1) + "]");
    cout << setw(18) << "Error Est (E_a)\n";
    cout << string(24 + n * 18, '-') << "\n";

    int iter = 0;
    double error_est = 0.0;

    // === 4. BUCLE PRINCIPAL DE GAUSS-SEIDEL ===
    do {
        iter++;
        x_old = x; // Guardamos el estado anterior para calcular el error al final del paso
        error_est = 0.0;

        // Recorrido ecuación por ecuación
        for (int i = 0; i < n; i++) {
            double suma = b[i];
            for (int j = 0; j < n; j++) {
                // En Gauss-Seidel, 'x[j]' utiliza los valores actualizados inmediatamente (si j < i)
                if (i != j) suma -= A[i][j] * x[j];
            }
            // Despeje de la variable i-ésima: x_i = (b_i - sum(a_ij * x_j)) / a_ii
            x[i] = suma / A[i][i];
        }

        // Cálculo del Error Estimado (Norma Infinito: diferencia máxima entre x_nueva y x_anterior)
        for (int i = 0; i < n; i++) {
            error_est = max(error_est, fabs(x[i] - x_old[i]));
        }

        // Muestra de los resultados de la iteración actual
        cout << setw(6) << iter;
        for (int i = 0; i < n; i++) cout << setw(18) << fixed << setprecision(4) << x[i];
        cout << setw(18) << scientific << setprecision(4) << error_est << "\n";

    } while (error_est > tol && iter < max_iter); // Continúa hasta cumplir tolerancia o max_iter

    // === 5. RESUMEN DE RESULTADOS ===
    cout << "\n================ REPORTES FINALES ================\n";
    cout << "Iteraciones empleadas: " << iter << "\n";
    cout << "Error absoluto estimado (E_a): " << scientific << setprecision(6) << error_est << "\n";

    return 0;
}