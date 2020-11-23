#!/bin/bash
#SBATCH -N 1
#SBATCH -p RM
#SBATCH --ntasks-per-node 28
#SBATCH -t 5:00:00
# echo commands to stdout
set -x
# move to your appropriate pylon5 directory
# this job assumes:
# - all input data is stored in this directory
# - all output should be stored in this directory
cd /home/rsolorza/MatrixMultiply

module load mpi
# compile MPI program

make clean
make

# run MPI program
mpiexec -pernode mm 1
echo "-----------------------------"

mpiexec -pernode mm 2
echo "-----------------------------"

mpiexec -pernode mm 4
echo "-----------------------------"

mpiexec -pernode mm 8
echo "-----------------------------"

mpiexec -pernode mm 16
echo "-----------------------------"

mpiexec -pernode mm 28
echo "-----------------------------"
