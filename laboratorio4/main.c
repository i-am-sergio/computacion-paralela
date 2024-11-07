#include <stdio.h>      // Incluye la biblioteca estándar de E/S para usar printf
#include <stdlib.h>     // Incluye la biblioteca estándar de C para funciones generales
#include <pthread.h>    // Incluye la biblioteca pthread para trabajar con hilos

#define NUM_THREADS 4   // Define el número de hilos
#define ARRAY_SIZE 20   // Define el tamaño del array

// Array de enteros a sumar
int array[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

// Array para guardar las sumas parciales de cada hilo
int partial_sums[NUM_THREADS] = {0}; 

// Estructura que contiene el rango de índices que procesará cada hilo y el índice del hilo
typedef struct {
    int start;  // Índice inicial del segmento que procesará el hilo
    int end;    // Índice final del segmento (no inclusive)
    int index;  // Índice del hilo (0 a NUM_THREADS-1)
} ThreadData;

// Función que ejecutará cada hilo para calcular su suma parcial
void* sum_array_section(void* arg) {
    ThreadData* data = (ThreadData*)arg; // Convierte el argumento a tipo ThreadData
    int sum = 0; // Variable para almacenar la suma parcial del hilo

    // Mensaje de depuración: muestra el índice del hilo y el rango que va a procesar
    printf("Hilo %d: procesando elementos desde el índice %d hasta el índice %d\n", data->index, data->start, data->end - 1);
    
    // Suma los elementos en el rango asignado al hilo
    for (int i = data->start; i < data->end; i++) {
        sum += array[i]; // Agrega el valor del elemento actual a la suma parcial
        // Muestra el índice del elemento actual y la suma parcial
        printf("Hilo %d: sumando array[%d] = %d, suma parcial = %d\n", data->index, i, array[i], sum);
    }
    
    // Almacena la suma parcial en el array partial_sums en la posición correspondiente al hilo
    partial_sums[data->index] = sum;
    // Mensaje de depuración: muestra la suma parcial final del hilo
    printf("Hilo %d: suma parcial completada = %d\n", data->index, sum);
    
    return NULL; // Retorna NULL, ya que no se necesita valor de retorno
}

int main() {
    pthread_t threads[NUM_THREADS];      // Array de identificadores para los hilos
    ThreadData thread_data[NUM_THREADS]; // Array para los datos que pasaremos a cada hilo
    int total_sum = 0;                   // Variable para almacenar la suma total

    // Bucle para configurar y crear los hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * (ARRAY_SIZE / NUM_THREADS); // Calcula el índice inicial
        thread_data[i].end = (i + 1) * (ARRAY_SIZE / NUM_THREADS); // Calcula el índice final (no inclusive)
        thread_data[i].index = i; // Asigna el índice del hilo

        // Crea el hilo y le pasa la estructura con los datos
        // pthread_create params: 
        // 1. Identificador del hilo
        // 2. Atributos del hilo (NULL para usar los valores por defecto)
        // 3. Función que ejecutará el hilo
        if (pthread_create(&threads[i], NULL, sum_array_section, (void*)&thread_data[i])) {
            // Si hay un error al crear el hilo, imprime un mensaje y finaliza el programa
            fprintf(stderr, "Error al crear el hilo %d\n", i);
            return 1;
        }
    }

    // Espera a que todos los hilos terminen de ejecutar
    for (int i = 0; i < NUM_THREADS; i++) {
        // pthread_join params:
        // 1. Identificador del hilo
        // 2. Dirección de memoria para almacenar el valor de retorno del hilo (NULL si no se necesita)
        if (pthread_join(threads[i], NULL)) {
            // Si hay un error al unir el hilo, imprime un mensaje y finaliza el programa
            fprintf(stderr, "Error al unir el hilo %d\n", i);
            return 2;
        }
    }

    // Bucle para sumar las sumas parciales de todos los hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Hilo %d: suma parcial = %d\n", i, partial_sums[i]); // Mensaje de depuración: muestra la suma parcial de cada hilo
        total_sum += partial_sums[i]; // Agrega la suma parcial al total
    }

    // Imprime la suma total de todos los elementos del array
    printf("La suma total del arreglo es: %d\n", total_sum);
    return 0; // Finaliza el programa exitosamente
}
