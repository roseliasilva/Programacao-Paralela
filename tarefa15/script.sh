#!/bin/bash 
#SBATCH --job-name=diffusao_mpi 
#SBATCH --output=saida%j.out 
#SBATCH --error=erro%j.err 
#SBATCH --partition=amd-512
#SBATCH --nodes=1 
#SBATCH --ntasks-per-node=32
#SBATCH --time=0-0:50 

# Compila os três códigos
mpicc -O2 -o diffusao_sendrecv diffusao_sendrecv.c 
mpicc -O2 -o diffusao_isendwait diffusao_isendwait.c 
mpicc -O2 -o diffusao_isendtest diffusao_isendtest.c 

echo "Iniciando benchmarks de difusão de calor com MPI..."
# Loop para executar com 2, 4, 8 e 16 processos
for np in 2 4 8 16 32; do 
    echo "Executando com $np processos..." 
    mpirun -np $np ./diffusao_sendrecv 
    mpirun -np $np ./diffusao_isendwait 
    mpirun -np $np ./diffusao_isendtest 
    echo "-----------------------------" 
done