#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// Evalua el polinomio en un punto x_eval (Inciso a)
double interpolarLagrange(const vector<double>& x, const vector<double>& y, double x_eval) {
    int n = x.size();
    double resultado = 0.0;

    for (int i = 0; i < n; i++) {
        double L_i = 1.0;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                L_i *= (x_eval - x[j]) / (x[i] - x[j]);
            }
        }
        resultado += y[i] * L_i;
    }
    return resultado;
}

// Obtiene los coeficientes del polinomio desarrollado c0 + c1*x + c2*x^2 ... (Inciso b)
vector<double> obtenerCoeficientes(const vector<double>& x, const vector<double>& y) {
    int n = x.size();
    vector<double> coefs(n, 0.0);

    for (int i = 0; i < n; i++) {
        double denom = 1.0;
        for (int j = 0; j < n; j++) {
            if (i != j) denom *= (x[i] - x[j]);
        }

        vector<double> num_poly = {1.0};
        for (int j = 0; j < n; j++) {
            if (i != j) {
                vector<double> temp(num_poly.size() + 1, 0.0);
                for (size_t k = 0; k < num_poly.size(); k++) {
                    temp[k + 1] += num_poly[k];
                    temp[k]     -= num_poly[k] * x[j];
                }
                num_poly = temp;
            }
        }

        double factor = y[i] / denom;
        for (int k = 0; k < n; k++) {
            coefs[k] += factor * num_poly[k];
        }
    }
    return coefs;
}

int main() {
    int n;
    cout << "================ INTERPOLACION DE LAGRANGE GENERICA ================\n";
    cout << "Ingrese la cantidad de puntos conocidos (N): ";
    cin >> n;

    vector<double> x(n), y(n);
    cout << "\n--- Ingrese los puntos tabulados (x_i y_i) ---\n";
    for (int i = 0; i < n; i++) {
        cout << "Punto " << i + 1 << ": ";
        cin >> x[i] >> y[i];
    }

    // --- EVALUACION DE PUNTOS ---
    int m;
    cout << "\n¿Cuantos valores de x desea evaluar?: ";
    cin >> m;

    cout << fixed << setprecision(6);
    cout << "\n=== INCISO A: EVALUACIONES ===\n";
    for (int k = 0; k < m; k++) {
        double x_eval;
        cout << "Ingrese el valor de x [" << k + 1 << "]: ";
        cin >> x_eval;
        double y_eval = interpolarLagrange(x, y, x_eval);
        cout << " -> P(" << x_eval << ") = " << y_eval << "\n";
    }

    // --- POLINOMIO DESARROLLADO ---
    vector<double> c = obtenerCoeficientes(x, y);

    cout << "\n=== INCISO B: POLINOMIO INTERPOLADOR ===\n";
    cout << "Coeficientes del polinomio (desde grado 0 hasta " << n - 1 << "):\n";
    for (int i = 0; i < n; i++) {
        cout << "c[" << i << "] (x^" << i << ") = " << c[i] << "\n";
    }

    cout << "\nEcuacion del polinomio:\nP(x) = ";
    for (int i = n - 1; i >= 0; i--) {
        if (i < n - 1 && c[i] >= 0) cout << " + ";
        else if (c[i] < 0) cout << " - ";

        cout << fabs(c[i]);
        if (i > 0) cout << "*x";
        if (i > 1) cout << "^" << i;
    }
    cout << "\n";

    return 0;
}