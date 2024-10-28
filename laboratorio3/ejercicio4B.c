#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, commSize;
    long long localSum = 0, globalSum = 0;
    int numIterations = 0;

    // Inicialización de MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtener el rango del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &commSize); // Obtener el tamaño del comunicador

    // Inicializar la suma local con un valor basado en el rango del proceso
    localSum = rank + 1; // Ejemplo: sumar el rango + 1

    // Algoritmo Butterfly
    for (numIterations = 0; (1 << numIterations) < commSize; numIterations++) {
        int partner = rank ^ (1 << numIterations); // Calcular el compañero

        // Comprobar si el compañero está en el rango
        if (partner < commSize) {
            long long receivedValue;
            MPI_Send(&localSum, 1, MPI_LONG_LONG, partner, 0, MPI_COMM_WORLD);
            MPI_Recv(&receivedValue, 1, MPI_LONG_LONG, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            localSum += receivedValue; // Sumar el valor recibido
        }
    }

    // Sumar en el proceso 0
    if (rank == 0) {
        globalSum = localSum; // Guardar la suma global en el proceso 0
        printf("Suma global calculada: %lld\n", globalSum);
    }

    // Finalización de MPI
    MPI_Finalize();
    return 0;
}
