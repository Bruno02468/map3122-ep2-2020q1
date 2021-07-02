
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../matrizes.h"
#include "../direto.h"
#include "../utils.h"

/* printar help e sair */
void falhou(char **argv) {
  printf("\nInvocação: %s [c^2=10] [beta=10] [T=1] [X=1] [xc=0.7] "
    "[nx=100] [nt=350] [arq. saída]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* esse programa resolve o problema direto: dados os parâmetros de onda e uma
 * função de fonte, estimar a função da onda em todo o tempo e espaço, ambos
 * discretizados. ele imprime esses valores de maneira a facilitar que o
 * programa em python plote tudo. */
int main(int argc, char **argv) {
  double ti, xj;
  int i, j;
  char *fname;
  FILE *fout;

  /* parâmetros da função de onda e seus valores-padrão */
  struct waveparam wp;
  matriz *u_vals;
  wp.c = 10; /* raiz vai ser tirada depois */
  wp.beta = 10;
  wp.T = 1;
  wp.X = 1;
  wp.xc = 0.7;
  wp.nx = 100;
  wp.nt = 350;


  /* obter argumentos do terminal */
  if (argc != 9) {
    printf("Número de argumentos incorreto!\n");
    falhou(argv);
  }
  if (getarg(argc, argv, "%lf", "c^2", &(wp.c), sizeof(double))
    * getarg(argc, argv, "%lf", "beta", &(wp.beta), sizeof(double))
    * getarg(argc, argv, "%lf", "T", &(wp.T), sizeof(double))
    * getarg(argc, argv, "%lf", "X", &(wp.X), sizeof(double))
    * getarg(argc, argv, "%lf", "xc", &(wp.xc), sizeof(double))
    * getarg(argc, argv, "%d", "nx", &(wp.nx), sizeof(int))
    * getarg(argc, argv, "%d", "nt", &(wp.nt), sizeof(int)) != 1) {
    falhou(argv);
  }
  fname = argv[8];

  /* calcular outros parâmetros para a solução numérica */
  wp.c = sqrt(wp.c);
  wp.dt = wp.T / wp.nt;
  wp.dx = wp.X / wp.nx;
  wp.alpha = wp.c*wp.dt/wp.dx;
  
  /* certificar-nos que o arquivo pode ser escrito */
  fout = fopen(fname, "w");
  if (!fout) {
    printf("Impossível abrir %s para leitura!\n", fname);
    falhou(argv);
  }

  /* resolver o problema direto e escrever os valores, já no formato certo para
   * serem lidos pelo programa em python que faz a animação */
  u_vals = solve_direto(&wp);
  for (i = 0; i <= wp.nt; i++) {
    ti = i*wp.dt;
    for (j = 0; j <= wp.nx; j++) {
      xj = j*wp.dx;
      fprintf(fout, "%g %g %g\n", ti, xj, u_vals->entradas[i][j]);
    }
  }
  fclose(fout);
  printf("Arquivo %s escrito.\n", fname);
  
  /* liberar todos os dados dinamicamente alocados e sair. */
  deletar_matriz(u_vals);
  return EXIT_SUCCESS;
}
