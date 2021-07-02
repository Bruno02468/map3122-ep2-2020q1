/* === direto.h ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: cabeçalho das funções e definições exportadas de direto.c
 */

#ifndef DIRETO_H
#define DIRETO_H

#include "matrizes.h"

/* estrutura para acumular os parâmetros da função de onda... a ideia é que o
 * stack frame da estimação seja o menor possível, para minimizar o risco de
 * estourarmos o limite de recursão. */
struct waveparam {
  double c, beta, T, X, xc, dx, dt, alpha;
  int nt, nx;
};

/* estimação recursiva da solução */
double u(const struct waveparam *wp, matriz *vu, int i, int j);

/* estima todos os valores da função no tempo e espaço considerados.
 * preserva valores parciais, o que torna mais eficiente. */
matriz *solve_direto(const struct waveparam *wp);

/* faz várias resoluções do sistema direto, e prepara um sismograma.
 * usado nos cálculos de resíduo do ex2 e erroinv */
matriz *sismograma(const struct waveparam *wp, const matriz *xf,
const matriz *ak);

#endif
