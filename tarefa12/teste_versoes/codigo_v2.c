// ===================================================================
// Versão 2 (V2) - Otimização com Cache Blocking (Tiling)
// ===================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

// ==================== MUDANÇA DA V2 ====================
// Define o tamanho da aresta do bloco. Este valor pode ser ajustado (8, 16, 32).
#define BLOCK_SIZE 16
// =======================================================

int N;
const double L = 1.0;
#define NU 0.0005
#define DT 0.00001
#define N_STEPS 500

void *u_data, *v_data, *w_data;
void *u_new_data, *v_new_data, *w_new_data;
double h;

void initialize_fluid() { /* ...código sem alterações... */ }
void apply_perturbation() { /* ...código sem alterações... */ }

void simulation_step() {
    double (*u)[N][N] = u_data;
    double (*v)[N][N] = v_data;
    double (*w)[N][N] = w_data;
    double (*u_new)[N][N] = u_new_data;
    double (*v_new)[N][N] = v_new_data;
    double (*w_new)[N][N] = w_new_data;
    double h2 = h * h;

    // ==================== MUDANÇA DA V2 ====================
    // A paralelização agora acontece nos loops de bloco, não nos loops de elementos.
    #pragma omp parallel for collapse(3) schedule(static)
    for (int i_block = 1; i_block < N - 1; i_block += BLOCK_SIZE) {
        for (int j_block = 1; j_block < N - 1; j_block += BLOCK_SIZE) {
            for (int k_block = 1; k_block < N - 1; k_block += BLOCK_SIZE) {
                
                // Loops internos que iteram dentro de um único bloco.
                for (int i = i_block; i < i_block + BLOCK_SIZE && i < N - 1; ++i) {
                    for (int j = j_block; j < j_block + BLOCK_SIZE && j < N - 1; ++j) {
                        for (int k = k_block; k < k_block + BLOCK_SIZE && k < N - 1; ++k) {
                            double laplacian_u = (u[i+1][j][k] - 2.0*u[i][j][k] + u[i-1][j][k]) / h2 +
                                                 (u[i][j+1][k] - 2.0*u[i][j][k] + u[i][j-1][k]) / h2 +
                                                 (u[i][j][k+1] - 2.0*u[i][j][k] + u[i][j][k-1]) / h2;
                            double laplacian_v = (v[i+1][j][k] - 2.0*v[i][j][k] + v[i-1][j][k]) / h2 +
                                                 (v[i][j+1][k] - 2.0*v[i][j][k] + v[i][j-1][k]) / h2 +
                                                 (v[i][j][k+1] - 2.0*v[i][j][k] + v[i][j][k-1]) / h2;
                            double laplacian_w = (w[i+1][j][k] - 2.0*w[i][j][k] + w[i-1][j][k]) / h2 +
                                                 (w[i][j+1][k] - 2.0*w[i][j][k] + w[i][j-1][k]) / h2 +
                                                 (w[i][j][k+1] - 2.0*w[i][j][k] + w[i][j][k-1]) / h2;
                            u_new[i][j][k] = u[i][j][k] + DT * NU * laplacian_u;
                            v_new[i][j][k] = v[i][j][k] + DT * NU * laplacian_v;
                            w_new[i][j][k] = w[i][j][k] + DT * NU * laplacian_w;
                        }
                    }
                }
            }
        }
    }
    // =======================================================
}

void update_fields() { /* ...código sem alterações... */ }
int main(int argc, char *argv[]) { /* ...código sem alterações, exceto o printf("Iniciando V2...") talvez... */ 
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);
    h = L / (N - 1);

    u_data = malloc(N * sizeof(double[N][N]));
    v_data = malloc(N * sizeof(double[N][N]));
    w_data = malloc(N * sizeof(double[N][N]));
    u_new_data = malloc(N * sizeof(double[N][N]));
    v_new_data = malloc(N * sizeof(double[N][N]));
    w_new_data = malloc(N * sizeof(double[N][N]));

    initialize_fluid();
    apply_perturbation();

    printf("Iniciando V2...\n");
    printf("Grid: %d x %d x %d\n", N, N, N);
    printf("Usando %d threads.\n", omp_get_max_threads());

    double start_time = omp_get_wtime();
    for (int n = 0; n <= N_STEPS; ++n) {
        simulation_step();
        update_fields();
    }
    double end_time = omp_get_wtime();
    double total_time = end_time - start_time;

    printf("Simulação concluída.\n");
    printf("Tempo de execução do loop principal: %f segundos\n", total_time);

    free(u_data); free(v_data); free(w_data);
    free(u_new_data); free(v_new_data); free(w_new_data);

    return 0;
}