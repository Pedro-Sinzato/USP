#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "item.h"
#include "linked_list.h"

struct linked_list
{
    int size;
    ITEM *head;
    ITEM *tail;
};

extern LINKED_LIST *create_linked_list(void)
{
    LINKED_LIST *linked_list = calloc(1, sizeof(LINKED_LIST));

    if(linked_list == NULL)
    {
        perror("Failed to create Linked List.\n");
        return NULL;
    }

    linked_list->head = NULL;
    linked_list->tail = NULL;
    linked_list->size = 0;

    return linked_list;
}
extern bool delete_linked_list(LINKED_LIST **linked_list)
{
    if(linked_list == NULL || *linked_list == NULL)
    {
        return false;
    }

    LINKED_LIST *list = *linked_list;

    while(list->head != NULL)
    {
        ITEM *temp = get_next(list->head);
        free(list->head);
        list->head = temp;
    }

    free(list);
    *linked_list = NULL;
    return true;
}

extern bool add_start(LINKED_LIST *linked_list, int data)
{
    ITEM *new_item = create_item(data);
    if(new_item == NULL)
    {
        return false;
    }
    if(linked_list->head == NULL)
    {
        linked_list->head = new_item;
        linked_list->tail = linked_list->head;
        linked_list->size++;
    }

    ITEM *temp = linked_list->head;

    linked_list->head = new_item;
    set_next(new_item, temp);
    linked_list->size++;

    return true;
}
extern bool add_end(LINKED_LIST *linked_list, int data)
{
    ITEM *new_item = create_item(data);
    if(new_item == NULL)
    {
        return false;
    }
    if(linked_list->tail == NULL)
    {
        linked_list->head = new_item;
        linked_list->tail = linked_list->head;
        linked_list->size++;
        return true;
    }

    set_previus(new_item, linked_list->tail);
    linked_list->tail = new_item;
    linked_list->size++;

    return true;
}

extern bool remove_start(LINKED_LIST *linked_list)
{
    ITEM *temp = linked_list->head;
    if(temp == NULL)
    {
        return false;
    }
    linked_list->head = get_next(temp);

    delete_item(temp);
    linked_list->size++;

    return true;
}
extern bool remove_search(LINKED_LIST *linked_list, int key)
{
    ITEM *temp = search(*linked_list, key);

    if(temp == NULL)
    {
        return false;
    }

    ITEM *aux_previus = get_previus(temp);
    ITEM *aux_next = get_next(temp);

    set_next(aux_previus, aux_next);
    delete_item(temp);
    linked_list->size--;

    return true;
}
extern bool remove_end(LINKED_LIST *linked_list)
{
    ITEM *temp = linked_list->tail;
    if(temp == NULL)
    {
        return false;
    }
    linked_list->tail = get_previus(temp);

    delete_item(temp);
    linked_list->size--;

    return true;
}

static ITEM *search(LINKED_LIST linked_list, int key)
{
    ITEM *temp = linked_list.head;

    while(temp != NULL)
    {
        if(get_data(temp) == key)
        {
            return temp;
        }
        temp = get_next(temp);
    }

    return NULL;
}
