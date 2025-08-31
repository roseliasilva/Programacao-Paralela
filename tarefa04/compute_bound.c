#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Define o número de iterações para os cálculos
#define NUM_OPERATIONS 100000000

int main() {
    double result = 0.0;

    // Loop principal paralelizado com cálculos intensivos
    #pragma omp parallel for
    for (long i = 0; i < NUM_OPERATIONS; i++) {
        double x = sin(i) * log(i + 1) + cos(i) * exp(fmod(i, 1000));
        x = pow(x, 0.5) + sqrt(x + 1.0);
        result += x/(i+1);
    }

    return 0;
}