#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Definições do domínio da simulação
#define NX 50
#define NY 50
#define NZ 50
#define L 1.0

// Propriedades do fluido e da simulação
#define NU 0.0005
#define DT 0.001
#define N_STEPS 1000
#define OUTPUT_INTERVAL 100

// Arrays para armazenar os campos de velocidade
double u[NX][NY][NZ], v[NX][NY][NZ], w[NX][NY][NZ];
double u_new[NX][NY][NZ], v_new[NX][NY][NZ], w_new[NX][NY][NZ];

// Espaçamento da grade
const double dx = L / (NX - 1);
const double dy = L / (NY - 1);
const double dz = L / (NZ - 1);

// Protótipos
void initialize_fluid();
void apply_perturbation();
void simulation_step();
void update_fields();
void save_results_3d(int step);

int main() {
    printf("Iniciando a simulação 3D de fluido...\n");
    initialize_fluid();

    // Estabilidade (sem mudanças)
    printf("Verificando a estabilidade...\n");
    for (int n = 0; n < 100; ++n) {
        simulation_step();
        update_fields();
    }
    printf("O campo permaneceu estável.\n");

    apply_perturbation();

    printf("Perturbação aplicada. Iniciando a simulação principal...\n");
    for (int n = 0; n <= N_STEPS; ++n) {
        simulation_step();
        update_fields();

        // Salva os dados em intervalos definidos
        if (n % OUTPUT_INTERVAL == 0 || n == 50) {
            printf("Passo: %d\n", n);
            save_results_3d(n);
        }
    }

    printf("Simulação concluída.\n");
    return 0;
}

void initialize_fluid() { 
    memset(u, 0, sizeof(u)); memset(v, 0, sizeof(v)); memset(w, 0, sizeof(w));
    memcpy(u_new, u, sizeof(u)); memcpy(v_new, v, sizeof(v)); memcpy(w_new, w, sizeof(w));
}
void apply_perturbation() { 
    int i_mid = NX/2, j_mid = NY/2, k_mid = NZ/2;
    u[i_mid][j_mid][k_mid] = 2.0; v[i_mid][j_mid][k_mid] = 2.0; w[i_mid][j_mid][k_mid] = 2.0;
}
void simulation_step() { 
    double dx2=dx*dx, dy2=dy*dy, dz2=dz*dz;
    for (int i=1; i<NX-1; ++i) for (int j=1; j<NY-1; ++j) for (int k=1; k<NZ-1; ++k) {
        double lap_u = (u[i+1][j][k] - 2*u[i][j][k] + u[i-1][j][k])/dx2 + (u[i][j+1][k] - 2*u[i][j][k] + u[i][j-1][k])/dy2 + (u[i][j][k+1] - 2*u[i][j][k] + u[i][j][k-1])/dz2;
        double lap_v = (v[i+1][j][k] - 2*v[i][j][k] + v[i-1][j][k])/dx2 + (v[i][j+1][k] - 2*v[i][j][k] + v[i][j-1][k])/dy2 + (v[i][j][k+1] - 2*v[i][j][k] + v[i][j][k-1])/dz2;
        double lap_w = (w[i+1][j][k] - 2*w[i][j][k] + w[i-1][j][k])/dx2 + (w[i][j+1][k] - 2*w[i][j][k] + w[i][j-1][k])/dy2 + (w[i][j][k+1] - 2*w[i][j][k] + w[i][j][k-1])/dz2;
        u_new[i][j][k] = u[i][j][k] + DT*NU*lap_u; v_new[i][j][k] = v[i][j][k] + DT*NU*lap_v; w_new[i][j][k] = w[i][j][k] + DT*NU*lap_w;
    }
}
void update_fields() { 
    memcpy(u, u_new, sizeof(u)); memcpy(v, v_new, sizeof(v)); memcpy(w, w_new, sizeof(w));
}

void save_results_3d(int step) {
    char filename[50];
    // Salva em um novo diretório para não misturar com os resultados 2D
    sprintf(filename, "results_3d/velocity_3d_%04d.dat", step);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita. Verifique se o diretório 'results_3d' existe.\n");
        return;
    }

    // Escreve um cabeçalho no arquivo
    fprintf(file, "x y z magnitude\n");

    // Loop por todo o domínio 3D
    for (int i = 0; i < NX; ++i) {
        for (int j = 0; j < NY; ++j) {
            for (int k = 0; k < NZ; ++k) {
                double magnitude = sqrt(u[i][j][k]*u[i][j][k] + v[i][j][k]*v[i][j][k] + w[i][j][k]*w[i][j][k]);
                
                // Otimização: só salva pontos com movimento significativo
                if (magnitude > 0.01) {
                    fprintf(file, "%f %f %f %f\n", i * dx, j * dy, k * dz, magnitude);
                }
            }
        }
    }

    fclose(file);
}