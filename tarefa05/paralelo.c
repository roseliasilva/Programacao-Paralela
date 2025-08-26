#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>   // para usar gettimeofday

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

int main() {
    long n = 10000000;  // valor fixo

    int count = 0;

    // Exemplo de uso de gettimeofday (comentado):
    struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);

    #pragma omp parallel for
    for (long i = 2; i <= n; i++) {
        if (eh_primo(i)) {
            count++;
        }
    }

    gettimeofday(&fim, NULL);
    double tempo = (fim.tv_sec - inicio.tv_sec) + 
                   (fim.tv_usec - inicio.tv_usec) / 1000000.0;
    printf("Tempo paralelo: %f segundos\n", tempo);

    printf("Paralelo: Encontrados %d primos até %ld\n", count, n);

    return 0;
}