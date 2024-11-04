#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 20

int array[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
int partial_sums[NUM_THREADS] = {0}; // Array para guardar las sumas parciales de cada hilo

// Estructura para pasar los límites de los segmentos del array a cada hilo
typedef struct {
    int start;
    int end;
    int index;
} ThreadData;

// Función que ejecutará cada hilo para calcular la suma parcial
void* sum_array_section(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sum = 0;
    
    printf("Hilo %d: procesando elementos desde el índice %d hasta el índice %d\n", data->index, data->start, data->end - 1);
    
    for (int i = data->start; i < data->end; i++) {
        sum += array[i];
        printf("Hilo %d: sumando array[%d] = %d, suma parcial = %d\n", data->index, i, array[i], sum);
    }
    
    partial_sums[data->index] = sum;
    printf("Hilo %d: suma parcial completada = %d\n", data->index, sum);
    
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int total_sum = 0;

    // Configurar y crear los hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * (ARRAY_SIZE / NUM_THREADS);
        thread_data[i].end = (i + 1) * (ARRAY_SIZE / NUM_THREADS);
        thread_data[i].index = i;
        
        if (pthread_create(&threads[i], NULL, sum_array_section, (void*)&thread_data[i])) {
            fprintf(stderr, "Error al crear el hilo %d\n", i);
            return 1;
        }
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Error al unir el hilo %d\n", i);
            return 2;
        }
    }

    // Sumar las sumas parciales
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Hilo %d: suma parcial = %d\n", i, partial_sums[i]);
        total_sum += partial_sums[i];
    }

    printf("La suma total del arreglo es: %d\n", total_sum);
    return 0;
}
