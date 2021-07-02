/* === sistemas.h ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: cabeçalho das funções e definições exportadas de sistemas.c
 */

#ifndef SISTEMAS_H
#define SISTEMAS_H

#include "matrizes.h"

/* faz a fatoração de cholesky de uma matriz M, tal que M = LL' */
matriz *cholesky(const matriz *m);

/* resolve um sistema Ba = c por fatoração de cholesky */
matriz *sistema_cholesky(const matriz *b, const matriz *c);

/* resolve um sistema Ba = c por SOR */
matriz *sistema_sor(const matriz *b, const matriz *c, double omega,
int iteracoes);

/* resolve um sistema Ba = c invertendo (lento, para debug) */
matriz *sistema_lento(const matriz *b, const matriz *c);

#endif
