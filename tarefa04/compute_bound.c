#include <stdio.h>
#include <math.h> // Para funções matemáticas como sin, cos, etc.
#include <omp.h>

// Número de iterações do laço principal.
#define TAMANHO 50000

// Quantidade de cálculos pesados dentro de cada iteração do laço principal.
// Aumente este valor se o seu CPU for muito rápido.
#define ITER_INTERNAS 5000

int main() {
    double resultado_final = 0.0;

    // --- Início da seção crítica para medição de tempo ---
    // Paraleliza o laço principal. Cada thread executará um conjunto de iterações 'i'.
    // A cláusula 'reduction' soma os resultados parciais de cada thread de forma segura.
    #pragma omp parallel for
    for (int i = 0; i < TAMANHO; i++) {
        double valor = (double)i / TAMANHO; // Valor inicial para os cálculos

        // Este laço interno realiza os cálculos pesados.
        // Os dados (a variável 'valor') permanecem na cache ou registradores da CPU,
        // minimizando o acesso à RAM. O gargalo é puramente o poder de cálculo.
        for (int j = 0; j < ITER_INTERNAS; j++) {
            valor = sin(valor) + cos(valor) + sqrt(fabs(valor));
            // Normaliza para evitar que o valor cresça demais ou vá para NaN (Not a Number)
            if (valor > 10.0) {
                valor = fmod(valor, 10.0);
            }
        }
        resultado_final += valor;
    }
    // --- Fim da seção crítica ---

    printf("Resultado final (para evitar otimização do compilador): %f\n", resultado_final);

    return 0;
}