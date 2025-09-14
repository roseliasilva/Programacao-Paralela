#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define maximum_lists 100

typedef struct Node {
    int value;
    struct Node* next;
} Node;

// Função para inserir no início da lista
void insert(Node** head, int value) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = *head;
    *head = newNode;
}

// Função para imprimir a lista
void print_list(Node* head, const char* label, const char* method) {
    printf("[Método %s] %s: ", method, label);
    for (Node* curr = head; curr != NULL; curr = curr->next)
        printf("%d -> ", curr->value);
    printf("NULL\n");
}

void critical_nomeado(){
    printf("==== CRITICAL NOMEADO (duas listas, OpenMP tasks) ====\n");

    Node* list1 = NULL;
    Node* list2 = NULL;
    int N = 20;
    int num_threads = 2;
    int insertions_per_thread = N / num_threads;

    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp single
        {
            for (int i = 0; i < insertions_per_thread; i++) {
                #pragma omp task
                {
                    unsigned int seed = (unsigned int)time(NULL) ^ omp_get_thread_num() ^ i;
                    int value = rand_r(&seed) % 100;
                    int choice = rand_r(&seed) % 2;

                    if (choice == 0) {
                        #pragma omp critical(lista1)
                        insert(&list1, value);
                    }else{
                        #pragma omp critical(lista2)
                        insert(&list2, value);
                    }
                }
            }
        }
    }

    print_list(list1, "Lista 1", "Critical Nomeado");
    print_list(list2, "Lista 2", "Critical Nomeado");
}

void locks_explicitos(){
    printf("\n==== LOCKS EXPLÍCITOS (M listas, OpenMP tasks + locks) ====\n");

    int M, N;
    printf("Quantidade de listas (M, até 10): ");
    scanf("%d", &M);
    if (M < 1 || M > 10) {
        printf("M deve ser entre 1 e 10.\n");
        return;
    }
    printf("Quantidade de inserções (N): ");
    scanf("%d", &N);

    Node* lists[maximum_lists] = {NULL};
    omp_lock_t locks[maximum_lists];

    for (int i = 0; i < M; i++) {
        omp_init_lock(&locks[i]);
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < N; i++) {
                #pragma omp task
                {
                    unsigned int seed = (unsigned int)time(NULL) ^ omp_get_thread_num() ^ i;
                    int value = rand_r(&seed) % 1000;
                    int idx = rand_r(&seed) % M; 

                    omp_set_lock(&locks[idx]);
                    insert(&lists[idx], value);
                    omp_unset_lock(&locks[idx]);
                }
            }
        }
    }

    for (int i = 0; i < M; i++) {
        char label[50];
        snprintf(label, sizeof(label), "Lista %d", i);
        print_list(lists[i], label, "Locks Explícitos");
        omp_destroy_lock(&locks[i]);
    }
}

int main() {
    critical_nomeado();
    locks_explicitos();
    return 0;
}