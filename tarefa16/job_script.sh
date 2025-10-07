#!/bin/bash
#SBATCH --job-name=matvec_mpi_testes
#SBATCH --output=saida_%j.out
#SBATCH --error=erro_%j.err
#SBATCH --partition=intel-128
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --time=0-0:50

# Define o nome do arquivo de resultados
OUTPUT_CSV="resultados.csv"

# Compila o código C com flag de otimização
echo "Compilando o código matvec_mpi.c..."
mpicc -O3 -o matvec_mpi matvec_mpi.c
echo "Compilação concluída."
echo ""

# Cria o cabeçalho do arquivo CSV
echo "M,N,Processos,Tempo_s" > $OUTPUT_CSV

# Loop para os tamanhos da matriz (M=N)
for size in 512 1024 2048 4096 8192; do
    # Loop para o número de processos
    for np in 2 4 8 16 32; do
        # Garante que o número de linhas é divisível pelo número de processos
        if [ $(($size % $np)) -eq 0 ]; then
            echo "Executando: Matriz ${size}x${size} com $np processos..."
            
            # Executa o programa MPI e anexa a saída formatada ao arquivo CSV
            # O programa C foi feito para imprimir "M,N,np,tempo"
            mpirun -np $np ./matvec_mpi $size $size >> $OUTPUT_CSV
        else
            echo "Pulando: Matriz ${size}x${size} com $np processos (não divisível)."
        fi
    done
    echo "---------------------------------------------"
done

echo "Todos os testes foram concluídos."
echo "Resultados salvos em $OUTPUT_CSV"