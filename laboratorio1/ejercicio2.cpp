#include <iostream>
#include <cstdlib>
#include <ctime>

/**
 * This function multiplies two square matrices of size N
 */
void multiplyMatrices(int** A, int** B, int** C, int N) {
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

/**
 *  This function prints a square matrix of size N
 */
void printMatrix(int** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // srand(static_cast<unsigned int>(time(0))); // seed for random numbers

    if (argc != 2) { // Check the number of arguments
        std::cerr << "Try: " << argv[0] << " <size_matrix>\n";
        std::cerr << "Tamaños válidos: 100, 200, 300, etc\n";
        return 1;
    }

    int N = std::atoi(argv[1]); // Convert the argument to integer

    // Allocate memory for the matrices
    int** A = allocateMatrix(N);
    int** B = allocateMatrix(N);
    int** C = allocateMatrix(N);

    // Fill the matrices with random values
    fillMatrix(A, N);
    fillMatrix(B, N);

    // Measure the time to multiply the matrices
    clock_t start = clock();
    multiplyMatrices(A, B, C, N);
    clock_t end = clock();

    // Print the time
    double cpuTimeUsed = ((double) (end - start)) / CLOCKS_PER_SEC;
    std::cout << "Execution time: " << cpuTimeUsed << " seconds\n";

    // Print the matrices
    // std::cout << "Matriz A:\n";
    // printMatrix(A, N);
    // std::cout << "\nMatriz B:\n";
    // printMatrix(B, N);
    // std::cout << "\nMatriz C (resultado):\n";
    // printMatrix(C, N);

    // Free the memory
    freeMatrix(A, N);
    freeMatrix(B, N);
    freeMatrix(C, N);
    
    return 0;
}
