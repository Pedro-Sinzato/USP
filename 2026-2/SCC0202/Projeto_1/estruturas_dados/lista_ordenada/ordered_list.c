#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../item.h"
#include "ordered_list.h"
#include "../estruturas_dados.h"
#include "../../query/query.h"
#include "../../query/sim.h"

typedef struct node NODE;

struct node
{
    ITEM *item;
    NODE *previous;
    NODE *next;
};

struct ordered_list
{
    ED base;
    NODE *start;
    int len;
    bool order_by_sim;
    int sortingCriteria; /* posição do campo (ignorado se order_by_sim) */
    ORDEM order_dir;
    SIM **sims; /* referências aos SIMs da QUERY */
    int n_sims;
};

static bool order_list_destroy(ED **self)
{
    if (self == NULL || *self == NULL)
    {
        return true;
    }

    ORDERED_LIST *order_list = (ORDERED_LIST *)*self;
    bool ok = delete_ordered_list(&order_list);
    *self = NULL;

    return ok;
}

static bool ordered_list_insert_ed(ED *self, ITEM *item)
{
    return ordered_list_add_item((ORDERED_LIST *)self, item);
}

static void ordered_list_for_each(const ED *self, ED_VISIT visit, void *ctx)
{
    const ORDERED_LIST *order_list = (const ORDERED_LIST *)self;

    for (const NODE *atual = order_list->start; atual != NULL; atual = atual->next)
    {
        if (!visit(atual->item, ctx))
        {
            return;
        }
    }
}

static const ED_VTABLE order_list_vtable =
    {
        order_list_destroy,
        ordered_list_insert_ed,
        ordered_list_for_each};

extern ORDERED_LIST *create_ordered_list(const QUERY *query)
{
    ORDERED_LIST *ordered_list = calloc(1, sizeof(ORDERED_LIST));
    if (ordered_list == NULL)
    {
        fprintf(stderr, "Falha na alocacao da lista ordenada\n");
        return NULL;
    }

    ordered_list->base.ed_vtable = &order_list_vtable;
    ordered_list->base.ed_types = LISTA_ORD;

    if (query != NULL && query_has_order(query))
    {
        ordered_list->order_by_sim = query_order_by_similarity(query);
        ordered_list->sortingCriteria = query_order_campo(query);
        ordered_list->order_dir = query_order_dir(query);
    }
    else
    {
        /* sem ORDER (ex.: a lista é a base): primeiro campo, ascendente */
        ordered_list->order_by_sim = false;
        ordered_list->sortingCriteria = 0;
        ordered_list->order_dir = ORDEM_ASC;
    }

    if (ordered_list->order_by_sim)
    {
        int n = query_n_sims(query);
        if (n <= 0)
        {
            fprintf(stderr, "Lista ordenada por similaridade, mas a query nao tem SIM\n");
            free(ordered_list);
            return NULL;
        }

        ordered_list->sims = calloc((size_t)n, sizeof(SIM *));
        if (ordered_list->sims == NULL)
        {
            free(ordered_list);
            return NULL;
        }
        for (int i = 0; i < n; i++)
        {
            ordered_list->sims[i] = query_get_sim(query, i);
        }
        ordered_list->n_sims = n;
    }

    return ordered_list;
}

bool delete_ordered_list(ORDERED_LIST **ordered_list)
{
    if (ordered_list == NULL || *ordered_list == NULL)
    {
        return true;
    }

    NODE *atual = (*ordered_list)->start;
    while (atual != NULL)
    {
        NODE *proximo = atual->next;
        delete_item(&atual->item);
        free(atual);
        atual = proximo;
    }

    free((*ordered_list)->sims); /* só o vetor: os SIMs são da QUERY */
    free(*ordered_list);
    *ordered_list = NULL;

    return true;
}

/*
 * Compara a e b na ordem ASCENDENTE da lista (devolve <0, 0 ou >0).
 * Devolve false em *ok se o tipo do campo não puder ser ordenado.
 * Por similaridade, "ascendente" = menos similar primeiro.
 */
static int compare_items(const ORDERED_LIST *l, const ITEM *a, const ITEM *b, bool *ok)
{
    *ok = true;

    if (l->order_by_sim)
    {
        double sa = total_similarity(l->sims, l->n_sims, a);
        double sb = total_similarity(l->sims, l->n_sims, b);
        return (sa > sb) - (sa < sb);
    }

    return item_compare_field(a, b, l->sortingCriteria, ok);
}

bool ordered_list_add_item(ORDERED_LIST *ordered_list, ITEM *item)
{
    if (ordered_list == NULL || item == NULL)
    {
        fprintf(stderr, "Item ou lista ordenada invalidas para insercao\n");
        return false;
    }

    /* Valida o tipo antes de alocar (e compara o item com ele mesmo). */
    bool ok;
    compare_items(ordered_list, item, item, &ok);
    if (!ok)
    {
        fprintf(stderr, "Campo de ordenacao invalido para insercao na lista ordenada\n");
        return false;
    }

    NODE *no = calloc(1, sizeof(NODE));
    if (no == NULL)
    {
        return false;
    }
    no->item = item;

    /* Insere antes do primeiro nó que deve vir DEPOIS do novo item;
     * em empate o novo vai depois (estável). */
    int sinal = (ordered_list->order_dir == ORDEM_DESC) ? -1 : 1;
    NODE *anterior = NULL;
    NODE *atual = ordered_list->start;

    while (atual != NULL)
    {
        int cmp = compare_items(ordered_list, item, atual->item, &ok);
        if (!ok)
        {
            free(no);
            return false;
        }
        if (cmp * sinal < 0)
        {
            break;
        }
        anterior = atual;
        atual = atual->next;
    }

    no->previous = anterior;
    no->next = atual;
    if (anterior == NULL)
    {
        ordered_list->start = no;
    }
    else
    {
        anterior->next = no;
    }
    if (atual != NULL)
    {
        atual->previous = no;
    }

    ordered_list->len++;
    return true;
}

ITEM *ordered_list_search(const ORDERED_LIST *ordered_list, const void *key, int field)
{
    if (ordered_list == NULL || key == NULL)
    {
        return NULL;
    }

    for (NODE *aux = ordered_list->start; aux != NULL; aux = aux->next)
    {
        if (item_field_equals(aux->item, field, key))
        {
            return aux->item;
        }
    }

    return NULL;
}
