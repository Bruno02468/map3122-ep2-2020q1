/* === sistemas.c ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: implementa funções úteis
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "matrizes.h"

/* testar proximidade de doubles */
int close_enough(double a, double b, double eps) {
  if (eps < 0) eps = 1e-1;
  return fabs(a - b) <= eps;
}

/* usada para obter argumentos numéricos do terminal */
int getarg(int argc, char **argv, const char *formatter, const char *name,
void *var, size_t siz) {
  static int narg = 1;
  if (narg >= argc) {
    /* printf("Valor de \"%s\" não informado. Usando padrão.", name); */
  } else {
    if (!sscanf(argv[narg], formatter, var)) {
      fprintf(stderr, "Valor inválido informado para \"%s\"!\n", name);
      return 0;
    }
  }
  narg++;
  return 1;
}

/* c89 não define um round */
int c89_round(double s) {
  double parte_int, parte_frac;
  parte_frac = modf(s, &parte_int);
  if (fabs(parte_frac) < 0.5) return parte_int;
  return (parte_int > 0.0) ? (parte_int + 1) : (parte_int - 1);
}

/* aproxima as integrais pelo método do trapézio */
double integral_trapezio(int imin, int imax, const matriz *vc, double dt) {
  double somatorio;
  int i;
  somatorio = (vc->entradas[imin][0] + vc->entradas[imax][0]) / 2;
  for (i = imin + 1; i < imax; i++) {
    somatorio += vc->entradas[i][0];
  }
  return somatorio * dt;
}

/* retorna uma double aleatória, pressupõe srand */
double rand_entre(double menor, double maior) {
   return (rand() / (double) (RAND_MAX)) * abs(menor - maior) + menor;
}
