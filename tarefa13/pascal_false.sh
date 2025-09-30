#!/bin/bash
#SBATCH --job-name=pascal_false
#SBATCH --time=1-2:03
#SBATCH --cpus-per-task=64
#SBATCH --exclusive
#SBATCH --partition=amd-512

export OMP_NUM_THREADS=64
export OMP_PLACES=cores
export OMP_PROC_BIND=false

pascalanalyzer ./codigo \
    -c '1, 2, 4, 8, 16, 32, 64' \
    -i '50, 62, 79, 100, 125, 158, 200' \
    -o 'saida_false.json' \
    -r '3'