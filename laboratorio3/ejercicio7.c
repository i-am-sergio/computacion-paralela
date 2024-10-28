#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, size;
    const int PING_PONG_COUNT = 1000; // Número de ping-pong a realizar
    double startTime, endTime; // Variables para el tiempo con MPI_Wtime
    clock_t startClock, endClock; // Variables para el tiempo con clock
    int pingPongMessage = 0; // Mensaje que se envía
    int partnerRank; // Rango del proceso socio

    // Inicialización de MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Asegúrate de que solo hay dos procesos
    if (size != 2) {
        if (rank == 0) {
            fprintf(stderr, "Este programa requiere exactamente 2 procesos.\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // Definir el rango del socio
    partnerRank = (rank + 1) % 2; // Proceso 0 se comunica con 1 y viceversa

    // Medición de tiempo con MPI_Wtime
    startTime = MPI_Wtime();
    for (int i = 0; i < PING_PONG_COUNT; i++) {
        if (rank == 0) {
            printf("Proceso %d: Enviando ping %d a proceso %d.\n", rank, pingPongMessage, partnerRank);
            MPI_Send(&pingPongMessage, 1, MPI_INT, partnerRank, 0, MPI_COMM_WORLD);
            MPI_Recv(&pingPongMessage, 1, MPI_INT, partnerRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Proceso %d: Recibido pong %d de proceso %d.\n", rank, pingPongMessage, partnerRank);
        } else {
            MPI_Recv(&pingPongMessage, 1, MPI_INT, partnerRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Proceso %d: Recibido ping %d de proceso %d.\n", rank, pingPongMessage, partnerRank);
            MPI_Send(&pingPongMessage, 1, MPI_INT, partnerRank, 0, MPI_COMM_WORLD);
            printf("Proceso %d: Enviando pong %d a proceso %d.\n", rank, pingPongMessage, partnerRank);
        }
    }
    endTime = MPI_Wtime();

    // Medición de tiempo con clock
    startClock = clock();
    for (int i = 0; i < PING_PONG_COUNT; i++) {
        if (rank == 0) {
            MPI_Send(&pingPongMessage, 1, MPI_INT, partnerRank, 0, MPI_COMM_WORLD);
            MPI_Recv(&pingPongMessage, 1, MPI_INT, partnerRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(&pingPongMessage, 1, MPI_INT, partnerRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&pingPongMessage, 1, MPI_INT, partnerRank, 0, MPI_COMM_WORLD);
        }
    }
    endClock = clock();

    // Resultados
    if (rank == 0) {
        printf("Tiempo total (MPI_Wtime) para %d ping-pong: %f segundos.\n", PING_PONG_COUNT, endTime - startTime);
        printf("Tiempo total (clock) para %d ping-pong: %f segundos.\n", PING_PONG_COUNT, (double)(endClock - startClock) / CLOCKS_PER_SEC);
    }

    // Finalización de MPI
    MPI_Finalize();
    return EXIT_SUCCESS;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <mpi.h>
// #include <time.h>

// #define NUM_PINGS 1000000

// int main(int argc, char *argv[]) {
//     int rank, size;
//     MPI_Status status;
    
//     // Inicializar MPI
//     MPI_Init(&argc, &argv);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     // Asegurarse de que hay exactamente 2 procesos
//     if (size != 2) {
//         fprintf(stderr, "Este programa debe ser ejecutado con 2 procesos.\n");
//         MPI_Abort(MPI_COMM_WORLD, 1);
//     }

//     if (rank == 0) {
//         // Proceso A
//         clock_t start_clock = clock(); // Tiempo de inicio con clock
//         double start_mpi = MPI_Wtime(); // Tiempo de inicio con MPI_Wtime

//         for (int i = 0; i < NUM_PINGS; i++) {
//             MPI_Send(NULL, 0, MPI_BYTE, 1, 0, MPI_COMM_WORLD); // Enviar ping
//             MPI_Recv(NULL, 0, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &status); // Recibir pong
//         }

//         clock_t end_clock = clock(); // Tiempo de fin con clock
//         double end_mpi = MPI_Wtime(); // Tiempo de fin con MPI_Wtime

//         // Calcular y mostrar tiempos
//         double cpu_time = (double)(end_clock - start_clock) / CLOCKS_PER_SEC;
//         double real_time = end_mpi - start_mpi;

//         printf("Proceso A:\n");
//         printf("Tiempo CPU (clock): %f segundos\n", cpu_time);
//         printf("Tiempo real (MPI_Wtime): %f segundos\n", real_time);
//     } else if (rank == 1) {
//         // Proceso B
//         for (int i = 0; i < NUM_PINGS; i++) {
//             MPI_Recv(NULL, 0, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status); // Recibir ping
//             MPI_Send(NULL, 0, MPI_BYTE, 0, 0, MPI_COMM_WORLD); // Enviar pong
//         }
//     }

//     // Finalizar MPI
//     MPI_Finalize();
//     return 0;
// }
