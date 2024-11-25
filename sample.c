// Primer bucle: Enviar mensajes y, al mismo tiempo, intentar recibirlos
for (sent_msgs = 0; sent_msgs < send_max; sent_msgs++) {
    // Envía un mensaje a otro hilo
    Send_msg();
    
    // Intenta recibir un mensaje (si hay alguno disponible)
    Try_receive();
}

// Segundo bucle: Continuar recibiendo mensajes hasta que todos hayan terminado de enviar
while (!Done()) {
    // Intenta recibir un mensaje (si hay alguno disponible)
    Try_receive();
}


mesg = random(); // Generar un mensaje aleatorio
dest = random() % thread_count; // Generar un destino aleatorio
#pragma omp critical
Enqueue(queue, dest, my_rank, mesg); // Encolar el mensaje de forma segura



queue_size = enqueued − dequeued;
if (queue_size == 0) return; // No hay mensajes
else if (queue_size == 1) {
    #pragma omp critical
    Dequeue(queue, &src, &mesg); // Desencolar con sincronización
} else {
    Dequeue(queue, &src, &mesg); // Desencolar sin necesidad de sincronización
}
Print_message(src, mesg); // Imprimir el mensaje recibido


queue_size = enqueued − dequeued;
if (queue_size == 0 && done_sending == thread_count)
    return TRUE; // Todos los hilos han terminado
else
    return FALSE; // Aún quedan mensajes o hilos enviando


omp_set_lock(&q->lock);    // Bloquear la cola antes de acceder
Enqueue(q, my_rank, mesg); // Operación crítica
omp_unset_lock(&q->lock);  // Desbloquear después de acceder


for (phase = 0; phase < n; phase++) {          // Para cada fase desde 0 hasta n-1
    if (phase % 2 == 0) {                      // Si la fase es par (0, 2, 4, ...)
        #pragma omp parallel for num_threads(thread_count) \
        default(none) shared(a, n) private(i, tmp) // Paralelizar el bucle usando OpenMP con un número específico de hilos
        for (i = 1; i < n; i += 2) {           // Iterar sobre los índices impares (1, 3, 5, ...)
            if (a[i−1] > a[i]) {               // Si el elemento anterior es mayor que el actual
                tmp = a[i−1];                  // Almacenar temporalmente el valor de a[i-1]
                a[i−1] = a[i];                 // Asignar a[i] a a[i-1]
                a[i] = tmp;                    // Asignar el valor temporal a a[i]
            }
        }
    } else {                                   // Si la fase es impar (1, 3, 5, ...)
        #pragma omp parallel for num_threads(thread_count) \
        default(none) shared(a, n) private(i, tmp) // Paralelizar el bucle usando OpenMP con un número fijo de hilos
        for (i = 1; i < n−1; i += 2) {         // Iterar sobre los índices pares (0, 2, 4, ...)
            if (a[i] > a[i+1]) {               // Si el elemento actual es mayor que el siguiente
                tmp = a[i+1];                  // Almacenar temporalmente el valor de a[i+1]
                a[i+1] = a[i];                 // Asignar a[i] a a[i+1]
                a[i] = tmp;                    // Asignar el valor temporal a a[i]
            }
        }
    }
}



/* q_p = msg_queues[my_rank] */
omp_set_lock(&q_p->lock);
Dequeue(q_p, &src, &mesg);
omp_unset_lock(&q_p->lock);


