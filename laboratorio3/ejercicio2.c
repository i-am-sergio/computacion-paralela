#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// Función para generar un número aleatorio entre -1 y 1
double generarAleatorio() {
    return (double)rand() / RAND_MAX * 2.0 - 1.0;
}

int main(int argc, char* argv[]) {
    int rank, totalProcesos;
    long long totalLanzamientos, lanzamientosLocales, conteoCirculoLocal = 0, conteoCirculoGlobal;
    double x, y, distanciaCuadrada, piEstimado;

    // Inicialización de MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtener el rank: identificador unique de cada proceso
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesos); // Obtener el número total de procesos

    // Proceso 0 solicita al usuario el número total de lanzamientos
    if (rank == 0) {
        printf("Ingrese el número total de lanzamientos: ");
        scanf("%lld", &totalLanzamientos);
    }

    // Transmitir el número total de lanzamientos a todos los procesos
    MPI_Bcast(&totalLanzamientos, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Calcular el número de lanzamientos que cada proceso debe realizar
    lanzamientosLocales = totalLanzamientos / totalProcesos;

    // Inicializar la semilla aleatoria
    srand(time(NULL) + rank); // Semilla única para cada proceso

    // Cada proceso lanza sus dardos y cuenta cuántos caen dentro del círculo
    for (long long i = 0; i < lanzamientosLocales; i++) {
        x = generarAleatorio();
        y = generarAleatorio();
        distanciaCuadrada = x * x + y * y;
        if (distanciaCuadrada <= 1.0) {
            conteoCirculoLocal++;
        }
    }

    // Reducir el conteo local de cada proceso en un conteo global en el proceso 0
    MPI_Reduce(&conteoCirculoLocal, &conteoCirculoGlobal, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // Proceso 0 calcula y muestra la estimación de pi
    if (rank == 0) {
        piEstimado = 4.0 * (double)conteoCirculoGlobal / (double)totalLanzamientos;
        printf("Estimación de π con %lld lanzamientos: %f\n", totalLanzamientos, piEstimado);
    }

    // Finalización de MPI
    MPI_Finalize();
    return 0;
}
