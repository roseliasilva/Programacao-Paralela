import matplotlib.pyplot as plt

programa1 = {"O0": 0.631, "O2": 0.452, "O3": 0.444}
programa2_2vars = {"O0": 0.532, "O2": 0.432, "O3": 0.396}
programa2_4vars = {"O0": 0.485, "O2": 0.332, "O3": 0.330}
programa2_8vars = {"O0": 0.489, "O2": 0.376, "O3": 0.370}

# Preparar os dados
otimizacoes = ["O0", "O2", "O3"]
p1 = [programa1[o] for o in otimizacoes]
p2_2 = [programa2_2vars[o] for o in otimizacoes]
p2_4 = [programa2_4vars[o] for o in otimizacoes]
p2_8 = [programa2_8vars[o] for o in otimizacoes]

# Criar gráfico de linhas
plt.figure(figsize=(10,6))
plt.plot(otimizacoes, p1, marker="o", label="Programa 1 (Laço 1+2)")
plt.plot(otimizacoes, p2_2, marker="o", label="Programa 2 (Laço 1+3, 2 vars)")
plt.plot(otimizacoes, p2_4, marker="o", label="Programa 2 (Laço 1+3, 4 vars)")
plt.plot(otimizacoes, p2_8, marker="o", label="Programa 2 (Laço 1+3, 8 vars)")

plt.title("Comparação de Tempos de Execução com Diferentes Níveis de Otimização")
plt.xlabel("Nível de Otimização")
plt.ylabel("Tempo de Execução (s)")
plt.legend()
plt.grid(True)
plt.show()