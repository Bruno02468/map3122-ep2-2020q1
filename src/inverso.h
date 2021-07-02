/* === direto.h ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: cabeçalho das funções e definições exportadas de direto.c
 */

#ifndef INVERSO_H
#define INVERSO_H

/* constrói o sistema linear ga = c com os dados do problema inverso */
void monta_inverso(const struct waveparam *wp, const matriz *dr,
const matriz *xf, matriz **g, matriz **c, int imin, int imax);

/* determina o resíduo de uma solução encontrada */
double residuo(const struct waveparam *wp, const matriz *dr, const matriz *xf,
const matriz *a, int imin, int imax);

#endif
