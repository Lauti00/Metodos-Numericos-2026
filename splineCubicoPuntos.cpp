#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

// Evaluar el polinomio cubico de un intervalo k
double evaluarSpline(double x, int k, const vector<double>& sol) {
    double a = sol[4 * k];
    double b = sol[4 * k + 1];
    double c = sol[4 * k + 2];
    double d = sol[4 * k + 3];

    return a * x * x * x + b * x * x + c * x + d;
}

int main() {

    //========================================
    // 1. LEER LOS PUNTOS DESDE ARCHIVO
    //========================================

    ifstream fp("puntos.txt");

    if (!fp) {
        cout << "Error: No se pudo abrir puntos.txt\n";
        return 1;
    }

    int n;
    fp >> n;

    if (n < 2) {
        cout << "Error: Se necesitan al menos 2 puntos.\n";
        return 1;
    }

    vector<double> x(n), y(n);

    for (int i = 0; i < n; i++) {
        if (!(fp >> x[i] >> y[i])) {
            cout << "Error leyendo las coordenadas en el punto " << i + 1 << ".\n";
            return 1;
        }
    }

    fp.close();

    // Verificar orden estrictamente creciente en x
    for (int i = 0; i < n - 1; i++) {
        if (x[i + 1] <= x[i]) {
            cout << "Error: Los valores de x deben ser estrictamente crecientes.\n";
            return 1;
        }
    }

    int m = n - 1;       // Cantidad de intervalos
    int N = 4 * m;       // Cantidad de incognitas (4 por cada subintervalo)

    //========================================
    // 2. ARMAR EL SISTEMA DE ECUACIONES A * X = B
    //========================================

    vector<vector<double>> A(N, vector<double>(N, 0.0));
    vector<double> B(N, 0.0);
    vector<double> sol(N, 0.0);

    // Condicion 1: Interpolacion (el polinomio pasa por los puntos extremos de su intervalo)
    for (int k = 0; k < m; k++) {
        for (int j = 0; j < 4; j++) {
            A[2 * k][4 * k + j] = pow(x[k], 3 - j);
            A[2 * k + 1][4 * k + j] = pow(x[k + 1], 3 - j);
        }
        B[2 * k] = y[k];
        B[2 * k + 1] = y[k + 1];
    }

    // Condicion 2: Continuidad de la primera derivada en nodos internos
    for (int k = 0; k < m - 1; k++) {
        double xp = x[k + 1];
        for (int j = 0; j <= 2; j++) {
            double coef = (3 - j) * pow(xp, 2 - j);
            A[2 * m + k][4 * k + j] += coef;
            A[2 * m + k][4 * (k + 1) + j] -= coef;
        }
    }

    // Condicion 3: Continuidad de la segunda derivada en nodos internos
    for (int k = 0; k < m - 1; k++) {
        double xp = x[k + 1];
        A[3 * m - 1 + k][4 * k] += 3.0 * xp;
        A[3 * m - 1 + k][4 * k + 1] += 1.0;

        A[3 * m - 1 + k][4 * (k + 1)] -= 3.0 * xp;
        A[3 * m - 1 + k][4 * (k + 1) + 1] -= 1.0;
    }

    // Condicion 4: Spline Natural (segunda derivada = 0 en los extremos)
    A[4 * m - 2][0] = 3.0 * x[0];
    A[4 * m - 2][1] = 1.0;

    A[4 * m - 1][4 * (m - 1)] = 3.0 * x[n - 1];
    A[4 * m - 1][4 * (m - 1) + 1] = 1.0;

    //========================================
    // 3. ELIMINACION GAUSSIANA CON PIVOTEO PARCIAL
    //========================================

    for (int k = 0; k < N; k++) {
        int maxRow = k;
        double maxVal = fabs(A[k][k]);

        for (int i = k + 1; i < N; i++) {
            if (fabs(A[i][k]) > maxVal) {
                maxVal = fabs(A[i][k]);
                maxRow = i;
            }
        }

        if (maxVal < 1e-12) {
            cout << "Error: El sistema lineal es singular o mal condicionado.\n";
            return 1;
        }

        if (maxRow != k) {
            swap(A[k], A[maxRow]);
            swap(B[k], B[maxRow]);
        }

        for (int i = k + 1; i < N; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < N; j++) {
                A[i][j] -= factor * A[k][j];
            }
            B[i] -= factor * B[k];
        }
    }

    // Sustitucion hacia atras
    for (int i = N - 1; i >= 0; i--) {
        double suma = B[i];
        for (int j = i + 1; j < N; j++) {
            suma -= A[i][j] * sol[j];
        }
        sol[i] = suma / A[i][i];
    }

    //========================================
    // 4. MOSTRAR POLINOMIOS POR INTERVALO
    //========================================

    cout << "\n========================================\n";
    cout << " POLINOMIOS DEL SPLINE CUBICO NATURAL\n";
    cout << "========================================\n\n";

    cout << setprecision(8);

    for (int k = 0; k < m; k++) {
        cout << "Intervalo " << k + 1 << ": [" << x[k] << ", " << x[k + 1] << "]\n";
        cout << "S_" << k << "(x) = (" << sol[4 * k] << ") x^3 + ("
             << sol[4 * k + 1] << ") x^2 + ("
             << sol[4 * k + 2] << ") x + ("
             << sol[4 * k + 3] << ")\n\n";
    }

    //========================================
    // 5. EVALUACION E INTERPOLACION
    //========================================

    double h;
    cout << "Ingrese el paso h para muestrear el spline: ";
    cin >> h;

    if (h <= 0) {
        cout << "Error: El paso debe ser positivo.\n";
        return 1;
    }

    ofstream salida("resultados.txt");

    if (!salida) {
        cout << "Error: No se pudo crear resultados.txt\n";
        return 1;
    }

    cout << "\nTABLA DE VALORES INTERPOLADOS\n";
    cout << "-------------------------------\n";
    cout << setw(12) << "x" << setw(16) << "S(x)" << "\n";
    cout << "-------------------------------\n";

    salida << fixed << setprecision(6);
    salida << setw(12) << "x" << setw(16) << "S(x)" << "\n";

    double xmin = x[0];
    double xmax = x[n - 1];

    int pasos = (int)floor((xmax - xmin) / h + 1e-10);

    for (int j = 0; j <= pasos; j++) {
        double xv = xmin + j * h;

        // Garantizar que no exceda el limite por precision flotante
        if (xv > xmax) xv = xmax;

        // Ubicar el subintervalo correspondiente
        int k = -1;
        for (int i = 0; i < m; i++) {
            if (xv >= x[i] && xv <= x[i + 1] + 1e-9) {
                k = i;
                break;
            }
        }

        if (xv >= xmax - 1e-9) {
            k = m - 1;
        }

        if (k != -1) {
            double valorSpline = evaluarSpline(xv, k, sol);

            cout << fixed << setprecision(6)
                 << setw(12) << xv
                 << setw(16) << valorSpline << "\n";

            salida << setw(12) << xv
                   << setw(16) << valorSpline << "\n";
        }
    }

    salida.close();
    cout << "\nResultados guardados exitosamente en 'resultados.txt'.\n";

    return 0;
}