#include <iostream>
#include <vector>
#include <iomanip>

// Función genérica: recibe los 4 vectores de la matriz tridiagonal
std::vector<double> resolverThomas(int N,
                                   const std::vector<double>& a, // Subdiagonal (tamaño N-1)
                                   const std::vector<double>& b, // Diagonal principal (tamaño N)
                                   const std::vector<double>& c, // Superdiagonal (tamaño N-1)
                                   const std::vector<double>& d) // Lado derecho / término independiente (tamaño N)
{
    std::vector<double> c_star(N - 1);
    std::vector<double> d_star(N);
    std::vector<double> x(N);

    // Eliminación hacia adelante
    c_star[0] = c[0] / b[0];
    d_star[0] = d[0] / b[0];

    for (int i = 1; i < N; ++i) {
        double denom = b[i] - a[i - 1] * c_star[i - 1];
        if (i < N - 1) {
            c_star[i] = c[i] / denom;
        }
        d_star[i] = (d[i] - a[i - 1] * d_star[i - 1]) / denom;
    }

    // Sustitución hacia atrás
    x[N - 1] = d_star[N - 1];
    for (int i = N - 2; i >= 0; --i) {
        x[i] = d_star[i] - c_star[i] * x[i + 1];
    }

    return x;
}

int main() {
    // =========================================================
    // 1. TAMAÑO DE LA MATRIZ
    // =========================================================
    int N = 100; // <-- CAMBIA ESTE NÚMERO para cualquier dimensión (ej: 500, 1000)

    // =========================================================
    // 2. DECLARACIÓN DE LOS VECTORES DE LA MATRIZ
    // =========================================================
    std::vector<double> a(N - 1); // Subdiagonal (debajo de la principal)
    std::vector<double> b(N);     // Diagonal principal
    std::vector<double> c(N - 1); // Superdiagonal (encima de la principal)
    std::vector<double> d(N);     // Vector de términos independientes

    // =========================================================
    // 3. CARGA DE DATOS (AQUÍ DEFINES LOS VALORES)
    // =========================================================
    
    // Opción A: Si la matriz sigue un patrón o ecuación
    for (int i = 0; i < N; ++i) {
        b[i] = 2.0; // Valor de la diagonal principal
        d[i] = 6.0; // Valor por defecto del lado derecho
        
        if (i < N - 1) {
            a[i] = 1.0; // Valor de la subdiagonal
            c[i] = 1.0; // Valor de la superdiagonal
        }
    }

    // Ajustes específicos (como las esquinas en este problema)
    d[0] = 4.5;
    d[N - 1] = 4.5;

    /* 
    // Opción B: Si los valores NO son uniformes, puedes asignarlos manualmente uno a uno:
    b[0] = 2.0; b[1] = 3.5; b[2] = 4.0; ...
    a[0] = 1.0; a[1] = 0.5; ...
    c[0] = 1.0; c[1] = 2.0; ...
    d[0] = 4.5; d[1] = 8.1; ...
    */

    // =========================================================
    // 4. RESOLVER EL SISTEMA
    // =========================================================
    std::vector<double> x = resolverThomas(N, a, b, c, d);

    // Muestra los primeros resultados
    std::cout << "x[1] = " << x[0] << "\n";
    std::cout << "x[2] = " << x[1] << "\n";

    return 0;
}