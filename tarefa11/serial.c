#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Parâmetros da simulação 3D
#define NX 61
#define NY 61
#define NZ 61
#define NT 500
// --- ALTERAÇÃO 1: NU reduzido para uma difusão mais lenta ---
#define NU 0.005     // Viscosidade cinemática (era 0.05)
#define DT 0.1
#define H 1.0

// A checagem de estabilidade continua válida.

// Função para salvar o corte (slice) - sem alterações
void save_slice_to_file(const char* filename, double u[NX][NY][NZ], double v[NX][NY][NZ], double w[NX][NY][NZ]) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) { printf("Erro ao abrir o arquivo %s\n", filename); return; }
    fprintf(file, "x,y,u,v,w,magnitude\n");
    int k_slice = NZ / 2;
    for (int i = 0; i < NX; i++) {
        for (int j = 0; j < NY; j++) {
            double u_val = u[i][j][k_slice];
            double v_val = v[i][j][k_slice];
            double w_val = w[i][j][k_slice];
            double magnitude = sqrt(u_val*u_val + v_val*v_val + w_val*w_val);
            fprintf(file, "%d,%d,%.6f,%.6f,%.6f,%.6f\n", i, j, u_val, v_val, w_val, magnitude);
        }
    }
    fclose(file);
}

// --- ALTERAÇÃO 2: Inicialização com uma Gaussiana ---
void initialize_grid_gaussian(double u[NX][NY][NZ], double v[NX][NY][NZ], double w[NX][NY][NZ]) {
    // Inicializa tudo com zero
    for (int i = 0; i < NX; i++) {
        for (int j = 0; j < NY; j++) {
            for (int k = 0; k < NZ; k++) {
                u[i][j][k] = 0.0; v[i][j][k] = 0.0; w[i][j][k] = 0.0;
            }
        }
    }

    // Parâmetros da Gaussiana
    double amplitude = 2.0; // Altura do pico inicial
    double sigma = 4.0;     // Largura da "colina"
    double cx = NX / 2.0;   // Centro em x
    double cy = NY / 2.0;   // Centro em y
    double cz = NZ / 2.0;   // Centro em z

    // Aplica a perturbação Gaussiana
    for (int i = 0; i < NX; i++) {
        for (int j = 0; j < NY; j++) {
            for (int k = 0; k < NZ; k++) {
                double dx = i - cx;
                double dy = j - cy;
                double dz = k - cz;
                double r_squared = dx*dx + dy*dy + dz*dz;
                double val = amplitude * exp(-r_squared / (2.0 * sigma * sigma));
                
                // Aplicar a perturbação em todas as componentes de velocidade
                u[i][j][k] = val;
                v[i][j][k] = val;
                w[i][j][k] = 0.0; // Vamos deixar w=0 para simplificar a visualização da magnitude
            }
        }
    }
}


int main() {
    static double u[NX][NY][NZ], v[NX][NY][NZ], w[NX][NY][NZ];
    static double un[NX][NY][NZ], vn[NX][NY][NZ], wn[NX][NY][NZ];

    // Chama a nova função de inicialização
    initialize_grid_gaussian(u, v, w);
    
    // O resto do código permanece exatamente o mesmo...
    save_slice_to_file("output_slice_serial_0000.csv", u, v, w);
    double factor = NU * DT / (H * H);
    clock_t start_time = clock();
    for (int t = 1; t <= NT; t++) {
        for (int i = 0; i < NX; i++) for (int j = 0; j < NY; j++) for(int k = 0; k < NZ; k++){
            un[i][j][k] = u[i][j][k]; vn[i][j][k] = v[i][j][k]; wn[i][j][k] = w[i][j][k];
        }
        for (int i = 1; i < NX - 1; i++) for (int j = 1; j < NY - 1; j++) for (int k = 1; k < NZ - 1; k++) {
            u[i][j][k] = un[i][j][k] + factor * (un[i+1][j][k] + un[i-1][j][k] + un[i][j+1][k] + un[i][j-1][k] + un[i][j][k+1] + un[i][j][k-1] - 6.0 * un[i][j][k]);
            v[i][j][k] = vn[i][j][k] + factor * (vn[i+1][j][k] + vn[i-1][j][k] + vn[i][j+1][k] + vn[i][j-1][k] + vn[i][j][k+1] + vn[i][j][k-1] - 6.0 * vn[i][j][k]);
            w[i][j][k] = wn[i][j][k] + factor * (wn[i+1][j][k] + wn[i-1][j][k] + wn[i][j+1][k] + wn[i][j-1][k] + wn[i][j][k+1] + wn[i][j][k-1] - 6.0 * wn[i][j][k]);
        }
        if (t % 50 == 0) {
            char filename[50];
            sprintf(filename, "output_slice_serial_%04d.csv", t);
            save_slice_to_file(filename, u, v, w);
        }
    }
    clock_t end_time = clock();
    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Simulação 3D serial (corrigida) concluída.\n");
    printf("Tempo de execução (CPU time): %f segundos\n", cpu_time_used);
    return 0;
}