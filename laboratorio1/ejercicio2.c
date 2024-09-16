#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void multiply_matrices(int size, double** A, double** B, double** C) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0; // Inicializar el elemento C[i][j]
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


void initialize_matrix(int size, double** matrix) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 10; // Rellenar con valores aleatorios
        }
    }
}

void print_matrix(int size, double** matrix) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%5.1f ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int sizes[] = {100, 200, 300}; // Diferentes tamaños de matrices
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("num_sizes: %d\n", num_sizes);
    return 0;
}
