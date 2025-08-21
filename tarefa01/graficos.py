import matplotlib.pyplot as plt # type: ignore

# Dados fornecidos
N = [10, 100, 500, 1000, 2000, 3000]
linhas = [0.000001, 0.000064, 0.001569, 0.002887, 0.011961, 0.025887]
colunas = [0.000001, 0.000074, 0.002560, 0.005932, 0.026463, 0.077994]

# Criar gráfico
plt.figure(figsize=(8, 6))
plt.plot(N, linhas, marker="o", label="Acesso por Linhas")
plt.plot(N, colunas, marker="s", label="Acesso por Colunas")

plt.xlabel("Tamanho da matriz (N)")
plt.ylabel("Tempo (s)")
plt.title("Comparação de Desempenho: Multiplicação MxV")
plt.legend()
plt.grid(True, linestyle="--", alpha=0.6)
plt.tight_layout()
plt.show()