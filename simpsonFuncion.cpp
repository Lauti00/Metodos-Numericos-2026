#include <stdio.h>
#include <math.h>

// Función que se va a integrar
double f(double x) {
    double f =exp(2*sin(x)) * (1+ log(x));
    return f;
}

int main() {
    double a, b, h, x, suma, I, error;
    int n, i;

    printf("Ingrese el numero de subintervalos (n): ");
    scanf("%d", &n);
    if(n <= 0 || n % 2 != 0) {
        printf("El numero de subintervalos debe ser un entero positivo par.\n");
        return 1;
    }
    printf("Ingrese el limite inferior (a): ");
    scanf("%lf", &a);
    printf("Ingrese el limite superior (b): ");
    scanf("%lf", &b);

    // Algoritmo de Integración
    h = (b - a) / n;
    suma = f(a) + f(b);

    for (i = 1; i <= (n/2) - 1; i++) {
        x = a + (2 * i * h);
        suma = suma + 2 * f(x) + 4 * f(x - h);
    }
    suma = suma + 4 * f(b - h);
    I = (h / 3.0) * suma;

    printf("\nEl resultado de la integral es: %f\n", I);

    return 0;
}