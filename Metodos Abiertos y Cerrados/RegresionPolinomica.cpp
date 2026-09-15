#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Resolver sistema de ecuaciones A * x = B usando Eliminación Gaussiana
vector<double> resolverGauss(vector<vector<double>>& A, vector<double>& B) {
    int n = B.size();

    for (int i = 0; i < n; i++) {
        int maxFila = i;
        for (int k = i + 1; k < n; k++) {
            if (abs(A[k][i]) > abs(A[maxFila][i])) {
                maxFila = k;
            }
        }
        swap(A[i], A[maxFila]);
        swap(B[i], B[maxFila]);

        for (int k = i + 1; k < n; k++) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < n; j++) {
                A[k][j] -= factor * A[i][j];
            }
            B[k] -= factor * B[i];
        }
    }

    vector<double> coeficientes(n);
    for (int i = n - 1; i >= 0; i--) {
        double suma = 0.0;
        for (int j = i + 1; j < n; j++) {
            suma += A[i][j] * coeficientes[j];
        }
        coeficientes[i] = (B[i] - suma) / A[i][i];
    }

    return coeficientes;
}

// Armado del sistema de Ecuaciones Normales para la Regresión Polinómica
vector<double> regresionPolinomica(const vector<double>& x, const vector<double>& y, int grado) {
    int n = x.size();
    int m = grado + 1;

    vector<vector<double>> A(m, vector<double>(m, 0.0));
    vector<double> B(m, 0.0);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            double sumaX = 0.0;
            for (int k = 0; k < n; k++) {
                sumaX += pow(x[k], i + j);
            }
            A[i][j] = sumaX;
        }

        double sumaXY = 0.0;
        for (int k = 0; k < n; k++) {
            sumaXY += pow(x[k], i) * y[k];
        }
        B[i] = sumaXY;
    }

    return resolverGauss(A, B);
}

int main() {
    int n, grado;

    cout << "========================================" << endl;
    cout << "     REGRESION POLINOMICA EN C++        " << endl;
    cout << "========================================" << endl;

    cout << "Ingrese la cantidad de puntos (n): ";
    cin >> n;

    cout << "Ingrese el grado del polinomio: ";
    cin >> grado;

    if (n <= grado) {
        cout << "\n[ERROR] La cantidad de puntos debe ser mayor al grado del polinomio." << endl;
        return 1;
    }

    vector<double> x(n);
    vector<double> y(n);

    cout << "\n--- CARGA DE PUNTOS ---" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Punto " << i + 1 << ":" << endl;
        cout << "  x[" << i << "] = ";
        cin >> x[i];
        cout << "  y[" << i << "] = ";
        cin >> y[i];
    }

    vector<double> a = regresionPolinomica(x, y, grado);

    // Cálculo de St, Sr, ECM, r^2 y r
    double sumaY = 0.0;
    for (int i = 0; i < n; i++) sumaY += y[i];
    double y_prom = sumaY / n;

    double St = 0.0;
    double Sr = 0.0;

    for (int k = 0; k < n; k++) {
        double y_pred = 0.0;
        for (size_t i = 0; i < a.size(); i++) {
            y_pred += a[i] * pow(x[k], i);
        }
        St += pow(y[k] - y_prom, 2);
        Sr += pow(y[k] - y_pred, 2);
    }

    double ecm = Sr / n; // Error Cuadrático Medio
    double r2 = (St - Sr) / St;
    double r = sqrt(r2);

    // Impresión de resultados
    cout << "\n========================================" << endl;
    cout << "               RESULTADOS               " << endl;
    cout << "========================================" << endl;
    cout << fixed << setprecision(4);

    cout << "Coeficientes f(x) = a*x^2 + b*x + c:" << endl;
    cout << "  a = " << a[2] << endl;
    cout << "  b = " << a[1] << endl;
    cout << "  c = " << a[0] << endl;

    cout << "\nPolinomio resultante:" << endl;
    cout << "y = " << a[2] << "*x^2 ";
    if (a[1] >= 0) cout << "+ " << a[1] << "*x ";
    else cout << "- " << abs(a[1]) << "*x ";
    if (a[0] >= 0) cout << "+ " << a[0];
    else cout << "- " << abs(a[0]);
    cout << endl;

    cout << "\nEstadisticos del ajuste:" << endl;
    cout << "  St (Suma total de cuadrados)   = " << St << endl;
    cout << "  Sr (Suma de residuos al cuad.) = " << Sr << endl;
    cout << "  ECM (Error Cuadratico Medio)   = " << ecm << endl;
    cout << "  r^2 (Coef. de determinacion)   = " << r2 << endl;
    cout << "  r   (Coef. de correlacion)     = " << r << endl;
    cout << "========================================" << endl;

    return 0;
}