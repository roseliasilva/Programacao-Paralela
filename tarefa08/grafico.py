import matplotlib.pyplot as plt # type: ignore
import numpy as np # type: ignore

# Dados do experimento (tempos representativos em segundos)
versoes = [
    'Versão 1\n(critical + rand)',
    'Versão 2\n(vetor + rand)',
    'Versão 3\n(critical + rand_r)',
    'Versão 4\n(vetor + rand_r)'
]
tempos = [2.378044, 3.512394, 0.017340, 0.100079]

# Criando o gráfico de barras
plt.style.use('seaborn-v0_8-whitegrid')
fig, ax = plt.subplots(figsize=(10, 6))

bars = ax.bar(versoes, tempos, color=['#d9534f', '#f0ad4e', '#5bc0de', '#5cb85c'])

# Adicionando título e rótulos
ax.set_ylabel('Tempo de Execução (segundos)', fontsize=12)
ax.set_title('Comparação de Desempenho entre Versões Paralelas', fontsize=16, pad=20)
ax.set_xticklabels(versoes, rotation=0, ha='center')

# Adicionando os valores no topo de cada barra
for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2.0, yval + 0.05, f'{yval:.2f}s', ha='center', va='bottom')

# Melhorando o layout para evitar sobreposição
plt.tight_layout()

# Exibindo o gráfico
plt.show()