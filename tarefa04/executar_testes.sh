#!/bin/bash

echo "========================================="
echo "INICIANDO TESTE: Memory-Bound"
echo "========================================="

for threads in 1 2 4 8 12 16
do
  echo ""
  echo "--- Executando com $threads thread(s) ---"
  export OMP_NUM_THREADS=$threads
  # O comando 'time' medirá o tempo de execução do programa
  time ./memory_bound
done


echo ""
echo "========================================="
echo "INICIANDO TESTE: Compute-Bound"
echo "========================================="

for threads in 1 2 4 8 12 16
do
  echo ""
  echo "--- Executando com $threads thread(s) ---"
  export OMP_NUM_THREADS=$threads
  time ./compute_bound
done