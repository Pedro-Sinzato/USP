#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"

typedef struct
{
    DATATYPE type;
    union
    {
        int i;
        double d;
        char c;
        char *s;
        bool b;
    } v;
} FIELD;

struct item
{
    int n_fields;
    FIELD *fields;
};

extern ITEM *create_item(int n_fields, const DATATYPE types[])
{
    if (n_fields <= 0 || types == NULL)
    {
        return NULL;
    }

    ITEM *item = malloc(sizeof(ITEM));
    if (item == NULL)
    {
        return NULL;
    }

    item->fields = calloc(n_fields, sizeof(FIELD));
    if (item->fields == NULL)
    {
        free(item);
        return NULL;
    }

    item->n_fields = n_fields;
    for (int i = 0; i < n_fields; i++)
    {
        item->fields[i].type = types[i];
    }

    return item;
}

extern bool item_set(ITEM *item, int idx, const void *value)
{
    if (item == NULL || value == NULL || idx < 0 || idx >= item->n_fields)
    {
        return false;
    }

    FIELD *f = &item->fields[idx];
    switch (f->type)
    {
        case ITEM_INT:    f->v.i = *(const int *)value;    break;
        case ITEM_DOUBLE: f->v.d = *(const double *)value; break;
        case ITEM_CHAR:   f->v.c = *(const char *)value;   break;
        case ITEM_BOOL:   f->v.b = *(const bool *)value;   break;
        case ITEM_PCHAR:
        {
            const char *src = (const char *)value;
            char *copy = malloc(strlen(src) + 1);
            if (copy == NULL)
            {
                return false;
            }
            strcpy(copy, src);
            free(f->v.s);   /* libera a string anterior, se houver */
            f->v.s = copy;
            break;
        }
        default:
            return false;
    }

    return true;
}

extern const void *item_get(const ITEM *item, int idx)
{
    if (item == NULL || idx < 0 || idx >= item->n_fields)
    {
        return NULL;
    }

    const FIELD *f = &item->fields[idx];
    switch (f->type)
    {
        case ITEM_INT:    return &f->v.i;
        case ITEM_DOUBLE: return &f->v.d;
        case ITEM_CHAR:   return &f->v.c;
        case ITEM_BOOL:   return &f->v.b;
        case ITEM_PCHAR:  return f->v.s;
        default:          return NULL;
    }
}

extern bool delete_item(ITEM **item)
{
    if (item == NULL || *item == NULL)
    {
        return false;
    }

    for (int i = 0; i < (*item)->n_fields; i++)
    {
        if ((*item)->fields[i].type == ITEM_PCHAR)
        {
            free((*item)->fields[i].v.s);
        }
    }

    free((*item)->fields);
    free(*item);
    *item = NULL;

    return true;
}
