import matplotlib.pyplot as plt # type: ignore
import numpy as np # type: ignore

# 1. Dados
tamanho_bytes = np.array([
    8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096,
    8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576
])

tempo_us = np.array([
    0.26, 0.24, 0.27, 0.27, 0.47, 0.46, 0.47, 0.60, 0.63, 0.83,
    1.10, 2.02, 3.54, 6.18, 11.26, 26.36, 57.37, 99.65
])

# 2. Configurando os gráficos
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

# --- Gráfico 1: Escala Linear ---
ax1.plot(tamanho_bytes, tempo_us, marker='o', linestyle='-', color='b')
ax1.set_title('Desempenho da Comunicação MPI (Escala Linear)')
ax1.set_xlabel('Tamanho da Mensagem (Bytes)')
ax1.set_ylabel('Tempo Médio de Ida e Volta (µs)')
ax1.grid(True)

# --- Gráfico 2: Escala Log-Log ---
ax2.loglog(tamanho_bytes, tempo_us, marker='o', linestyle='-', color='r')
ax2.set_title('Desempenho da Comunicação MPI (Escala Log-Log)')
ax2.set_xlabel('Tamanho da Mensagem (Bytes)')
ax2.set_ylabel('Tempo Médio de Ida e Volta (µs)')
ax2.grid(True, which="both", ls="--")

# Adicionando anotações para explicar os regimes no gráfico log-log
ax2.annotate('Regime de Latência\n(Tempo quase constante)',
             xy=(32, 0.3), xytext=(20, 1),
             arrowprops=dict(facecolor='black', shrink=0.05, width=1, headwidth=8))

ax2.annotate('Regime de Largura de Banda\n(Tempo cresce com o tamanho)',
             xy=(16384, 2.5), xytext=(2048, 20),
             arrowprops=dict(facecolor='black', shrink=0.05, width=1, headwidth=8))


# 3. Exibir os gráficos
plt.tight_layout()
plt.show()