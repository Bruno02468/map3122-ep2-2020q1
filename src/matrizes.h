/* === matrizes.h ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: cabeçalho das funções e definições exportadas de matrizes.c
 */

#ifndef MATRIZES_H
#define MATRIZES_H

/* tipo-base para uma matriz */
struct matriz {
  int linhas, colunas;
  double **entradas;
};

/* por simplicidade */
typedef struct matriz matriz;

/* inicializa uma matriz de zeros */
matriz *matriz_nula(int linhas, int colunas);

/* deleta uma matriz */
void deletar_matriz(matriz *m);

/* clona uma matriz */
matriz *clonar_matriz(const matriz *m);

/* sobrescreve uma matriz com outra */
void sobrescrever_matriz(const matriz *src, matriz *dest);

/* somar duas matrizes, criando uma nova */
matriz *matriz_soma(const matriz *a, const matriz *b);

/* soma a matriz b em a */
void somar_matriz(matriz *a, const matriz *b);

/* multiplica uma matriz por um escalar */
void escalar_matriz(matriz *m, double s);

/* subtrai duas matrizes */
matriz *matriz_diferenca(const matriz *a, const matriz *b);

/* subtrai a matriz b de a */
void subtrair_matriz(matriz *a, const matriz *b);

/* extrai um vetor linha de uma matriz */
matriz *vetor_linha(const matriz *m, int linha);

/* extrai um vetor coluna de uma matriz */
matriz *vetor_coluna(const matriz *m, int coluna);

/* calcula o produto escalar entre um vetor-linha e um vetor-coluna */
double produto_escalar(const matriz *vl, const matriz *vc);

/* multiplica duas matrizes (produto usual) */
matriz *matriz_produto(const matriz *a, const matriz *b);

/* computa a soma dos quadrados dos módulos das entradas de uma matriz */
double magnitude_matriz(const matriz *m);

/* computa a transposta real de uma matriz */
matriz *matriz_transposta(const matriz *m);

/* computa uma "matriz menor", excluindo uma linha e uma coluna */
matriz *matriz_menor(const matriz *m, int linha, int coluna);

/* computa um cofator individual */
double cofator(const matriz *m, int linha, int coluna);

/* computa a matriz dos cofatores de uma matriz */
matriz *matriz_cofatores(const matriz *m);

/* computa o determinante de uma matriz */
double determinante(const matriz *m);

/* computa a matriz adjunta de uma matriz */
matriz *matriz_adjunta(const matriz *m);

/* computa a inversa de uma matriz */
matriz *matriz_inversa(const matriz *m);

/* computa o produto de hadamard de duas matrizes */
matriz *matriz_hadamard(const matriz *a, const matriz *b);

/* imprime uma matriz de um jeito meio torto */
void imprimir_matriz(const matriz *m);

/* torna todos os itens de uma matriz seus fabs */
void positivar_matriz(matriz *m);

#endif
