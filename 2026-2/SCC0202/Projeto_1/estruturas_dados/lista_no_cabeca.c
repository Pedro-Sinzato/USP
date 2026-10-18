
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"
#include "item.h"

typedef struct no_ {
    ITEM *item;
    struct no_ *proximo;
} NO;

struct lista_ {
    NO *cabeca;
    NO *fim;
    int tamanho;
};

LISTA *lista_criar(void) {
    LISTA *lista = malloc(sizeof(LISTA));
    if (lista == NULL) return NULL;

    lista->cabeca = malloc(sizeof(NO));
    if (lista->cabeca == NULL) {
        free(lista);
        return NULL;
    }

    lista->cabeca->item = NULL;
    lista->cabeca->proximo = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;

    return lista;
}

void lista_apagar(LISTA **lista) {
    if (lista == NULL || *lista == NULL) return;

    NO *atual = (*lista)->cabeca->proximo;

    while (atual != NULL) {
        NO *proximo = atual->proximo;
        item_apagar(&atual->item);
        free(atual);
        atual = proximo;
    }

    free((*lista)->cabeca);
    free(*lista);
    *lista = NULL;
}

bool lista_inserir(LISTA *lista, ITEM *item) {
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

ITEM *lista_remover(LISTA *lista, int chave) {
    if (lista == NULL) return NULL;

    NO *anterior = lista->cabeca;

    while (anterior->proximo != NULL &&
           item_chave(anterior->proximo->item) != chave) {
        anterior = anterior->proximo;
    }

    if (anterior->proximo == NULL) return NULL;

    NO *removido = anterior->proximo;
    ITEM *item = removido->item;

    anterior->proximo = removido->proximo;

    if (lista->fim == removido) {
        lista->fim = (anterior == lista->cabeca) ? NULL : anterior;
    }

    free(removido);
    lista->tamanho--;

    return item;
}

bool lista_vazia(const LISTA *lista) {
    return lista != NULL &&
           lista->cabeca->proximo == NULL;
}

int lista_tamanho(const LISTA *lista) {
    if (lista == NULL) return 0;
    return lista->tamanho;
}

void lista_imprimir(const LISTA *lista) {
    if (lista == NULL) return;

    NO *atual = lista->cabeca->proximo;

    while (atual != NULL) {
        item_imprimir(atual->item);
        atual = atual->proximo;
    }
}
