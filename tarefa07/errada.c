#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

typedef struct Node {
    char fileName[100];
    struct Node* next;
} Node;

Node *createNode(const char *fileName) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Falha ao alocar memória para o novo nó");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->fileName, fileName, sizeof(newNode->fileName) - 1);
    newNode->fileName[sizeof(newNode->fileName)-1] = '\0'; // Garantir terminação nula
    newNode->next = NULL;
    return newNode;
}

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

    for(int i=0; i<5; i++) {
        char filename[100];
        snprintf(filename, sizeof(filename), "arquivo_%02d.txt", i+1);
        insertNode(&head, filename);
    }

    // fora da região paralela
    Node *current = head;

    #pragma omp parallel
    {
        while(current!=NULL){
            Node *node = current;

            #pragma omp task firstprivate(node)
            {
                int thread_id = omp_get_thread_num();
                printf("Arquivo: %s, processado pela thread %d\n",
                        node->fileName, thread_id);
            }
            current = current->next;
        }
        #pragma omp taskwait
    }
    
    freeList(head);
    return EXIT_SUCCESS;
}