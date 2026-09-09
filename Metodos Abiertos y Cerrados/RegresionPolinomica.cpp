#include <iostream>  // Entrada y salida por consola (cout, cin)
#include <vector>    // Manejo de arreglos dinámicos
#include <cmath>     // Funciones matemáticas (pow, abs)
#include <iomanip>   // Formato de salida de números (fixed, setprecision)

using namespace std;

// Resolver sistema de ecuaciones A * x = B usando Eliminación Gaussiana
vector<double> resolverGauss(vector<vector<double>>& A, vector<double>& B) {
    int n = B.size();

    for (int i = 0; i < n; i++) {
        // Pivoteo parcial para evitar división por cero o errores de precisión
        int maxFila = i;
        for (int k = i + 1; k < n; k++) {
            if (abs(A[k][i]) > abs(A[maxFila][i])) {
                maxFila = k;
            }
        }
        swap(A[i], A[maxFila]);
        swap(B[i], B[maxFila]);

        // Eliminación hacia adelante
        for (int k = i + 1; k < n; k++) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < n; j++) {
                A[k][j] -= factor * A[i][j];
            }
            B[k] -= factor * B[i];
        }
    }

    // Sustitución hacia atrás
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

    // Validación
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

    // Impresión de resultados con formato
    cout << "\n========================================" << endl;
    cout << "            RESULTADOS                  " << endl;
    cout << "========================================" << endl;
    cout << fixed << setprecision(4);

    cout << "Coeficientes obtenidos:" << endl;
    for (size_t i = 0; i < a.size(); i++) {
        cout << "  a" << i << " = " << a[i] << endl;
    }

    cout << "\nPolinomio resultante:" << endl;
    cout << "y = " << a[0];
    for (size_t i = 1; i < a.size(); i++) {
        if (a[i] >= 0) {
            cout << " + " << a[i] << "*x^" << i;
        } else {
            cout << " - " << abs(a[i]) << "*x^" << i;
        }
    }
    cout << endl << "========================================" << endl;

    return 0;
}