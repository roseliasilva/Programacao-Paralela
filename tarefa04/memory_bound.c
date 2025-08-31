#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Define um tamanho grande para os vetores
#define VECTOR_SIZE 100000000
// Define um "salto" para dificultar o acesso à memória
#define STRIDE 16

int main() {
    // Alocação de memória para os vetores
    double *a = (double*)malloc(VECTOR_SIZE * sizeof(double));
    double *b = (double*)malloc(VECTOR_SIZE * sizeof(double));
    double *c = (double*)malloc(VECTOR_SIZE * sizeof(double));

    if (a == NULL || b == NULL || c == NULL) {
        fprintf(stderr, "Falha na alocação de memória.\n");
        return 1;
    }

    // Inicialização dos vetores
    for (int i = 0; i < VECTOR_SIZE; i++) {
        a[i] = (double)i;
        b[i] = (double)i;
    }

    // Loop principal paralelizado com acesso não sequencial
    #pragma omp parallel for
    for (int i = 0; i < VECTOR_SIZE; i++) {
        // O acesso a b[i] é modificado para criar um padrão de acesso com saltos
        c[i] = a[i] + b[(i + (i / STRIDE)) % VECTOR_SIZE];
    }

    // Liberando a memória alocada
    free(a);
    free(b);
    free(c);

    return 0;
}