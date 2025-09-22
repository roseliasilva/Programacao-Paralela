#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

// Total de pontos a serem gerados
#define NUM_PONTOS 100000000

// Função para calcular o tempo decorrido em segundos
double get_time_sec(struct timeval t_start, struct timeval t_end) {
    return (t_end.tv_sec - t_start.tv_sec) + (t_end.tv_usec - t_start.tv_usec) / 1000000.0;
}

// Versão 1: Contador Compartilhado + rand_r + critical
void versao1_compartilhado_critical() {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int total_acertos = 0;

    #pragma omp parallel
    {
        unsigned int seed = time(NULL) ^ omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < NUM_PONTOS; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                #pragma omp critical
                total_acertos++;
            }
        }
    }

    double pi_estimado = 4.0 * total_acertos / NUM_PONTOS;
    gettimeofday(&end, NULL);
    printf("1. Comp. Critical: π ≈ %f | Time: %f s\n", pi_estimado, get_time_sec(start, end));
}

// Versão 2: Contador Compartilhado + rand_r + atomic
void versao2_compartilhado_atomic() {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int total_acertos = 0;

    #pragma omp parallel
    {
        unsigned int seed = time(NULL) ^ omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < NUM_PONTOS; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                #pragma omp atomic
                total_acertos++;
            }
        }
    }

    double pi_estimado = 4.0 * total_acertos / NUM_PONTOS;
    gettimeofday(&end, NULL);
    printf("2. Comp. Atomic:   π ≈ %f | Time: %f s\n", pi_estimado, get_time_sec(start, end));
}

// Versão 3: Contador Privado + rand_r + critical
void versao3_privado_critical() {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int total_acertos = 0;

    #pragma omp parallel
    {
        int acertos_privado = 0;
        unsigned int seed = time(NULL) ^ omp_get_thread_num();
        
        #pragma omp for
        for (int i = 0; i < NUM_PONTOS; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                acertos_privado++;
            }
        }
        
        #pragma omp critical
        total_acertos += acertos_privado;
    }
    double pi_estimado = 4.0 * total_acertos / NUM_PONTOS;
    gettimeofday(&end, NULL);
    printf("3. Priv. Critical: π ≈ %f | Time: %f s\n", pi_estimado, get_time_sec(start, end));
}

// Versão 4: Contador Privado + rand_r + atomic
void versao4_privado_atomic() {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int total_acertos = 0;

    #pragma omp parallel
    {
        int acertos_privado = 0;
        unsigned int seed = time(NULL) ^ omp_get_thread_num();
        
        #pragma omp for
        for (int i = 0; i < NUM_PONTOS; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                acertos_privado++;
            }
        }
        
        #pragma omp atomic
        total_acertos += acertos_privado;
    }
    double pi_estimado = 4.0 * total_acertos / NUM_PONTOS;
    gettimeofday(&end, NULL);
    printf("4. Priv. Atomic:   π ≈ %f | Time: %f s\n", pi_estimado, get_time_sec(start, end));
}

// Versão 5: Reduction
void versao5_reduction() {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int count = 0;
    
    #pragma omp parallel reduction(+:count)
    {
        unsigned int seed = time(NULL) ^ omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < NUM_PONTOS; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                count++;
            }
        }
    }

    double pi_estimado = 4.0 * count / NUM_PONTOS;
    gettimeofday(&end, NULL);
    printf("5. Reduction:      π ≈ %f | Time: %f s\n", pi_estimado, get_time_sec(start, end));
}

int main() {
    printf("Comparando 5 versões com %d pontos.\n\n", NUM_PONTOS);
    
    versao1_compartilhado_critical();
    versao2_compartilhado_atomic();
    versao3_privado_critical();
    sleep(5);
    versao4_privado_atomic();
    sleep(5);
    versao5_reduction();

    printf("\nFim da execução.\n");

    return 0;
}