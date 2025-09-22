#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#define NX 50 
#define NY 50
#define NZ 50
#define L 1.0 

// Parâmetros da simulação
#define NU 0.0005     
#define DT 0.00001    
#define N_STEPS 500   
#define OUTPUT_INTERVAL 100

// Variáveis globais para os campos de velocidade
double (*u)[NY][NZ], (*v)[NY][NZ], (*w)[NY][NZ];
double (*u_new)[NY][NZ], (*v_new)[NY][NZ], (*w_new)[NY][NZ];

const double dx = L / (NX - 1);
const double dy = L / (NY - 1);
const double dz = L / (NZ - 1);

// --- Parâmetros para teste de Desempenho OpenMP ---
#define CHUNK_SIZE 12

void allocate_memory() {
    u = malloc(NX * sizeof(*u)); v = malloc(NX * sizeof(*v)); w = malloc(NX * sizeof(*w));
    u_new = malloc(NX * sizeof(*u_new)); v_new = malloc(NX * sizeof(*v_new)); w_new = malloc(NX * sizeof(*w_new));
}

void initialize_fluid() {
    memset(u, 0, sizeof(double[NX][NY][NZ])); 
    memset(v, 0, sizeof(double[NX][NY][NZ])); 
    memset(w, 0, sizeof(double[NX][NY][NZ]));
}

void apply_perturbation() {
    int i_mid = NX / 2; int j_mid = NY / 2; int k_mid = NZ / 2;
    u[i_mid][j_mid][k_mid] = 2.0; 
    v[i_mid][j_mid][k_mid] = 2.0; 
    w[i_mid][j_mid][k_mid] = 2.0;
}

void simulation_step() {
    double dx2 = dx * dx; double dy2 = dy * dy; double dz2 = dz * dz;

    #pragma omp parallel for collapse(3) schedule(guided, CHUNK_SIZE)
    for (int i = 1; i < NX - 1; ++i) {
        for (int j = 1; j < NY - 1; ++j) {
            for (int k = 1; k < NZ - 1; ++k) {
                double laplacian_u = (u[i+1][j][k] - 2.0 * u[i][j][k] + u[i-1][j][k]) / dx2 +
                                     (u[i][j+1][k] - 2.0 * u[i][j][k] + u[i][j-1][k]) / dy2 +
                                     (u[i][j][k+1] - 2.0 * u[i][j][k] + u[i][j][k-1]) / dz2;
                
                double laplacian_v = (v[i+1][j][k] - 2.0 * v[i][j][k] + v[i-1][j][k]) / dx2 +
                                     (v[i][j+1][k] - 2.0 * v[i][j][k] + v[i][j-1][k]) / dy2 +
                                     (v[i][j][k+1] - 2.0 * v[i][j][k] + v[i][j][k-1]) / dz2;

                double laplacian_w = (w[i+1][j][k] - 2.0 * w[i][j][k] + w[i-1][j][k]) / dx2 +
                                     (w[i][j+1][k] - 2.0 * w[i][j][k] + w[i][j-1][k]) / dy2 +
                                     (w[i][j][k+1] - 2.0 * w[i][j][k] + w[i][j][k-1]) / dz2;
                
                u_new[i][j][k] = u[i][j][k] + DT * NU * laplacian_u;
                v_new[i][j][k] = v[i][j][k] + DT * NU * laplacian_v;
                w_new[i][j][k] = w[i][j][k] + DT * NU * laplacian_w;
            }
        }
    }
}

void update_fields() {
    double (*temp_u)[NY][NZ] = u; u = u_new; u_new = temp_u;
    double (*temp_v)[NY][NZ] = v; v = v_new; v_new = temp_v;
    double (*temp_w)[NY][NZ] = w; w = w_new; w_new = temp_w;
}

int main() {
    allocate_memory();
    initialize_fluid();
    apply_perturbation();

    printf("Iniciando simulação paralela com OpenMP...\n");
    printf("Grid: %d x %d x %d\n", NX, NY, NZ);
    
    printf("Usando %d threads.\n", omp_get_max_threads());
    printf("Chunk size: %d\n", CHUNK_SIZE);

    double start_time = omp_get_wtime();

    for (int n = 0; n <= N_STEPS; ++n) {
        simulation_step();
        update_fields();
    }

    double end_time = omp_get_wtime();
    double total_time = end_time - start_time;

    printf("Simulação concluída.\n");
    printf("Tempo de execução do loop principal: %f segundos\n", total_time);
    
    free(u); free(v); free(w);
    free(u_new); free(v_new); free(w_new);

    return 0;
}