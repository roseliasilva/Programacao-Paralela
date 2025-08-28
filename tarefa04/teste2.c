#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h> // <<< HEADER NECESSÁRIO

#define NUM_ITERATIONS 100000000

int main() {
    double soma_total = 0.0;
    
    // <<< DECLARAÇÃO DAS VARIÁVEIS DE TEMPO
    struct timeval start, end;
    double elapsedTime;

    printf("Iniciando cálculo matemático intensivo (Compute-Bound)...\n");

    // <<< INÍCIO DA MEDIÇÃO
    gettimeofday(&start, NULL);

    #pragma omp parallel for
    for (long long i = 0; i < NUM_ITERATIONS; i++) {
        soma_total += sin(i * 0.001) * cos(i * 0.001) + sqrt((double)i);
    }
    
    // <<< FIM DA MEDIÇÃO
    gettimeofday(&end, NULL);

    // <<< CÁLCULO DO TEMPO DECORRIDO
    elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0;   // us to ms
    
    printf("Cálculo concluído.\n");
    printf("Resultado final: %f\n", soma_total);
    printf("Tempo de execução: %.4f segundos.\n", elapsedTime / 1000.0);

    return 0;
}