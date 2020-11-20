CC = gcc
CFLAGS = -Wall -Werror -g -std=c89 -fopenmp -O3

simple: lab3.c
                $(CC) $(CFLAGS) -o mm $<

clean:
                rm mm