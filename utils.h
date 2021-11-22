/* utils.h
 *
 * Protótipo das funções utilitárias
 * 
 * Artur Freire dos Santos
 * Gabriel Gonçales
 * Lucas Silva dos Anjos
 * Vinicius Freire Pereira
 * 
 * Ciências da Computação
 * 
 * 10/OUT/2020
*/

#ifndef _UTILS_H
#define _UTILS_H
#define MAX_LENGTH 25
#define SPACING 18
#define LIST_LIMIT 51
#define COMP_AB(A, B) A > B
#define DIF_AB(A, B) (A - B)

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Função que aguarda o usuário pressionar ENTER para continuar a 
execução do programa. */
void congelar_tela();

/* Função recebe um vetor de caractere e seu inteiro de tamanho e lê as 
informações digitadas na linha. */
void read_line (char line[], int max_length);

/* Função recebe um inteiro para ser lido como entrada do teclado. */
bool read_int (int * var);

bool search_name(int * length, char names[][*], char * name, int * result);

bool add_path(
  int * rows,
  int * cols,
  int custos[][*],
  char origens[][*],
  char destinos[][*]
);

void print_table(
  int rows,
  int cols,
  char origens[][*],
  char destinos[][*],
  int table[][*],
  int ofertas[],
  int demandas[],
  bool camp_nulo
);

char ajusta_quadro(
  int * rows,
  int * cols,
  char origens[][*],
  char destinos[][*],
  int ofertas[],
  int demandas[]
);

void canto_noroeste(
  int rows,
  int cols,
  int * custo_inicial,
  int ofertas[],
  int demandas[],
  int custos[][*],
  int inicial[][*]
);

int valor_total (int max_length, int list[]);

int pegar_menor_valor (
  int row,
  int col,
  int posicao,
  int custos[][*],
  int usados[][*],
  int ordenados[],
  int * x,
  int * y
);

/**
 * Código dispobível em: https://www.treinaweb.com.br/blog/conheca-os-principais-algoritmos-de-ordenacao
 * Acesso: 11/NOV/2021 às 20:10.
*/
void mergeSort(int *vetor, int posicaoInicio, int posicaoFim);

int pegar_maior_valor (
  int rows, 
  int cols,
  int * x,
  int * y,
  int table[][*]
);

void menor_custo (
  int rows,
  int cols,
  int * custo_inicial,
  int ofertas[],
  int demandas[],
  int custos[][*],
  int inicial[][*],
  bool hasOthers,
  char where
);

void calcular_custos(
  int rows,
  int cols,
  int dx[],
  int dy[],
  int custos[][*],
  int inicial[][*],
  int economy[][*]
);

void calcular_economia (
  int rows,
  int cols,
  int dx[],
  int dy[],
  int custos[][*],
  int economy[][*]
);

bool otimizacao (int rows, int cols, int * custo_inicial, int custos[][*], int inicial[][*]);

void listener (
  int rows,
  int cols,
  int k_row,
  int k_col,
  int inicial[][*],
  int usados[][*],
  int * menor_pos,
  int * menor_neg
);

void resetar_table(int rows, int cols, int k_row, int k_col, int table[LIST_LIMIT][LIST_LIMIT]);

#endif
