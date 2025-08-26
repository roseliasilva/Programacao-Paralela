// ilp_dep.c
#include <stdio.h>
#include <stdlib.h>

#define N 100000000

int main() {
    double *vetor = (double*) malloc(N * sizeof(double));
    if (vetor == NULL) {
        printf("Erro ao alocar memória\n");
        return 1;
    }

    // 1) Inicialização (independente)
    for (long i = 0; i < N; i++) {
        vetor[i] = i * 0.5;
    }

    // 2) Soma acumulativa (dependente)
    double soma = 0.0;
    for (long i = 0; i < N; i++) {
        soma += vetor[i];
    }

    printf("Soma acumulativa (dependente) = %.2f\n", soma);

    free(vetor);
    return 0;
}