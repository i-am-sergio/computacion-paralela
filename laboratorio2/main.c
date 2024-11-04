#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4  // Tamaño de la matriz N x N

void printMatrix(int matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int rank, size, i, j, k;
    int A[N][N], B[N][N], C[N][N] = {0};  // Matrices A, B y C (resultado)

    // Inicializar MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtener el ID del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obtener el número total de procesos

    // Imprimir el proceso en ejecución desde cada máquina
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    printf("Proceso %d ejecutado en %s\n", rank, processor_name);

    // Inicializar matrices A y B en el proceso maestro (rank 0)
    if (rank == 0) {
        printf("Matriz A:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                A[i][j] = i + j;  // Puedes usar cualquier lógica para inicializar
                B[i][j] = i * j;
            }
        }
        printMatrix(A);
        printf("Matriz B:\n");
        printMatrix(B);
    }

    // Enviar las matrices A y B a todos los procesos
    MPI_Bcast(A, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso calcula una parte de la matriz C
    for (i = rank; i < N; i += size) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Reunir los resultados de la matriz C en el proceso maestro
    MPI_Gather(C, N * N / size, MPI_INT, C, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    // El proceso maestro imprime el resultado final
    if (rank == 0) {
        printf("Resultado de la multiplicación de matrices C:\n");
        printMatrix(C);
    }

    // Finalizar MPI
    MPI_Finalize();
    return 0;
}
