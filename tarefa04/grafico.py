import pandas as pd # type: ignore
import matplotlib.pyplot as plt # type: ignore

# --- Carregar os dados do arquivo CSV ---
try:
    df = pd.read_csv('resultados.csv')
except FileNotFoundError:
    print("Erro: Arquivo 'resultados.csv' não encontrado.")
    print("Por favor, crie o arquivo com seus dados antes de rodar o script.")
    exit()

# --- Gráfico 1: Tempo de Execução vs. Número de Threads ---
plt.figure(figsize=(12, 7))
plt.plot(df['Threads'], df['Tempo_MemoryBound'], marker='o', linestyle='-', label='Memory-Bound')
plt.plot(df['Threads'], df['Tempo_ComputeBound'], marker='s', linestyle='--', color='red', label='Compute-Bound (com Condição de Corrida)')

plt.title('Tempo de Execução vs. Número de Threads')
plt.xlabel('Número de Threads')
plt.ylabel('Tempo de Execução (segundos)')
plt.xticks(df['Threads'])
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.legend()

# Salvar o gráfico em um arquivo
plt.savefig('grafico_tempo_execucao_problema.png')
print("Gráfico 'grafico_tempo_execucao_problema.png' salvo com sucesso.")