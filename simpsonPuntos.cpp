#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <iostream>

using namespace std;

#define TOL 1e-6


// Función para calcular los coeficientes de Spline Cúbico mediante Gauss
void splineCubico(double x[], double y[], int n, double sol[]) {
    int m = n - 1;       // cantidad de intervalos
    int N = 4 * m;       // cantidad de incógnitas

    double a[N][N], b[N];

    // Inicializo en 0
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) a[i][j] = 0.0;
        b[i] = 0.0;
        sol[i] = 0.0;
    }

    //-------- Primeras 2n filas --------
    for (int k = 0; k < m; k++) {
        for (int j = 0; j < 4; j++) {
            a[2*k][4*k + j]     = pow(x[k],   3 - j);
            a[2*k+1][4*k + j]   = pow(x[k+1], 3 - j);
        }
        b[2*k]   = y[k];
        b[2*k+1] = y[k+1];
    }

    //-------- n-1 ecuaciones derivadas primeras en internos --------
    for (int k = 0; k <= m - 2; k++) {
        double xp1 = x[k+1];
        for (int j = 0; j <= 2; j++) {
            double coeff = (3 - j) * pow(xp1, 2 - j);
            a[2*m + k][4*k + j]     += coeff;
            a[2*m + k][4*(k+1) + j] -= coeff;
        }
        b[2*m + k] = 0.0;
    }

    //-------- n-1 ecuaciones derivadas segundas en internos --------
    for (int k = 0; k <= m - 2; k++) {
        double xp1 = x[k+1];
        a[3*m - 1 + k][4*k + 0]     += 3.0 * xp1;
        a[3*m - 1 + k][4*k + 1]     += 1.0;
        a[3*m - 1 + k][4*(k+1) + 0] -= 3.0 * xp1;
        a[3*m - 1 + k][4*(k+1) + 1] -= 1.0;
        b[3*m - 1 + k] = 0.0;
    }

    //-------- derivadas en frontera --------
    a[4*m - 2][0] = 3.0 * x[0];
    a[4*m - 2][1] = 1.0;
    b[4*m - 2] = 0.0;

    a[4*m - 1][4*(m-1) + 0] = 3.0 * x[m];
    a[4*m - 1][4*(m-1) + 1] = 1.0;
    b[4*m - 1] = 0.0;

    //-------- Gauss con pivoteo --------
    for (int k = 0; k < N; k++) {
        int maxRow = k;
        double maxVal = fabs(a[k][k]);
        for (int i = k+1; i < N; i++) {
            if (fabs(a[i][k]) > maxVal) {
                maxVal = fabs(a[i][k]);
                maxRow = i;
            }
        }
        if (maxRow != k) {
            for (int j = k; j < N; j++) {
                double tmp = a[k][j];
                a[k][j] = a[maxRow][j];
                a[maxRow][j] = tmp;
            }
            double tmpb = b[k];
            b[k] = b[maxRow];
            b[maxRow] = tmpb;
        }
        for (int i = k+1; i < N; i++) {
            double factor = a[i][k] / a[k][k];
            for (int j = k; j < N; j++) {
                a[i][j] -= factor * a[k][j];
            }
            b[i] -= factor * b[k];
        }
    }

    for (int i = N-1; i >= 0; i--) {
        double s = b[i];
        for (int j = i+1; j < N; j++) s -= a[i][j] * sol[j];
        sol[i] = s / a[i][i];
    }
}

int main() {
    FILE *fp = fopen("puntos.txt", "r");
    if (!fp) {
        cout<< "No se pudo abrir 'puntos.txt'\n" << endl;
        return 1;
    }

    int n;  // Cantidad de puntos
    fscanf(fp, "%d", &n);
    if((n - 1) <= 0 || (n - 1) % 2 != 0) {
        cout<< "El numero de subintervalos debe ser un entero positivo par.\n" << endl;
        return 1;
    }

    double x[n], y[n];
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%lf %lf", &x[i], &y[i]);
    }
    fclose(fp);

    // Verificación de equiespaciado
    double h = x[1] - x[0];
    int equiespaciados = 1;

    for (int i = 0; i < n - 1; i++) {
        if (fabs((x[i + 1] - x[i]) - h) > TOL) {
            equiespaciados = 0;
            break;
        }
    }

    double xp[n], yp[n];

    if (equiespaciados) {
        cout << "Los puntos ESTAN equiespaciados.\n" << endl;
        for (int i = 0; i < n; i++) {
            xp[i] = x[i];
            yp[i] = y[i];
        }
    } else {
        cout << "Los puntos NO estan equiespaciados. Reajustando con Spline Cubico...\n" << endl;

        int m = n - 1;
        int N = 4 * m;
        double sol[N];

        // Llamada a la función del Spline
        splineCubico(x, y, n, sol);

        // Armar vector xp y yp equiespaciados
        h = (x[n - 1] - x[0]) / (n - 1);

        for (int i = 0; i < n; i++) {
            xp[i] = x[0] + i * h;

            // Buscar en qué intervalo k cae el punto xp[i]
            int k = -1;
            for (int j = 0; j < n - 1; j++) {
                if (xp[i] >= x[j] && xp[i] <= x[j + 1]) {
                    k = j;
                    break;
                }
            }
            if (k == -1) k = m - 1; // Ajuste para el límite superior

            // Valuar en el polinomio correspondiente S_k(xp[i])
            double ak = sol[4*k + 0];
            double bk = sol[4*k + 1];
            double ck = sol[4*k + 2];
            double dk = sol[4*k + 3];

            yp[i] = ak * pow(xp[i], 3) + bk * pow(xp[i], 2) + ck * xp[i] + dk;
        }
    }

    // Simpson sobre xp y yp
    double suma = yp[0] + yp[n - 1];

    for (int i = 1; i <= (n/2) - 1; i++) {
        suma = suma + 2 * yp[2*i] + 4 * yp[2*i - 1];
    }
    suma = suma + 4 * yp[n-2];
    double I = (h / 3.0) * suma;

   cout << "\nEl resultado de la integral es: "
     << fixed << setprecision(10) << I << endl;

    return 0;
}