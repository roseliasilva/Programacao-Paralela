#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

const int N = 99999999;

void teste_paralelo_incorreto(){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int count = 0;

    #pragma omp parallel for
    for(int i=0; i<N; i++){
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if(x*x + y*y <=1.0){
            count++;
        }
    }

    double pi = 4.0 * count/N;
    gettimeofday(&end, NULL);
    double tempo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("1. Teste Paralelo Incorreto:                      π ≈ %-15.15f | Time: %.3fs (race conidition)\n", pi, tempo);
}

void teste_com_critical(){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int count = 0;

    #pragma omp parallel
    {
        #pragma omp for
        for(int i=0; i<N; i++){
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if(x*x + y*y <=1.0){
                #pragma omp critical
                count++;
            }
        }
    }
    double pi = 4.0 * count/N;
    gettimeofday(&end, NULL);
    double tempo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("2. Teste Paralelo com Critical:                   π ≈ %-15.15f | Time: %.3fs\n", pi, tempo);
}

void teste_com_private(){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int count = 0;
    int local_count = 0;

    #pragma omp parallel private(local_count)
    {
        local_count = 0;
        #pragma omp for
        for(int i=0; i<N; i++){
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if(x*x + y*y <=1.0){
                local_count++;
            }
        }
        #pragma omp critical
        count += local_count;
    }

    double pi = 4.0 * count/N;
    gettimeofday(&end, NULL);
    double tempo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("3. Teste Paralelo com Private + Critical:         π ≈ %-15.15f | Time: %.3fs\n", pi, tempo);
}

void teste_com_firstprivate(){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int count = 0;
    int local_count = 0;

    #pragma omp parallel firstprivate(local_count)
    {
        #pragma omp for
        for(int i=0; i<N; i++){
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if(x*x + y*y <=1.0){
                local_count++;
            }
        }
        #pragma omp critical
        count += local_count;
    }
    double pi = 4.0 * count/N;
    gettimeofday(&end, NULL);
    double tempo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("4. Teste Paralelo com Firstprivate + Critical:    π ≈ %-15.15f | Time: %.3fs\n", pi, tempo);
}

void teste_com_lastprivate(){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int count = 0;
    int last_i = -1;

    #pragma omp parallel
    {
        int local_count = 0;

        #pragma omp for lastprivate(last_i)
        for(int i=0; i<N; i++){
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if(x*x + y*y <=1.0){
                local_count++;
            }
            last_i = i;
        }
        #pragma omp critical
        count += local_count;
    }

    double pi = 4.0 * count/N;
    gettimeofday(&end, NULL);
    double tempo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("5. Teste Paralelo com Lastprivate + Critical:     π ≈ %-15.15f | Time: %.3fs | Last i: %d\n", pi, tempo, last_i);
}

void teste_com_default_none(){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int count = 0;

    #pragma omp parallel default(none) shared(count, N)
    {
        int local_count = 0;
        #pragma omp for
        for(int i=0; i<N; i++){
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if(x*x + y*y <=1.0){
                local_count++;
            }
        }
        #pragma omp critical
        count += local_count;
    }

    double pi = 4.0 * count/N;
    gettimeofday(&end, NULL);
    double tempo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("6. Teste Paralelo com Default(none) + Critical:   π ≈ %-15.15f | Time: %.3fs\n", pi, tempo);
}

int main(){
    printf("π ≈ 3.141592653589793\n");
    printf("Estimativas: \n\n");

    teste_paralelo_incorreto();
    teste_com_critical();
    teste_com_private();
    teste_com_firstprivate();
    teste_com_lastprivate();
    teste_com_default_none();

    return 0;
}