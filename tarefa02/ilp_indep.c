// ilp_indep.c
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

    // 3) Soma quebrando dependência
    double soma1 = 0.0, soma2 = 0.0, soma3 = 0.0, soma4 = 0.0, soma5 = 0.0, soma6 = 0.0, soma7 = 0.0, soma8 = 0.0;
    for (long i = 0; i < N; i += 4) {
        soma1 += vetor[i];
        soma2 += vetor[i+1];
        soma3 += vetor[i+2];
        soma4 += vetor[i+3];
        soma5 += vetor[i+4];
        soma6 += vetor[i+5];
        soma7 += vetor[i+6];
        soma8 += vetor[i+7];
    }
    double soma_final = soma1 + soma2 + soma3 + soma4 + soma5 + soma6 + soma7 + soma8;

    printf("Soma quebrando dependência = %.2f\n", soma_final);

    free(vetor);
    return 0;
}