#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gen_list.h"
#include "../item.h"
#include "../estruturas_dados.h"
#include "../../query/query.h"

typedef struct gen_node GEN_NODE;

/*
 * Um elemento da lista: átomo (is_group == false) ou grupo (sublista).
 * Átomo: item é o ITEM guardado.
 * Grupo: first/last delimitam a sublista, e item é só uma referência ao
 * primeiro ITEM do grupo (representante da chave, que pertence a um átomo
 * mais abaixo e não é liberado pelo grupo).
 */
struct gen_node
{
    GEN_NODE *next;
    bool is_group;
    ITEM *item;
    GEN_NODE *first;
    GEN_NODE *last;
};

struct gen_list
{
    ED base;
    GEN_NODE *first;
    GEN_NODE *last;
    int *fields;   /* campos do GROUP, do nível mais externo ao mais interno */
    int n_levels;
    int len;       /* número de ITEMs */
};

static bool visit_nodes(const GEN_NODE *no, ED_VISIT visit, void *ctx)
{
    for (; no != NULL; no = no->next)
    {
        if (no->is_group)
        {
            if (!visit_nodes(no->first, visit, ctx))
            {
                return false;
            }
        }
        else if (!visit(no->item, ctx))
        {
            return false;
        }
    }
    return true;
}

static void free_nodes(GEN_NODE *no)
{
    while (no != NULL)
    {
        GEN_NODE *proximo = no->next;
        if (no->is_group)
        {
            free_nodes(no->first);
        }
        else
        {
            delete_item(&no->item);
        }
        free(no);
        no = proximo;
    }
}

static ITEM *search_nodes(const GEN_NODE *no, const void *key, int field)
{
    for (; no != NULL; no = no->next)
    {
        if (no->is_group)
        {
            ITEM *achado = search_nodes(no->first, key, field);
            if (achado != NULL)
            {
                return achado;
            }
        }
        else if (item_field_equals(no->item, field, key))
        {
            return no->item;
        }
    }
    return NULL;
}

static void print_key(const ITEM *item, int pos)
{
    const void *c = get_field_content(item, pos);
    if (c == NULL)
    {
        printf("?");
        return;
    }
    switch (item_get_type(item, pos))
    {
    case ITEM_INT:
        printf("%d", *(const int *)c);
        break;
    case ITEM_DOUBLE:
        printf("%.15g", *(const double *)c);
        break;
    case ITEM_CHAR:
        printf("'%c'", *(const char *)c);
        break;
    case ITEM_BOOL:
        printf("%s", *(const bool *)c ? "true" : "false");
        break;
    case ITEM_STRING:
        printf("\"%s\"", (const char *)c);
        break;
    default:
        printf("?");
    }
}

static void print_nodes(const GEN_LIST *l, const GEN_NODE *no, int level)
{
    for (; no != NULL; no = no->next)
    {
        printf("%*s", 2 * level, "");
        if (no->is_group)
        {
            printf("grupo nivel %d (campo %d = ", level, l->fields[level]);
            print_key(no->item, l->fields[level]);
            printf(")\n");
            print_nodes(l, no->first, level + 1);
        }
        else
        {
            item_print(no->item);
        }
    }
}

static bool gen_list_destroy(ED **self)
{
    if (self == NULL || *self == NULL)
    {
        return true;
    }

    GEN_LIST *gen_list = (GEN_LIST *)*self;
    bool ok = delete_gen_list(&gen_list);
    *self = NULL;

    return ok;
}

static bool gen_list_insert_ed(ED *self, ITEM *item)
{
    return gen_list_add_item((GEN_LIST *)self, item);
}

static void gen_list_for_each(const ED *self, ED_VISIT visit, void *ctx)
{
    visit_nodes(((const GEN_LIST *)self)->first, visit, ctx);
}

static const ED_VTABLE gen_list_vtable =
{
    gen_list_destroy,
    gen_list_insert_ed,
    gen_list_for_each
};

GEN_LIST *create_gen_list(const QUERY *query)
{
    GEN_LIST *gen_list = calloc(1, sizeof(GEN_LIST));
    if (gen_list == NULL)
    {
        fprintf(stderr, "Falha na alocacao da lista generalizada\n");
        return NULL;
    }

    gen_list->base.ed_vtable = &gen_list_vtable;
    gen_list->base.ed_types = LISTA_GEN;

    int n = query_n_group(query); /* 0 se query for NULL */
    if (n > 0)
    {
        gen_list->fields = calloc((size_t)n, sizeof(int));
        if (gen_list->fields == NULL)
        {
            free(gen_list);
            return NULL;
        }
        for (int i = 0; i < n; i++)
        {
            gen_list->fields[i] = query_group(query, i);
        }
        gen_list->n_levels = n;
    }

    return gen_list;
}

bool delete_gen_list(GEN_LIST **gen_list)
{
    if (gen_list == NULL || *gen_list == NULL)
    {
        return true;
    }

    free_nodes((*gen_list)->first);
    free((*gen_list)->fields);
    free(*gen_list);
    *gen_list = NULL;

    return true;
}

static GEN_NODE *find_group(GEN_NODE *no, const ITEM *item, int field)
{
    for (; no != NULL; no = no->next)
    {
        if (item_compare_field(no->item, item, field, NULL) == 0)
        {
            return no;
        }
    }
    return NULL;
}

/* Libera uma cadeia de nós criada por gen_list_add_item, sem tocar nos ITEM's. */
static void free_chain(GEN_NODE *no)
{
    while (no != NULL)
    {
        GEN_NODE *abaixo = no->first;
        free(no);
        no = abaixo;
    }
}

bool gen_list_add_item(GEN_LIST *gen_list, ITEM *item)
{
    if (gen_list == NULL || item == NULL)
    {
        fprintf(stderr, "Item ou lista generalizada invalidas para insercao\n");
        return false;
    }

    for (int k = 0; k < gen_list->n_levels; k++)
    {
        bool ok;
        item_compare_field(item, item, gen_list->fields[k], &ok);
        if (!ok)
        {
            fprintf(stderr, "Campo de agrupamento invalido para insercao na lista generalizada\n");
            return false;
        }
    }

    /* Desce pelos grupos que já existem; (first, last) delimitam a sublista atual. */
    GEN_NODE **first = &gen_list->first;
    GEN_NODE **last = &gen_list->last;
    int nivel = 0;

    while (nivel < gen_list->n_levels)
    {
        GEN_NODE *grupo = find_group(*first, item, gen_list->fields[nivel]);
        if (grupo == NULL)
        {
            break;
        }
        first = &grupo->first;
        last = &grupo->last;
        nivel++;
    }

    /* Cria o átomo e os grupos que faltam, de baixo para cima, antes de ligar
     * qualquer coisa: se faltar memória, nada fica pela metade. */
    GEN_NODE *novo = calloc(1, sizeof(GEN_NODE));
    if (novo == NULL)
    {
        return false;
    }
    novo->item = item;

    for (int k = gen_list->n_levels - 1; k >= nivel; k--)
    {
        GEN_NODE *grupo = calloc(1, sizeof(GEN_NODE));
        if (grupo == NULL)
        {
            free_chain(novo);
            return false;
        }
        grupo->is_group = true;
        grupo->item = item; /* representante da chave do grupo */
        grupo->first = novo;
        grupo->last = novo;
        novo = grupo;
    }

    /* Liga no fim da sublista em que parou. */
    if (*last == NULL)
    {
        *first = novo;
    }
    else
    {
        (*last)->next = novo;
    }
    *last = novo;

    gen_list->len++;
    return true;
}

/* ---------- consultas ---------- */

ITEM *gen_list_search(const GEN_LIST *gen_list, const void *key, int field)
{
    if (gen_list == NULL || key == NULL)
    {
        return NULL;
    }
    return search_nodes(gen_list->first, key, field);
}

int gen_list_size(const GEN_LIST *gen_list)
{
    return (gen_list == NULL) ? 0 : gen_list->len;
}

int gen_list_depth(const GEN_LIST *gen_list)
{
    return (gen_list == NULL) ? 0 : gen_list->n_levels + 1;
}

void gen_list_print(const GEN_LIST *gen_list)
{
    if (gen_list == NULL)
    {
        return;
    }
    print_nodes(gen_list, gen_list->first, 0);
}
