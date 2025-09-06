import matplotlib.pyplot as plt # type: ignore
import numpy as np # type: ignore

# --- 1. Dados do Experimento ---
labels = [
    '1. Incorreto',
    '2. Critical',
    '3. Private',
    '4. Firstprivate',
    '5. Lastprivate',
    '6. Default(none)'
]
tempos = [26.393, 32.189, 26.273, 25.481, 26.137, 26.256]
valores_pi = [3.11990, 3.14133, 3.14163, 3.14183, 3.14149, 3.14152]
pi_real = 3.14159265

# --- 2. Comparativo de Tempo de Execução ---
# Cria a figura e os eixos para o primeiro gráfico
fig1, ax1 = plt.subplots(figsize=(10, 6))

# Cria as barras
bars1 = ax1.bar(labels, tempos, color='skyblue')

# Adiciona títulos e rótulos
ax1.set_ylabel('Tempo de Execução (s)')
ax1.set_title('Comparativo de Desempenho das Abordagens OpenMP', fontsize=16)
plt.xticks(rotation=30, ha='right') # Rotaciona os rótulos do eixo X para não sobrepor

# Adiciona o valor exato em cima de cada barra para clareza
for bar in bars1:
    yval = bar.get_height()
    ax1.text(bar.get_x() + bar.get_width()/2.0, yval, f'{yval:.3f}s', va='bottom', ha='center')

# Ajusta o layout e exibe o gráfico
plt.tight_layout()
plt.show()