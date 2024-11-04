#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 500 // Tamaño de las matrices

void multiply_matrices(double a[SIZE][SIZE], double b[SIZE][SIZE], double c[SIZE][SIZE], int rank, int size) {
    int i, j, k;
    // Cada proceso calcula las filas correspondientes
    for (i = rank; i < SIZE; i += size) {
        for (j = 0; j < SIZE; j++) {
            c[i][j] = 0; // Inicializar elemento de la matriz resultado
            for (k = 0; k < SIZE; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main(int argc, char **argv) {
    int rank, num_procs;
    double a[SIZE][SIZE], b[SIZE][SIZE], c[SIZE][SIZE];
    double local_a[SIZE][SIZE], local_c[SIZE][SIZE];

    // Inicializar el entorno MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Inicializar las matrices en el proceso 0
    if (rank == 0) {
        // Inicializar la matriz A
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                a[i][j] = i + j; // Puedes modificar la inicialización según tus necesidades
            }
        }

        // Inicializar la matriz B
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                b[i][j] = i - j; // Puedes modificar la inicialización según tus necesidades
            }
        }
    }

    // Broadcast de las matrices A y B a todos los procesos
    MPI_Bcast(b, SIZE * SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(a, SIZE * SIZE / num_procs, MPI_DOUBLE, local_a, SIZE * SIZE / num_procs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Multiplicar las matrices locales
    multiply_matrices(local_a, b, local_c, rank, num_procs);

    // Recolectar los resultados en la matriz C en el proceso 0
    MPI_Gather(local_c, SIZE * SIZE / num_procs, MPI_DOUBLE, c, SIZE * SIZE / num_procs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Imprimir la matriz resultado en el proceso 0
    if (rank == 0) {
        printf("Matriz Resultante C:\n");
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                printf("%f ", c[i][j]);
            }
            printf("\n");
        }
    }

    // Finalizar el entorno MPI
    MPI_Finalize();
    return 0;
}
