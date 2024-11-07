#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct LinkedList {
    Node* head;
    pthread_mutex_t mutex;          // Mutex para proteger la lista
    pthread_cond_t readersCond;     // Condición para lectores
    pthread_cond_t writersCond;     // Condición para escritores
    int readers;                     // Contador de lectores activos
    int writers;                     // Contador de escritores activos
} LinkedList;


// puntero al head, inicializar mutex, inicializar condición de lectores, inicializar condición de escritores, inicializar contadores de lectores y escritores
LinkedList list = {NULL, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, 0, 0};

// Función para agregar un nuevo nodo a la lista
void addNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;

    pthread_mutex_lock(&list.mutex); // Bloquear el mutex para acceder a la lista
    while (list.writers > 0) { // Esperar si hay escritores: si hay escritores, no se puede escribir
        pthread_cond_wait(&list.writersCond, &list.mutex);
    }
    list.writers++; // Incrementar el contador de escritores

    // Agregar el nuevo nodo al principio de la lista
    newNode->next = list.head;
    list.head = newNode;

    list.writers--; // Decrementar el contador de escritores porque ya terminó de escribir
    // despertar a todos los hilos que estén esperando en readersCond y writersCond
    pthread_cond_broadcast(&list.readersCond); // Despertar a los lectores
    pthread_cond_broadcast(&list.writersCond); // Despertar a los escritores
    pthread_mutex_unlock(&list.mutex);
}

void deleteNode(int value) {
    pthread_mutex_lock(&list.mutex);
    while (list.writers > 0) { // Esperar si hay escritores
        pthread_cond_wait(&list.writersCond, &list.mutex);
    }
    list.writers++; // Incrementar el contador de escritores

    Node* current = list.head;
    Node* previous = NULL;
    while (current != NULL) {
        if (current->data == value) {
            if (previous == NULL) {
                list.head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }

    list.writers--; // Decrementar el contador de escritores
    if (list.writers == 0) { // Si no hay escritores, permitir que los lectores accedan
        pthread_cond_broadcast(&list.readersCond); // Despertar a los lectores
    }
    pthread_mutex_unlock(&list.mutex);
}

// Función para leer todos los nodos de la lista
void readList() {
    pthread_mutex_lock(&list.mutex);
    while (list.writers > 0) { // Esperar si hay escritores
        pthread_cond_wait(&list.readersCond, &list.mutex);
    }
    list.readers++; // Incrementar el contador de lectores

    // Leer la lista
    Node* current = list.head;
    while (current != NULL) {
        printf("Node: %d ", current->data);
        current = current->next;
    }
    printf("\n");
    list.readers--; // Decrementar el contador de lectores
    if (list.readers == 0) { // Si no hay lectores, permitir que los escritores accedan
        pthread_cond_broadcast(&list.writersCond); // Despertar a los escritores
    }
    pthread_mutex_unlock(&list.mutex);
}

// Función que ejecuta el hilo de escritura
void* writer(void* arg) {
    int value = *((int*)arg);
    addNode(value);
    return NULL;
}

// Función que ejecuta el hilo de lectura
void* reader(void* arg) {
    readList();
    return NULL;
}

int main() {
    pthread_t readers[5], writers[5];
    int writerValues[5] = {1, 2, 3, 4, 5};

    // Crear hilos de escritores
    for (int i = 0; i < 5; i++) {
        // dirección del hilo, atributos del hilo, función a ejecutar, argumentos de la función
        pthread_create(&writers[i], NULL, writer, &writerValues[i]);
    }

    // Crear hilos de lectores
    for (int i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, NULL);
    }

    // Esperar a que todos los escritores terminen
    for (int i = 0; i < 5; i++) {
        pthread_join(writers[i], NULL);
    }

    // Esperar a que todos los lectores terminen
    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }

    printf("Lista final:\n");
    // Print the list
    readList();

    // Liberar recursos
    pthread_mutex_destroy(&list.mutex);
    pthread_cond_destroy(&list.readersCond);
    pthread_cond_destroy(&list.writersCond);

    return 0;
}
