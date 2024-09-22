#!/bin/bash

# Comprobar si se proporcionaron los argumentos necesarios
if [ "$#" -lt 2 ]; then
    echo "Uso: $0 <nombre_del_programa> <tamaño_matriz_1> [<tamaño_matriz_2> ... <tamaño_matriz_n>]"
    exit 1
fi

# Asignar el nombre del programa y los tamaños de matriz a variables
PROGRAM=$1
shift
SIZES=("$@")

# Compilar el programa
g++ -o "$PROGRAM" "$PROGRAM.cpp"

# Comprobar si la compilación fue exitosa
if [ $? -ne 0 ]; then
    echo "Error en la compilación."
    exit 1
fi

# Ejecutar el programa para cada tamaño de matriz
for SIZE in "${SIZES[@]}"; do
    echo "Ejecutando $PROGRAM con tamaño de matriz $SIZE..."
    ./"$PROGRAM" "$SIZE"
done
