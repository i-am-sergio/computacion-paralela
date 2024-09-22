#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int MAX = 1000; // Matrix size
double A[MAX][MAX], x[MAX], y[MAX]; // Matrix and vectors

/*
* This function initializes the matrix A and vectors x and y with random values
*/
void initialize() {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            A[i][j] = rand() % 10;
        }
        x[i] = rand() % 10;
        y[i] = 0;
    }
}

/*
* This function calculates the matrix-vector product using the first pair of loops
* The first loop iterates over the rows of the matrix and the second loop iterates over the columns
*/
void first_loop() {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

/*
* This function calculates the matrix-vector product using the second pair of loops
* The first loop iterates over the columns of the matrix and the second loop iterates over the rows
*/
void second_loop() {
    for (int j = 0; j < MAX; j++) {
        for (int i = 0; i < MAX; i++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

int main() {
    clock_t start, end; // Variables to measure time
    double cpu_time_used; // Variable to store the time

    initialize(); // Initialize matrix and vectors

    // First pair of loops
    start = clock();
    first_loop();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("First loop time: %f seconds\n", cpu_time_used);

    // Reset y array
    for (int i = 0; i < MAX; i++) {
        y[i] = 0;
    }

    // Second pair of loops
    start = clock();
    second_loop();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Second loop time: %f seconds\n", cpu_time_used);

    return 0;
}

