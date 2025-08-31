#!/bin/bash

# --- Script para compilar e testar programas OpenMP ---

# Cores para uma saída mais legível
BLUE='\033[0;34m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # Sem Cor

# Lista de números de threads para testar
THREADS_LIST="1 2 3 4 5 6 7 8 9 10 11 12"

echo -e "${BLUE}--- Iniciando a compilação dos programas ---${NC}"

# Compila o programa memory-bound
gcc -fopenmp memory_bound.c -o memory_bound
if [ $? -ne 0 ]; then
    echo "Erro na compilação do memory_bound.c"
    exit 1
fi

# Compila o programa compute-bound
gcc -fopenmp compute_bound.c -o compute_bound -lm
if [ $? -ne 0 ]; then
    echo "Erro na compilação do compute_bound.c"
    exit 1
fi

echo -e "${GREEN}Programas compilados com sucesso!${NC}"
echo ""


# --- Teste do Programa Memory-Bound ---
echo -e "${YELLOW}===========================================${NC}"
echo -e "${YELLOW}    INICIANDO TESTE: Memory-Bound        ${NC}"
echo -e "${YELLOW}===========================================${NC}"

for num_threads in $THREADS_LIST
do
  echo -e "\n${BLUE}Executando memory_bound com ${YELLOW}$num_threads thread(s)...${NC}"
  export OMP_NUM_THREADS=$num_threads
  # O comando `time` é executado em um subshell para formatar a saída
  { time ./memory_bound; } 2>&1
done


# --- Teste do Programa Compute-Bound ---
echo -e "\n${YELLOW}===========================================${NC}"
echo -e "${YELLOW}    INICIANDO TESTE: Compute-Bound       ${NC}"
echo -e "${YELLOW}===========================================${NC}"

for num_threads in $THREADS_LIST
do
  echo -e "\n${BLUE}Executando compute_bound com ${YELLOW}$num_threads thread(s)...${NC}"
  export OMP_NUM_THREADS=$num_threads
  { time ./compute_bound; } 2>&1
done


# --- Limpeza ---
echo -e "\n${BLUE}--- Limpando arquivos executáveis ---${NC}"
rm memory_bound compute_bound
echo "Testes concluídos."