/* === ex3.c ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: este programa resolve o problema inverso com e sem ruído,
 *            permitindo uma comparação dos resultados.
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
#include "../ruido.h"

/* printar help e sair */
void falhou(char **argv) {
  printf("\nInvocação: %s [c^2=20] [beta=10] [T=1] [X=1] [xr=0.7] "
    "[nx=100] [nt=1000] [ti=0] [tf=1] [omega=1.6] [itmax=10000] [eta=0.001] "
    "[K] [arquivo de dados] [metodo=sor|cholesky] x1 x2 x3 {a1} {a2} {a3}...\n",
    argv[0]);
  printf("\nAs intensidades reais são opcionais, para cálculo do erro L2.\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  double ti, tf, omega, eta, ruido, sem, com, per;
  int i, k, itmax, imin, imax, farg;
  char *fname, *metodo;
  matriz *xf, *dr, *g, *c, *a, *drr, *gr, *cr, *ar, *ra;
  FILE *drf;
  struct waveparam wp;

  /* parâmetros da função de onda e seus valores-padrão */
  farg = 16;
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
  eta = 0.001;

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
    * getarg(argc, argv, "%lf", "eta", &eta, sizeof(double))
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
  fname = argv[14];
  metodo = argv[15];
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
  xf = matriz_nula(1, k);
  ra = matriz_nula(k, 1);
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
      if (!sscanf(argv[farg+i+k], "%lf", &(ra->entradas[i][0]))) {
        printf("Valor inválido informado para a intensidade da fonte #%d!\n",
          i+1);
        falhou(argv);
      }
    }
  }
  /* ler o arquivo e criar versão com ruído */
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
  printf("Elaborando ruído... ");
  fflush(stdout);
  drr = clonar_matriz(dr);
  ruido = inserir_ruido(drr, eta, imin, imax, wp.dt);
  printf("Feito (%g%%)\n", ruido);

  printf("Montando os sistemas com e sem ruído...\n");

  /* montar o sistema linear */
  monta_inverso(&wp, dr, xf, &g, &c, imin, imax);
  monta_inverso(&wp, drr, xf, &gr, &cr, imin, imax);

  /* resolver o sistema linear pelo método escolhido */
  printf("Resolvendo os sistemas por ");
  if (!strcmp(metodo, "sor")) {
    printf("SOR...\n");
    a = sistema_sor(g, c, omega, itmax);
    ar = sistema_sor(gr, cr, omega, itmax);
  } else {
    /* já temos a garantia que é um dos dois... outro strcmp é redundante. */
    printf("fatoração de Cholesky...\n");
    a = sistema_cholesky(g, c);
    ar = sistema_cholesky(gr, cr);
  }

  printf("Pronto!\n\n");

  /* imprimir os resultados */
  for (i = 0; i < k; i++) {
    sem = a->entradas[i][0];
    com = ar->entradas[i][0];
    per = 100*(com - sem)/sem;
    printf("Intensidade da fonte #%d: %g vs. %g com ruído (", i+1, sem, com);  
    if (per > 0) printf("+");
    printf("%.2f%%", per);
    if (argc > (farg + k)) {
      printf(", real: %g", ra->entradas[i][0]);
    }
    printf(").\n");
  }
  printf("\nResíduo sem ruído: %g.\n", residuo(&wp, dr, xf, a, imin, imax));
  printf("Resíduo com ruído: %g.\n", residuo(&wp, dr, xf, ar, imin, imax));
  if (argc > (farg + k)) {
    subtrair_matriz(a, ra);
    subtrair_matriz(ar, ra);
    printf("\nErro L2 sem ruído: %g.\n", magnitude_matriz(a));
    printf("Erro L2 com ruído: %g.\n", magnitude_matriz(ar));
  }

   
   /* liberar tudo que foi dinamicamente alocado e sair */
  deletar_matriz(dr);
  deletar_matriz(drr);
  deletar_matriz(xf);
  deletar_matriz(g);
  deletar_matriz(c);
  deletar_matriz(a);
  deletar_matriz(gr);
  deletar_matriz(cr);
  deletar_matriz(ar);
  deletar_matriz(ra);

  return EXIT_SUCCESS;
}
