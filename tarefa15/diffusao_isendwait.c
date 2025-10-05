// diffusao_isendwait.c
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

        // Espera todas as comunicações terminarem ANTES da computação
        MPI_Waitall(4, requests, statuses);

        // Computação de todos os pontos (após comunicação)
        for (int i = 1; i <= local_n; i++) {
            if ((rank == 0 && i == 1) || (rank == size - 1 && i == local_n)) {
                u_new[i] = u[i];
            } else {
                u_new[i] = u[i] + C * (u[i+1] - 2 * u[i] + u[i-1]);
            }
        }
        
        double *temp = u;
        u = u_new;
        u_new = temp;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Versao Isend/Wait: %d processos, Tempo: %f segundos\n", size, end_time - start_time);
    }

    free(u);
    free(u_new);
    MPI_Finalize();
    return 0;
}