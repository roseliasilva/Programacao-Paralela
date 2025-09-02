//este código tem single e firtsprivate, a idéia é testar sem nada antes, e só depois ir adicionando

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// Estrutura do nó da lista encadeada
typedef struct Node {
    char filename[50];
    struct Node *next;
} Node;

// Função para criar um novo nó
Node* create_node(const char *name) {
    Node *new_node = (Node*) malloc(sizeof(Node));
    strcpy(new_node->filename, name);
    new_node->next = NULL;
    return new_node;
}

// Função para adicionar nó no final da lista
void append(Node **head, const char *name) {
    Node *new_node = create_node(name);
    if (*head == NULL) {
        *head = new_node;
    } else {
        Node *temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_node;
    }
}

int main() {
    Node *head = NULL;

    // Criando lista de arquivos fictícios
    append(&head, "arquivo1.txt");
    append(&head, "arquivo2.txt");
    append(&head, "arquivo3.txt");
    append(&head, "arquivo4.txt");

    // Região paralela com tasks
    #pragma omp parallel
    {
        #pragma omp single
        {
            Node *current = head;
            while (current != NULL) {
                // Cria uma task para cada nó
                #pragma omp task firstprivate(current)
                {
                    printf("Processando %s na thread %d\n",
                           current->filename, omp_get_thread_num());
                }
                current = current->next;
            }
        }
    }

    // Liberar memória
    Node *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }

    return 0;
}