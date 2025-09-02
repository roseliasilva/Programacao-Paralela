#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

// Função para verificar se um número é primo
int eh_primo(int num) {
    if (num < 2) return 0;
    if (num == 2) return 1;
    if (num % 2 == 0) return 0;
    int limite = (int) sqrt((double) num);
    for (int i = 3; i <= limite; i += 2) {
        if (num % i == 0) return 0;
    }
    return 1;
}

double tempo_execucao(struct timeval inicio, struct timeval fim) {
    return (fim.tv_sec - inicio.tv_sec) +
           (fim.tv_usec - inicio.tv_usec) / 1000000.0;
}

int main() {
    long n = 10000000;
    int count_seq = 0, count_par = 0;

    struct timeval inicio, fim;

    //Versão Sequencial
    gettimeofday(&inicio, NULL);

    for (long i = 2; i <= n; i++) {
        if (eh_primo(i)) {
            count_seq++;
        }
    }

    gettimeofday(&fim, NULL);
    double tempo_seq = tempo_execucao(inicio, fim);

    printf("Sequencial: Encontrados %d primos até %ld\n", count_seq, n);
    printf("Tempo sequencial: %f segundos\n\n", tempo_seq);
    
    // Versão Paralela
    gettimeofday(&inicio, NULL);

    #pragma omp parallel for
    for (long i = 2; i <= n; i++) {
        if (eh_primo(i)) {
            count_par++;
        }
    }

    gettimeofday(&fim, NULL);
    double tempo_par = tempo_execucao(inicio, fim);

    printf("Paralelo:   Encontrados %d primos até %ld\n", count_par, n);
    printf("Tempo paralelo: %f segundos\n", tempo_par);

    return 0;
}