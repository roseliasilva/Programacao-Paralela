#!/bin/bash
#SBATCH --job-name=pascalanalyzer_job
#SBATCH --time=1-2:03
#SBATCH --cpus-per-task=64
#SBATCH --exclusive
#SBATCH --partition=amd-512

export OMP_NUM_THREADS=64

pascalanalyzer ./codigo \
    -c '1, 2, 4, 8, 16, 32, 64' \
    -i '50, 62, 79, 100, 125, 158, 200' \
    -o 'saida.json' \
    -r '3' \