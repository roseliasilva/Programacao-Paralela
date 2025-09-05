#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h> // Para gettimeofday
#include <time.h>     // Para time()

#define NUM_POINTS 100000000 // 100 milhões de pontos
#define NUM_THREADS 8

int main() {
    long long total_hits = 0;
    struct timeval start, end;

    printf("Iniciando Versão 1: #pragma omp critical\n");
    printf("Número de pontos: %d\n", NUM_POINTS);
    printf("Número de threads: %d\n\n", NUM_THREADS);

    // Medição de tempo
    gettimeofday(&start, NULL);

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        // Cada thread terá sua própria contagem de acertos privada
        long long private_hits = 0;
        
        // Semente única para cada thread para garantir sequências aleatórias diferentes
        // Usamos rand_r que é thread-safe
        unsigned int seed = time(NULL) ^ omp_get_thread_num();

        // O laço 'for' é dividido entre as threads
        #pragma omp for
        for (long long i = 0; i < NUM_POINTS; i++) {
            // Gera coordenadas x e y entre 0.0 e 1.0
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            // Verifica se o ponto está dentro do círculo (primeiro quadrante)
            if (x * x + y * y <= 1.0) {
                private_hits++;
            }
        }

        // Seção Crítica: Apenas uma thread pode executar este bloco por vez
        // para atualizar a variável global de forma segura.
        #pragma omp critical
        {
            total_hits += private_hits;
        }
    }

    // Fim da medição de tempo
    gettimeofday(&end, NULL);

    // Cálculo da estimativa de Pi
    double pi_estimate = 4.0 * total_hits / NUM_POINTS;

    // Cálculo do tempo de execução em segundos
    double execution_time = (end.tv_sec - start.tv_sec) + 
                            (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Estimativa de Pi: %f\n", pi_estimate);
    printf("Tempo de execução (critical): %f segundos\n", execution_time);

    return 0;
}