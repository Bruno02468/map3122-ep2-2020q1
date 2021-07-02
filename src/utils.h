/* === utils.h ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: cabeçalho das funções e definições exportadas de utils.c
 */

#ifndef UTILS_H
#define UTILS_H

#include "matrizes.h"

/* headers do C89 não definem pi */
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

/* headers do C89 não definem NAN nem isnan() */
#ifndef NAN
  #define NAN 0./0.
  #define isnan(x) (x != x)
  #define isinf(x) (!isnan(x) && isnan(x - x))
#endif

/* muito menos MAX e MIN */
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/* testar proximidade de doubles */
int close_enough(double a, double b, double eps);

/* usada para obter argumentos numéricos do terminal */
int getarg(int argc, char **argv, const char *formatter, const char *name,
void *var, size_t siz);

/* c89 não define um round */
int c89_round(double s);

/* nem um fmax */
int c89_fmax(double a, double b);

/* aproxima as integrais pelo método do trapézio */
double integral_trapezio(int imin, int imax, const matriz *vc, double dt);

/* retorna uma double aleatória, pressupõe srand */
double rand_entre(double menor, double maior);

#endif
