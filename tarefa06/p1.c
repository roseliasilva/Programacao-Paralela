#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h> // Incluir para gettimeofday

// Para compilar com gcc:
// gcc -o pi_paralelo_gettimeofday pi_paralelo_gettimeofday.c -fopenmp -lm

int main() {
    long long int num_pontos = 100000000; // 100 milhões de pontos
    long long int pontos_no_circulo = 0;
    double pi_estimado;
    
    // Estruturas para armazenar o tempo de início e fim
    struct timeval start, end;

    // Semente para geração de números aleatórios
    srand(time(NULL));

    // Captura o tempo de início
    gettimeofday(&start, NULL);

    #pragma omp parallel for
    for (long long int i = 0; i < num_pontos; i++) {
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            // CONDIÇÃO DE CORRIDA AINDA PRESENTE
            pontos_no_circulo++; 
        }
    }

    // Captura o tempo de fim
    gettimeofday(&end, NULL);

    // Calcula o tempo decorrido em segundos
    double tempo_decorrido = (end.tv_sec - start.tv_sec) + 
                             (end.tv_usec - start.tv_usec) / 1000000.0;

    pi_estimado = 4.0 * pontos_no_circulo / num_pontos;

    printf("Estimativa (incorreta) de pi: %f\n", pi_estimado);
    printf("Tempo de execução (gettimeofday): %f segundos\n", tempo_decorrido);

    return 0;
}