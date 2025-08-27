#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
    long long iteracoes[] = {1000, 10000, 100000, 1000000, 10000000, 100000000};
    int num_testes = sizeof(iteracoes) / sizeof(iteracoes[0]);

    printf("PI = 3.14159265359\n");

    for (int t = 0; t < num_testes; t++) {
        long long n = iteracoes[t];
        double pi = 0.0;
        int sinal = 1;

        struct timeval inicio, fim;
        gettimeofday(&inicio, NULL);

        for (long long k = 0; k < n; k++) {
            pi += sinal * (1.0 / (2 * k + 1));
            sinal = -sinal;
        }

        pi *= 4.0;

        gettimeofday(&fim, NULL);

        double tempo = (fim.tv_sec - inicio.tv_sec) +
                       (fim.tv_usec - inicio.tv_usec) / 1e6;

        printf("Iterações: %lld | Aproximação: %.15f | Tempo: %.6f s\n",
               n, pi, tempo);
    }

    return 0;
}