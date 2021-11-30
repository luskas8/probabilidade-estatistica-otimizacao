/* main.c
 *
 * Implementação do programa para otimização do
 * ṕroblema do transporte
 * 
 * Artur Freire dos Santos
 * Gabriel Gonçales
 * Lucas Silva dos Anjos
 * Vinicius Freire Pereira
 * 
 * Ciências da Computação
 * 
 * 29/OUT/2020
*/

#include <stdio.h>
#include <stdbool.h>
#include "utils.h"

int main() {
  int custos[LIST_LIMIT][LIST_LIMIT] = { 0 },
    inicial[LIST_LIMIT][LIST_LIMIT] = { 0 },
    demandas[LIST_LIMIT] = { 0 },
    ofertas[LIST_LIMIT] = { 0 },
    rows = 0,
    cols = 0,
    custo_inicial = 0;

  char destinos[LIST_LIMIT][MAX_LENGTH],
    origens[LIST_LIMIT][MAX_LENGTH];

  bool maisRotas = false;

  do {
    maisRotas = add_path( &rows, &cols, custos, origens, destinos);
    printf("\n");
  } while(maisRotas);

  printf("Origens:\n");
  for (int i = 0; i < rows; i++) {
    int index = -1, oferta = 0;

    if (!search_name(&cols, destinos, origens[i], &index)) {
      printf("ofertas %s: ", &origens[i][0]);
      read_int(&oferta);

      ofertas[i] = oferta;
    }
  }

  printf("\nDestinos:\n");
  for (int i = 0; i < cols; i++) {
    int index = -1, demanda = 0;

    if (!search_name(&rows, origens, destinos[i], &index)) {
      printf("demanda %s: ", &destinos[i][0]);
      read_int(&demanda);

      demandas[i] = demanda;
    }
  }

  for (int i = 0; i < rows; i++) {
    int index_d = -1, sum_ofertas = 0, sum_demandas = 0;

    if (search_name(&cols, destinos, origens[i], &index_d)) {
      for (int j = 0; j < rows; j++) {
        sum_ofertas += ofertas[j];
      }

      for (int j = 0; j < cols; j++) {
        sum_demandas += demandas[j];
      }

      if (sum_ofertas > sum_demandas) {
        ofertas[i] = sum_ofertas;
        demandas[index_d] = sum_ofertas;
      } else {
        ofertas[i] = sum_demandas;
        demandas[index_d] = sum_demandas;
      }
    }
  }

  int temp_x = 0, temp_y = 0,
      maior_custo = pegar_maior_valor(rows, cols, &temp_x, &temp_y, custos) * 10;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (custos[i][j] == 0) {
        if (strcasecmp(&origens[i][0], &destinos[j][0]) != 0) {
          custos[i][j] = maior_custo;
        }
      }
    }
  }
  char where = ajusta_quadro(&rows, &cols, origens, destinos, ofertas, demandas);
  bool hasOther = (where != ' ');

  printf("%s\n", "Quadro de Custos Unitarios");
  print_table(rows, cols, origens, destinos, custos, ofertas, demandas, false);

  canto_noroeste(rows, cols, &custo_inicial, ofertas, demandas, custos, inicial);
  printf("\n%s\n", "Solução Inicial Canto Noroeste");
  print_table(rows, cols, origens, destinos, inicial, ofertas, demandas, true);
  printf("Custo Inicial: %d\n", custo_inicial);
  int cpyCustoInicial = custo_inicial;

  menor_custo(rows, cols, &custo_inicial, ofertas, demandas, custos, inicial, hasOther, where);
  printf("\n%s\n", "Solução Inicial Menor Custo");
  print_table(rows, cols, origens, destinos, inicial, ofertas, demandas, true);
  printf("Custo Inicial: %d\n", custo_inicial);

  if (inicial[0][0] == -1) {
    canto_noroeste(rows, cols, &custo_inicial, ofertas, demandas, custos, inicial);
    print_table(rows, cols, origens, destinos, inicial, ofertas, demandas, true);
    
    bool otimizado = false;
    int interacaoN = 1;
    while (!otimizado) {
      otimizado = !otimizacao(rows, cols, &custo_inicial, custos, inicial);
      if (!otimizado) {
        printf("\nInteracao %d\n", interacaoN++);
        print_table(rows, cols, origens, destinos, inicial, ofertas, demandas, true);
        printf("Custo Inicial: %d\n", custo_inicial);
      }
    }
  } else {
    bool otimizado = false;
    int interacaoN = 1;
    while (!otimizado) {
      otimizado = !otimizacao(rows, cols, &custo_inicial, custos, inicial);
      if (!otimizado) {
        printf("\nInteracao %d\n", interacaoN++);
        print_table(rows, cols, origens, destinos, inicial, ofertas, demandas, true);
        printf("Custo Inicial: %d\n", custo_inicial);
      }
    }
  }


  return 0;
}
