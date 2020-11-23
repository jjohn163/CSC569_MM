CC = gcc
CFLAGS = -Wall -Werror -g -std=c89 -fopenmp -O3
MPICC = mpicc
MPIFLAGS = -g -mp -O3

mpi: mm.c
	$(MPICC) $(MPIFLAGS) -o mm $<

simple: mm.c
	$(CC) $(CFLAGS) -o mm $<

clean:
	rm mm
