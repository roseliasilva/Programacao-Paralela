#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

// Total de pontos a serem gerados
#define NUM_PONTOS 10000000

// Função para calcular o tempo decorrido em segundos
double get_time_sec(struct timeval t_start, struct timeval t_end) {
    return (t_end.tv_sec - t_start.tv_sec) + (t_end.tv_usec - t_start.tv_usec) / 1000000.0;
}

// Versão 1: rand() com #pragma omp critical
void estimar_pi_v1_critical_rand() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int total_acertos = 0;

    #pragma omp parallel
    {
        int acertos_privado = 0;
        
        #pragma omp for
        for (int i = 0; i < NUM_PONTOS; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                acertos_privado++;
            }
        }
        #pragma omp critical
        total_acertos += acertos_privado;
    }
    
    double pi_estimado = 4.0 * total_acertos / NUM_PONTOS;
    gettimeofday(&end, NULL);
    printf("Versão 1 - critical + rand():     π ≈ %f | Time: %f segundos\n", pi_estimado, get_time_sec(start, end));
}

// Versão 2: rand() com vetor compartilhado
void estimar_pi_v2_vetor_rand() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int total_acertos = 0;
    int num_threads = 0;
    int *acertos_vetor = NULL;

    #pragma omp parallel
    {
        #pragma omp single
        {
            num_threads = omp_get_num_threads();
            acertos_vetor = calloc(num_threads, sizeof(int));
        }
        int thread_id = omp_get_thread_num(); //cada thread descobre seu ID
        
        #pragma omp for
        for (int i = 0; i < NUM_PONTOS; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                acertos_vetor[thread_id]++;
            }
        }
    }
    
    for (int i = 0; i < num_threads; i++) {
        total_acertos += acertos_vetor[i];
    }
    free(acertos_vetor);
    double pi_estimado = 4.0 * total_acertos / NUM_PONTOS;
    gettimeofday(&end, NULL);
    printf("Versão 2 - array + rand():        π ≈ %f | Time: %f segundos\n", pi_estimado, get_time_sec(start, end));
}

// Versão 3: rand_r() com #pragma omp critical
void estimar_pi_v3_critical_rand_r() {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int total_acertos = 0;

    #pragma omp parallel
    {
        int acertos_privado = 0;
        // Cada thread precisa de sua própria semente para rand_r
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
    printf("Versão 3 - critical + rand_r():   π ≈ %f | Time: %f segundos\n", pi_estimado, get_time_sec(start, end));
}

// Versão 4: rand_r() com vetor compartilhado
void estimar_pi_v4_vetor_rand_r() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int total_acertos = 0;
    int num_threads = 0;
    int *acertos_vetor = NULL;

    #pragma omp parallel
    {
        #pragma omp single
        {
            num_threads = omp_get_num_threads();
            acertos_vetor = calloc(num_threads, sizeof(int));
        }
        //cada thread descobre seu ID
        int thread_id = omp_get_thread_num();
        unsigned int seed = time(NULL) ^ thread_id;
        
        #pragma omp for
        for (int i = 0; i < NUM_PONTOS; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                acertos_vetor[thread_id]++;
            }
        }
    }

    for (int i = 0; i < num_threads; i++) {
        total_acertos += acertos_vetor[i];
    }
    free(acertos_vetor);
    double pi_estimado = 4.0 * total_acertos / NUM_PONTOS;
    gettimeofday(&end, NULL);
    printf("Versão 4 - array + rand_r():      π ≈ %f | Time: %f segundos\n", pi_estimado, get_time_sec(start, end));
}

int main() {
    estimar_pi_v1_critical_rand();
    estimar_pi_v2_vetor_rand();
    estimar_pi_v3_critical_rand_r();
    sleep(5); // para evitar que as sementes sejam iguais
    estimar_pi_v4_vetor_rand_r();

    return 0;
}