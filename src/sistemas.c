/* === sistemas.c ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: implementa métodos de resolução de sistemas lineares
 */

#include <math.h>
#include "matrizes.h"
#include "sistemas.h"

/* faz a fatoração de cholesky de uma matriz M, tal que M = LL' */
matriz *cholesky(const matriz *m) {
  int i, j, k;
  double s;
  matriz *l;
  l = matriz_nula(m->linhas, m->colunas);
  for (i = 0; i < m->linhas; i++) {
    for (j = 0; j < (i+1); j++) {
    s = 0;
    for (k = 0; k < j; k++) s += l->entradas[i][k] * l->entradas[j][k];
    l->entradas[i][j] = (i == j)
      ? sqrt(m->entradas[i][i] - s)
      : (1.0 / l->entradas[j][j] * (m->entradas[i][j] - s));
    }
  }
  return l;
}

/* função não exportada: resolve um sistema triangular inferior La = c */
matriz *sistema_ti(const matriz *l, const matriz *c) {
  int i, j;
  matriz *a;
  a = clonar_matriz(c);
  for (i = 0; i < c->linhas; i++) {
    for (j = 0; j < i; j++) {
      a->entradas[i][0] -= a->entradas[j][0] * l->entradas[i][j];
    }
    a->entradas[i][0] /= l->entradas[i][i];
  }
  return a;
}

/* função não exportada: resolve um sistema triangular superior La = c */
matriz *sistema_ts(const matriz *l, const matriz *c) {
  int i, j;
  matriz *a;
  a = clonar_matriz(c);
  for (i = c->linhas - 1; i >= 0; i--) {
    for (j = c->linhas - 1; j > i; j--) {
      a->entradas[i][0] -= a->entradas[j][0] * l->entradas[i][j];
    }
    a->entradas[i][0] /= l->entradas[i][i];
  }
  return a;
}

/* resolve um sistema Ba = c por fatoração de cholesky */
/* detalhe da implementação:
 *   primeiro determinamos L tal que LL' = B.
 *   depois, resovlemos Ly = c com L triangular inferior.
 *   finalmente, resolvemos L'a = y com L' triangular superior.
 */
matriz *sistema_cholesky(const matriz *b, const matriz *c) {
  matriz *l, *lt, *y, *a;
  /* determinar L e L' */
  l = cholesky(b);
  lt = matriz_transposta(l);
  /* determinar y e a */
  y = sistema_ti(l, c);
  a = sistema_ts(lt, y);
  /* limpar intermediários */
  deletar_matriz(l);
  deletar_matriz(lt);
  deletar_matriz(y);
  return a;
}

/* função não-exportada: avança um passo na iteração SOR */
void avanca_sor(const matriz *b, matriz *a, const matriz *c, double omega) {
  int i, j;
  double termo_somatorio;
  matriz *pa, *na;
  pa = clonar_matriz(a);
  na = matriz_nula(a->linhas, a->colunas);
  escalar_matriz(pa, 1 - omega);
  for (i = 0; i < c->linhas; i++) {
    termo_somatorio = c->entradas[i][0];
    for (j = 0; j < c->linhas; j++) {
      if (j < i) {
        termo_somatorio -= b->entradas[i][j] * na->entradas[j][0];
      } else if (j > i) {
        termo_somatorio -= b->entradas[i][j] * a->entradas[j][0];
      }
    }
    na->entradas[i][0] = pa->entradas[i][0]
      + omega/b->entradas[i][i] * termo_somatorio;
  }
  sobrescrever_matriz(na, a);
  deletar_matriz(na);
  deletar_matriz(pa);
}

/* resolve um sistema Ba = c por SOR */
matriz *sistema_sor(const matriz *b, const matriz *c, double omega,
int iteracoes) {
  int iter;
  matriz *a;
  a = matriz_nula(c->linhas, 1);
  for (iter = 0; iter < iteracoes; iter++) {
    avanca_sor(b, a, c, omega);
  }
  return a;
}

/* resolve um sistema Ba = c invertendo (lento, para debug) */
matriz *sistema_lento(const matriz *b, const matriz *c) {
  matriz *i, *a;
  i = matriz_inversa(b);
  a = matriz_produto(i, c);
  deletar_matriz(i);
  return a;
}
