#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {
    int n;
    cout << "================ REGRESION LINEAL (MINIMOS CUADRADOS) ================\n";
    cout << "Ingrese la cantidad de puntos (N): ";
    cin >> n;

    vector<double> x(n), y(n);
    cout << "\n--- Ingrese los puntos (x y) ---\n";
    for (int i = 0; i < n; i++) {
        cout << "Punto " << i + 1 << ": ";
        cin >> x[i] >> y[i];
    }

    // Calculo de sumatorias necesarias
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (int i = 0; i < n; i++) {
        sumX += x[i];
        sumY += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
    }

    // Promedios
    double x_bar = sumX / n;
    double y_bar = sumY / n;

    // Calculo de coeficientes a1 (pendiente) y a0 (ordenada al origen)
    double a1 = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double a0 = y_bar - a1 * x_bar;

    // Calculo del coeficiente de determinacion r^2
    double St = 0, Sr = 0;
    for (int i = 0; i < n; i++) {
        double y_predicha = a1 * x[i] + a0;
        St += pow(y[i] - y_bar, 2);
        Sr += pow(y[i] - y_predicha, 2);
    }

    double r2 = (St - Sr) / St;
    double r = sqrt(r2);

    // Muestra de Resultados
    cout << "\n================ RESULTADOS DEL AJUSTE ================\n";
    cout << fixed << setprecision(6);
    cout << "Ecuacion de la recta: y = " << a1 << " * x + (" << a0 << ")\n";
    cout << "Pendiente (a1):      " << a1 << "\n";
    cout << "Ordenada (a0):       " << a0 << "\n";
    cout << "--------------------------------------------------------\n";
    cout << "Suma de residuos (Sr):            " << Sr << "\n";
    cout << "Coeficiente de determinacion (r^2): " << r2 << " (" << r2 * 100 << "%)\n";
    cout << "Coeficiente de correlacion (r):   " << r << "\n";

    return 0;
}