import pandas as pd # type: ignore
import matplotlib.pyplot as plt # type: ignore

# 1. Dados extraídos
data = {
    'Processes': [2, 2, 2, 4, 4, 4, 8, 8, 8, 16, 16, 16, 32, 32, 32],
    'Version': [
        'Send/Recv', 'Isend/Wait', 'Isend/Test',
        'Send/Recv', 'Isend/Wait', 'Isend/Test',
        'Send/Recv', 'Isend/Wait', 'Isend/Test',
        'Send/Recv', 'Isend/Wait', 'Isend/Test',
        'Send/Recv', 'Isend/Wait', 'Isend/Test'
    ],
    'Time': [
        0.251648, 0.251508, 0.186613,
        0.161507, 0.129248, 0.095770,
        0.100079, 0.091738, 0.083034,
        0.066196, 0.071436, 0.052965,
        0.067758, 0.060072, 0.035814
    ]
}

# 2. Criar um DataFrame do pandas para facilitar a manipulação
df = pd.DataFrame(data)

# 3. Pivotar a tabela para que cada 'Versão' se torne uma coluna.
df_pivot = df.pivot(index='Processes', columns='Version', values='Time')

# Opcional: Reordenar as colunas para uma legenda mais lógica no gráfico
df_pivot = df_pivot[['Send/Recv', 'Isend/Wait', 'Isend/Test']]

print("Dados organizados para o plot:")
print(df_pivot)

# 4. Gerar o gráfico
# Define um estilo visual mais agradável para o gráfico
plt.style.use('seaborn-v0_8-whitegrid')

# Cria a figura e os eixos do gráfico, definindo o tamanho
fig, ax = plt.subplots(figsize=(10, 6))

# Plota os dados do DataFrame pivotado
df_pivot.plot(kind='line', marker='o', ax=ax)

# 5. Customizar o gráfico com títulos e legendas
ax.set_title('Desempenho da Difusão de Calor 1D com MPI', fontsize=16)
ax.set_xlabel('Número de Processos', fontsize=12)
ax.set_ylabel('Tempo de Execução (segundos)', fontsize=12)
ax.legend(title='Versão MPI')

# Garante que todos os pontos de dados no eixo x (2, 4, 8, 16, 32) sejam mostrados
ax.set_xticks(df_pivot.index)

# Adiciona uma grade para melhor visualização
ax.grid(True, which='both', linestyle='--', linewidth=0.5)

# 6. Salvar o gráfico em um arquivo de imagem
# O dpi=300 garante uma alta resolução
plt.savefig('mpi_performance_comparison.png', dpi=300)

# Mostra o gráfico na tela (se estiver executando em um ambiente com GUI)
plt.show()

print("\nGráfico 'mpi_performance_comparison.png' foi salvo com sucesso!")