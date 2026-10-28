#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"
#include "../item.h"

typedef struct node NODE;

struct node
{
    ITEM *item;
    NODE *next;
    NODE *previous;
};

struct linked_list
{
    ED base;    /* PRECISA ser o primeiro membro */
    NODE *head;
    NODE *tail;
    int size;
};

/* Adaptadores da vtable: ED ** / ED * -> LINKED_LIST ** / LINKED_LIST * */
static bool linked_list_destroy(ED **self)
{
    if (self == NULL || *self == NULL)
    {
        return true;
    }

    LINKED_LIST *linked_list = (LINKED_LIST *)*self;
    bool ok = delete_linked_list(&linked_list);
    *self = NULL;

    return ok;
}

static bool linked_list_insert_ed(ED *self, ITEM *item)
{
    return linked_list_insert((LINKED_LIST *)self, item);
}

static void linked_list_for_each(const ED *self, ED_VISIT visit, void *ctx)
{
    const LINKED_LIST *linked_list = (const LINKED_LIST *)self;

    for (const NODE *atual = linked_list->head; atual != NULL; atual = atual->next)
    {
        if (!visit(atual->item, ctx))
        {
            return;
        }
    }
}

static const ED_VTABLE linked_list_vtable =
{
    linked_list_destroy,
    linked_list_insert_ed,
    linked_list_for_each
};

extern LINKED_LIST *create_linked_list(void)
{
    LINKED_LIST *linked_list = calloc(1, sizeof(LINKED_LIST));
    if (linked_list == NULL)
    {
        return NULL;
    }

    linked_list->base.ed_vtable = &linked_list_vtable;
    linked_list->base.ed_types = LISTA_ENC;

    return linked_list;
}

extern bool delete_linked_list(LINKED_LIST **linked_list)
{
    if (linked_list == NULL || *linked_list == NULL)
    {
        return true;
    }

    NODE *atual = (*linked_list)->head;
    while (atual != NULL)
    {
        NODE *proximo = atual->next;
        delete_item(&atual->item);
        free(atual);
        atual = proximo;
    }

    free(*linked_list);
    *linked_list = NULL;

    return true;
}

extern bool linked_list_insert(LINKED_LIST *linked_list, ITEM *item)
{
    if (linked_list == NULL || item == NULL)
    {
        return false;
    }

    NODE *novo = calloc(1, sizeof(NODE));
    if (novo == NULL)
    {
        return false;
    }

    novo->item = item;
    novo->previous = linked_list->tail;     /* next já é NULL (calloc) */

    if (linked_list->tail == NULL)          /* lista vazia */
    {
        linked_list->head = novo;
    }
    else
    {
        linked_list->tail->next = novo;
    }

    linked_list->tail = novo;
    linked_list->size++;

    return true;
}
