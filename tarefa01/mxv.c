#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void mxv_por_linhas(double **M, double *x, double *y, int N) {
    for (int i = 0; i < N; i++) {
        double soma = 0.0;
        for (int j = 0; j < N; j++) {
            soma += M[i][j] * x[j];
        }
        y[i] = soma;
    }
}

void mxv_por_colunas(double **M, double *x, double *y, int N) {
    for (int i = 0; i < N; i++) y[i] = 0.0;

    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            y[i] += M[i][j] * x[j];
        }
    }
}

double tempo_execucao(void (*func)(double**, double*, double*, int),
                      double **M, double *x, double *y, int N) {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    func(M, x, y, N);

    gettimeofday(&end, NULL);

    return (end.tv_sec - start.tv_sec) +
           (end.tv_usec - start.tv_usec) / 1e6;
}

int main() {
    int tamanhos[] = {10, 100, 500, 1000, 2000, 3000};
    int num_testes = sizeof(tamanhos) / sizeof(tamanhos[0]);

    for (int t = 0; t < num_testes; t++) {
        int N = tamanhos[t];

        // aloca M1 e M2
        double **M1 = malloc(N * sizeof(double*));
        double **M2 = malloc(N * sizeof(double*));
        for (int i = 0; i < N; i++) {
            M1[i] = malloc(N * sizeof(double));
            M2[i] = malloc(N * sizeof(double));
        }

        double *x = malloc(N * sizeof(double));
        double *y = malloc(N * sizeof(double));

        // inicializa M1 e M2 com os mesmos valores
        for (int i = 0; i < N; i++) {
            x[i] = 1.0;
            for (int j = 0; j < N; j++) {
                double val = (double)(i + j);
                M1[i][j] = val;
                M2[i][j] = val;
            }
        }

        double t_linhas = tempo_execucao(mxv_por_linhas, M1, x, y, N);
        double t_colunas = tempo_execucao(mxv_por_colunas, M2, x, y, N);

        printf("N = %d | Linhas: %.6f s | Colunas: %.6f s\n", N, t_linhas, t_colunas);

        // libera memória
        for (int i = 0; i < N; i++) {
            free(M1[i]);
            free(M2[i]);
        }
        free(M1);
        free(M2);
        free(x);
        free(y);
    }

    return 0;
}