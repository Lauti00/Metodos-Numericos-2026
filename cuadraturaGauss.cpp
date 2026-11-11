#include <stdio.h>
#include <math.h>

// Funcion que se va a integrar
double f(double x) {
    return 2-0.5*(x-1) - 2.25*pow(x-1,2)+0.75*pow(x-1,3); ;
}

int main() {

    double a, b, I;
    int puntos;

    printf("Ingrese el limite inferior (a): ");
    scanf("%lf", &a);
    printf("Ingrese el limite superior (b): ");
    scanf("%lf", &b);

    printf("Ingrese la cantidad de puntos a utilizar (2 a 6): ");
    scanf("%d", &puntos);
    switch(puntos) {

        case 2: {
            double c0 = 1.00000000;
            double c1 = 1.00000000;

            double x0 = -0.577350269;
            double x1 =  0.577350269;

            I = (b - a) / 2.0 * (
                    c0 * f(((b - a) * x0 + (b + a)) / 2.0) +
                    c1 * f(((b - a) * x1 + (b + a)) / 2.0)
                );

            break;
        }

        case 3: {
            double c0 = 0.55555556;
            double c1 = 0.88888889;
            double c2 = 0.55555556;

            double x0 = -0.774596669;
            double x1 =  0.000000000;
            double x2 =  0.774596669;

            I = (b - a) / 2.0 * (
                    c0 * f(((b - a) * x0 + (b + a)) / 2.0) +
                    c1 * f(((b - a) * x1 + (b + a)) / 2.0) +
                    c2 * f(((b - a) * x2 + (b + a)) / 2.0)
                );

            break;
        }

        case 4: {
            double c0 = 0.3478548;
            double c1 = 0.6521452;
            double c2 = 0.6521452;
            double c3 = 0.3478548;

            double x0 = -0.861136312;
            double x1 = -0.339981044;
            double x2 =  0.339981044;
            double x3 =  0.861136312;

            I = (b - a) / 2.0 * (
                    c0 * f(((b - a) * x0 + (b + a)) / 2.0) +
                    c1 * f(((b - a) * x1 + (b + a)) / 2.0) +
                    c2 * f(((b - a) * x2 + (b + a)) / 2.0) +
                    c3 * f(((b - a) * x3 + (b + a)) / 2.0)
                );

            break;
        }

        case 5: {
            double c0 = 0.2369269;
            double c1 = 0.4786287;
            double c2 = 0.5688889;
            double c3 = 0.4786287;
            double c4 = 0.2369269;

            double x0 = -0.906179846;
            double x1 = -0.538469310;
            double x2 =  0.000000000;
            double x3 =  0.538469310;
            double x4 =  0.906179846;

            I = (b - a) / 2.0 * (
                    c0 * f(((b - a) * x0 + (b + a)) / 2.0) +
                    c1 * f(((b - a) * x1 + (b + a)) / 2.0) +
                    c2 * f(((b - a) * x2 + (b + a)) / 2.0) +
                    c3 * f(((b - a) * x3 + (b + a)) / 2.0) +
                    c4 * f(((b - a) * x4 + (b + a)) / 2.0)
                );

            break;
        }

        case 6: {
            double c0 = 0.1713245;
            double c1 = 0.3607616;
            double c2 = 0.4679139;
            double c3 = 0.4679139;
            double c4 = 0.3607616;
            double c5 = 0.1713245;

            double x0 = -0.932469514;
            double x1 = -0.661209386;
            double x2 = -0.238619186;
            double x3 =  0.238619186;
            double x4 =  0.661209386;
            double x5 =  0.932469514;

            I = (b - a) / 2.0 * (
                    c0 * f(((b - a) * x0 + (b + a)) / 2.0) +
                    c1 * f(((b - a) * x1 + (b + a)) / 2.0) +
                    c2 * f(((b - a) * x2 + (b + a)) / 2.0) +
                    c3 * f(((b - a) * x3 + (b + a)) / 2.0) +
                    c4 * f(((b - a) * x4 + (b + a)) / 2.0) +
                    c5 * f(((b - a) * x5 + (b + a)) / 2.0)
                );

            break;
        }

        default:
            printf("IDIOTA, la cantidad de puntos debe estar entre 2 y 6.\n");
            return 1;
    }

    printf("\nEl resultado de la integral es: %f\n", I);

    return 0;
}