/* utils.c
 *
 * Implementação de funções utilitárias
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

#include "utils.h"

void congelar_tela() {
    printf("Pressione ENTER para voltar ao menu...\n");
    char ch;
    read_line(&ch, 1);
}

void read_line(char line[], int max_length) {
    int i = 0;
    char ch;

    while ((ch = getchar()) != '\n') {
        if (i < max_length)
            line[i++] = ch;
    }
    line[i] = '\0';
}

bool read_int(int * var) {
    int result;
    char line[MAX_LENGTH + 1];
    read_line(line, MAX_LENGTH);
    result = sscanf(line, "%d", var);
    return (result == 1);
}

bool search_name(int * length, char names[][MAX_LENGTH], char * name, int * result) {
  if (*length == 0) return false;

  for (int i = 0; i < *length; i++) {
    if (strcasecmp(&names[i][0], name) == 0) {
      *result = i;
      return true;
    }
  }

  return false;
}

bool add_path(
  int * rows,
  int * cols,
  int custos[LIST_LIMIT][LIST_LIMIT],
  char origens[LIST_LIMIT][MAX_LENGTH],
  char destinos[LIST_LIMIT][MAX_LENGTH]
) {
  int indexOrigem = -1, indexDestino = -1, custo = 0;
  char nomeOrigem[MAX_LENGTH], nomeDestino[MAX_LENGTH], maisPaths;

  printf("origem: ");
  read_line(nomeOrigem, MAX_LENGTH);
  printf("destino: ");
  read_line(nomeDestino, MAX_LENGTH);
  printf("custo: ");
  read_int(&custo);

  if (!search_name(rows, origens, nomeOrigem, &indexOrigem)) {
    strcpy(origens[*rows], nomeOrigem);
  }

  if (!search_name(cols, destinos, nomeDestino, &indexDestino)) {
    strcpy(destinos[*cols], nomeDestino);
  }

  if (indexOrigem == -1) {
    indexOrigem = *rows;
    *rows += 1;
  }

  if (indexDestino == -1) {
    indexDestino = *cols;
    *cols += 1;
  }

  custos[indexOrigem][indexDestino] = custo;

  printf("mais rotas? (S/N): ");
  read_line(&maisPaths, 1);

  while (toupper(maisPaths) != 'S' && toupper(maisPaths) != 'N') {
    printf("mais rotas? (S/N): ");
    read_line(&maisPaths, 1);
  }

  return toupper(maisPaths) == 'S';
}

void print_table(
  int rows,
  int cols,
  char origens[LIST_LIMIT][MAX_LENGTH],
  char destinos[LIST_LIMIT][MAX_LENGTH],
  int table[LIST_LIMIT][LIST_LIMIT],
  int ofertas[],
  int demandas[],
  bool campo_nulo
) {
  printf("%*s", -(2*SPACING), " ");
  printf("Destino\n%*s", -SPACING, "Origem");
  for (int i = 0; i < cols; i++) {
    printf("%*s", -SPACING, &destinos[i][0]);
  }
  printf("%*s\n", -SPACING, "Oferta");

  for (int i = 0; i < rows; i++) {
    printf("%*s", -SPACING, &origens[i][0]);

    for (int j = 0; j < cols; j++) {
      if (campo_nulo && table[i][j] == -1) {
        printf("%*s", -SPACING, " ");
      } else {
        printf("%*d", -SPACING, table[i][j]);
      }
    }

    printf("%*d\n", -SPACING, ofertas[i]);
  }

  printf("%*s", -SPACING, "Demanda");
  for (int i = 0; i < cols; i++) {
    printf("%*d", -SPACING, demandas[i]);
  }
  printf("\n");
}

char ajusta_quadro(
  int * rows,
  int * cols,
  char origens[LIST_LIMIT][MAX_LENGTH],
  char destinos[LIST_LIMIT][MAX_LENGTH],
  int ofertas[LIST_LIMIT],
  int demandas[LIST_LIMIT]

) {
  int sum_o = 0, sum_d = 0;
  char where = ' ';

  for (int i = 0; i < *rows; i++) {
    sum_o += ofertas[i];
  }

  for (int i = 0; i < *cols; i++) {
    sum_d += demandas[i];
  }

  int diff = DIF_AB(sum_o, sum_d);
  if (diff > 0) {
    demandas[*cols] =  diff;
    strcpy(destinos[*cols], "Outros");
    where = 'd';
    *cols += 1;
  } else if (diff < 0) {
    ofertas[*rows] = (-1 * diff);
    strcpy(origens[*rows], "Outros");
    where = 'o';
    *rows += 1;
  }

  return where;
}

void canto_noroeste(
  int rows,
  int cols,
  int * custo_inicial,
  int ofertas[LIST_LIMIT],
  int demandas[LIST_LIMIT],
  int custos[LIST_LIMIT][LIST_LIMIT],
  int inicial[LIST_LIMIT][LIST_LIMIT]
) {
  *custo_inicial = 0;
  int cpyOfertas[LIST_LIMIT] = { 0 },
      cpyDemandas[LIST_LIMIT] = { 0 };
  
 for (int i = 0; i < rows; i++) {
    cpyOfertas[i] = ofertas[i];
    for (int j = 0; j < cols; j++) {
      cpyDemandas[j] = demandas[j];
      inicial[i][j] = 0;
    }
  }

  int i = 0, j = 0;
  while (i < rows && j < cols) {
    if (cpyDemandas[j] > cpyOfertas[i]) {
      inicial[i][j] = cpyOfertas[i];
      cpyDemandas[j] -= cpyOfertas[i];
      i += 1;
    } else {
      inicial[i][j] = cpyDemandas[j];
      cpyOfertas[i] -= cpyDemandas[j];
      j += 1;
    }
  }

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      *custo_inicial += custos[i][j] * inicial[i][j];
      if (inicial[i][j] == 0) inicial[i][j] = -1;
    }
  }
}

int valor_total (int max_length, int list[]) {
  int result = 0;
  for (int i = 0; i < max_length; i++) {
    result += list[i];
  }

  return result;
}

int pegar_menor_valor (
  int rows, 
  int cols,
  int posicao,
  int custos[LIST_LIMIT][LIST_LIMIT],
  int usados[LIST_LIMIT][LIST_LIMIT],
  int ordenados[],
  int * x,
  int * y
) {
  int result = ordenados[posicao];

  while (result == -1) {
    posicao += 1;
    result = ordenados[posicao];
  }

  for (int i = 0; i < rows; i++) {
    for ( int j = 0; j < cols; j++) {
      if (custos[i][j] == result && usados[i][j] != -1) {
        *x = i;
        *y = j;
        usados[*x][*y] = -1;

        return custos[i][j];
      }
    }
  }
}

void mergeSort(int *vetor, int posicaoInicio, int posicaoFim) {
    int i, j, k, metadeTamanho, *vetorTemp;
    if(posicaoInicio == posicaoFim) return;
    metadeTamanho = (posicaoInicio + posicaoFim ) / 2;

    mergeSort(vetor, posicaoInicio, metadeTamanho);
    mergeSort(vetor, metadeTamanho + 1, posicaoFim);

    i = posicaoInicio;
    j = metadeTamanho + 1;
    k = 0;
    vetorTemp = (int *) malloc(sizeof(int) * (posicaoFim - posicaoInicio + 1));

    while(i < metadeTamanho + 1 || j  < posicaoFim + 1) {
        if (i == metadeTamanho + 1 ) { 
            vetorTemp[k++] = vetor[j++];
        }
        else {
            if (j == posicaoFim + 1) {
                vetorTemp[k++] = vetor[i++];
            }
            else {
                if (vetor[i] < vetor[j]) {
                    vetorTemp[k++] = vetor[i++];
                }
                else {
                    vetorTemp[k++] = vetor[j++];
                }
            }
        }

    }
    for(i = posicaoInicio; i <= posicaoFim; i++) {
        vetor[i] = vetorTemp[i - posicaoInicio];
    }
    free(vetorTemp);
}

int pegar_maior_valor (
  int rows, 
  int cols,
  int * x,
  int * y,
  int table[LIST_LIMIT][LIST_LIMIT]
) {
  int index = 0, * ordenados, result = 0, max_length = (rows * cols);

  ordenados = (int *) malloc(sizeof(int) * (rows * cols + 1));

  for (int i = 0; i < rows; i++) {
    for ( int j = 0; j < cols; j++) {
      ordenados[index++] = table[i][j];
    }
  }

  mergeSort(ordenados, 0, (rows * cols - 1));
  result = ordenados[max_length-1];

  for (int i = 0; i < rows; i++) {
    for ( int j = 0; j < cols; j++) {
      if (table[i][j] == result) {
        *x = i;
        *y = j;

        free(ordenados);
        return table[i][j];
      }
    }
  }
}

void menor_custo (
  int rows,
  int cols,
  int * custo_inicial,
  int ofertas[LIST_LIMIT],
  int demandas[LIST_LIMIT],
  int custos[LIST_LIMIT][LIST_LIMIT],
  int inicial[LIST_LIMIT][LIST_LIMIT],
  bool hasOthers,
  char where
) {
  *custo_inicial = 0;
  int cpyOfertas[LIST_LIMIT] = { 0 },
      cpyDemandas[LIST_LIMIT] = { 0 },
      ordenado[LIST_LIMIT * LIST_LIMIT] = { 0 },
      usados[LIST_LIMIT][LIST_LIMIT] = { 0 };
  
  int row = 0, col = 0, passed = 0, max_length = 0;

  if (hasOthers) {
    if (where == 'd') cols -= 1;
    else rows -= 1;
  }

  for (int i = 0; i < rows; i++) {
    cpyOfertas[i] = ofertas[i];
    for (int j = 0; j < cols; j++) {
      ordenado[max_length++] = custos[i][j];
      cpyDemandas[j] = demandas[j];
      inicial[i][j] = 0;
    }
  }
  mergeSort(ordenado, 0, max_length-1);

  while (passed < max_length) {
    pegar_menor_valor(rows, cols, passed, custos, usados, ordenado, &row, &col);

    if (cpyDemandas[col] > cpyOfertas[row]) {
      if (cpyDemandas[col] > 0 || cpyOfertas[row] > 0 && cpyDemandas[col] - cpyOfertas[row] >= 0) {
        inicial[row][col] = cpyOfertas[row];

        cpyDemandas[col] -= cpyOfertas[row];
        cpyOfertas[row] -= cpyOfertas[row];
      }
    } else if (cpyDemandas[col] < cpyOfertas[row]) {
      if (cpyDemandas[col] > 0 || cpyOfertas[row] > 0 && cpyDemandas[col] - cpyOfertas[row] >= 0) {
        inicial[row][col] = cpyDemandas[col];

        cpyOfertas[row] -= cpyDemandas[col];
        cpyDemandas[col] -= cpyDemandas[col];
      }
    } else if (cpyDemandas[col] == cpyOfertas[row] && cpyDemandas[col] != 0 && cpyOfertas[row] != 0) {
      inicial[row][col] = cpyOfertas[row];
      cpyDemandas[col] = cpyOfertas[row] = 0;
    }

    passed += 1;
  }

  if (hasOthers) {
    if (where == 'o') {
      for (int i = 0; i < cols; i++) {
        if (cpyDemandas[i] != 0) {
          inicial[rows][i] = cpyDemandas[i];
        } else {
          inicial[rows][i] = -1;
        }
      }
    } else {
      for (int i = 0; i < rows; i++) {
        if (cpyOfertas[i] != 0) {
          inicial[i][cols] = cpyOfertas[i];
        } else {
          inicial[i][cols] = -1;
        }
      }
    }
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      *custo_inicial += custos[i][j] * inicial[i][j];
      if (inicial[i][j] == 0) inicial[i][j] = -1;
    }
  }
}

bool otimizacao (
  int rows,
  int cols,
  int * custo_inicial,
  int custos[LIST_LIMIT][LIST_LIMIT],
  int inicial[LIST_LIMIT][LIST_LIMIT]
) {
  *custo_inicial = 0;
  int economy[LIST_LIMIT][LIST_LIMIT] = { 0 },
      ordenados[LIST_LIMIT * LIST_LIMIT] = { 0 },
      usados[LIST_LIMIT][LIST_LIMIT] = { 0 },
      dx[LIST_LIMIT] = { 0 },
      dy[LIST_LIMIT] = { 0 },
      k = 0, k_row = 0, k_col = 0,
      hasEconomy = 0, menor_pos = -1, menor_neg = -1;

  calcular_custos(rows, cols, dx, dy, custos, inicial, economy);
  calcular_economia(rows, cols, dx, dy, custos, economy);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (economy[i][j] <= 0) hasEconomy++;
    }
  }

  if (hasEconomy <= 0) return false;

  k = pegar_maior_valor(rows, cols, &k_row, &k_col, economy);

  while (inicial[k_row][k_col] != -1) {
    economy[k_row][k_col] = -1;
    k = pegar_maior_valor(rows, cols, &k_row, &k_col, economy);
  }

  listener(rows, cols, k_row, k_col, inicial, usados, &menor_pos, &menor_neg);
  usados[k_row][k_col] = 1;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (
        (inicial[i][j] == 0 && !(usados[i][j] == -1 || usados[i][j] == 1))
        || (inicial[i][j] == 1 && !(usados[i][j] == -1 || usados[i][j] == 1))
      ) {
        inicial[i][j] = -1;
      }
    }
  }

  int sum_pos = 0, sum_neg = 0;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (usados[i][j] == 1) {
        sum_pos++;
      } else if (usados[i][j] == -1) {
        sum_neg++;
      }
    }
  }

  if (sum_neg != sum_pos) return false;

  int x = 0, y = 0, menor = -1;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (usados[i][j] == 1) {
        if (i == k_row && j == k_col) inicial[i][j] = menor_neg;
        else {
          if (inicial[i][j] == -1) inicial[i][j] += menor_neg + 1;
          else inicial[i][j] += menor_neg;
        }
      } else if (usados[i][j] == -1) {
        inicial[i][j] -= menor_neg;

        if (menor == -1) {
          menor = inicial[i][j];
          x = i;
          y = j;
        } else if (inicial[i][j] <= 0 && inicial[i][j] <= menor) {
          menor = inicial[i][j];
          x = i;
          y = j;
        }
      }
    }
  }
  inicial[x][y] = -1;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int aux1 = 0, aux2 = 0;
      if (custos[i][j] != -1) aux1 = custos[i][j];
      if (inicial[i][j] != -1) aux2 = inicial[i][j];

      *custo_inicial += aux1 * aux2;
    }
  }

  return true;
}

void listener (
  int rows,
  int cols,
  int k_row,
  int k_col,
  int inicial[LIST_LIMIT][LIST_LIMIT],
  int usados[LIST_LIMIT][LIST_LIMIT],
  int * menor_pos,
  int * menor_neg
) {
  int inicial_row = k_row,
      inicial_col = k_col,
      round = -1;
  
  resetar_table(rows, cols, k_row, k_col, usados);
  do {
    inicial_row += 1;

    if (
      inicial_row < rows && inicial[inicial_row][inicial_col] != -1
      && usados[inicial_row][inicial_col] == 0
    ) {
      usados[inicial_row][inicial_col] = -1;
      
      if (inicial[inicial_row][inicial_col] == -1) *menor_neg = 0;
      else *menor_neg = inicial[inicial_row][inicial_col];

      do {
        inicial_col += 1;

        if (
          inicial_col < cols && inicial[inicial_row][inicial_col] != -1
          && usados[inicial_row][inicial_col] == 0
          && inicial[inicial_row][inicial_col] != -1
          && inicial[k_row][inicial_col] != -1
        ) {
          usados[inicial_row][inicial_col] = 1;

          usados[k_row][inicial_col] = -1;
          if (inicial[k_row][inicial_col] < *menor_neg) {
            if (inicial[k_row][inicial_col] == -1) *menor_neg = 0;
            else *menor_neg = inicial[k_row][inicial_col];
          }
          return;
        }
      } while (inicial_col < cols);

      resetar_table(rows, cols, k_row, k_col, usados);
      inicial_col = k_col;
      usados[inicial_row][inicial_col] = -1;
      do {
        inicial_col -= 1;

        if (
          inicial_col >= 0 && inicial[inicial_row][inicial_col] != -1
          && usados[inicial_row][inicial_col] == 0
          && inicial[inicial_row][inicial_col] != -1
          && inicial[k_row][inicial_col] != -1
        ) {
          usados[inicial_row][inicial_col] = 1;

          usados[k_row][inicial_col] = -1;
          if (inicial[k_row][inicial_col] < *menor_neg) {
            if (inicial[k_row][inicial_col] == -1) *menor_neg = 0;
            else *menor_neg = inicial[k_row][inicial_col];
          }
          return;
        }
      } while (inicial_col >= 0);
      
      inicial_col = k_col;
      usados[inicial_row][inicial_col] = -1;
    }
  } while(inicial_row < rows);

  resetar_table(rows, cols, k_row, k_col, usados);
  inicial_row = k_row;
  do {
    inicial_row -= 1;

    if (
      inicial_row >= 0 && inicial[inicial_row][inicial_col] != -1
      && usados[inicial_row][inicial_col] == 0
    ) {
      usados[inicial_row][inicial_col] = -1;
      if (inicial[inicial_row][inicial_col] == -1) *menor_neg = 0;
      else *menor_neg = inicial[inicial_row][inicial_col];

      do {
        inicial_col += 1;

        if (
          inicial_col < cols && inicial[inicial_row][inicial_col] != -1
          && usados[inicial_row][inicial_col] == 0
          && inicial[inicial_row][inicial_col] != -1
          && inicial[k_row][inicial_col] != -1
        ) {
           usados[inicial_row][inicial_col] = 1;

          usados[k_row][inicial_col] = -1;
          if (inicial[k_row][inicial_col] < *menor_neg) {
            if (inicial[k_row][inicial_col] == -1) *menor_neg = 0;
            else *menor_neg = inicial[k_row][inicial_col];
          }
          return;
        }
      } while (inicial_col < cols);

      resetar_table(rows, cols, k_row, k_col, usados);
      inicial_col = k_col;
      usados[inicial_row][inicial_col] = 0;
      do {
        inicial_col -= 1;

        if (
          inicial_col >= 0 && inicial[inicial_row][inicial_col] != -1
          && usados[inicial_row][inicial_col] == 0
          && inicial[inicial_row][inicial_col] != -1
          && inicial[k_row][inicial_col] != -1
        ) {
          usados[inicial_row][inicial_col] = 1;

          usados[k_row][inicial_col] = -1;
          if (inicial[k_row][inicial_col] < *menor_neg) {
            if (inicial[k_row][inicial_col] == -1) *menor_neg = 0;
            else *menor_neg = inicial[k_row][inicial_col];
          }
          return;
        }
      } while (inicial_col >= 0);
      
      inicial_col = k_col;
      usados[inicial_row][inicial_col] = 0;
    }
  } while(inicial_row >= 0);

  resetar_table(rows, cols, k_row, k_col, usados);
  do {
    inicial_col += 1;

    if (
      inicial_col < cols && inicial[inicial_row][inicial_col] != -1
      && usados[inicial_row][inicial_col] == 0
    ) {
      usados[inicial_row][inicial_col] = -1;
      if (inicial[inicial_row][inicial_col] == -1) *menor_neg = 0;
      else *menor_neg = inicial[inicial_row][inicial_col];

      do {
        inicial_row += 1;

        if (
          inicial_row < rows && inicial[inicial_row][inicial_col] != -1
          && usados[inicial_row][inicial_col] == 0
          && inicial[inicial_row][inicial_col] != -1
          && inicial[inicial_row][k_col] != -1
        ) {
          usados[inicial_row][inicial_col] = 1;

          usados[k_row][inicial_col] = -1;
          if (inicial[inicial_row][k_col] < *menor_neg) {
            if (inicial[inicial_row][k_col] == -1) *menor_neg = 0;
            else *menor_neg = inicial[inicial_row][k_col];
          }
          return;
        }
      } while (inicial_row < rows);
      

      resetar_table(rows, cols, k_row, k_col, usados);
      inicial_row = k_row;
      usados[inicial_row][inicial_col] = -1;
      do {
        inicial_row -= 1;

        if (
          inicial_row >= 0 && inicial[inicial_row][inicial_col] != -1
          && usados[inicial_row][inicial_col] == 0
          && inicial[inicial_row][inicial_col] != -1
          && inicial[inicial_row][k_col] != -1
        ) {
          usados[inicial_row][inicial_col] = 1;

          usados[k_row][inicial_col] = -1;
          if (inicial[inicial_row][k_col] < *menor_neg) {
            if (inicial[inicial_row][k_col] == -1) *menor_neg = 0;
            else *menor_neg = inicial[inicial_row][k_col];
          }
          return;
        }
      } while (inicial_row >= 0);

      inicial_row = k_row;
      usados[inicial_row][inicial_col] = -1;
    }
  } while (inicial_col < cols);

  resetar_table(rows, cols, k_row, k_col, usados);
  do {
    inicial_col -= 1;

    if (
      inicial_col >= 0 && inicial[inicial_row][inicial_col] != -1
      && usados[inicial_row][inicial_col] == 0
    ) {
      usados[inicial_row][inicial_col] = -1;
      if (inicial[inicial_row][inicial_col] == -1) *menor_neg = 0;
      else *menor_neg = inicial[inicial_row][inicial_col];

      do {
        inicial_row += 1;

        if (
          inicial_row < rows && inicial[inicial_row][inicial_col] != -1
          && usados[inicial_row][inicial_col] == 0
          && inicial[inicial_row][inicial_col] != -1
          && inicial[inicial_row][k_col] != -1
        ) {
          usados[inicial_row][inicial_col] = 1;

          usados[k_row][inicial_col] = -1;
          if (inicial[inicial_row][k_col] < *menor_neg) {
            if (inicial[inicial_row][k_col] == -1) *menor_neg = 0;
            else *menor_neg = inicial[inicial_row][k_col];
          }
          return;
        }
      } while (inicial_row < rows);

      resetar_table(rows, cols, k_row, k_col, usados);
      inicial_row = k_row;
      usados[inicial_row][inicial_col] = -1;
      do {
        inicial_row -= 1;

        if (
          inicial_row >= 0 && inicial[inicial_row][inicial_col] != -1
          && usados[inicial_row][inicial_col] == 0
          && inicial[inicial_row][inicial_col] != -1
          && inicial[inicial_row][k_col] != -1
        ) {
          usados[inicial_row][inicial_col] = 1;

          usados[k_row][inicial_col] = -1;
          if (inicial[inicial_row][k_col] < *menor_neg) {
            if (inicial[inicial_row][k_col] == -1) *menor_neg = 0;
            else *menor_neg = inicial[inicial_row][k_col];
          }
          return;
        }
      } while (inicial_row >= 0);

      inicial_row = k_row;
      usados[inicial_row][inicial_col] = -1;
    }
  } while (inicial_col >= 0);
}

void resetar_table(int rows, int cols, int k_row, int k_col, int table[LIST_LIMIT][LIST_LIMIT]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j< cols; j++) {
      table[i][j] = 0;
    }
  }
}

void calcular_custos(
  int rows,
  int cols,
  int dx[LIST_LIMIT],
  int dy[LIST_LIMIT],
  int custos[LIST_LIMIT][LIST_LIMIT],
  int inicial[LIST_LIMIT][LIST_LIMIT],
  int economy[LIST_LIMIT][LIST_LIMIT]
) {
  int rotas = -1, e = 0;

  for (int i = 0; i < rows; i++) {
    dx[i] = -1;
    for (int j = 0; j < cols; j++) {
      dy[j] = -1;
      if (inicial[i][j] == -1) {
        economy[i][j] = 0;
        ++rotas;
      } else {
        economy[i][j] = -1;
      }
    }
  }
  dx[0] = 0;

  int x = 0, y = 0;

  do {
    if (inicial[x][y] != -1) {
      if (dx[x] != -1) {
        dy[y] = (custos[x][y] - dx[x]);
      } else if (dy[y] != -1) {
        dx[x] = (custos[x][y] - dy[y]);
      }
    }
    y++;
    
    if (y >= cols) {
      x++; y = 0;
    } 
  } while (x < rows);
  x = -1;

  while (rows+cols > rotas) {
    for (int i = 0; i < rows; i++) {
      if (dx[i] == -1) {
        x = i;
        break;
      }
    }
    if (x == -1) break;
    for (int i = 0; i < cols; i++) {
      if (x != -1 && dy[i] != -1 && inicial[x][i] == -1) {
        inicial[x][i] = e++;
        dx[x] = inicial[x][i] - dy[i];
        y = i;
        break;
      }
    }

    for (int i = 0; i < cols; i++) {
      if (i != y && inicial[x][i] != -1) {
        dy[i] = inicial[x][i] - dx[x];
      }
    }

    rotas++;
  }
}

void calcular_economia (
  int rows,
  int cols,
  int dx[LIST_LIMIT],
  int dy[LIST_LIMIT],
  int custos[LIST_LIMIT][LIST_LIMIT],
  int economy[LIST_LIMIT][LIST_LIMIT]
) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (economy[i][j] != -1) {
        economy[i][j] = -1 * (custos[i][j] - dx[i] - dy[j]);
      }
    }
  }
}

