#include <iostream>
#include <cstdlib>
#include <ctime>

/**
 * Multiply two matrices A and B of size N using the classic matrix multiplication algorithm
 */
void multiplyMatricesClassic(int** A, int** B, int** C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0; // Inicializar el valor de la matriz resultante
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/**
 * Multiply two matrices A and B of size N using the block matrix multiplication algorithm
 */
void multiplyMatricesBlocked(int** A, int** B, int** C, int N, int blockSize) {
    for (int i = 0; i < N; i += blockSize) {
        for (int j = 0; j < N; j += blockSize) {
            for (int k = 0; k < N; k += blockSize) {
                // Multiplicación de subbloques
                for (int ii = i; ii < i + blockSize && ii < N; ii++) {
                    for (int jj = j; jj < j + blockSize && jj < N; jj++) {
                        for (int kk = k; kk < k + blockSize && kk < N; kk++) {
                            C[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

/** 
 * This function allocates memory for a square matrix of size N
 */
int** allocateMatrix(int N) {
    int** matrix = new int*[N]; // Crear un arreglo de punteros
    for (int i = 0; i < N; i++) {
        matrix[i] = new int[N]; // Asignar memoria para cada fila
    }
    return matrix;
}

/**
 * This function fills a square matrix of size N with random values
 */
void fillMatrix(int** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10; // Fill with random values
        }
    }
}

/**
 * This function frees the memory allocated for a square matrix of size N
 */
void freeMatrix(int** matrix, int N) {
    for (int i = 0; i < N; i++) {
        delete[] matrix[i]; // Free memory for each row
    }
    delete[] matrix; // Free memory for the array of pointers
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <tamaño_matriz> <tamaño_bloque>\n";
        return 1;
    }

    int N = std::atoi(argv[1]); // Size of the matrix
    int blockSize = std::atoi(argv[2]); // Size of the block

    int** A = allocateMatrix(N);
    int** B = allocateMatrix(N);
    int** C1 = allocateMatrix(N); // Matrix for classic multiplication
    int** C2 = allocateMatrix(N); // Matrix for blocked multiplication

    fillMatrix(A, N);
    fillMatrix(B, N);

    // Measure time for classic multiplication
    clock_t startClassic = clock();
    multiplyMatricesClassic(A, B, C1, N);
    clock_t endClassic = clock();
    double timeClassic = static_cast<double>(endClassic - startClassic) / CLOCKS_PER_SEC;
    std::cout << "Execution time for classic multiplication: " << timeClassic << " seconds\n";

    // Measure time for blocked multiplication
    clock_t startBlocked = clock();
    multiplyMatricesBlocked(A, B, C2, N, blockSize);
    clock_t endBlocked = clock();
    double timeBlocked = static_cast<double>(endBlocked - startBlocked) / CLOCKS_PER_SEC;
    std::cout << "Execution time for blocked multiplication: " << timeBlocked << " seconds\n";

    // Free memory
    freeMatrix(A, N);
    freeMatrix(B, N);
    freeMatrix(C1, N);
    freeMatrix(C2, N);

    return 0;
}
