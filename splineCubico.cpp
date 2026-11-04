
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

// Funcion exacta del ejercicio
double funcionExacta(double x) {
    return exp(2.0 * sin(x)) * (1.0 + log(x));
}

// Evaluar un polinomio cubico
double evaluarSpline(double x, int k, const vector<double>& sol) {
    double a = sol[4*k];
    double b = sol[4*k+1];
    double c = sol[4*k+2];
    double d = sol[4*k+3];

    return a*x*x*x + b*x*x + c*x + d;
}

int main() {

    //========================================
    // 1. LEER LOS PUNTOS
    //========================================

    ifstream fp("puntos.txt");

    if (!fp) {
        cout << "No se pudo abrir puntos.txt\n";
        return 1;
    }

    int n;
    fp >> n;

    if (n < 2) {
        cout << "Se necesitan al menos 2 puntos.\n";
        return 1;
    }

    vector<double> x(n), y(n);

    for (int i = 0; i < n; i++) {
        if (!(fp >> x[i] >> y[i])) {
            cout << "Error leyendo los puntos.\n";
            return 1;
        }
    }

    fp.close();

    // Verificar que los puntos esten ordenados
    for (int i = 0; i < n-1; i++) {
        if (x[i+1] <= x[i]) {
            cout << "Los valores de x deben ser crecientes.\n";
            return 1;
        }
    }

    int m = n - 1;       // Cantidad de intervalos
    int N = 4 * m;       // Cantidad de incognitas

    //========================================
    // 2. ARMAR EL SISTEMA DE ECUACIONES
    //========================================

    vector<vector<double>> A(N, vector<double>(N, 0.0));
    vector<double> B(N, 0.0);
    vector<double> sol(N, 0.0);

    // Interpolacion: cada polinomio pasa por
    // los dos extremos de su intervalo

    for (int k = 0; k < m; k++) {

        for (int j = 0; j < 4; j++) {

            A[2*k][4*k+j] =
                pow(x[k], 3-j);

            A[2*k+1][4*k+j] =
                pow(x[k+1], 3-j);
        }

        B[2*k] = y[k];
        B[2*k+1] = y[k+1];
    }

    // Continuidad de la primera derivada
    for (int k = 0; k < m-1; k++) {

        double xp = x[k+1];

        for (int j = 0; j <= 2; j++) {

            double coef = (3-j) * pow(xp, 2-j);

            A[2*m+k][4*k+j] += coef;
            A[2*m+k][4*(k+1)+j] -= coef;
        }
    }

    // Continuidad de la segunda derivada
    for (int k = 0; k < m-1; k++) {

        double xp = x[k+1];

        A[3*m-1+k][4*k] += 3.0*xp;
        A[3*m-1+k][4*k+1] += 1.0;

        A[3*m-1+k][4*(k+1)] -= 3.0*xp;
        A[3*m-1+k][4*(k+1)+1] -= 1.0;
    }

    // Condiciones de spline NATURAL:
    // Segunda derivada igual a cero
    // en el primer y ultimo punto

    A[4*m-2][0] = 3.0*x[0];
    A[4*m-2][1] = 1.0;

    A[4*m-1][4*(m-1)] = 3.0*x[n-1];
    A[4*m-1][4*(m-1)+1] = 1.0;

    //========================================
    // 3. GAUSS CON PIVOTEO
    //========================================

    for (int k = 0; k < N; k++) {

        int maxRow = k;
        double maxVal = fabs(A[k][k]);

        for (int i = k+1; i < N; i++) {

            if (fabs(A[i][k]) > maxVal) {
                maxVal = fabs(A[i][k]);
                maxRow = i;
            }
        }

        if (maxVal < 1e-12) {
            cout << "El sistema no puede resolverse.\n";
            return 1;
        }

        // Intercambiar filas
        if (maxRow != k) {

            swap(A[k], A[maxRow]);
            swap(B[k], B[maxRow]);
        }

        // Eliminacion
        for (int i = k+1; i < N; i++) {

            double factor = A[i][k] / A[k][k];

            for (int j = k; j < N; j++) {
                A[i][j] -= factor * A[k][j];
            }

            B[i] -= factor * B[k];
        }
    }

    // Sustitucion hacia atras
    for (int i = N-1; i >= 0; i--) {

        double suma = B[i];

        for (int j = i+1; j < N; j++) {
            suma -= A[i][j] * sol[j];
        }

        sol[i] = suma / A[i][i];
    }

    //========================================
    // 4. MOSTRAR LOS POLINOMIOS
    //========================================

    cout << "\nSPLINE CUBICO NATURAL\n\n";

    cout << setprecision(8);

    for (int k = 0; k < m; k++) {

        cout << "Intervalo [" << x[k]
             << ", " << x[k+1] << "]\n";

        cout << "S_" << k << "(x) = "
             << sol[4*k] << " x^3 + "
             << sol[4*k+1] << " x^2 + "
             << sol[4*k+2] << " x + "
             << sol[4*k+3] << "\n\n";
    }

    //========================================
    // 5. GENERAR TABLA DE RESULTADOS
    //========================================

    double h;

    cout << "Ingrese el paso h para evaluar: ";
    cin >> h;

    if (h <= 0) {
        cout << "El paso debe ser positivo.\n";
        return 1;
    }

    ofstream salida("resultados.txt");

    if (!salida) {
        cout << "No se pudo crear resultados.txt\n";
        return 1;
    }

    cout << "\nTABLA DE RESULTADOS\n\n";

    cout << fixed << setprecision(6);

    cout << "     x       Spline       Exacta       Error\n";

    salida << fixed << setprecision(6);
    salida << "     x       Spline       Exacta       Error\n";

    double xmin = x[0];
    double xmax = x[n-1];

    int pasos = (int)floor((xmax-xmin)/h + 1e-10);

    for (int j = 0; j <= pasos; j++) {

        double xv = xmin + j*h;

        // Buscar el intervalo
        int k = -1;

        for (int i = 0; i < m; i++) {

            if (xv >= x[i] && xv <= x[i+1]) {
                k = i;
                break;
            }
        }

        // Si estamos exactamente en el ultimo punto
        if (fabs(xv-xmax) < 1e-10) {
            k = m-1;
        }

        if (k != -1) {

            double spline = evaluarSpline(xv, k, sol);

            double exacta = funcionExacta(xv);

            double error = fabs(exacta-spline);

            cout << setw(8) << xv
                 << setw(13) << spline
                 << setw(13) << exacta
                 << setw(13) << error << "\n";

            salida << setw(8) << xv
                   << setw(13) << spline
                   << setw(13) << exacta
                   << setw(13) << error << "\n";
        }
    }

    salida.close();

    cout << "\nLos resultados se guardaron en resultados.txt\n";

    return 0;
}
