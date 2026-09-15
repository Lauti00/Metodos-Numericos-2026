#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Función exacta dada
double funcionExacta(double x) {
    return log(x * x + 1.0) - sin(x);
}

// 1. Calcula las diferencias divididas
vector<vector<double>> calcularDiferenciasDivididas(const vector<double>& x, const vector<double>& y) {
    int N = x.size();
    vector<vector<double>> diff(N, vector<double>(N, 0.0));

    for (int i = 0; i < N; i++) diff[i][0] = y[i];

    for (int j = 1; j < N; j++) {
        for (int i = 0; i < N - j; i++) {
            diff[i][j] = (diff[i + 1][j - 1] - diff[i][j - 1]) / (x[i + j] - x[i]);
        }
    }
    return diff;
}

// Multiplica un polinomio P(x) por el binomio (x - r)
vector<double> multiplicarPorBinomio(const vector<double>& p, double r) {
    size_t size = p.size();
    vector<double> res(size + 1, 0.0);
    for (size_t i = 0; i < size; ++i) {
        res[i + 1] += p[i];     // p[i] * x
        res[i] -= p[i] * r;      // p[i] * (-r)
    }
    return res;
}

// 2. Convierte los coeficientes de Newton a la forma desarrollada c0 + c1*x + c2*x^2 + ...
vector<double> convertirAFormaCanonica(const vector<double>& a, const vector<double>& x) {
    size_t N = a.size();
    vector<double> canonica(N, 0.0);
    vector<double> prod = {1.0}; // Comienza representando la constante 1

    for (size_t k = 0; k < N; ++k) {
        for (size_t i = 0; i < prod.size(); ++i) {
            canonica[i] += a[k] * prod[i];
        }
        if (k < N - 1) {
            prod = multiplicarPorBinomio(prod, x[k]);
        }
    }
    return canonica; // canonica[i] es el coeficiente de x^i
}

// 3. Imprime la forma canónica resuelta (de mayor a menor grado)
void mostrarFormaCanonica(const vector<double>& c) {
    int grado = c.size() - 1;
    cout << "P(x) = ";
    
    bool primerTermino = true;
    for (int i = grado; i >= 0; --i) {
        double coef = c[i];
        if (fabs(coef) < 1e-9) continue; // Saltea coeficientes despreciables

        if (primerTermino) {
            if (coef < 0) cout << "- ";
            primerTermino = false;
        } else {
            if (coef >= 0) cout << " + ";
            else cout << " - ";
        }

        double valAbs = fabs(coef);
        if (i == 0) {
            cout << valAbs;
        } else if (i == 1) {
            cout << valAbs << "x";
        } else {
            cout << valAbs << "x^" << i;
        }
    }
    cout << "\n";
}

int main() {
    int N;
    cout << "Ingrese la cantidad de puntos: ";
    cin >> N;

    vector<double> x(N), y(N);
    cout << "\n--- INGRESO DE DATOS ---\n";
    for (int i = 0; i < N; i++) {
        cout << "Punto " << i + 1 << " - x: ";
        cin >> x[i];
        cout << "Punto " << i + 1 << " - y: ";
        cin >> y[i];
    }

    // Proceso numérico
    vector<vector<double>> tabla = calcularDiferenciasDivididas(x, y);
    vector<double> coefNewton(N);
    for (int i = 0; i < N; i++) coefNewton[i] = tabla[0][i];

    // Desarrollo algebraico
    vector<double> canonica = convertirAFormaCanonica(coefNewton, x);

    cout << fixed << setprecision(6);
    cout << "\n==========================================================\n";
    cout << "          POLINOMIO INTERPOLADOR RESUELTO (CANONICO)      \n";
    cout << "==========================================================\n";
    mostrarFormaCanonica(canonica);

    // Evaluación
    double x_eval;
    cout << "\nIngrese 'x' a evaluar: ";
    cin >> x_eval;

    // Evaluación usando la forma desarrollada
    double p_val = 0.0;
    for (size_t i = 0; i < canonica.size(); ++i) {
        p_val += canonica[i] * pow(x_eval, i);
    }

    double f_exact = funcionExacta(x_eval);
    double error_abs = fabs(p_val - f_exact);

    cout << "\n==========================================================\n";
    cout << "               EVALUACION NUMERICA EN X = " << x_eval << "\n";
    cout << "==========================================================\n";
    cout << "P(" << x_eval << ") Aproximado : " << p_val << "\n";
    cout << "f(" << x_eval << ") Exacto     : " << f_exact << "\n";
    cout << "Error Absoluto    : " << error_abs << "\n";
    cout << "==========================================================\n";

    return 0;
}