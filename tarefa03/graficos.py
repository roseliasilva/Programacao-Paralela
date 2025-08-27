import matplotlib.pyplot as plt # type: ignore
import numpy as np # type: ignore

# Dados experimentais
iteracoes = [1000, 10000, 100000, 1000000, 10000000, 100000000]
aproximacoes = [
    3.140592653839794,
    3.141492653590034,
    3.141582653589720,
    3.141591653589774,
    3.141592553589792,
    3.141592643589326
]
tempos = [0.000003, 0.000021, 0.000209, 0.002571, 0.022150, 0.200006]

# Valor real de pi
pi_real = 3.14159265359

# Calcula erro absoluto
erros = [abs(pi_real - p) for p in aproximacoes]

# --- Gráfico 1: Iterações x Tempo ---
plt.figure(figsize=(8,5))
plt.plot(iteracoes, tempos, marker='o')
plt.xscale("log")
plt.yscale("log")
plt.xlabel("Número de Iterações (escala log)")
plt.ylabel("Tempo de execução (s) [escala log]")
plt.title("Tempo de execução vs Iterações")
plt.grid(True, which="both", ls="--")
plt.show()

# --- Gráfico 2: Iterações x Erro ---
plt.figure(figsize=(8,5))
plt.plot(iteracoes, erros, marker='o', color='red')
plt.xscale("log")
plt.yscale("log")
plt.xlabel("Número de Iterações (escala log)")
plt.ylabel("Erro absoluto |π_aprox - π_real|")
plt.title("Erro na aproximação de π vs Iterações")
plt.grid(True, which="both", ls="--")
plt.show()