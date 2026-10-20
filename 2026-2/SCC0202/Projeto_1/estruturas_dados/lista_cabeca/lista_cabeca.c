#include <stdlib.h>
#include <stdbool.h>
#include "lista_cabeca.h"

struct no
{
    ITEM *item;
    NO *proximo;
};

struct lista_cabeca
{
    NO *cabeca; /* nó sentinela: não guarda item */
    NO *fim;    /* último nó (aponta para cabeca se a lista está vazia) */
    int tamanho;
};

LISTA_CABECA *lista_criar(void)
{
    LISTA_CABECA *lista = malloc(sizeof(LISTA_CABECA));
    if (lista == NULL)
    {
        return NULL;
    }

    /* calloc: item = NULL e proximo = NULL */
    lista->cabeca = calloc(1, sizeof(NO));
    if (lista->cabeca == NULL)
    {
        free(lista);
        return NULL;
    }

    lista->fim = lista->cabeca;
    lista->tamanho = 0;

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

bool lista_inserir(LISTA_CABECA *lista, ITEM *item)
{
    if (lista == NULL || item == NULL)
    {
        return false;
    }

    NO *novo = malloc(sizeof(NO));
    if (novo == NULL)
    {
        return false;
    }

    novo->item = item;
    novo->proximo = NULL;

    lista->fim->proximo = novo;
    lista->fim = novo;
    lista->tamanho++;

    return true;
}
