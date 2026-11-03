#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "../item.h"
#include "../estruturas_dados.h"

typedef struct node NODE;

struct node
{
    ITEM *item;
    NODE *proximo;
};

struct queue
{
    ED base;
    NODE *first;
    NODE *last;
    int size;
};

static bool queue_destroy(ED **self)
{
    if (self == NULL || *self == NULL)
    {
        return true;
    }

    QUEUE *queue = (QUEUE *)*self;
    bool ok = delete_queue(&queue);
    *self = NULL;

    return ok;
}

static bool queue_insert_ed(ED *self, ITEM *item)
{
    return queue_insert((QUEUE *)self, item);
}

static void queue_for_each(const ED *self, ED_VISIT visit, void *ctx)
{
    const QUEUE *queue = (const QUEUE *)self;

    for (const NODE *atual = queue->first; atual != NULL; atual = atual->proximo)
    {
        if (!visit(atual->item, ctx))
        {
            return;
        }
    }
}

static const ED_VTABLE queue_vtable =
{
    queue_destroy,
    queue_insert_ed,
    queue_for_each
};

extern QUEUE *create_queue(void)
{
    QUEUE *queue = calloc(1, sizeof(QUEUE));
    if (queue == NULL)
    {
        return NULL;
    }

    queue->base.ed_vtable = &queue_vtable;
    queue->base.ed_types = FILA;

    return queue;
}

extern bool delete_queue(QUEUE **queue)
{
    if (queue == NULL || *queue == NULL)
    {
        return true;
    }

    NODE *atual = (*queue)->first;
    while (atual != NULL)
    {
        NODE *proximo = atual->proximo;
        delete_item(&atual->item);
        free(atual);
        atual = proximo;
    }

    free(*queue);
    *queue = NULL;

    return true;
}

extern bool queue_insert(QUEUE *queue, ITEM *item)
{
    if (queue == NULL || item == NULL)
    {
        return false;
    }

    NODE *novo = calloc(1, sizeof(NODE));
    if (novo == NULL)
    {
        return false;
    }

    novo->item = item;                  /* proximo já é NULL (calloc) */

    if (queue->last == NULL)            /* fila vazia */
    {
        queue->first = novo;
    }
    else
    {
        queue->last->proximo = novo;
    }

    queue->last = novo;
    queue->size++;

    return true;
}

extern ITEM *queue_remove(QUEUE *queue)
{
    if (queue == NULL || queue->first == NULL)
    {
        return NULL;
    }

    NODE *removido = queue->first;
    ITEM *item = removido->item;

    queue->first = removido->proximo;
    if (queue->first == NULL)           /* esvaziou */
    {
        queue->last = NULL;
    }
    queue->size--;

    free(removido);
    return item;
}

extern ITEM *queue_front(const QUEUE *queue)
{
    if (queue == NULL || queue->first == NULL)
    {
        return NULL;
    }

    return queue->first->item;
}
