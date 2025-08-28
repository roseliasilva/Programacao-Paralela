#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h> // Necessário para sin, cos, sqrt

// Número de iterações para manter a CPU ocupada.
#define NUM_ITERATIONS 100000000 // 500 milhões de iterações

int main() {
    double soma_total = 0.0;

    printf("Iniciando cálculo matemático intensivo (Compute-Bound)...\n");

    #pragma omp parallel for
    for (long long i = 0; i < NUM_ITERATIONS; i++) {
        // Operações matemáticas complexas para sobrecarregar a CPU
        soma_total += sin(i * 0.001) * cos(i * 0.001) + sqrt((double)i);
    }
    // -------------------------------

    printf("Cálculo concluído.\n");
    printf("Resultado final: %f\n", soma_total);

    return 0;
}