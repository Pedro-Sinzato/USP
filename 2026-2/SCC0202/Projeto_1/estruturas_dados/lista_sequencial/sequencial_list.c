#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#include "sequencial_list.h"
#include "../item.h"
#include "../estruturas_dados.h"

#define INITIAL_CAPACITY 64

struct sequencial_list
{
    ED base;
    ITEM **list;
    int size;
    int capacity;
};

static bool sl_destroy(ED **self)
{
    if (self == NULL || *self == NULL)
    {
        return true;
    }

    SEQUENCIAL_LIST *sequencial_list = (SEQUENCIAL_LIST *)*self;
    bool ok = delete_sequencial_list(&sequencial_list);
    *self = NULL;

    return ok;
}

static bool sl_insert_ed(ED *self, ITEM *item)
{
    return sl_add_end((SEQUENCIAL_LIST *)self, item);
}

static void sl_for_each(const ED *self, ED_VISIT visit, void *ctx)
{
    const SEQUENCIAL_LIST *sequencial_list = (const SEQUENCIAL_LIST *)self;

    for (int i = 0; i < sequencial_list->size; i++)
    {
        if (!visit(sequencial_list->list[i], ctx))
        {
            return;
        }
    }
}

static const ED_VTABLE sl_vtable =
    {
        sl_destroy,
        sl_insert_ed,
        sl_for_each};

/* ---------- criação / destruição ---------- */

extern SEQUENCIAL_LIST *create_sequencial_list(void)
{
    SEQUENCIAL_LIST *sequencial_list = calloc(1, sizeof(SEQUENCIAL_LIST));
    if (sequencial_list == NULL)
    {
        fprintf(stderr, "Failed to create Sequencial List.\n");
        return NULL;
    }

    sequencial_list->list = calloc(INITIAL_CAPACITY, sizeof(ITEM *));
    if (sequencial_list->list == NULL)
    {
        free(sequencial_list);
        return NULL;
    }

    sequencial_list->capacity = INITIAL_CAPACITY;
    sequencial_list->base.ed_vtable = &sl_vtable;
    sequencial_list->base.ed_types = LISTA_SEQ;

    return sequencial_list;
}

extern bool delete_sequencial_list(SEQUENCIAL_LIST **sequencial_list)
{
    if (sequencial_list == NULL || *sequencial_list == NULL)
    {
        return true;
    }

    for (int i = 0; i < (*sequencial_list)->size; i++)
    {
        delete_item(&(*sequencial_list)->list[i]);
    }

    free((*sequencial_list)->list);
    free(*sequencial_list);
    *sequencial_list = NULL;

    return true;
}

/* Garante espaço para mais um ITEM, dobrando a capacidade se preciso. */
static bool ensure_capacity(SEQUENCIAL_LIST *sequencial_list)
{
    if (sequencial_list->size < sequencial_list->capacity)
    {
        return true;
    }
    if (sequencial_list->capacity > INT_MAX / 2)
    {
        return false;
    }

    int nova = sequencial_list->capacity * 2;
    ITEM **novo = realloc(sequencial_list->list, (size_t)nova * sizeof(ITEM *));
    if (novo == NULL)
    {
        return false;
    }

    sequencial_list->list = novo;
    sequencial_list->capacity = nova;
    return true;
}

/*
 * Desloca para a DIREITA os elementos do intervalo [start, end), uma posição.
 * Precisa haver espaço em list[end].
 */
static void shift_right(SEQUENCIAL_LIST *sequencial_list, int start, int end)
{
    for (int i = end; i > start; i--)
    {
        sequencial_list->list[i] = sequencial_list->list[i - 1];
    }
}

/*
 * Desloca para a ESQUERDA os elementos do intervalo (start, end), uma
 * posição, sobrescrevendo list[start].
 */
static void shift_left(SEQUENCIAL_LIST *sequencial_list, int start, int end)
{
    for (int i = start; i < end - 1; i++)
    {
        sequencial_list->list[i] = sequencial_list->list[i + 1];
    }
}

/*
 * Posição de inserção ordenada (busca binária): primeiro índice cujo ITEM é
 * MAIOR que o novo, então iguais ficam na ordem de chegada.
 * Devolve -1 se o campo não puder ser comparado.
 */
static int insert_pos(const SEQUENCIAL_LIST *sequencial_list, const ITEM *item, int field)
{
    int start = 0;
    int end = sequencial_list->size;

    while (start < end)
    {
        int middle = start + (end - start) / 2;
        bool ok;
        int cmp = item_compare_field(sequencial_list->list[middle], item, field, &ok);

        if (!ok)
        {
            return -1;
        }
        if (cmp <= 0)
        {
            start = middle + 1;
        }
        else
        {
            end = middle;
        }
    }
    return start;
}

/* Compara o campo do ITEM com a chave (<0, 0, >0); *ok = false se não der. */
static int compare_with_key(const ITEM *item, int field, const void *key, bool *ok)
{
    *ok = true;
    const void *c = get_field_content(item, field);
    if (c == NULL || key == NULL)
    {
        *ok = false;
        return 0;
    }

    switch (item_get_type(item, field))
    {
    case ITEM_INT:
    {
        int x = *(const int *)c, y = *(const int *)key;
        return (x > y) - (x < y);
    }
    case ITEM_DOUBLE:
    {
        double x = *(const double *)c, y = *(const double *)key;
        return (x > y) - (x < y);
    }
    case ITEM_CHAR:
    {
        char x = *(const char *)c, y = *(const char *)key;
        return (x > y) - (x < y);
    }
    case ITEM_BOOL:
        return (int)*(const bool *)c - (int)*(const bool *)key;
    case ITEM_STRING:
        return strcmp((const char *)c, (const char *)key);
    default:
        *ok = false;
        return 0;
    }
}

extern bool sl_add_start(SEQUENCIAL_LIST *sequencial_list, ITEM *item)
{
    if (sequencial_list == NULL || item == NULL || !ensure_capacity(sequencial_list))
    {
        return false;
    }

    shift_right(sequencial_list, 0, sequencial_list->size);
    sequencial_list->list[0] = item;
    sequencial_list->size++;

    return true;
}

extern bool sl_add_end(SEQUENCIAL_LIST *sequencial_list, ITEM *item)
{
    if (sequencial_list == NULL || item == NULL || !ensure_capacity(sequencial_list))
    {
        return false;
    }

    sequencial_list->list[sequencial_list->size] = item;
    sequencial_list->size++;

    return true;
}

extern bool sl_add_ordering(SEQUENCIAL_LIST *sequencial_list, ITEM *item, int field)
{
    if (sequencial_list == NULL || item == NULL)
    {
        return false;
    }

    bool ok;
    item_compare_field(item, item, field, &ok);
    if (!ok || !ensure_capacity(sequencial_list))
    {
        return false;
    }

    int pos = insert_pos(sequencial_list, item, field);
    if (pos < 0)
    {
        return false;
    }

    shift_right(sequencial_list, pos, sequencial_list->size);
    sequencial_list->list[pos] = item;
    sequencial_list->size++;

    return true;
}

extern bool sl_remove_start(SEQUENCIAL_LIST *sequencial_list)
{
    if (sequencial_list == NULL || sequencial_list->size == 0)
    {
        return false;
    }

    delete_item(&sequencial_list->list[0]);
    shift_left(sequencial_list, 0, sequencial_list->size);
    sequencial_list->size--;
    sequencial_list->list[sequencial_list->size] = NULL;

    return true;
}

extern bool sl_remove_end(SEQUENCIAL_LIST *sequencial_list)
{
    if (sequencial_list == NULL || sequencial_list->size == 0)
    {
        return false;
    }

    sequencial_list->size--;
    delete_item(&sequencial_list->list[sequencial_list->size]);

    return true;
}

extern ITEM *binary_search(const SEQUENCIAL_LIST *sequencial_list, const void *key, int field)
{
    if (sequencial_list == NULL || key == NULL)
    {
        return NULL;
    }

    /* primeiro índice com campo >= key */
    int left = 0;
    int right = sequencial_list->size;

    while (left < right)
    {
        int middle = left + (right - left) / 2;
        bool ok;
        int cmp = compare_with_key(sequencial_list->list[middle], field, key, &ok);

        if (!ok)
        {
            return NULL;
        }
        if (cmp < 0)
        {
            left = middle + 1;
        }
        else
        {
            right = middle;
        }
    }

    if (left < sequencial_list->size)
    {
        bool ok;
        if (compare_with_key(sequencial_list->list[left], field, key, &ok) == 0 && ok)
        {
            return sequencial_list->list[left];
        }
    }
    return NULL;
}

extern int sl_size(const SEQUENCIAL_LIST *sequencial_list)
{
    return (sequencial_list == NULL) ? 0 : sequencial_list->size;
}
