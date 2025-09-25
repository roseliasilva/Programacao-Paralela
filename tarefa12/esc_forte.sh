#!/bin/bash
#SBATCH --job-name=esc_forte
#sbatch --time=00:10:00
#SBATCH --partition=intel-128
#SBATCH --output=esc_forte.out

for threads in 1 2 4 8 16 32 64
do
    export OMP_NUM_THREADS=$threads
    echo "Executando com $threads threads"
    ./codigo
done