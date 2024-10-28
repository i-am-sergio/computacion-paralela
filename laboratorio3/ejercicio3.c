#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, totalProcesses;
    long long localValue, globalSum;

    // Inicialización de MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses);

    // Inicializar el valor local para cada proceso
    localValue = rank + 1;
    printf("Proceso %d: valor local inicial = %lld\n", rank, localValue);

    // Suma global estructurada en árbol
    int step = 1;
    while (step < totalProcesses) { // Mientras el paso sea menor al total de procesos
        if (rank % (2 * step) == 0) { 
            // Si es par entonces recibe el valor de otro proceso 
            if (rank + step < totalProcesses) { // Verificar que el rango es válido
                long long receivedValue;
                MPI_Recv(&receivedValue, 1, MPI_LONG_LONG, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                localValue += receivedValue;
                printf("Proceso %d: recibió %lld de proceso %d, nueva suma local = %lld\n", rank, receivedValue, rank + step, localValue);
            }
        } else if (rank % (2 * step) == step) {
            // Si es impar entonces envía el valor a otro proceso
            if (rank - step >= 0) { // Verificar que el rango es válido
                MPI_Send(&localValue, 1, MPI_LONG_LONG, rank - step, 0, MPI_COMM_WORLD);
                printf("Proceso %d: envió %lld a proceso %d\n", rank, localValue, rank - step);
            }
            break; // Salir del ciclo
        }
        step *= 2; // Duplicar el paso
    }

    // Solo el proceso 0 tiene el resultado final
    if (rank == 0) {
        globalSum = localValue;
        printf("Proceso %d: suma global final = %lld\n", rank, globalSum);
    }

    // Finalización de MPI
    MPI_Finalize();
    return 0;
}
