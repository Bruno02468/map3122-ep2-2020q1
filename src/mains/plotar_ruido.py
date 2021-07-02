#!/usr/bin/env python3
# EP2 - Bruno Borges Paschoalinoto - 10687472
# 
# este programa plota um sismograma e uma versão dele com ruído lado a lado

import sys, os, re

try:
  import matplotlib.pyplot as plt
except:
  print("Erro ao importar o matplotlib! Certifique-se de tê-lo instalado.")
  sys.exit(0)

if len(sys.argv) < 3:
  print("Especificique um sismograma.txt!")
  sys.exit(0)

# ler o sismograma
fname = sys.argv[1]
sismograma = []
with open(fname) as fd:
  sismograma += list(map(float, fd.readlines()))

# produzir dados básicos sobre ele
nt = len(sismograma) - 1
dt = 1/nt
xises = [i*dt for i in range(nt+1)]

# produzir uma versão com ruído
eta = sys.argv[2]
os.system("make ruidificador-bin && ./ruidificador %d 1 0 1 %s %s ex3.mat n "
          "> ruid.out"
          % (nt, eta, fname))
ruidoso = []
with open("ex3.mat") as fd:
  ruidoso += list(map(float, fd.readlines()))

# puxar o nível de ruído da saída do programa
ruido = "???"
with open("ruid.out") as fd:
  for linha in fd:
    m = re.search("([0-9.]+%).$", linha)
    if m:
      ruido = m.group(1)

os.system("rm ex3.mat")
os.system("rm ruid.out")

# plotar as duas lado a lado
plt.tight_layout()

plt.subplot(2, 1, 1)
plt.title("Sismograma %s sem ruído" % (fname,))
plt.plot(xises, sismograma)
plt.xlabel("t")
plt.ylabel("dr_10(t)")

plt.subplot(2, 1, 2)
plt.title("Sismograma %s com %s de ruído (eta = %s)" % (fname, ruido, eta))
plt.plot(xises, ruidoso)
plt.xlabel("t")
plt.ylabel("dr_10_ruido(t)")

plt.show()
