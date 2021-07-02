/* === direto.c ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: implementa funções de manipulação de matrizes
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "matrizes.h"


/* inicializa uma matriz de zeros */
matriz *matriz_nula(int linhas, int colunas) {
  matriz *m;
  int i;
  m = malloc(sizeof(matriz));
  m->linhas = linhas;
  m->colunas = colunas;
  m->entradas = malloc(linhas * sizeof(double *));
  for (i = 0; i < linhas; i++) {
    m->entradas[i] = calloc(colunas, sizeof(double));
  }
  return m;
}

/* deleta uma matriz */
void deletar_matriz(matriz *m) {
  int i;
  for (i = 0; i < m->linhas; i++) {
    free(m->entradas[i]);
  }
  free(m->entradas);
  free(m);
}

/* clona uma matriz */
matriz *clonar_matriz(const matriz *m) {
  int i;
  matriz *a;
  a = matriz_nula(m->linhas, m->colunas);
  for (i = 0; i < m->linhas; i++) {
    memcpy(a->entradas[i], m->entradas[i], m->colunas * sizeof(double));
  }
  return a;
}

/* sobrescreve uma matriz com outra */
void sobrescrever_matriz(const matriz *src, matriz *dest) {
  int i, j;
  for (i = 0; i < src->linhas; i++) {
    for (j = 0; j < src->colunas; j++) {
      dest->entradas[i][j] = src->entradas[i][j];
    }
  }
}

/* somar duas matrizes, criando uma nova */
matriz *matriz_soma(const matriz *a, const matriz *b) {
  int i, j;
  matriz *m;
  m = matriz_nula(a->linhas, a->colunas);
  for (i = 0; i < m->linhas; i++) {
    for (j = 0; j < m->colunas; j++) {
      m->entradas[i][j] = a->entradas[i][j] + b->entradas[i][j];
    }
  }
  return m;
}

/* soma a matriz b em a */
void somar_matriz(matriz *a, const matriz *b) {
  int i, j;
  for (i = 0; i < a->linhas; i++) {
    for (j = 0; j < a->colunas; j++) {
      a->entradas[i][j] += b->entradas[i][j];
    }
  }
}

/* multiplica uma matriz por um escalar */
void escalar_matriz(matriz *m, double s) {
  int i, j;
  for (i = 0; i < m->linhas; i++) {
    for (j = 0; j < m->colunas; j++) {
      m->entradas[i][j] *= s;
    }
  }
}

/* subtrai duas matrizes */
matriz *matriz_diferenca(const matriz *a, const matriz *b) {
  int i, j;
  matriz *m;
  m = matriz_nula(a->linhas, a->colunas);
  for (i = 0; i < m->linhas; i++) {
    for (j = 0; j < m->colunas; j++) {
      m->entradas[i][j] = a->entradas[i][j] - b->entradas[i][j];
    }
  }
  return m;
}

/* subtrai a matriz b de a */
void subtrair_matriz(matriz *a, const matriz *b) {
  int i, j;
  for (i = 0; i < a->linhas; i++) {
    for (j = 0; j < a->colunas; j++) {
      a->entradas[i][j] -= b->entradas[i][j];
    }
  }
}

/* extrai um vetor linha de uma matriz */
matriz *vetor_linha(const matriz *m, int linha) {
  matriz *v;
  v = matriz_nula(1, m->colunas);
  memcpy(v->entradas[0], m->entradas[linha], m->colunas * sizeof(double));
  return v;
}

/* extrai um vetor coluna de uma matriz */
matriz *vetor_coluna(const matriz *m, int coluna) {
  matriz *v;
  int i;
  v = matriz_nula(m->linhas, 1);
  for (i = 0; i < m->linhas; i++) {
    v->entradas[i][0] = m->entradas[i][coluna];
  }
  return v;
}

/* calcula o produto escalar entre um vetor-linha e um vetor-coluna */
double produto_escalar(const matriz *vl, const matriz *vc) {
  double s;
  int i;
  s = 0;
  for (i = 0; i < vc->linhas; i++) {
    s += vl->entradas[0][i] * vc->entradas[i][0];
  }
  return s;
}

/* multiplica duas matrizes (produto usual) */
matriz *matriz_produto(const matriz *a, const matriz *b) {
  int i, j;
  matriz *m, *vl, *vc;
  m = matriz_nula(a->linhas, b->colunas);
  for (i = 0; i < m->linhas; i++) {
    vl = vetor_linha(a, i);
    for (j = 0; j < m->colunas; j++) {
      vc = vetor_coluna(b, j);
      m->entradas[i][j] = produto_escalar(vl, vc);
      deletar_matriz(vc);
    }
    deletar_matriz(vl);
  }
  return m;
}

/* computa a soma dos quadrados dos módulos das entradas de uma matriz */
double magnitude_matriz(const matriz *m) {
  double s = 0;
  int i, j;
  for (i = 0; i < m->linhas; i++) {
    for (j = 0; j < m->colunas; j++) {
      s += m->entradas[i][j] * m->entradas[i][j];
    }
  }
  return sqrt(s);
}

/* computa a transposta real de uma matriz */
matriz *matriz_transposta(const matriz *m) {
  int i, j;
  matriz *t;
  t = matriz_nula(m->colunas, m->linhas);
  for (i = 0; i < m->linhas; i++) {
    for (j = 0; j < m->colunas; j++) {
      t->entradas[j][i] = m->entradas[i][j];
    }
  }
  return t;
}

/* computa uma "matriz menor", excluindo uma linha e uma coluna */
matriz *matriz_menor(const matriz *m, int linha, int coluna) {
  matriz *u;
  int i, j, a, b;
  u = matriz_nula(m->linhas - 1, m->colunas - 1);
  a = 0;
  for (i = 0; i < m->linhas; i++) {
    if (i == linha) continue;
    b = 0;
    for (j = 0; j < m->colunas; j++) {
      if (j == coluna) continue;
      u->entradas[a][b] = m->entradas[i][j];
      b++;
    }
    a++;
  }
  /* para suprimir a warning the que i e j não são usadas, quando na verdade
   * elas controlam os loops acima... */
  (void) i;
  (void) j;
  return u;
}

/* computa um cofator individual */
double cofator(const matriz *m, int linha, int coluna) {
  matriz *u;
  double cof;
  u = matriz_menor(m, linha, coluna);
  cof = (((linha + coluna) % 2) ? -1 : 1 ) * determinante(u);
  deletar_matriz(u);
  return cof;
}

/* computa a matriz dos cofatores de uma matriz */
matriz *matriz_cofatores(const matriz *m) {
  matriz *u;
  int i, j;
  u = matriz_nula(m->linhas, m->colunas);
  for (i = 0; i < m->linhas; i++) {
    for (j = 0; j < m->colunas; j++) {
      u->entradas[i][j] = cofator(m, i, j);
    }
  }
  return u;
}

/* computa o determinante de uma matriz */
double determinante(const matriz *m) {
  int i;
  double d;
  if (m->linhas == 1) {
    return m->entradas[0][0];
  } else if (m->linhas == 2) {
    return m->entradas[0][0] * m->entradas[1][1]
         - m->entradas[0][1] * m->entradas[1][0];
  } else {
    d = 0;
    for (i = 0; i < m->colunas; i++) {
      d += m->entradas[0][i] * cofator(m, 0, i);
    }
    return d;
  }
}

/* computa a matriz adjunta de uma matriz */
matriz *matriz_adjunta(const matriz *m) {
  matriz *cof, *adj;
  cof = matriz_cofatores(m);
  adj = matriz_transposta(cof);
  deletar_matriz(cof);
  return adj;
}

/* computa a inversa de uma matriz */
matriz *matriz_inversa(const matriz *m) {
  matriz *adj;
  adj = matriz_adjunta(m);
  escalar_matriz(adj, 1/determinante(m));
  return adj;
}

/* computa o produto de hadamard de duas matrizes */
matriz *matriz_hadamard(const matriz *a, const matriz *b) {
  int i, j;
  matriz *m;
  m = matriz_nula(a->linhas, a->colunas);
  for (i = 0; i < m->linhas; i++) {
    for (j = 0; j < m->colunas; j++) {
      m->entradas[i][j] = a->entradas[i][j] * b->entradas[i][j];
    }
  }
  return m;
}

/* imprime uma matriz de um jeito meio torto */
void imprimir_matriz(const matriz *m) {
  int i, j;
  for (i = 0; i < m->linhas; i++) {
    printf("| ");
    for (j = 0; j < m->colunas; j++) {
      printf("%3.5g ", m->entradas[i][j]);
    }
    printf("|\n");
  }
}

/* torna todos os itens de uma matriz seus fabs */
void positivar_matriz(matriz *m) {
  int i, j;
  for (i = 0; i < m->linhas; i++) {
    for (j = 0; j < m->colunas; j++) {
      m->entradas[i][j] = fabs(m->entradas[i][j]);
    }
  }
}
