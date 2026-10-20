#ifndef LISTA_CABECA_H
#define LISTA_CABECA_H

#include <stdbool.h>

#include "../item.h"

typedef struct lista_cabeca LISTA_CABECA;
typedef struct no NO;

/* Cria uma lista vazia (com nó cabeça). Retorna NULL em caso de erro. */
extern LISTA_CABECA *lista_criar(void);

/*
 * Apaga a lista, liberando todos os nós e também os itens armazenados
 * (via delete_item). Zera *lista.
 */
extern bool lista_apagar(LISTA_CABECA **lista);

/*
 * Insere o item no final da lista. A lista passa a ser dona do item:
 * ele será liberado em lista_apagar. Retorna false em caso de erro.
 */
extern bool lista_inserir(LISTA_CABECA *lista, ITEM *item);

#endif
