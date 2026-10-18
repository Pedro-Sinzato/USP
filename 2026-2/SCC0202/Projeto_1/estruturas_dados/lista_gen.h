
#ifndef LISTAGEN_H
#define LISTAGEN_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_ LISTA;

LISTA *lista_criar(void);
void lista_apagar(LISTA **lista);

bool lista_inserir_atomo(LISTA *lista, ITEM *item);
bool lista_inserir_sublista(LISTA *lista, LISTA *sublista);

ITEM *lista_buscar(LISTA *lista, int chave);

void lista_imprimir(const LISTA *lista);
int lista_tamanho(const LISTA *lista);
int lista_profundidade(const LISTA *lista);
bool lista_vazia(const LISTA *lista);

#endif
