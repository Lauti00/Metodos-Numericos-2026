#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// Evalua el polinomio de Lagrange en un punto x_eval
double interpolarLagrange(const vector<double>& x, const vector<double>& y, double x_eval) {
    int n = x.size();
    double resultado = 0.0;

    for (int i = 0; i < n; i++) {
        double L_i = 1.0; // Polinomio base L_i(x)
        
        for (int j = 0; j < n; j++) {
            if (i != j) {
                L_i *= (x_eval - x[j]) / (x[i] - x[j]);
            }
        }
        
        resultado += y[i] * L_i;
    }

    return resultado;
}

int main() {
    int n;
    cout << "================ INTERPOLACION DE LAGRANGE ================\n";
    cout << "Ingrese la cantidad de puntos: ";
    cin >> n;

    vector<double> x(n), y(n);
    cout << "\n--- Ingrese los puntos (x_i, y_i) ---\n";
    for (int i = 0; i < n; i++) {
        cout << "Punto " << i + 1 << " (x y): ";
        cin >> x[i] >> y[i];
    }

    double x_eval;
    cout << "\nIngrese el valor de x a interpolar: ";
    cin >> x_eval;

    double y_eval = interpolarLagrange(x, y, x_eval);

    cout << "\n================ RESULTADO ================\n";
    cout << "P(" << fixed << setprecision(4) << x_eval << ") = " << setprecision(6) << y_eval << "\n";

    return 0;
}