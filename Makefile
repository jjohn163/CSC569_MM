CC = gcc
CFLAGS = -Wall -Werror -g -std=c89 -fopenmp -O3
MPICC = mpic++
MPIFLAGS = -Werror -g -openmp -O3

mpi: mm.c
	$(MPICC) $(MPIFLAGS) -o mm $<

simple: mm.c
	$(CC) $(CFLAGS) -o mm $<

test: mpi
	module load mpi
	mpiexec -n 1 mm 1
	echo "-----------------------------"
	mpiexec -n 2 mm 2
	echo "-----------------------------"
	mpiexec -n 4 mm 4
	echo "-----------------------------"
	mpiexec -n 8 mm 8
	echo "-----------------------------"
	mpiexec -n 16 mm 16
	echo "-----------------------------"
	mpiexec -n 28 mm 28
	echo "-----------------------------"

clean:
	rm mm
