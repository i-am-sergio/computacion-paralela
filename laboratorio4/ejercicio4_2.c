#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Variables globales
long long totalLanzamientos;
long long totalEnCirculo = 0; // cayeron en el círculo
int numeroHilos;


// Mutex para proteger el acceso a la variable totalEnCirculo
pthread_mutex_t mutexEnCirculo; 
// Mutex: garantizar que solo un hilo pueda acceder a un recurso a la vez

// Función que ejecuta cada hilo
void* lanzarDardos(void* rango) {
    long long lanzamientosPorHilo = totalLanzamientos / numeroHilos;
    long long enCirculo = 0; // solo en este hilo
    unsigned int semilla = (unsigned int)time(NULL) ^ (unsigned int)pthread_self(); // semilla única para cada hilo

    // Lanzar los dardos
    for (long long lanzamiento = 0; lanzamiento < lanzamientosPorHilo; lanzamiento++) {
        double x = ((double)rand_r(&semilla) / RAND_MAX) * 2.0 - 1.0; // Generar un número aleatorio entre -1 y 1
        double y = ((double)rand_r(&semilla) / RAND_MAX) * 2.0 - 1.0; // Generar un número aleatorio entre -1 y 1
        double distanciaCuadrada = x * x + y * y;

        if (distanciaCuadrada <= 1) {// Si el punto está dentro del círculo
            enCirculo++;
        }
    }

    // Agregar el conteo local al total global usando un mutex
    pthread_mutex_lock(&mutexEnCirculo); // El hilo bloquear el mutex para usarlo y evitar que otros hilos lo usen
    totalEnCirculo += enCirculo; // Sección crítica: Modificar la variable compartida
    pthread_mutex_unlock(&mutexEnCirculo); // El hilo desbloquea el mutex para que otros hilos puedan usarlo

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <número de lanzamientos> <número de hilos>\n", argv[0]);
        return -1;
    }

    // Leer el número de lanzamientos y de hilos
    totalLanzamientos = atoll(argv[1]);
    numeroHilos = atoi(argv[2]);
    
    pthread_t* hilos = malloc(numeroHilos * sizeof(pthread_t)); // Reservar memoria para los id de hilos
    pthread_mutex_init(&mutexEnCirculo, NULL); // Inicializar el mutex que protegerá la variable totalEnCirculo

    // Crear hilos
    for (int hilo = 0; hilo < numeroHilos; hilo++) {
         // dirección del hilo, atributos del hilo, función a ejecutar, argumentos de la función
        pthread_create(&hilos[hilo], NULL, lanzarDardos, (void*)(long)hilo);
    }

    // Esperar a que todos los hilos terminen
    for (int hilo = 0; hilo < numeroHilos; hilo++) {
        pthread_join(hilos[hilo], NULL); // Esperar a que el hilo "n" termine
    }

    // Calcular el valor estimado de pi
    double estimacionPi = 4 * ((double)totalEnCirculo / (double)totalLanzamientos);
    printf("Estimación de π: %f\n", estimacionPi);

    // Liberar recursos
    pthread_mutex_destroy(&mutexEnCirculo);
    free(hilos); // libera memoria asignada a hilos

    return 0;
}
