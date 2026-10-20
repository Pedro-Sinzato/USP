#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lista_cabeca.h"
#include "../item.h"

struct lista_cabeca
{
    NO *cabeca;
    NO *fim;
    int tamanho;
};
struct no
{
    ITEM *item;
    NO *proximo;
};

LISTA_CABECA *lista_criar(void)
{
    //Calloc zera ou NULL nos campos
    LISTA_CABECA *lista = calloc(1, sizeof(LISTA_CABECA));
    if (lista == NULL)
    {
        return NULL;
    }

    return lista;
}

bool lista_apagar(LISTA_CABECA **lista)
{
    if (lista == NULL || *lista == NULL)
    {
        return false;
    }

    NO *atual = (*lista)->cabeca->proximo;
    while (atual != NULL)
    {
        NO *proximo = atual->proximo;
        delete_item(&atual->item);
        free(atual);
        atual = proximo;
    }

    free((*lista)->cabeca);
    free(*lista);
    *lista = NULL;

    return true;
}

bool lista_inserir(LISTA_CABECA *lista, ITEM *item) {
    if (lista == NULL || item == NULL) return false;

    NO *novo = malloc(sizeof(NO));
    if (novo == NULL) return false;

    novo->item = item;
    novo->proximo = NULL;

    if (lista->fim == NULL) {
        lista->cabeca->proximo = novo;
    } else {
        lista->fim->proximo = novo;
    }

    lista->fim = novo;
    lista->tamanho++;

    return true;
}

ITEM *lista_buscar(LISTA *lista, int chave) {
    if (lista == NULL) return NULL;

    NO *atual = lista->cabeca->proximo;

    while (atual != NULL) {
        if (item_chave(atual->item) == chave)
            return atual->item;

        atual = atual->proximo;
    }

    return NULL;
}
