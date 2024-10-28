#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, totalProcesses;
    long long localValue, globalSum;
    
    // Inicialización de MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtener el rank: identificador único de cada proceso
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses); // Obtener el número total de procesos

    // Inicializar el valor local para cada proceso
    localValue = rank + 1; // Por ejemplo, cada proceso tiene un valor igual a su rango + 1
    printf("Proceso %d: valor local inicial = %lld\n", rank, localValue);

    // Suma global estructurada en árbol (suponiendo que totalProcesses es una potencia de dos)
    int step = 1;
    while (step < totalProcesses) {
        if (rank % (2 * step) == 0) {
            // Proceso par
            long long receivedValue;
            MPI_Recv(&receivedValue, 1, MPI_LONG_LONG, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            localValue += receivedValue; // Sumar el valor recibido
            printf("Proceso %d: recibió %lld de proceso %d, nueva suma local = %lld\n", rank, receivedValue, rank + step, localValue);
        } else if (rank % (2 * step) == step) {
            // Proceso impar
            MPI_Send(&localValue, 1, MPI_LONG_LONG, rank - step, 0, MPI_COMM_WORLD);
            printf("Proceso %d: envió %lld a proceso %d\n", rank, localValue, rank - step);
            break; // Salir del ciclo ya que no es necesario continuar
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
