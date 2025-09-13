#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// Define a estrutura do nó da lista encadeada
typedef struct Node {
    char fileName[100];
    struct Node* next;
} Node;

// Função para criar um novo nó
Node *createNode(const char *fileName) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Falha ao alocar memória para o novo nó");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->fileName, fileName, sizeof(newNode->fileName) - 1);
    newNode->fileName[sizeof(newNode->fileName)-1] = '\0';
    newNode->next = NULL;
    return newNode;
}

// Função para inserir um nó no final da lista
void insertNode(Node **head, const char *fileName) {
    Node *newNode = createNode(fileName);
    if (*head == NULL) {
        *head = newNode;
    }else{
        Node *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Função para liberar a memória da lista
void freeList(Node *head) {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Node *head = NULL;

    for(int i=0; i<7; i++) {
        char filename[100];
        snprintf(filename, sizeof(filename), "arquivo_%02d.txt", i+1);
        insertNode(&head, filename);
    }

    Node *current = head;

    #pragma omp parallel
    {
        #pragma omp single
        {
            while(current!=NULL){
                Node *node = current;

                #pragma omp task
                {
                    int thread_id = omp_get_thread_num();
                    printf("Arquivo: %s, processado pela thread %d\n",
                           node->fileName, thread_id);
                }
                current = current->next;
            }
            #pragma omp taskwait
        }
    }

    freeList(head);
    return EXIT_SUCCESS;
}