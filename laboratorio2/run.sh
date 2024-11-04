!#/bin/bash
mpicc -o main main.c
scp ./main leon@10.x.x.x:/home/leon/

mpirun -np 2 --hostfile hosts ./main
