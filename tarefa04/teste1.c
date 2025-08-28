#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h> // <<< HEADER NECESSÁRIO

// Usando o valor reduzido para evitar o erro "Killed"
#define TAMANHO_VETOR 100000000 

int main() {
    double *a, *b, *c;

    // <<< DECLARAÇÃO DAS VARIÁVEIS DE TEMPO
    struct timeval start, end;
    double elapsedTime;

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
    
    // <<< INÍCIO DA MEDIÇÃO
    gettimeofday(&start, NULL);

    #pragma omp parallel for
    for (int i = 0; i < TAMANHO_VETOR; i++) {
        c[i] = a[i] + b[i];
    }

    // <<< FIM DA MEDIÇÃO
    gettimeofday(&end, NULL);

    // <<< CÁLCULO DO TEMPO DECORRIDO
    elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0;   // us to ms
    // Convertendo para segundos para a exibição
    printf("Soma concluída.\n");
    printf("Tempo de execução: %.4f segundos.\n", elapsedTime / 1000.0);
    
    // Verificação simples
    // printf("Resultado de amostra c[100] = %f\n", c[100]);

    free(a);
    free(b);
    free(c);

    return 0;
}