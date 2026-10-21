#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Funcion diferencial 1
double f1(double x, double y1, double y2) {
    y1 = y2;
    return y1;
}

// Funcion diferencial 2
double f2(double x, double y1, double y2) {
    y2 = 4 * cos(x) - y1;
    return y2;
}

int main() {
    double x0, xf, h, y10, y20;
    int n, metodo;

    printf("Ingrese x0: ");
    scanf("%lf", &x0);
    printf("Ingrese y10: ");
    scanf("%lf", &y10);
    printf("Ingrese y20: ");
    scanf("%lf", &y20);
    printf("Ingrese xf: ");
    scanf("%lf", &xf);
    printf("Ingrese el numero de subintervalos n: ");
    scanf("%d", &n);

    if(n <= 0) {
        printf("n debe ser un entero positivo.\n");
        return 1;
    }

    printf("\nSeleccione el metodo:\n");
    printf("1. Euler simple\n");
    printf("2. Heun\n");
    printf("3. Punto medio\n");
    printf("4. RK4 clasico\n");
    printf("Ingrese el metodo: ");
    scanf("%d", &metodo);

    if(metodo < 1 || metodo > 4) {
        printf("Ingrese un metodo entre 1 y 4.\n");
        return 1;
    }

    h = (xf - x0) / n;

    double x[n+1], y1[n+1], y2[n+1];

    // Condiciones iniciales
    x[0] = x0;
    y1[0] = y10;
    y2[0] = y20;

    // Calculo de la solucion
    for(int i = 0; i < n; i++) {

        x[i+1] = x[i] + h;

        switch(metodo) {

            //-------- Euler simple --------
            case 1:
                y1[i+1] = y1[i] + h * f1(x[i], y1[i], y2[i]);
                y2[i+1] = y2[i] + h * f2(x[i], y1[i], y2[i]);
                break;

            //-------- Heun --------
            case 2: {
                double k11 = f1(x[i], y1[i], y2[i]);
                double k12 = f2(x[i], y1[i], y2[i]);

                double yt1 = y1[i] + h * k11;
                double yt2 = y2[i] + h * k12;

                double k21 = f1(x[i+1], yt1, yt2);
                double k22 = f2(x[i+1], yt1, yt2);

                y1[i+1] = y1[i] + (h/2.0) * (k11 + k21);
                y2[i+1] = y2[i] + (h/2.0) * (k12 + k22);
                break;
            }

            //-------- Punto medio --------
            case 3: {
                double k11 = f1(x[i], y1[i], y2[i]);
                double k12 = f2(x[i], y1[i], y2[i]);

                double xm = x[i] + h/2.0;
                double ym1 = y1[i] + (h/2.0) * k11;
                double ym2 = y2[i] + (h/2.0) * k12;

                double k21 = f1(xm, ym1, ym2);
                double k22 = f2(xm, ym1, ym2);

                y1[i+1] = y1[i] + h * k21;
                y2[i+1] = y2[i] + h * k22;
                break;
            }

            //-------- RK4 clasico --------
            case 4: {
                double k11 = f1(x[i], y1[i], y2[i]);
                double k12 = f2(x[i], y1[i], y2[i]);

                double k21 = f1(x[i] + h/2.0, y1[i] + (h/2.0)*k11, y2[i] + (h/2.0)*k12);
                double k22 = f2(x[i] + h/2.0, y1[i] + (h/2.0)*k11, y2[i] + (h/2.0)*k12);

                double k31 = f1(x[i] + h/2.0, y1[i] + (h/2.0)*k21, y2[i] + (h/2.0)*k22);
                double k32 = f2(x[i] + h/2.0, y1[i] + (h/2.0)*k21, y2[i] + (h/2.0)*k22);

                double k41 = f1(x[i] + h, y1[i] + h*k31, y2[i] + h*k32);
                double k42 = f2(x[i] + h, y1[i] + h*k31, y2[i] + h*k32);

                y1[i+1] = y1[i] + (h/6.0) * (k11 + 2*k21 + 2*k31 + k41);
                y2[i+1] = y2[i] + (h/6.0) * (k12 + 2*k22 + 2*k32 + k42);
                break;
            }
        }
    }

    // Ajustar el ultimo punto al limite final
    x[n] = xf;

    // Grabar los puntos en grafico.txt
    FILE *fp = fopen("grafico.txt", "w");

    if(fp == NULL) {
        printf("No se pudo crear grafico.txt\n");
        return 1;
    }

    for(int i = 0; i <= n; i++) {
        fprintf(fp, "%.3f\t%.3f\t%.3f\n", x[i], y1[i], y2[i]);
    }

    fclose(fp);

    printf("\nResultado de la aproximacion:\n");
    printf("x = %.3f\n", x[n]);
    printf("y1 = %.3f\n", y1[n]);
    printf("y2 = %.3f\n", y2[n]);
    printf("Los puntos se guardaron en grafico.txt\n");

    return 0;
}