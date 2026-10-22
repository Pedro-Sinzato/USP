#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lista_cabeca.h"
#include "../item.h"

typedef struct no NO;

struct no
{
    ITEM *item;
    NO *proximo;
};

struct lista_cabeca
{
    ED base;        /* PRECISA ser o primeiro membro */
    NO *cabeca;     /* nó sentinela: não guarda item */
    NO *fim;        /* último nó (aponta para cabeca se a lista está vazia) */
    int tamanho;
};

/* Adaptadores da vtable: ED ** / ED * -> HEAD_LIST ** / HEAD_LIST * */
static bool lista_cabeca_destroy(ED **self)
{
    if (self == NULL || *self == NULL)
    {
        return true;
    }

    HEAD_LIST *lista = (HEAD_LIST *)*self;
    bool ok = delete_lista_cabeca(&lista);
    *self = NULL;

    return ok;
}

static bool lista_cabeca_insert_ed(ED *self, ITEM *item)
{
    return lista_cabeca_insert((HEAD_LIST *)self, item);
}

static void lista_cabeca_for_each(const ED *self, ED_VISIT visit, void *ctx)
{
    const HEAD_LIST *lista = (const HEAD_LIST *)self;

    for (const NO *atual = lista->cabeca->proximo; atual != NULL; atual = atual->proximo)
    {
        if (!visit(atual->item, ctx))
        {
            return;
        }
    }
}

static const ED_VTABLE lista_cabeca_vtable =
{
    lista_cabeca_destroy,
    lista_cabeca_insert_ed,
    lista_cabeca_for_each
};

extern HEAD_LIST *create_lista_cabeca(void)
{
    HEAD_LIST *lista = calloc(1, sizeof(HEAD_LIST));
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

    lista->base.ed_vtable = &lista_cabeca_vtable;
    lista->base.ed_types = LISTA_CABECA;
    lista->fim = lista->cabeca;

    return lista;
}

extern bool delete_lista_cabeca(HEAD_LIST **lista)
{
    if (lista == NULL || *lista == NULL)
    {
        return true;
    }

    NO *atual = (*lista)->cabeca->proximo;      /* pula o sentinela */
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

extern bool lista_cabeca_insert(HEAD_LIST *lista, ITEM *item)
{
    if (lista == NULL || item == NULL)
    {
        return false;
    }

    NO *novo = calloc(1, sizeof(NO));
    if (novo == NULL)
    {
        return false;
    }

    novo->item = item;                      /* proximo já é NULL (calloc) */

    lista->fim->proximo = novo;             /* funciona também com a lista vazia: fim == cabeca */
    lista->fim = novo;
    lista->tamanho++;

    return true;
}
