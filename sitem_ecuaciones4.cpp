
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Funcion diferencial: y' = f(t,y)
double f(double t, double y) {
    return -2 * t * y;
}

// Solucion analitica exacta
double solucionExacta(double t) {
    return exp(-pow(t, 2));
}

int main() {

    // Datos del problema
    double t0 = 0.0;
    double y0 = 1.0;
    double h = 0.2;
    double tf = 0.2;

    // Calculamos la cantidad de subintervalos
    int n = (int)((tf - t0) / h);

    // Condiciones iniciales
    double t = t0;
    double y = y0;

    //-------- Runge-Kutta de cuarto orden --------

    for (int i = 0; i < n; i++) {

        // Primera pendiente
        double k1 = f(t, y);

        // Segunda pendiente
        double k2 = f(t + h/2.0,
                      y + (h/2.0)*k1);

        // Tercera pendiente
        double k3 = f(t + h/2.0,
                      y + (h/2.0)*k2);

        // Cuarta pendiente
        double k4 = f(t + h,
                      y + h*k3);

        // Calculamos el siguiente valor de y
        y = y + (h/6.0) *
            (k1 + 2*k2 + 2*k3 + k4);

        // Actualizamos t
        t = t + h;

        // Mostramos las pendientes
        printf("Iteracion %d\n", i + 1);
        printf("k1 = %.10f\n", k1);
        printf("k2 = %.10f\n", k2);
        printf("k3 = %.10f\n", k3);
        printf("k4 = %.10f\n", k4);
    }

    //-------- Calculo del error --------

    double exacta = solucionExacta(tf);
    double error = fabs(exacta - y);

    printf("\n===== RESULTADOS RK4 =====\n");

    printf("Paso h = %.2f\n", h);
    printf("t final = %.2f\n", t);

    printf("Resultado aproximado RK4 = %.10f\n", y);
    printf("Solucion exacta = %.10f\n", exacta);
    printf("Error absoluto = %.10f\n", error);

    return 0;
}
