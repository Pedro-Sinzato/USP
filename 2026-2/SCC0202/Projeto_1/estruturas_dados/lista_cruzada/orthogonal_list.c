#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "orthogonal_list.h"
#include "../item.h"
#include "../estruturas_dados.h"
#include "../../query/query.h"

/* Eixos da lista cruzada: cada nó participa de duas listas duplamente ligadas. */
enum
{
    LINE = 0,
    COLUMN = 1,
    N_AXES = 2
};

typedef struct node NODE;

struct node
{
    ITEM *item;
    NODE *previous[N_AXES];
    NODE *next[N_AXES];
};

struct orthogonal_list
{
    ED base;
    NODE *start[N_AXES];
    int sortingField[N_AXES]; /* posição do campo de ordenação de cada lista */
    ORDEM sortingDirection[N_AXES];
    int len;
};

static bool l_cruz_destroy(ED **self)
{
    if (self == NULL || *self == NULL)
    {
        return true;
    }

    CROSS_LIST *l_cruz = (CROSS_LIST *)*self;
    bool ok = delete_cross_list(&l_cruz);
    *self = NULL;

    return ok;
}

static bool l_cruz_insert_ed(ED *self, ITEM *item)
{
    return cross_list_add_item((CROSS_LIST *)self, item);
}

/* Percorre a lista-linha (cada ITEM aparece uma única vez). */
static void l_cruz_for_each(const ED *self, ED_VISIT visit, void *ctx)
{
    const CROSS_LIST *l_cruz = (const CROSS_LIST *)self;

    for (const NODE *no = l_cruz->start[LINE]; no != NULL; no = no->next[LINE])
    {
        if (!visit(no->item, ctx))
        {
            return;
        }
    }
}

static const ED_VTABLE l_cruz_vtable =
{
    l_cruz_destroy,
    l_cruz_insert_ed,
    l_cruz_for_each
};


CROSS_LIST *create_cross_list(const int cross_fields_pos[2], const ORDEM cross_dir[2])
{
    if (cross_fields_pos == NULL || cross_dir == NULL)
    {
        fprintf(stderr, "create_cross_list: campos ou direcoes invalidos\n");
        return NULL;
    }

    CROSS_LIST *xList = calloc(1, sizeof(CROSS_LIST));
    if (xList == NULL)
    {
        fprintf(stderr, "Falha na alocacao da lista cruzada\n");
        return NULL;
    }

    xList->base.ed_vtable = &l_cruz_vtable;
    xList->base.ed_types = LISTA_CRUZ;
    for (int a = 0; a < N_AXES; a++)
    {
        xList->sortingField[a] = cross_fields_pos[a];
        xList->sortingDirection[a] = cross_dir[a];
    }

    return xList;
}

bool delete_cross_list(CROSS_LIST **cross_list)
{
    if (cross_list == NULL || *cross_list == NULL)
    {
        return true;
    }

    /* Um nó só: basta percorrer a lista-linha. */
    NODE *atual = (*cross_list)->start[LINE];
    while (atual != NULL)
    {
        NODE *proximo = atual->next[LINE];
        delete_item(&atual->item);
        free(atual);
        atual = proximo;
    }

    free(*cross_list);
    *cross_list = NULL;

    return true;
}

static void link_sorted(CROSS_LIST *l, NODE *no, int a)
{
    int sinal = (l->sortingDirection[a] == ORDEM_DESC) ? -1 : 1;
    NODE *anterior = NULL;
    NODE *atual = l->start[a];

    while (atual != NULL &&
           item_compare_field(no->item, atual->item, l->sortingField[a], NULL) * sinal >= 0)
    {
        anterior = atual;
        atual = atual->next[a];
    }

    no->previous[a] = anterior;
    no->next[a] = atual;
    if (anterior == NULL)
    {
        l->start[a] = no;
    }
    else
    {
        anterior->next[a] = no;
    }
    if (atual != NULL)
    {
        atual->previous[a] = no;
    }
}

bool cross_list_add_item(CROSS_LIST *cross_list, ITEM *item)
{
    if (cross_list == NULL || item == NULL)
    {
        fprintf(stderr, "Item ou lista cruzada invalidas para insercao\n");
        return false;
    }

    for (int a = 0; a < N_AXES; a++)
    {
        bool ok;
        item_compare_field(item, item, cross_list->sortingField[a], &ok);
        if (!ok)
        {
            fprintf(stderr, "Campo de ordenacao invalido para insercao na lista cruzada\n");
            return false;
        }
    }

    NODE *no = calloc(1, sizeof(NODE));
    if (no == NULL)
    {
        return false;
    }
    no->item = item;

    for (int a = 0; a < N_AXES; a++)
    {
        link_sorted(cross_list, no, a);
    }

    cross_list->len++;
    return true;
}

ITEM *cross_list_search(const CROSS_LIST *cross_list, const void *key, int field)
{
    if (cross_list == NULL || key == NULL)
    {
        return NULL;
    }

    /* Lista-linha por padrão; a lista-coluna se o campo for o dela. */
    int a = (field == cross_list->sortingField[COLUMN]) ? COLUMN : LINE;

    for (NODE *aux = cross_list->start[a]; aux != NULL; aux = aux->next[a])
    {
        if (item_field_equals(aux->item, field, key))
        {
            return aux->item;
        }
    }

    return NULL;
}
