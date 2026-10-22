#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "query.h"
#include "../item.h"
#include "../estruturas_dados.h"

typedef struct node NODE;

struct node
{
    ITEM *item;
    NODE *proximo;
};

struct query
{
    ED base;    /* PRECISA ser o primeiro membro */
    NODE *first;
    NODE *last;
    int size;
};

static bool query_destroy(ED **self)
{
    if (self == NULL || *self == NULL)
    {
        return true;
    }

    QUERY *query = (QUERY *)*self;
    bool ok = delete_query(&query);
    *self = NULL;

    return ok;
}

static bool query_insert_ed(ED *self, ITEM *item)
{
    return query_insert((QUERY *)self, item);
}

static void query_for_each(const ED *self, ED_VISIT visit, void *ctx)
{
    const QUERY *query = (const QUERY *)self;

    for (const NODE *atual = query->first; atual != NULL; atual = atual->proximo)
    {
        if (!visit(atual->item, ctx))
        {
            return;
        }
    }
}

static const ED_VTABLE query_vtable =
{
    query_destroy,
    query_insert_ed,
    query_for_each
};

extern QUERY *create_query(void)
{
    QUERY *query = calloc(1, sizeof(QUERY));
    if (query == NULL)
    {
        return NULL;
    }

    query->base.ed_vtable = &query_vtable;
    query->base.ed_types = FILA;

    return query;
}

extern bool delete_query(QUERY **query)
{
    if (query == NULL || *query == NULL)
    {
        return true;
    }

    NODE *atual = (*query)->first;
    while (atual != NULL)
    {
        NODE *proximo = atual->proximo;
        delete_item(&atual->item);
        free(atual);
        atual = proximo;
    }

    free(*query);
    *query = NULL;

    return true;
}

extern bool query_insert(QUERY *query, ITEM *item)
{
    if (query == NULL || item == NULL)
    {
        return false;
    }

    NODE *novo = calloc(1, sizeof(NODE));
    if (novo == NULL)
    {
        return false;
    }

    novo->item = item;                  /* proximo já é NULL (calloc) */

    if (query->last == NULL)            /* fila vazia */
    {
        query->first = novo;
    }
    else
    {
        query->last->proximo = novo;
    }

    query->last = novo;
    query->size++;

    return true;
}

extern ITEM *query_remove(QUERY *query)
{
    if (query == NULL || query->first == NULL)
    {
        return NULL;
    }

    NODE *removido = query->first;
    ITEM *item = removido->item;

    query->first = removido->proximo;
    if (query->first == NULL)           /* esvaziou */
    {
        query->last = NULL;
    }
    query->size--;

    free(removido);
    return item;
}

extern ITEM *query_front(const QUERY *query)
{
    if (query == NULL || query->first == NULL)
    {
        return NULL;
    }

    return query->first->item;
}
