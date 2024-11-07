#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void* funcionHilo(void* arg) {
    // Simula algún trabajo durmiendo por una corta duración
    // Esto puede ser removido si solo se mide el tiempo de creación/terminación del hilo.
    // sleep(1);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <número_de_hilos> <número_de_pruebas>\n", argv[0]);
        return 1;
    }

    int numHilos = atoi(argv[1]);
    int numPruebas = atoi(argv[2]);
    double tiempoTotal = 0.0;

    for (int prueba = 0; prueba < numPruebas; prueba++) { // Realizar múltiples pruebas
        pthread_t* hilos = malloc(numHilos * sizeof(pthread_t)); // Reservar memoria para los id de hilos
        if (hilos == NULL) {
            perror("Fallo al asignar memoria para los hilos");
            return 1;
        }

        clock_t tiempoInicio = clock(); // Iniciar el cronómetro

        for (int i = 0; i < numHilos; i++) {
            // dirección del hilo, atributos del hilo, función a ejecutar, argumentos de la función
            pthread_create(&hilos[i], NULL, funcionHilo, NULL);
        }

        for (int i = 0; i < numHilos; i++) {
            // Esperar a que el hilo termine: dirección del hilo, valor de retorno
            pthread_join(hilos[i], NULL);
        }

        clock_t tiempoFin = clock(); // Detener el cronómetro
        double tiempoTranscurrido = (double)(tiempoFin - tiempoInicio) / CLOCKS_PER_SEC;
        tiempoTotal += tiempoTranscurrido;

        printf("Tiempo transcurrido en la prueba %d: %f segundos\n", prueba + 1, tiempoTranscurrido);

        free(hilos); // libero memoria asignada
    }

    double tiempoPromedio = tiempoTotal / numPruebas;
    printf("Tiempo promedio para crear y terminar %d hilos en %d pruebas: %f segundos\n", numHilos, numPruebas, tiempoPromedio);

    return 0;
}
