#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, commSize;
    long long localSum = 0, globalSum = 0;
    int numIterations;

    // Inicialización de MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtener el rango del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &commSize); // Obtener el tamaño del comunicador

    // Inicializar la suma local con un valor basado en el rango del proceso
    localSum = rank + 1; // Ejemplo: sumar el rango + 1

    // Comprobar si el número de procesos es potencia de dos
    if (commSize & (commSize - 1)) {
        if (rank == 0) {
            printf("El número de procesos no es una potencia de dos.\n");
        }
        MPI_Finalize();
        return 0;
    }

    // Algoritmo Butterfly
    for (numIterations = 0; (1 << numIterations) < commSize; numIterations++) {
        int partner = rank ^ (1 << numIterations); // Calcular el compañero

        // Enviar y recibir el valor de la suma local
        if (partner < commSize) {
            long long receivedValue;
            // Enviar la suma local al compañero
            MPI_Send(&localSum, 1, MPI_LONG_LONG, partner, 0, MPI_COMM_WORLD);
            // Recibir el valor de la suma local del compañero
            MPI_Recv(&receivedValue, 1, MPI_LONG_LONG, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // Actualizar la suma local
            localSum += receivedValue;
        }
    }

    // El proceso 0 obtiene la suma global
    if (rank == 0) {
        globalSum = localSum; // Guardar la suma global en el proceso 0
        printf("Suma global calculada: %lld\n", globalSum);
    }

    // Finalización de MPI
    MPI_Finalize();
    return 0;
}
