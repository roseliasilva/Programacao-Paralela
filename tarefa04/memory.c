#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Define um tamanho grande para os vetores para forçar o uso da RAM
// e não caber apenas nos caches da CPU.
#define TAMANHO_VETOR 100000000 // 100 milhões de elementos

int main() {
    // Usamos double para aumentar o consumo de memória (8 bytes por elemento)
    double *a, *b, *c;

    // Alocação de memória para os vetores
    a = (double*) malloc(sizeof(double) * TAMANHO_VETOR);
    b = (double*) malloc(sizeof(double) * TAMANHO_VETOR);
    c = (double*) malloc(sizeof(double) * TAMANHO_VETOR);

    if (a == NULL || b == NULL || c == NULL) {
        fprintf(stderr, "Erro na alocação de memória.\n");
        return 1;
    }

    // Inicializa os vetores a e b
    for (int i = 0; i < TAMANHO_VETOR; i++) {
        a[i] = 1.0;
        b[i] = 2.0;
    }

    printf("Iniciando a soma dos vetores (Memory-Bound)...\n");

    // ----- SEÇÃO PARALELIZADA -----
    // O loop é dividido entre as threads. Cada thread executa uma parte das somas.
    // O gargalo aqui é a largura de banda da memória.
    #pragma omp parallel for
    for (int i = 0; i < TAMANHO_VETOR; i++) {
        c[i] = a[i] + b[i];
    }
    // -------------------------------

    printf("Soma concluída.\n");
    // Verificação simples para garantir que o compilador não otimize o loop fora
    printf("Resultado de amostra c[100] = %f\n", c[100]);

    // Libera a memória alocada
    free(a);
    free(b);
    free(c);

    return 0;
}