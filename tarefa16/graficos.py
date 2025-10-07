import pandas as pd # type: ignore
import matplotlib.pyplot as plt # type: ignore
import seaborn as sns # type: ignore

def gerar_graficos(caminho_csv='resultados.csv'):
    """
    Função para ler os dados de desempenho e gerar os três gráficos de análise.
    
    Argumentos:
        caminho_csv (str): O nome do arquivo CSV com os resultados.
    """
    try:
        # Tenta carregar o arquivo CSV para um DataFrame do pandas
        df = pd.read_csv(caminho_csv)
    except FileNotFoundError:
        print(f"Erro: O arquivo '{caminho_csv}' não foi encontrado.")
        print("Por favor, verifique se o arquivo está no mesmo diretório que o script.")
        return

    # Define um estilo visual mais agradável para os gráficos
    sns.set_theme(style="whitegrid")

    # --- Gráfico 1: Tempo de Execução vs. Tamanho da Matriz ---
    print("Gerando Gráfico 1: Tempo vs. Tamanho da Matriz...")
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=df, x='M', y='Tempo_s', hue='Processos', marker='o', palette='viridis')
    plt.title('Tempo de Execução vs. Tamanho da Matriz', fontsize=16)
    plt.xlabel('Tamanho da Matriz (M=N)', fontsize=12)
    plt.ylabel('Tempo de Execução (segundos)', fontsize=12)
    plt.xscale('log') # Escala logarítmica para melhor visualização
    plt.yscale('log')
    # Garante que os ticks do eixo X correspondam aos tamanhos de matriz testados
    plt.xticks(df['M'].unique(), labels=df['M'].unique())
    plt.legend(title='Nº de Processos')
    plt.tight_layout()
    plt.savefig('tempo_vs_tamanho_matriz.png')
    plt.close() # Fecha a figura para liberar memória

    # --- Gráfico 2: Tempo de Execução vs. Número de Processos ---
    print("Gerando Gráfico 2: Tempo vs. Número de Processos...")
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=df, x='Processos', y='Tempo_s', hue='M', marker='o', palette='plasma')
    plt.title('Tempo de Execução vs. Número de Processos', fontsize=16)
    plt.xlabel('Número de Processos', fontsize=12)
    plt.ylabel('Tempo de Execução (segundos)', fontsize=12)
    plt.xscale('log', base=2) # Escala logarítmica na base 2 para processos
    plt.yscale('log')
    # Garante que os ticks do eixo X correspondam ao número de processos
    plt.xticks(df['Processos'].unique(), labels=df['Processos'].unique())
    plt.legend(title='Tamanho da Matriz (M)')
    plt.tight_layout()
    plt.savefig('tempo_vs_processos.png')
    plt.close()

    # --- Gráfico 3: Speedup vs. Número de Processos ---
    print("Gerando Gráfico 3: Speedup vs. Número de Processos...")
    
    # Para calcular o Speedup, precisamos do tempo sequencial (T com 1 processo).
    # Como os dados começam com 2 processos, estimamos o tempo sequencial (T1)
    # como sendo o dobro do tempo com 2 processos (T2).
    # Speedup = T1 / Tp
    
    df_speedup = df.copy()
    
    # Encontra o tempo de execução para 2 processos para cada tamanho de matriz
    tempos_base_2_procs = df_speedup[df_speedup['Processos'] == 2].set_index('M')['Tempo_s']

    # Estima o tempo sequencial T(1) = 2 * T(2) e mapeia para o dataframe
    df_speedup['Tempo_Base_Estimado'] = df_speedup['M'].map(tempos_base_2_procs) * 2
    
    # Calcula o speedup para cada linha
    df_speedup['Speedup'] = df_speedup['Tempo_Base_Estimado'] / df_speedup['Tempo_s']

    plt.figure(figsize=(10, 7))

    # Plota o Speedup calculado para cada tamanho de matriz
    sns.lineplot(data=df_speedup, x='Processos', y='Speedup', hue='M', marker='o', palette='coolwarm')

    # Plota a linha de "Speedup Ideal" (uma reta y=x) para comparação
    processos = sorted(df['Processos'].unique())
    plt.plot(processos, processos, 'k--', label='Speedup Ideal')

    plt.title('Speedup vs. Número de Processos', fontsize=16)
    plt.xlabel('Número de Processos', fontsize=12)
    plt.ylabel('Speedup (T_seq_estimado / T_paralelo)', fontsize=12)
    plt.xticks(processos, labels=processos)
    plt.legend(title='Tamanho da Matriz (M)')
    plt.grid(True, which="both", ls="--")
    plt.tight_layout()
    plt.savefig('speedup_vs_processos.png')
    plt.close()

    print("\nGráficos gerados com sucesso e salvos como arquivos .png!")

if __name__ == '__main__':
    gerar_graficos()