/* === direto.c ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: funções para resolução do problema direto
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "direto.h"
#include "utils.h"
#include "matrizes.h"


/* função fonte */
double f(const struct waveparam *wp, int i, int j) {
  double m, t;
  int jc;
  jc = c89_round(wp->xc/wp->dx);
  if (j != jc) return 0;
  t = i*wp->dt;
  m = wp->beta*wp->beta*t*t*M_PI*M_PI;
  return 1000*wp->c*wp->c*(1 - 2*m) * exp(-m);
}

/* estimação recursiva da solução */
double u(const struct waveparam *wp, matriz *vu, int i, int j) {
  double n;
  /* condição de contorno */
  if (i < 0) return 0;
  if (j == 0 || j == wp->nx || i <= 1) return vu->entradas[i][j] = 0;
  /* valor já foi calculado? */
  if (!isnan(vu->entradas[i][j])) return vu->entradas[i][j];
  /* se não, solução recursiva */
  n =
    - u(wp, vu, i-2, j)
    + 2*(1-wp->alpha*wp->alpha)*u(wp, vu, i-1, j)
    + wp->alpha*wp->alpha*(u(wp, vu, i-1, j+1) + u(wp, vu, i-1, j-1))
    + wp->dt*wp->dt*f(wp, i-1, j);
  /* a atribuição acima serve para salvar valores calculados em chamadas
   * recursivas da função, economizando tempo e espaço */
  if (isnan(n) || isinf(n)) {
    fprintf(stderr, "Infinidade ou NaN detectada, ajuste os parâmetros.\n");
    exit(EXIT_FAILURE);
  }
  return vu->entradas[i][j] = n;
}

/* estima todos os valores da função no tempo e espaço considerados.
 * preserva valores parciais, o que torna mais eficiente. */
matriz *solve_direto(const struct waveparam *wp) {
  /* parâmetros da função de onda e seus valores-padrão */
  int i, j;
  matriz *u_vals;
  if (wp->alpha > 1) {
    fprintf(stderr, "CFL violada ou limífrofe; a solução poderá apresentar "
      "instabilidade.\n");
  }
  /* agora, vamos calcular u(x, t) para todo x e t dentro da aproximação.
   * o uso de uma matriz permite que chamadas recursivas reutilizem os valores
   * previamente calculados. */
  u_vals = matriz_nula(wp->nt + 1, wp->nx + 1);
  for (i = 0; i <= wp->nt; i++) {
    for (j = 0; j <= wp->nx; j++) {
      u_vals->entradas[i][j] = NAN;
    }
  }
  /* agora, vamos preencher a matriz. */
  for (i = 0; i <= wp->nt; i++) {
    for (j = 0; j <= wp->nx; j++) {
      if (isnan(u_vals->entradas[i][j]))
        u_vals->entradas[i][j] = u(wp, u_vals, i, j);
    }
  }
  return u_vals;
}

/* faz várias resoluções do sistema direto, e prepara um sismograma.
 * é usado nos testes de erros do exercício 2. */
matriz *sismograma(const struct waveparam *wp, const matriz *xf,
const matriz *ak) {
  matriz *res, *tmp, *vc;
  int k;
  struct waveparam wc;
  /* preparar a soma de todas as matrizes */
  res = matriz_nula(wp->nt + 1, wp->nx + 1);
  wc = *wp;
  /* somar as soluções, uma a uma */
  for (k = 0; k < xf->colunas; k++) {
    wc.xc = xf->entradas[0][k];
    tmp = solve_direto(&wc);
    escalar_matriz(tmp, ak->entradas[k][0]);
    somar_matriz(res, tmp);
    deletar_matriz(tmp);
  }
  /* extrair o vetor-coluna, limpar tudo, e sair */
  vc = vetor_coluna(res, c89_round(wp->xc/wp->dx));
  deletar_matriz(res);
  return vc;
}
