// diffusao_sendrecv.c
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

    // Inicialização do subdomínio local
    for (int i = 0; i < local_n + 2; i++) {
        u[i] = 0.0;
    }
    // Fonte de calor no centro do domínio global
    int global_mid = N / 2;
    if (rank * local_n <= global_mid && (rank + 1) * local_n > global_mid) {
        int local_mid = global_mid - rank * local_n;
        u[local_mid + 1] = 100.0; // +1 por causa da célula fantasma
    }

    double C = K * dt / (dx * dx);
    double start_time, end_time;

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    for (int t = 0; t < STEPS; t++) {
        // Troca de fronteiras com comunicação bloqueante (padrão par/ímpar)
        MPI_Status status;
        int left_neighbor = (rank == 0) ? MPI_PROC_NULL : rank - 1;
        int right_neighbor = (rank == size - 1) ? MPI_PROC_NULL : rank + 1;

        if (rank % 2 == 0) {
            // Par: Envia para a direita, recebe da direita, envia para a esquerda, recebe da esquerda
            MPI_Send(&u[local_n], 1, MPI_DOUBLE, right_neighbor, 0, MPI_COMM_WORLD);
            MPI_Recv(&u[local_n + 1], 1, MPI_DOUBLE, right_neighbor, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&u[1], 1, MPI_DOUBLE, left_neighbor, 0, MPI_COMM_WORLD);
            MPI_Recv(&u[0], 1, MPI_DOUBLE, left_neighbor, 0, MPI_COMM_WORLD, &status);
        } else {
            // Ímpar: Recebe da esquerda, envia para a esquerda, recebe da direita, envia para a direita
            MPI_Recv(&u[0], 1, MPI_DOUBLE, left_neighbor, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&u[1], 1, MPI_DOUBLE, left_neighbor, 0, MPI_COMM_WORLD);
            MPI_Recv(&u[local_n + 1], 1, MPI_DOUBLE, right_neighbor, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&u[local_n], 1, MPI_DOUBLE, right_neighbor, 0, MPI_COMM_WORLD);
        }

        // Computação dos pontos internos (após comunicação)
        for (int i = 1; i <= local_n; i++) {
             // Condição de contorno global (pontas da barra em 0.0)
            if ((rank == 0 && i == 1) || (rank == size - 1 && i == local_n)) {
                u_new[i] = u[i]; // Mantém fixo se for uma ponta, mas na prática é 0.0
            } else {
                u_new[i] = u[i] + C * (u[i+1] - 2 * u[i] + u[i-1]);
            }
        }

        // Atualiza o vetor u
        double *temp = u;
        u = u_new;
        u_new = temp;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Versao Send/Recv: %d processos, Tempo: %f segundos\n", size, end_time - start_time);
    }

    free(u);
    free(u_new);
    MPI_Finalize();
    return 0;
}