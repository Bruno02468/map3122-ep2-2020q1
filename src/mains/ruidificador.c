/* === ex2.c ===
 *
 * autor: Bruno Borges Paschoalinoto
 *
 * descrição: este programa resolve o problema inverso (exercício 2)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../matrizes.h"
#include "../utils.h"
#include "../ruido.h"

/* printar help e sair */
void falhou(char **argv) {
  printf("\nInvocação: %s [nt] [T] [ti] [tf] [eta] [arq. entrada] "
    "[arq. saída] [fatiar=n]\n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  double T, eta, ruido, ti, tf, dt;
  int i, nt, imin, imax;
  char *fn_in, *fn_out, *fatiar;
  FILE *fin, *fout;
  matriz *dr;

  /* obter argumentos do terminal e preparar arquivos */
  if (argc != 9
    || (getarg(argc, argv, "%d", "nt", &nt, sizeof(int))
    * getarg(argc, argv, "%lf", "T", &T, sizeof(double))
    * getarg(argc, argv, "%lf", "ti", &ti, sizeof(double))
    * getarg(argc, argv, "%lf", "tf", &tf, sizeof(double))
    * getarg(argc, argv, "%lf", "eta", &eta, sizeof(double)) < 1)) {
    falhou(argv);
  }
  dt = T/nt;
  imin = c89_round(ti/dt);
  imax = c89_round(tf/dt);
  fn_in = argv[6];
  fn_out = argv[7];
  fatiar = argv[8];
  if (strcmp(fatiar, "n") && strcmp(fatiar, "s")) {
    printf("O argumento \"fatiar\" deve ser \"s\" ou \"n\"!\n");
    falhou(argv);
  }
  fin = fopen(fn_in, "r");
  if (!fin) {
    printf("Impossível abrir %s para leitura.\n", fn_in);
    falhou(argv);
  }
  fout = fopen(fn_out, "w");
  if (!fout) {
    printf("Impossível abrir %s para escrita.\n", fn_out);
    falhou(argv);
  }
  
  /* ler e inserir ruído */
  dr = matriz_nula(nt + 1, 1);
  for (i = 0; i <= nt; i++) {
    if (!fscanf(fin, "%lf", &(dr->entradas[i][0]))) {
      printf("Valor inválido informado em dr (i=%d)!\n", i);
      falhou(argv);
    }
  }
  fclose(fin);
  ruido = inserir_ruido(dr, eta, imin, imax, dt);
  
  /* escrever arquivo, reportar nível de ruído, e sair */
  if (fatiar[0] == 'n') {
    imin = 0;
    imax = nt;
  }
  for (i = imin; i <= imax; i++) {
    fprintf(fout, "%g\n", dr->entradas[i][0]);
  }
  printf("Arquivo %s escrito (%d linhas). Nível de ruído: %.3f%%.\n",
    fn_out, imax - imin + 1, ruido);
  fclose(fout);
  deletar_matriz(dr);

  return EXIT_SUCCESS;
}
