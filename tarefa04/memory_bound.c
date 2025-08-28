#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Tamanho grande para garantir que o vetor não caiba na cache da CPU.
// ~4GB de dados (500M * 8 bytes por long long)
#define TAMANHO 100000000

// Um número primo grande para garantir saltos "pseudo-aleatórios" no vetor
#define SALTO 19997

int main() {
    // Aloca o vetor na heap. Usamos long long para ocupar mais espaço.
    long long* vetor = (long long*) malloc(TAMANHO * sizeof(long long));
    if (vetor == NULL) {
        perror("Falha ao alocar memória");
        return 1;
    }

    // Inicializa o vetor com alguns valores
    // Este laço também pode ser paralelizado para acelerar a inicialização.
    for (long long i = 0; i < TAMANHO; i++) {
        vetor[i] = i % 100; // Valores simples de 0 a 99
    }

    long long soma_total = 0;

    // --- Início da seção crítica para medição de tempo ---
    // O pragma abaixo paraleliza o laço 'for'. A cláusula 'reduction' garante
    // que a 'soma_total' de cada thread seja somada corretamente no final.
    #pragma omp parallel for
    for (long long i = 0; i < TAMANHO; i++) {
        // O acesso com (i * SALTO) % TAMANHO força a busca em posições
        // de memória muito distantes uma da outra. Isso causa muitos "cache misses",
        // tornando o gargalo a comunicação com a memória RAM.
        soma_total += vetor[(i * SALTO) % TAMANHO];
    }
    // --- Fim da seção crítica ---

    printf("Soma total (para evitar otimização do compilador): %lld\n", soma_total);

    // Libera a memória alocada
    free(vetor);

    return 0;
}