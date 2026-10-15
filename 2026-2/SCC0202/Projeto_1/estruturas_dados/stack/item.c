#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "item.h"

struct item
{
    int n_fields;
    void **fields;
};

static bool data_selector(void *fields, DATATYPE type)
{
    if(type == ITEM_INT)
    {
        fields = calloc(1, sizeof(int));
    }
    else if(type == ITEM_DOUBLE)
    {
        fields = calloc(1, sizeof(double));
    }
    else if(type == ITEM_CHAR)
    {
        fields = calloc(1, sizeof(char));
    }
    else if(type == ITEM_BOOL)
    {
        fields = calloc(1, sizeof(bool));
    }
    if(fields == NULL)
    {
        return false;
    }

    return true;
}

extern ITEM **create_item(int n_fields,DATATYPE types[4])
{
    ITEM **new_item = calloc(1, sizeof(ITEM));
    if(new_item == NULL)
    {
        return NULL;
    }
    (*new_item)->fields = calloc(n_fields, sizeof(void*));
    for(int i = 0; i < 4; i++)
    {
        data_selector((*new_item)->fields[i], types[i]);
    }

    (*new_item)->n_fields = n_fields;
    return new_item;
}
extern bool delete_item(ITEM **item)
{
    if(item == NULL || *item == NULL)
    {
        return false;
    }

    for(int i = 0; i < (*item)->n_fields; i++)
    {
        free((*item)->fields[i]);
    }

    free((*item)->fields);
    free(*item);
    *item = NULL;

    return true;
}
