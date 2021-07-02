#!/usr/bin/env python3
# EP2 - Bruno Borges Paschoalinoto - 10687472
# 
# este programa plota a animação de uma solução do exercício 1, usando um
# arquivo produzido pelo programa direto.c

import sys

try:
  import matplotlib.pyplot as plt
except:
  print("Erro ao importar o matplotlib! Certifique-se de tê-lo instalado.")
  sys.exit(0)

from matplotlib.animation import FuncAnimation

# carregar dados
temporal = {}
tempos = []
top_tempo = 0
top_valor = 0
min_valor = 0
with open(sys.argv[1]) as fdesc:
  for linha in fdesc:
    tempo, distancia, valor = map(float, linha.split(" "))
    if tempo not in temporal:
      temporal[tempo] = []
      tempos.append(tempo)
      top_tempo = tempo if tempo > top_tempo else top_tempo
    temporal[tempo].append((distancia, valor))
    top_valor = valor if valor > top_valor else top_valor
    min_valor = valor if valor < min_valor else min_valor

fig, ax = plt.subplots()
ln, = plt.plot([], [])
tempos.sort()
xises = [t[0] for t in temporal[0]]
ys = []
for f in tempos:
  ys.append([t[1] for t in temporal[f]])
nt = len(ys)

# preparar os eixos com valores máximos e mínimos
def init_anim():
  ax.set_xlim([0, max([t[0] for t in temporal[0]])])
  ax.set_ylim([min_valor * 1.1, top_valor * 1.1])
  return ln,

# função chamada pela animação, atualiza os dados das linhas
def update(frame):
  ax.set_title("Solução do problema direto (t=%1.5f)" % (frame/nt,))
  ln.set_data(xises, ys[frame])

ani = FuncAnimation(fig, update, frames=range(nt), init_func=init_anim,
                    interval=1)

plt.show()
