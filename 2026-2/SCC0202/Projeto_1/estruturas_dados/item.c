#include "item.h"
#include <stdio.h>
#include <stdlib.h>

struct item
{
    int data;
    ITEM *next;
    ITEM *previus;
};

ITEM *create_item(int data)
{
    ITEM *item = calloc(1, sizeof(ITEM));

    if(item == NULL)
    {
        perror("Failed to create Item.\n");
        return NULL;
    }

    item->next = NULL;
    item->previus = NULL;

    item->data = data;
    return item;
}
void delete_item(ITEM *item)
{
    free(item);
    item = NULL;
}

void set_next(ITEM *item, ITEM *next)
{
    item->next = next;
}
void set_previus(ITEM *item, ITEM *previus)
{
    item->previus = previus;
}

ITEM *get_next(ITEM *item)
{
    return item->next;
}
ITEM *get_previus(ITEM *item)
{
    return item->previus;
}
int get_data(ITEM *item)
{
    return item->data;
}
