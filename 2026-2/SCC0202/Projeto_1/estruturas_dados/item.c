#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"

typedef struct field
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

ITEM *create_item(int n_fields, const DATATYPE types[])
{
    if (n_fields <= 0 || types == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < n_fields; i++)
    {
        if (types[i] < ITEM_INT || types[i] > ITEM_BOOL)
        {
            return NULL;
        }
    }

    ITEM *item = malloc(sizeof(*item));
    if (item == NULL)
    {
        return NULL;
    }

    item->fields = calloc((size_t)n_fields, sizeof(*item->fields));
    if (item->fields == NULL)
    {
        free(item);
        return NULL;
    }

    item->n_fields = n_fields;

    for (int i = 0; i < n_fields; i++)
    {
        item->fields[i].type = types[i];
        /* calloc inicializa os valores; strings começam como NULL. */
    }

    return item;
}

bool delete_item(ITEM **item)
{
    if (item == NULL || *item == NULL)
    {
        return false;
    }

    for (int i = 0; i < (*item)->n_fields; i++)
    {
        if ((*item)->fields[i].type == ITEM_STRING)
        {
            free((*item)->fields[i].v.s);
        }
    }

    free((*item)->fields);
    free(*item);
    *item = NULL;

    return true;
}

bool item_set(ITEM *item, const void *value, int idx)
{
    if (item == NULL || value == NULL ||
        idx < 0 || idx >= item->n_fields)
    {
        return false;
    }

    FIELD *field = &item->fields[idx];

    switch (field->type)
    {
        case ITEM_INT:
            field->v.i = *(const int *)value;
            break;

        case ITEM_DOUBLE:
            field->v.d = *(const double *)value;
            break;

        case ITEM_CHAR:
            field->v.c = *(const char *)value;
            break;

        case ITEM_BOOL:
            field->v.b = *(const bool *)value;
            break;

        case ITEM_STRING:
        {
            const char *str = (const char *)value;
            size_t len = strlen(str) + 1;

            char *copy = malloc(len);
            if (copy == NULL)
            {
                return false;
            }

            memcpy(copy, str, len);

            free(field->v.s);
            field->v.s = copy;
            break;
        }

        default:
            return false;
    }

    return true;
}
