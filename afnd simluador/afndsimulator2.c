#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TransicaoEstendida(int num_estados, int num_transicoes, int **transicoes, int num_finais, int *finais, char *W, int tam_alfabeto, int *alfabeto);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  FILE *fp;
  char linha[100];
  int i, j;

  fp = fopen(argv[1], "entrada.txt");
  if (fp == NULL) {
    printf("Could not open input file: %s\n", argv[1]);
    return 1;
  }

  // L� o alfabeto
  fgets(linha, 100, fp);
  int tam_alfabeto = strlen(linha) - 1;
  int *alfabeto = (int*) malloc(tam_alfabeto * sizeof(int));
  for (i = 0; i < tam_alfabeto; i++) {
    alfabeto[i] = linha[i] - 'a';
  }

  // L� o n�mero de estados
  int num_estados;
  fscanf(fp, "%d", &num_estados);

  // L� o n�mero de estados finais e seus valores
  int num_finais;
  fscanf(fp, "%d", &num_finais);
  int *finais = (int*) malloc(num_finais * sizeof(int));
  for (i = 0; i < num_finais; i++) {
    fscanf(fp, "%d", &finais[i]);
  }

  // L� o n�mero de transi��es e suas configura��es
  int num_transicoes;
  fscanf(fp, "%d", &num_transicoes);
  int **transicoes = (int**) malloc(num_transicoes * sizeof(int*));
  for (i = 0; i < num_transicoes; i++) {
    transicoes[i] = (int*) malloc(3 * sizeof(int));
    int origem, destino;
    char simbolo;
    fscanf(fp, "%d %c %c %c %d", &origem, &simbolo, &simbolo, &simbolo, &destino);
    transicoes[i][0] = origem;
    transicoes[i][1] = simbolo - 'a';
    transicoes[i][2] = destino;
  }

  // L� o n�mero de palavras e as palavras
  int num_palavras;
  fscanf(fp, "%d", &num_palavras);
  char **palavras = (char**) malloc(num_palavras * sizeof(char*));
  for (i = 0; i < num_palavras; i++) {
    palavras[i] = (char*) malloc(100 * sizeof(char));
    fscanf(fp, "%s", palavras[i]);
  }

  fclose(fp);

  // Processamento das palavras
for (i = 0; i < num_palavras; i++) {
  int aceita = TransicaoEstendida(num_estados, num_transicoes, transicoes, num_finais, finais, palavras[i], tam_alfabeto, alfabeto);
  if (aceita) {
    printf("Sim\n");
  } else {
    printf("Nao\n");
  }
}

// Libera a mem�ria alocada
free(alfabeto);
free(finais);
for (i = 0; i < num_transicoes; i++) {
  free(transicoes[i]);
}
free(transicoes);
for (i = 0; i < num_palavras; i++) {
  free(palavras[i]);
}
free(palavras);

return 0;
}

int TransicaoEstendida(int num_estados, int num_transicoes, int **transicoes, int num_finais, int *finais, char *W, int tam_alfabeto, int *alfabeto) {
  int i, j, k;
  int *atual = (int*) malloc(num_estados * sizeof(int));
  int *prox = (int*) malloc(num_estados * sizeof(int));
  int *aux;

  // Inicializa o vetor de estados atuais
  for (i = 0; i < num_estados; i++) {
    atual[i] = 0;
  }
  atual[0] = 1;

  // Percorre a palavra e realiza a transi��o estendida
  for (i = 0; i < strlen(W); i++) {
    // Reinicializa o vetor de estados pr�ximos
    for (j = 0; j < num_estados; j++) {
      prox[j] = 0;
    }

    // Percorre os estados atuais e realiza as transi��es
    for (j = 0; j < num_estados; j++) {
      if (atual[j]) {
        // Percorre as transi��es procurando por aquelas que partem do estado atual e consomem o s�mbolo da palavra atual
        for (k = 0; k < num_transicoes; k++) {
          if (transicoes[k][0] == j && transicoes[k][1] == W[i] - 'a') {
            prox[transicoes[k][2]] = 1;
          }
        }
      }
    }

    // Atualiza os estados atuais com os estados pr�ximos
    aux = atual;
    atual = prox;
    prox = aux;
  }

  // Verifica se algum dos estados atuais � final
  for (i = 0; i < num_estados; i++) {
    if (atual[i]) {
      for (j = 0; j < num_finais; j++) {
        if (i == finais[j]) {
          free(atual);
          free(prox);
          return 1;
        }
      }
    }
  }

  free(atual);
  free(prox);
  return 0;
}


