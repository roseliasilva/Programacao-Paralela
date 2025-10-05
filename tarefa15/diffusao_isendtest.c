// diffusao_isendtest.c
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000000
#define STEPS 500
#define K 0.5
#define dt 0.1
#define dx 0.1

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = N / size;
    double *u = (double *)malloc((local_n + 2) * sizeof(double));
    double *u_new = (double *)malloc((local_n + 2) * sizeof(double));

    for (int i = 0; i < local_n + 2; i++) u[i] = 0.0;
    int global_mid = N / 2;
    if (rank * local_n <= global_mid && (rank + 1) * local_n > global_mid) {
        u[global_mid - rank * local_n + 1] = 100.0;
    }

    double C = K * dt / (dx * dx);
    double start_time, end_time;

    MPI_Request requests[4];
    MPI_Status statuses[4];
    int left_neighbor = (rank == 0) ? MPI_PROC_NULL : rank - 1;
    int right_neighbor = (rank == size - 1) ? MPI_PROC_NULL : rank + 1;

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    for (int t = 0; t < STEPS; t++) {
        // Inicia todas as comunicações não-bloqueantes
        MPI_Irecv(&u[0], 1, MPI_DOUBLE, left_neighbor, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(&u[local_n + 1], 1, MPI_DOUBLE, right_neighbor, 0, MPI_COMM_WORLD, &requests[1]);
        MPI_Isend(&u[1], 1, MPI_DOUBLE, left_neighbor, 0, MPI_COMM_WORLD, &requests[2]);
        MPI_Isend(&u[local_n], 1, MPI_DOUBLE, right_neighbor, 0, MPI_COMM_WORLD, &requests[3]);

        // SOBREPOSIÇÃO: Calcula os pontos internos enquanto a comunicação está em andamento
        // Loop vai de 2 a local_n-1, excluindo as bordas que dependem da comunicação
        for (int i = 2; i < local_n; i++) {
             u_new[i] = u[i] + C * (u[i+1] - 2 * u[i] + u[i-1]);
        }

        // Espera a comunicação terminar usando MPI_Test
        int flag_recv_left = 0, flag_recv_right = 0;
        if (left_neighbor != MPI_PROC_NULL) { // Só testa se realmente há um vizinho
            while (!flag_recv_left) {
                MPI_Test(&requests[0], &flag_recv_left, MPI_STATUS_IGNORE);
                // Em um cenário real, poderia fazer outro trabalho aqui
            }
        }
        if (right_neighbor != MPI_PROC_NULL) { // Só testa se realmente há um vizinho
            while (!flag_recv_right) {
                MPI_Test(&requests[1], &flag_recv_right, MPI_STATUS_IGNORE);
            }
        }

        // Agora que os dados chegaram, calcula os pontos de borda
        // Ponto da borda esquerda
        if ((rank == 0 && 1 == 1) == 0) // Nao eh a ponta global
            u_new[1] = u[1] + C * (u[2] - 2 * u[1] + u[0]);
        
        // Ponto da borda direita
        if (local_n > 1 && (rank == size - 1 && local_n == local_n) == 0) // Nao eh a ponta global e nao eh um dominio de 1 ponto
            u_new[local_n] = u[local_n] + C * (u[local_n+1] - 2 * u[local_n] + u[local_n-1]);
        
        // Espera os envios terminarem (boa prática para evitar problemas no próximo passo)
        MPI_Waitall(4, requests, statuses);

        double *temp = u;
        u = u_new;
        u_new = temp;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Versao Isend/Test: %d processos, Tempo: %f segundos\n", size, end_time - start_time);
    }

    free(u);
    free(u_new);
    MPI_Finalize();
    return 0;
}