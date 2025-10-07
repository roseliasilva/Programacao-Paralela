#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/*
 ============================================================================
 Código MPI para multiplicação Matriz-Vetor (y = A * x)
 
 Compilação:
   mpicc -O3 -o matvec_mpi matvec_mpi.c
 
 Execução:
   mpirun -np <num_procs> ./matvec_mpi <linhas_M> <colunas_N>
 ============================================================================
*/

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (argc != 3) {
        if (world_rank == 0) {
            fprintf(stderr, "Uso: mpirun -np <num_procs> %s <M_linhas> <N_colunas>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);

    // O número de linhas M deve ser divisível pelo número de processos para este algoritmo simples
    if (M % world_size != 0) {
        if (world_rank == 0) {
            fprintf(stderr, "Erro: O número de linhas (%d) deve ser divisível pelo número de processos (%d).\n", M, world_size);
        }
        MPI_Finalize();
        return 1;
    }

    double *A = NULL; // Matriz completa A (só no processo 0)
    double *x = malloc(N * sizeof(double)); // Vetor x (em todos os processos)
    double *y = NULL; // Vetor resultado y (só no processo 0)

    // O processo 0 (raiz) inicializa a matriz A e o vetor x
    if (world_rank == 0) {
        A = malloc(M * N * sizeof(double));
        y = malloc(M * sizeof(double));
        // Inicializa A e x com valores de exemplo (ex: 1.0 e 2.0)
        for (int i = 0; i < M * N; i++) A[i] = 1.0;
        for (int i = 0; i < N; i++) x[i] = 2.0;
    }

    // Sincroniza todos os processos antes de iniciar o cronômetro
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // 1. BROADCAST: O processo 0 envia o vetor x para todos os outros processos.
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Cada processo precisa de um buffer para receber sua parte da matriz A
    int rows_per_proc = M / world_size;
    double *sub_A = malloc(rows_per_proc * N * sizeof(double));

    // 2. SCATTER: O processo 0 distribui as linhas de A entre todos os processos.
    MPI_Scatter(A, rows_per_proc * N, MPI_DOUBLE,
                sub_A, rows_per_proc * N, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // Após o scatter, o processo 0 não precisa mais da matriz completa A
    if (world_rank == 0) {
        free(A);
    }

    // 3. CÁLCULO LOCAL: Cada processo calcula sua parte do resultado (sub_y)
    double *sub_y = malloc(rows_per_proc * sizeof(double));
    for (int i = 0; i < rows_per_proc; i++) {
        sub_y[i] = 0.0;
        for (int j = 0; j < N; j++) {
            sub_y[i] += sub_A[i * N + j] * x[j];
        }
    }

    // 4. GATHER: Todos os processos enviam seus resultados parciais (sub_y) para o processo 0,
    // que os junta no vetor final y.
    MPI_Gather(sub_y, rows_per_proc, MPI_DOUBLE,
               y, rows_per_proc, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    // Sincroniza novamente para garantir que a medição de tempo seja precisa
    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    // O processo 0 imprime o resultado final em formato CSV (M,N,Processos,Tempo)
    if (world_rank == 0) {
        printf("%d,%d,%d,%f\n", M, N, world_size, end_time - start_time);
        free(y);
    }

    // Libera a memória alocada por cada processo
    free(x);
    free(sub_A);
    free(sub_y);

    MPI_Finalize();
    return 0;
}