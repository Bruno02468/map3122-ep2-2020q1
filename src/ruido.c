/* === direto.c ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: implementa funções de inserção de ruído
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "matrizes.h"
#include "utils.h"
#include "ruido.h"

/* insere ruído num vetor-coluna, e retorna o nível de ruído */
double inserir_ruido(matriz *vc, double eta, int imin, int imax, double dt) {
  double top, ruido;
  int i;
  matriz *vr, *vp, *delta;

  /* determinar o máximo */
  top = vc->entradas[imin][0];
  for (i = imin; i <= imax; i++) {
    top = MAX(top, vc->entradas[i][0]);
  }

  /* inserir ruído */
  vr = clonar_matriz(vc);
  srand(time(NULL));
  for (i = imin; i <= imax; i++) {
    vr->entradas[i][0] *= 1 + eta*top*rand_entre(-1, 1);
  }

  /* calcular nível de ruído */
  delta = matriz_diferenca(vc, vr);
  positivar_matriz(delta);
  vp = clonar_matriz(vc);
  positivar_matriz(vp);

  ruido = 100 * integral_trapezio(imin, imax, delta, dt)
    / integral_trapezio(imin, imax, vp, dt);

  /* sobrescrever o vetor-coluna, dar free, e retornar o nível de ruído */
  sobrescrever_matriz(vr, vc);
  deletar_matriz(vr);
  deletar_matriz(vp);
  deletar_matriz(delta);

  return ruido;
}
