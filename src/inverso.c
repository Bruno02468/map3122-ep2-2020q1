/* === direto.c ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: funções para resolução do problema inverso
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "matrizes.h"
#include "sistemas.h"
#include "utils.h"
#include "direto.h"
#include "inverso.h"

/* função não-exportada, calcula as integrais de produto das fontes */
double gij(const struct waveparam *wp, const matriz *gi, const matriz *gj,
int imin, int imax) {
  matriz *p;
  double s;
  /* construir uma matriz-produto e integrar */
  p = matriz_hadamard(gi, gj);
  s = integral_trapezio(imin, imax, p, wp->dt);
  deletar_matriz(p);
  return s;
}

/* constrói o sistema linear ga = c com os dados do problema inverso */
void monta_inverso(const struct waveparam *wp, const matriz *dr,
const matriz *xf, matriz **g, matriz **c, int imin, int imax) {
  struct waveparam dwp;
  matriz **uk, *m;
  int i, j, k, jr;
  /* primeiro, resolver o problema direto para todas as fontes */
  dwp = *wp;
  k = xf->colunas;
  jr = c89_round(wp->xc/wp->dx);
  uk = malloc(k * sizeof(matriz *));
  for (i = 0; i < k; i++) {
    dwp.xc = xf->entradas[0][i];
    m = solve_direto(&dwp);
    /* como só nos interessa a coluna do sismograma, descartar o resto */
    uk[i] = vetor_coluna(m, jr);
    deletar_matriz(m);
  }
  /* só montar o sistema! primeiro, a matriz de integrais... */
  *g = matriz_nula(k, k);
  for (i = 0; i < k; i++) {
    for (j = 0; j <= i; j++) {
      (*g)->entradas[i][j] = gij(wp, uk[i], uk[j], imin, imax);
      if (j != i) {
        /* espelhar o valor, economizando chamadas a gij */
        (*g)->entradas[j][i] = (*g)->entradas[i][j];
      }
    }
  }
  /* ...e o vetor de termos independentes. */
  *c = matriz_nula(k, 1);
  for (i = 0; i < k; i++) {
    (*c)->entradas[i][0] = gij(wp, uk[i], dr, imin, imax);
  }
  /* deletar o que alocamos... */
  for (i = 0; i < k; i++) {
    deletar_matriz(uk[i]);
  }
  free(uk);
}

/* determina o resíduo de uma solução encontrada */
double residuo(const struct waveparam *wp, const matriz *dr, const matriz *xf,
const matriz *a, int imin, int imax) {
  double r;
  matriz *d, *dif, *sq;
  /* elaborar um sismograma e subtrair dele o sismograma dado */
  d = sismograma(wp, xf, a);
  dif = matriz_diferenca(d, dr);
  sq = matriz_hadamard(dif, dif);
  
  /* determinar o residuo, deletar as matrizes que criamos, e retornar */
  r = integral_trapezio(imin, imax, sq, wp->dt);
  deletar_matriz(d);
  deletar_matriz(dif);
  deletar_matriz(sq);
  
  return r / (2 * (imax - imin) * wp->dt);
}
