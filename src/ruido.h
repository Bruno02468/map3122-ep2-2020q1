/* === ruido.h ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: cabeçalho das funções e definições exportadas de ruido.c
 */

#ifndef RUIDO_H
#define RUIDO_H

/* insere ruído num vetor-coluna, e retorna o nível de ruído */
double inserir_ruido(matriz *vc, double eta, int imin, int imax, double dt);

#endif
