#include <stdio.h>
#include <math.h>
#include <omp.h>

#define N 50000000  // 50 milhões (ajuste se ficar muito lento)

int main() {
    double *a, result = 0.0;
    a = (double*) malloc(N * sizeof(double));

    // Inicializa vetor
    for (long i = 0; i < N; i++) {
        a[i] = (double)i / N;
    }

    double start = omp_get_wtime();

    // Cálculo pesado (compute-bound)
    #pragma omp parallel for reduction(+:result)
    for (long i = 0; i < N; i++) {
        result += sin(a[i]) * cos(a[i]) + sqrt(a[i]);
    }

    double end = omp_get_wtime();

    printf("Resultado: %f\n", result);
    printf("Tempo total (compute-bound): %f segundos\n", end - start);

    free(a);
    return 0;
}