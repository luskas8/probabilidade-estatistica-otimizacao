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
 * 29/OUT/2020
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

/* Função que adiciona nós */
bool add_path(
  int * rows,
  int * cols,
  int custos[][*],
  char origens[][*],
  char destinos[][*]
);

/* Função que escreve no console a tabela formatando com a devida tabulação */
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

/* Função que ajusta os nós fictícios e balanceia tabela inicial */
char ajusta_quadro(
  int * rows,
  int * cols,
  char origens[][*],
  char destinos[][*],
  int ofertas[],
  int demandas[]
);

/* Função que acha a solução inicial pelo canto noroeste */
void canto_noroeste(
  int rows,
  int cols,
  int * custo_inicial,
  int ofertas[],
  int demandas[],
  int custos[][*],
  int inicial[][*]
);

/* Função que soma os elementos da list e devolve esse valor  */
int valor_total (int max_length, int list[]);

/* Função que acha e devolve o valor e posição do 1° menor elemento duma tabela */
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
 * 
 * Função para ordenar crescentemente um dado vetor
*/
void mergeSort(int *vetor, int posicaoInicio, int posicaoFim);

/* Função que acha e devolve o valor e posição do 1° maior elemento dema tabela */
int pegar_maior_valor (
  int rows, 
  int cols,
  int * x,
  int * y,
  int table[][*]
);

/* Função que acha a solução inicial pelo método de menor custo */
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

/* Função que calcula o custo de transporte para certas rotas */
void calcular_custos(
  int rows,
  int cols,
  int dx[],
  int dy[],
  int custos[][*],
  int inicial[][*],
  int economy[][*]
);

/* Função que calcula a economia para certas rotas */
void calcular_economia (
  int rows,
  int cols,
  int dx[],
  int dy[],
  int custos[][*],
  int economy[][*]
);

/* Função que faz a otimização da tabela para um K */
bool otimizacao (int rows, int cols, int * custo_inicial, int custos[][*], int inicial[][*]);

/* Função que acha o melhor caminho para K */
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

/* Função que coloca os valores de determinada tabela zerados */
void resetar_table(int rows, int cols, int k_row, int k_col, int table[LIST_LIMIT][LIST_LIMIT]);

#endif
