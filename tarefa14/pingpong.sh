#!/bin/bash
#SBATCH --job-name=pingpong_mpi  
#SBATCH --output=saida%j.out
#SBATCH --error=erro%j.err
#SBATCH --partition=amd-512
#SBATCH --nodes=1
#SBATCH --exclusive
#SBATCH --ntasks-per-node=2
#SBATCH --time=0-0:20

# Compila o programa C com o compilador MPI
mpicc -o pingpong pingpong.c

# Executa o programa compilado com 2 processos
mpirun -np 2 ./pingpong