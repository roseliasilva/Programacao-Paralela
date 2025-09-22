import pandas as pd # type: ignore
import matplotlib.pyplot as plt # type: ignore
import numpy as np # type: ignore

# Dados dos resultados
data = {
    'Versão': [
        '1. Comp. Critical',
        '2. Comp. Atomic',
        '3. Priv. Critical',
        '4. Priv. Atomic',
        '5. Reduction'
    ],
    'Tempo (s)': [47.481081, 5.876946, 0.258147, 0.265687, 0.255684]
}

# Cria um DataFrame do pandas com os dados
df = pd.DataFrame(data)

# Ordena os dados pelo tempo para melhor visualização no gráfico
df_sorted = df.sort_values(by='Tempo (s)', ascending=False)

# --- Criação do Gráfico ---

# Define um estilo visual para o gráfico
plt.style.use('seaborn-v0_8-whitegrid')
fig, ax = plt.subplots(figsize=(12, 7))

# Cria as barras com cores distintas para cada categoria de desempenho
bars = ax.bar(df_sorted['Versão'], df_sorted['Tempo (s)'], color=[
              '#d9534f', '#f0ad4e', '#5cb85c', '#5cb85c', '#428bca'])

# Define a escala do eixo Y como logarítmica para visualizar todas as barras
ax.set_yscale('log')

# Adiciona os rótulos dos eixos e o título do gráfico
ax.set_ylabel('Tempo de Execução (s) - Escala Logarítmica', fontsize=12)
ax.set_xlabel('Implementação OpenMP', fontsize=12)
ax.set_title('Comparação de Desempenho na Estimativa de Pi', fontsize=16, fontweight='bold')
ax.tick_params(axis='x', rotation=15) # Rotaciona os rótulos do eixo X para não sobrepor

# Adiciona os valores exatos de tempo em cima de cada barra
for bar in bars:
    yval = bar.get_height()
    ax.text(bar.get_x() + bar.get_width()/2.0, yval * 1.1, f'{yval:.4f} s', ha='center', va='bottom')

# Melhora os limites do eixo Y e a grade para melhor visualização
ax.set_ylim(bottom=0.1, top=100)
ax.grid(True, which="both", ls="--", c='0.7')

# Ajusta o layout para garantir que nada seja cortado
plt.tight_layout()

# Salva o gráfico em um arquivo de imagem
plt.savefig('comparacao_desempenho_pi.png')

print("Gráfico 'comparacao_desempenho_pi.png' gerado com sucesso.")