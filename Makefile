# este Makefile contém comandos de compilação e invocação prontos para uso com
# o GNU Make, disponível na maioria dos sistemas Linux. na ausência do Make e
# compatíveis, recomenda-se ler as instruções em sem_make.txt
#
# para obter listar os comandos deste Makefile, recomenda-se usar "make help",
# ou simplesmente abrir o arquivo makehelp.txt.
MAKEFLAGS += --no-print-directory
CC=gcc
CFLAGS=--pedantic -ansi -Wall -Wno-unused-result
OPT=-O3
CLIBS=-lm
MY_CLIBS=src/*.c
MAINS_DIR=src/mains
XF_DR3=0.2 0.3 0.9
XF_DR10=0.03 0.15 0.17 0.25 0.33 0.34 0.40 0.44 0.51 0.73
XF_DR20=0.1 0.125 0.15 0.175 0.2 0.225 0.25 0.275 0.3 0.325 0.35 0.375 0.4 \
	0.425 0.45 0.475 0.5 0.525 0.55 0.575
AK_DR3=0.1 40 7.5
AK_DR10=7.3 2.4 5.7 4.7 0.1 20 5.1 6.1 2.8 15.3

.PHONY: help clean

help:
	@cat makehelp.txt

clean:
	rm -f ex1 ex2 ex3 ruidificador ex1.mat

ex1-bin:
	@make clean
	$(CC) $(CFLAGS) $(OPT) $(MY_CLIBS) $(MAINS_DIR)/ex1.c -o ex1 $(CLIBS)

ex1-anim-10:
	@make ex1-bin
	./ex1 10 10 1 1 0.7 200 6000 ex1.mat && $(MAINS_DIR)/plotar_ex1.py ex1.mat

ex1-anim-20:
	@make ex1-bin
	./ex1 20 10 1 1 0.7 200 6000 ex1.mat && $(MAINS_DIR)/plotar_ex1.py ex1.mat

ex2-bin:
	@make clean
	$(CC) $(CFLAGS) $(OPT) $(MY_CLIBS) $(MAINS_DIR)/ex2.c -o ex2 $(CLIBS)

ex2-dr3-cholesky:
	@make ex2-bin
	./ex2 20 10 1 1 0.7 100 1000 0.5 1 1.6 10000 3 dados/dr3.txt cholesky \
		$(XF_DR3) $(AK_DR3)

ex2-dr3-sor:
	@make ex2-bin
	./ex2 20 10 1 1 0.7 100 1000 0.5 1 1.6 10000 3 dados/dr3.txt sor \
		$(XF_DR3) $(AK_DR3)

ex2-dr10-cholesky:
	@make ex2-bin
	./ex2 20 10 1 1 0.7 100 1000 0.9 1 1.6 10000 10 dados/dr10.txt cholesky \
		$(XF_DR10) $(AK_DR10)


ex2-dr10-sor:
	@make ex2-bin
	./ex2 20 10 1 1 0.7 100 1000 0.9 1 1.6 10000 10 dados/dr10.txt sor \
		$(XF_DR10) $(AK_DR10)

ex2-dr20-cholesky:
	@make ex2-bin
	./ex2 20 10 1 1 0.7 200 2000 0.9 1 1.6 10000 20 dados/dr20.txt cholesky \
		$(XF_DR20)

ex2-dr20-sor:
	@make ex2-bin
	./ex2 20 10 1 1 0.7 200 2000 0.9 1 1.6 10000 20 dados/dr20.txt sor \
		$(XF_DR20)

ruidificador-bin:
	@make clean
	$(CC) $(CFLAGS) $(OPT) $(MY_CLIBS) $(MAINS_DIR)/ruidificador.c -o \
		ruidificador $(CLIBS)

ex3-bin:
	@make clean
	$(CC) $(CFLAGS) $(OPT) $(MY_CLIBS) $(MAINS_DIR)/ex3.c -o ex3 $(CLIBS)

ex3-3-cholesky:
	@make ex3-bin
	./ex3 20 10 1 1 0.7 100 1000 0.9 1 1.6 10000 0.001 10 dados/dr10.txt \
		cholesky $(XF_DR10) $(AK_DR10)

ex3-3-sor:
	@make ex3-bin
	./ex3 20 10 1 1 0.7 100 1000 0.9 1 1.6 10000 0.001 10 dados/dr10.txt \
		sor $(XF_DR10) $(AK_DR10)

ex3-4-cholesky:
	@make ex3-bin
	./ex3 20 10 1 1 0.7 100 1000 0.9 1 1.6 10000 0.0001 10 dados/dr10.txt \
		cholesky $(XF_DR10) $(AK_DR10)

ex3-4-sor:
	@make ex3-bin
	./ex3 20 10 1 1 0.7 100 1000 0.9 1 1.6 10000 0.0001 10 dados/dr10.txt \
		sor $(XF_DR10) $(AK_DR10)

ex3-5-cholesky:
	@make ex3-bin
	./ex3 20 10 1 1 0.7 100 1000 0.9 1 1.6 10000 0.00001 10 dados/dr10.txt \
		cholesky $(XF_DR10) $(AK_DR10)

ex3-5-sor:
	@make ex3-bin
	./ex3 20 10 1 1 0.7 100 1000 0.9 1 1.6 10000 0.00001 10 dados/dr10.txt \
		sor $(XF_DR10) $(AK_DR10)

plotar-ruido-3:
	src/mains/plotar_ruido.py dados/dr10.txt 0.001

plotar-ruido-4:
	src/mains/plotar_ruido.py dados/dr10.txt 0.0001

plotar-ruido-5:
	src/mains/plotar_ruido.py dados/dr10.txt 0.00001

