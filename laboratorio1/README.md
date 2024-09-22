# Laboratorio 1: Pruebas sobre el Comportamiento de la Memoria Caché

## Introducción

Este laboratorio tiene como objetivo analizar el comportamiento de la memoria caché en el contexto de la multiplicación de matrices. Se implementaron y compararon dos algoritmos de multiplicación de matrices: el algoritmo clásico y el algoritmo por bloques. Utilizando herramientas como Valgrind y KCachegrind, se evaluó el rendimiento de cada algoritmo en función de su uso de la memoria caché.

## Objetivos

1. **Comparar el rendimiento** de los algoritmos de multiplicación de matrices clásico y por bloques.
2. **Analizar el comportamiento de la memoria caché** al ejecutar ambos algoritmos, identificando la eficiencia en el uso de la memoria.
3. **Proporcionar resultados cuantitativos** sobre el tiempo de ejecución y la cantidad de fallos de caché.

## Implementación

Se desarrollaron dos funciones para realizar la multiplicación de matrices:

- `multiplyMatricesClassic`: Implementa la multiplicación de matrices clásica.
- `multiplyMatricesBlocked`: Implementa la multiplicación de matrices utilizando bloques para mejorar la localización de la memoria.

## Herramientas Utilizadas

- **Valgrind**: Se utilizó para medir el tiempo de ejecución y los eventos de memoria.
- **KCachegrind**: Se utilizó para visualizar el uso de la memoria caché y analizar el rendimiento de los algoritmos.

## Resultados

Los resultados obtenidos muestran que:

- El algoritmo por bloques es más eficiente en términos de uso de caché y rendimiento, especialmente en matrices de gran tamaño.
- La gestión efectiva del movimiento de datos entre la memoria principal y la caché resulta en un menor número de fallos de caché.

## Conclusiones

El laboratorio permitió observar claramente cómo el diseño del algoritmo afecta el rendimiento debido a la arquitectura de la memoria. Se confirmó que la optimización del acceso a la memoria es crucial para el rendimiento en aplicaciones que manejan grandes volúmenes de datos.

## Requisitos

Para compilar y ejecutar el código, asegúrate de tener instalados los siguientes requisitos:

- Compilador de C++ (por ejemplo, `g++`)
- Valgrind
- KCachegrind

## Ejecución

Puedes compilar el código utilizando el siguiente comando:

```bash
g++ -o ejercicio3 ejercicio3.cpp
```

