import matplotlib.pyplot as plt # type: ignore

tempos = {
    "Sequencial": 2.144511,
    "Paralelo": 0.408672
}

primos = {
    "Sequencial": 664579,
    "Paralelo": 658610
}

# --- Tempo de execução ---
plt.figure(figsize=(6,4))
plt.bar(tempos.keys(), tempos.values(), color=["skyblue","salmon"])
plt.ylabel("Tempo (s)")
plt.title("Tempo de Execução: Sequencial vs Paralelo")
plt.show()