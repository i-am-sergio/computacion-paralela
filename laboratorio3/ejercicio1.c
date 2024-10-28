// Ejercicio 1
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/*
    * Función para encontrar el bin correspondiente a un valor.
    * Parámetros:
    * - valor: valor a buscar en los bins.
    * - maximosBins: arreglo de valores máximos de los bins.
    * - numBins: número de bins.
    * - valorMin: valor mínimo de los datos.
    * Retorna:
    * - El índice del bin correspondiente al valor.
*/
int encontrarBin(float valor, float *maximosBins, int numBins, float valorMin) {
    for (int b = 0; b < numBins; b++) {
        if (valor < maximosBins[b]) {
            return b;
        }
    }
    return numBins - 1;
}

void printVector(float *vector, int size) {
    for (int i = 0; i < size; i++) {
        printf("%0.1f ", vector[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int rank, totalProcesos, cuentaDatos, numBins; // Identificación de rango y total de procesos
    float valorMin, valorMax, anchoBin; // Parámetros del histograma
    float * datos = NULL; // Datos de entrada 
    float * maximosBins = NULL; // Valores máximos de los bins: límites superiores de los bins
    int * conteoBinsGlobal = NULL; 
    int * conteoBinsLocal = NULL;
    int datosPorProceso, datosResto;

    MPI_Init(&argc, &argv); // Inicializar el entorno MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtener el rank: identificador único de cada proceso
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesos); // Identificar el número total de procesos

    printf("Proceso %d: iniciado.\n", rank);

    // Solo el proceso 0 lee y distribuye los datos
    if (rank == 0) {
        printf("Proceso %d: configurando datos y parámetros del histograma.\n", rank);

        // Configuración de los datos y parámetros del histograma
        cuentaDatos = 20; // Ejemplo: 20 datos
        valorMin = 0.0;
        valorMax = 5.0;
        numBins = 5;
        anchoBin = (valorMax - valorMin) / numBins;

        // Datos de ejemplo
        datos = (float *)malloc(cuentaDatos * sizeof(float));
        float datosEjemplo[] = {1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3, 4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9};
        for (int i = 0; i < cuentaDatos; i++) {
            datos[i] = datosEjemplo[i];
        }

        // Inicializar maximosBins
        maximosBins = (float *)malloc(numBins * sizeof(float));
        for (int b = 0; b < numBins; b++) {
            maximosBins[b] = valorMin + anchoBin * (b + 1);
        }
        printf("Proceso %d: datos y bins configurados.\n", rank);
    }

    // Distribuir parámetros del histograma a todos los procesos
    MPI_Bcast(&cuentaDatos, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&valorMin, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&valorMax, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&numBins, 1, MPI_INT, 0, MPI_COMM_WORLD);
    anchoBin = (valorMax - valorMin) / numBins;

    if (rank != 0) {
        maximosBins = (float *)malloc(numBins * sizeof(float));
    }
    // Distribuir los valores máximos de los bins a todos los procesos
    MPI_Bcast(maximosBins, numBins, MPI_FLOAT, 0, MPI_COMM_WORLD);

    printf("Proceso %d: recibió parámetros del histograma (min, max, bins).\n", rank);

    // Calcular los datos por proceso
    datosPorProceso = cuentaDatos / totalProcesos;
    datosResto = cuentaDatos % totalProcesos;

    int cuentaLocal = (rank < datosResto) ? datosPorProceso + 1 : datosPorProceso;
    float *datosLocal = (float *)malloc(cuentaLocal * sizeof(float));

    int *desplazamientos = NULL, *cuentas = NULL;
    if (rank == 0) {
        desplazamientos = (int *)malloc(totalProcesos * sizeof(int));
        cuentas = (int *)malloc(totalProcesos * sizeof(int));
        int offset = 0;
        for (int i = 0; i < totalProcesos; i++) {
            cuentas[i] = (i < datosResto) ? datosPorProceso + 1 : datosPorProceso;
            desplazamientos[i] = offset;
            offset += cuentas[i];
        }
        printf("Proceso %d: calculó desplazamientos y cuentas para Scatterv.\n", rank);
    }

    // Distribuir los datos a todos los procesos
    MPI_Scatterv(datos, cuentas, desplazamientos, MPI_FLOAT, datosLocal, cuentaLocal, MPI_FLOAT, 0, MPI_COMM_WORLD);
    printf("Proceso %d: recibió sus datos locales (total %d datos).\n", rank, cuentaLocal);

    // Ver los datos que le tocó a cada proceso
    printf("Proceso %d: datos locales = ", rank);
    printVector(datosLocal, cuentaLocal);

    // Inicializar conteo de bins local
    conteoBinsLocal = (int *)calloc(numBins, sizeof(int));
    for (int i = 0; i < cuentaLocal; i++) {
        int bin = encontrarBin(datosLocal[i], maximosBins, numBins, valorMin);
        conteoBinsLocal[bin]++;
    }
    printf("Proceso %d: completó el conteo de bins local.\n", rank);

    // Reducir los conteos locales en el proceso 0
    if (rank == 0) {
        conteoBinsGlobal = (int *)calloc(numBins, sizeof(int));
    }

    /*
        * Se utiliza MPI_Reduce para sumar los conteos locales en el proceso 0.
        * Parámetros:
        * - conteoBinsLocal: arreglo de conteos locales
        * - conteoBinsGlobal: arreglo de conteos globales.
        * - numBins: número de bins.
        * - MPI_INT: tipo de dato de los conteos.
        * - MPI_SUM: operación de reducción.
        * - 0: rango del proceso que recibe los conteos globales.
        * - MPI_COMM_WORLD: comunicador.
    */
    MPI_Reduce(conteoBinsLocal, conteoBinsGlobal, numBins, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Proceso %d: ha completado la reducción de conteos locales y generará el histograma.\n", rank);
        printf("Histograma:\n");
        for (int b = 0; b < numBins; b++) {
            printf("Bin %d [%0.1f - %0.1f): %d\n", b, (b == 0 ? valorMin : maximosBins[b - 1]), maximosBins[b], conteoBinsGlobal[b]);
        }
    }

    // Liberar memoria
    free(datosLocal);
    free(conteoBinsLocal);
    if (rank == 0) {
        free(datos);
        free(maximosBins);
        free(conteoBinsGlobal);
        free(desplazamientos);
        free(cuentas);
    }

    MPI_Finalize(); // Finalizar el entorno MPI
    printf("Proceso %d: finalizado.\n", rank);

    return 0;
}
