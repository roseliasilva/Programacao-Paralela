#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000  // 100 milhões

int main() {
    double *a, *b, *c;
    a = (double*) malloc(N * sizeof(double));
    b = (double*) malloc(N * sizeof(double));
    c = (double*) malloc(N * sizeof(double));

    // Inicializa vetores
    for (long i = 0; i < N; i++) {
        a[i] = 1.0;
        b[i] = 2.0;
    }

    double start = omp_get_wtime();

    // Soma vetores (memory-bound)
    #pragma omp parallel for
    for (long i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }

    double end = omp_get_wtime();

    printf("Tempo total (memory-bound): %f segundos\n", end - start);

    free(a);
    free(b);
    free(c);
    return 0;
}