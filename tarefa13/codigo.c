#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

// N será a dimensao da grade cubica, lido em tempo de execucao
int N;
const double L = 1.0;

// paramentros da simulacao
#define NU 0.0005
#define DT 0.00001
#define N_STEPS 500

void *u_data, *v_data, *w_data;
void *u_new_data, *v_new_data, *w_new_data;

double h;

void initialize_fluid() {
    double (*u)[N][N] = u_data;
    double (*v)[N][N] = v_data;
    double (*w)[N][N] = w_data;

    memset(u, 0, N * N * N * sizeof(double));
    memset(v, 0, N * N * N * sizeof(double));
    memset(w, 0, N * N * N * sizeof(double));
}

void apply_perturbation() {
    double (*u)[N][N] = u_data;
    double (*v)[N][N] = v_data;
    double (*w)[N][N] = w_data;
    
    int mid = N / 2;
    u[mid][mid][mid] = 2.0;
    v[mid][mid][mid] = 2.0;
    w[mid][mid][mid] = 2.0;
}

void simulation_step() {
    double (*u)[N][N] = u_data;
    double (*v)[N][N] = v_data;
    double (*w)[N][N] = w_data;
    double (*u_new)[N][N] = u_new_data;
    double (*v_new)[N][N] = v_new_data;
    double (*w_new)[N][N] = w_new_data;

    double h2 = h * h;

    #pragma omp parallel for collapse(3) schedule(static)
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 1; j < N - 1; ++j) {
            for (int k = 1; k < N - 1; ++k) {
                double laplacian_u = (u[i+1][j][k] - 2.0 * u[i][j][k] + u[i-1][j][k]) / h2 +
                                     (u[i][j+1][k] - 2.0 * u[i][j][k] + u[i][j-1][k]) / h2 +
                                     (u[i][j][k+1] - 2.0 * u[i][j][k] + u[i][j][k-1]) / h2;

                double laplacian_v = (v[i+1][j][k] - 2.0 * v[i][j][k] + v[i-1][j][k]) / h2 +
                                     (v[i][j+1][k] - 2.0 * v[i][j][k] + v[i][j-1][k]) / h2 +
                                     (v[i][j][k+1] - 2.0 * v[i][j][k] + v[i][j][k-1]) / h2;

                double laplacian_w = (w[i+1][j][k] - 2.0 * w[i][j][k] + w[i-1][j][k]) / h2 +
                                     (w[i][j+1][k] - 2.0 * w[i][j][k] + w[i][j-1][k]) / h2 +
                                     (w[i][j][k+1] - 2.0 * w[i][j][k] + w[i][j][k-1]) / h2;

                u_new[i][j][k] = u[i][j][k] + DT * NU * laplacian_u;
                v_new[i][j][k] = v[i][j][k] + DT * NU * laplacian_v;
                w_new[i][j][k] = w[i][j][k] + DT * NU * laplacian_w;
            }
        }
    }
}

void update_fields() {
    void *temp;
    temp = u_data; u_data = u_new_data; u_new_data = temp;
    temp = v_data; v_data = v_new_data; v_new_data = temp;
    temp = w_data; w_data = w_new_data; w_new_data = temp;
}

int main(int argc, char *argv[]) {
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

    double start_time = omp_get_wtime();

    for (int n = 0; n <= N_STEPS; ++n) {
        simulation_step();
        update_fields();
    }

    double end_time = omp_get_wtime();
    double total_time = end_time - start_time;

    printf("Tempo de execucao do loop principal: %f segundos\n", total_time);

    free(u_data); free(v_data); free(w_data);
    free(u_new_data); free(v_new_data); free(w_new_data);

    return 0;
}