/* === ex2.c ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: este programa resolve o problema inverso (exercício 2)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../matrizes.h"
#include "../sistemas.h"
#include "../utils.h"
#include "../direto.h"
#include "../inverso.h"

/* printar help e sair */
void falhou(char **argv) {
  printf("\nInvocação: %s [c^2=20] [beta=10] [T=1] [X=1] [xr=0.7] "
    "[nx=100] [nt=1000] [ti=0] [tf=1] [omega=1.6] [itmax=10000] "
    "[K] [arquivo de dados] [metodo=sor|cholesky] x1 x2 x3 {a1} {a2} {a3}...\n",
    argv[0]);
  printf("\nAs intensidades reais são opcionais, para cálculo do erro L2.\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  double ti, tf, omega;
  int i, k, itmax, imin, imax, farg;
  char *fname, *metodo;
  matriz *xf, *dr, *g, *c, *a, *ar;
  FILE *drf;
  struct waveparam wp;

  /* parâmetros da função de onda e seus valores-padrão */
  farg = 15;
  wp.c = 20;
  wp.beta = 10;
  wp.T = 1;
  wp.X = 1;
  wp.xc = 0.7;
  wp.nx = 100;
  wp.nt = 1000;
  ti = 0;
  tf = 1;
  k = 3;
  omega = 1.6;
  itmax = 10000;

  /* obter argumentos do terminal */
  if (getarg(argc, argv, "%lf", "c^2", &(wp.c), sizeof(double))
    * getarg(argc, argv, "%lf", "beta", &(wp.beta), sizeof(double))
    * getarg(argc, argv, "%lf", "T", &(wp.T), sizeof(double))
    * getarg(argc, argv, "%lf", "X", &(wp.X), sizeof(double))
    * getarg(argc, argv, "%lf", "xr", &(wp.xc), sizeof(double))
    * getarg(argc, argv, "%d", "nx", &(wp.nx), sizeof(int))
    * getarg(argc, argv, "%d", "nt", &(wp.nt), sizeof(int))
    * getarg(argc, argv, "%lf", "ti", &ti, sizeof(double))
    * getarg(argc, argv, "%lf", "tf", &tf, sizeof(double))
    * getarg(argc, argv, "%lf", "omega", &omega, sizeof(double))
    * getarg(argc, argv, "%d", "itmax", &itmax, sizeof(int))
    * getarg(argc, argv, "%d", "K", &k, sizeof(int)) != 1) {
    falhou(argv);
  }
  /* calcular outros parâmetros para a solução numérica */
  wp.dt = wp.T / wp.nt;
  wp.dx = wp.X / wp.nx;
  wp.c = sqrt(wp.c);
  wp.alpha = wp.c*wp.dt/wp.dx;
  imin = c89_round(ti/wp.dt);
  imax = c89_round(tf/wp.dt);
  fname = argv[13];
  metodo = argv[14];
  if (strcmp(metodo, "sor") && strcmp(metodo, "cholesky")) {
    printf("O método de resolução de sistemas lineares deve ser \"sor\" ou "
      "\"cholesky\"!\n");
    falhou(argv);
  }
  /* ler as posições das fontes */
  if (argc < (farg + k)) {
    /* verificar quantidade de parâmetros */
    printf("Parâmetros insuficientes!\n");
    falhou(argv);
  }
  /* ler as posições de cada fonte */
  xf = matriz_nula(1, k);
  ar = matriz_nula(k, 1);
  for (i = 0; i < k; i++) {
    if (!sscanf(argv[i+farg], "%lf", &(xf->entradas[0][i]))) {
      printf("Valor inválido informado para a posição da fonte #%d!\n", i+1);
      falhou(argv);
    }
    if (argc > (farg + k)) {
      /* foram fornecidas intensidades */
      if (farg + k + i >= argc) {
        /* evitar uma segfault caso o número de argumentos esteja incorreto */
        printf("Número inválido de argumentos (contando as intensidades)!\n");
        falhou(argv);
      }
      if (!sscanf(argv[farg+i+k], "%lf", &(ar->entradas[i][0]))) {
        printf("Valor inválido informado para a intensidade da fonte #%d!\n",
          i+1);
        falhou(argv);
      }
    }
  }
  /* ler o arquivo */
  drf = fopen(fname, "r");
  if (!drf) {
    printf("Impossível abrir arquivo \"%s\" para leitura!\n", fname);
    falhou(argv);
  }
  dr = matriz_nula(wp.nt + 1, 1);
  for (i = 0; i <= wp.nt; i++) {
    if (!fscanf(drf, "%lf", &(dr->entradas[i][0]))) {
      printf("Valor inválido informado em dr (i=%d)!\n", i);
      falhou(argv);
    }
  }
  fclose(drf);

  printf("Montando o sistema linear...\n");

  /* montar o sistema linear */
  monta_inverso(&wp, dr, xf, &g, &c, imin, imax);

  /* resolver o sistema linear pelo método escolhido */
  printf("Resolvendo o sistema por ");
  if (!strcmp(metodo, "sor")) {
    printf("SOR...\n");
    a = sistema_sor(g, c, omega, itmax);
  } else {
    /* já temos a garantia que é um dos dois... outro strcmp é redundante. */
    printf("fatoração de Cholesky...\n");
    a = sistema_cholesky(g, c);
  }

  printf("Pronto!\n\n");

  /* imprimir os resultados */
  for (i = 0; i < k; i++) {
    printf("Intensidade da fonte #%d: %g", i + 1, a->entradas[i][0]);
    if (argc > (farg + k)) {
      printf(" (real: %g)", ar->entradas[i][0]);
    }
    printf(".\n");
  }
  printf("\nResíduo: %g.\n", residuo(&wp, dr, xf, a, imin, imax));
  if (argc > (farg + k)) {
    subtrair_matriz(a, ar);
    printf("Erro L2: %g.\n", magnitude_matriz(a));
  }

  /* liberar tudo que foi dinamicamente alocado e sair */
  deletar_matriz(dr);
  deletar_matriz(xf);
  deletar_matriz(g);
  deletar_matriz(c);
  deletar_matriz(a);
  deletar_matriz(ar);

  return EXIT_SUCCESS;
}
