#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cstdio>
#include <string>

using namespace std;

// --- 1. FUNCIONES DE ANÁLISIS Y ACONDICIONAMIENTO DE MATRIZ ---

bool esEDD(const vector<vector<double>>& A) {
    int n = A.size();
    for (int i = 0; i < n; i++) {
        double suma = 0.0;
        for (int j = 0; j < n; j++) {
            if (i != j) suma += fabs(A[i][j]);
        }
        if (fabs(A[i][i]) <= suma) return false;
    }
    return true;
}

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
            return false;
        }
    }
    A = A_nueva;
    b = b_nueva;
    return true;
}

// --- 2. MÉTODOS ITERATIVOS ---

// Método de Jacobi (Usa exclusivamente x_old para todos los calculos de la iteracion)
void resolverJacobi(const vector<vector<double>>& A, const vector<double>& b, double tol, int max_iter) {
    int n = A.size();
    vector<double> x(n, 0.0);
    vector<double> x_old(n, 0.0);

    cout << "\n================ EJECUTANDO METODO DE JACOBI ================\n";
    cout << setw(6) << "Iter";
    for (int i = 0; i < n; i++) cout << setw(18) << ("x[" + to_string(i + 1) + "]");
    cout << setw(18) << "Error Est (E_a)\n";
    cout << string(24 + n * 18, '-') << "\n";

    int iter = 0;
    double error_est = 0.0;

    do {
        iter++;
        x_old = x;
        error_est = 0.0;

        for (int i = 0; i < n; i++) {
            double suma = b[i];
            for (int j = 0; j < n; j++) {
                if (i != j) suma -= A[i][j] * x_old[j]; // Jacobi: Siempre x_old
            }
            x[i] = suma / A[i][i];
        }

        for (int i = 0; i < n; i++) {
            error_est = max(error_est, fabs(x[i] - x_old[i]));
        }

        cout << setw(6) << iter;
        for (int i = 0; i < n; i++) cout << setw(18) << fixed << setprecision(4) << x[i];
        cout << setw(18) << scientific << setprecision(4) << error_est << "\n";

    } while (error_est > tol && iter < max_iter);

    cout << "\nIteraciones totales (Jacobi): " << iter << "\n";
}

// Método de Gauss-Seidel con Relajación (SOR)
// Si omega = 1.0 -> Gauss-Seidel estándar
// Si 1.0 < omega < 2.0 -> Sobrerrelajación
// Si 0.0 < omega < 1.0 -> Subrrelajación
void resolverGaussSeidelSOR(const vector<vector<double>>& A, const vector<double>& b, double omega, double tol, int max_iter) {
    int n = A.size();
    vector<double> x(n, 0.0);
    vector<double> x_old(n, 0.0);

    cout << "\n================ EJECUTANDO GAUSS-SEIDEL / SOR (omega = " << fixed << setprecision(2) << omega << ") ================\n";
    cout << setw(6) << "Iter";
    for (int i = 0; i < n; i++) cout << setw(18) << ("x[" + to_string(i + 1) + "]");
    cout << setw(18) << "Error Est (E_a)\n";
    cout << string(24 + n * 18, '-') << "\n";

    int iter = 0;
    double error_est = 0.0;

    do {
        iter++;
        x_old = x;
        error_est = 0.0;

        for (int i = 0; i < n; i++) {
            double suma = b[i];
            for (int j = 0; j < n; j++) {
                if (i != j) suma -= A[i][j] * x[j]; // Gauss-Seidel: Usa el x actualizado
            }
            double x_gs = suma / A[i][i];
            
            // Formula de Relajacion: x_nueva = w * x_gs + (1 - w) * x_anterior
            x[i] = omega * x_gs + (1.0 - omega) * x_old[i];
        }

        for (int i = 0; i < n; i++) {
            error_est = max(error_est, fabs(x[i] - x_old[i]));
        }

        cout << setw(6) << iter;
        for (int i = 0; i < n; i++) cout << setw(18) << fixed << setprecision(4) << x[i];
        cout << setw(18) << scientific << setprecision(4) << error_est << "\n";

    } while (error_est > tol && iter < max_iter);

    cout << "\nIteraciones totales: " << iter << "\n";
}

// --- 3. PROGRAMA PRINCIPAL ---

int main() {
    int n;
    FILE *fp = fopen("matriz.txt", "r");

    if (!fp) {
        cout << "Error: No se pudo abrir 'matriz.txt'.\n";
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

    cout << "================ SISTEMA [A|b] ================\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) cout << setw(8) << setprecision(2) << fixed << A[i][j] << " ";
        cout << " | " << setw(8) << setprecision(2) << fixed << b[i] << "\n";
    }

    if (!esEDD(A)) {
        cout << "\nEl sistema no es EDD. Reordenando...\n";
        if (forzarEDD(A, b)) {
            cout << "Sistema reordenado a EDD con exito.\n";
        } else {
            cout << "ADVERTENCIA: No se pudo forzar EDD. Podria no converger.\n";
        }
    }

    double tol = 1e-4;
    int max_iter = 100;
    int opcion;

    cout << "\n--- SELECCIONE EL METODO DE RESOLUCION ---\n";
    cout << "1. Método de Jacobi\n";
    cout << "2. Método de Gauss-Seidel Estándar\n";
    cout << "3. Método de Gauss-Seidel con Relajación (SOR)\n";
    cout << "4. Ejecutar TODOS y comparar iteraciones\n";
    cout << "Opcion: ";
    cin >> opcion;

    if (opcion == 1) {
        resolverJacobi(A, b, tol, max_iter);
    } else if (opcion == 2) {
        resolverGaussSeidelSOR(A, b, 1.0, tol, max_iter);
    } else if (opcion == 3) {
        double w;
        cout << "Ingrese el factor de relajacion w (0 < w < 2): ";
        cin >> w;
        resolverGaussSeidelSOR(A, b, w, tol, max_iter);
    } else if (opcion == 4) {
        resolverJacobi(A, b, tol, max_iter);
        resolverGaussSeidelSOR(A, b, 1.0, tol, max_iter);
        resolverGaussSeidelSOR(A, b, 1.25, tol, max_iter); // Ejemplo con w = 1.25
    }

    return 0;
}