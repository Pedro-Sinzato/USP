#include <stdio.h>
#include <stdlib.h>

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

    printf("Linked List created with success!\n");
    return linked_list;
}

extern void add_start(LINKED_LIST *linked_list, int data)
{
    if(linked_list->head == NULL)
    {
        linked_list->head = create_item(data);
        linked_list->tail = linked_list->head;
        linked_list->size++;
        return;
    }

    ITEM *temp = linked_list->head;

    linked_list->head = create_item(data);
    set_next(linked_list->head, temp);
    linked_list->size++;
    printf("Item added with success!\n");
}
extern void add_end(LINKED_LIST *linked_list, int data)
{
    if(linked_list->tail == NULL)
    {
        linked_list->head = create_item(data);
        linked_list->tail = linked_list->head;
        linked_list->size++;
        return;
    }

    ITEM *temp = create_item(data);

    set_previus(temp, linked_list->tail);
    linked_list->tail = temp;
    linked_list->size++;
    printf("Item added with success!\n");
}

extern void remove_start(LINKED_LIST *linked_list)
{
    ITEM *temp = linked_list->head;
    linked_list->head = get_next(temp);

    delete_item(temp);
    linked_list->size++;
    printf("Item deleted with success!\n");
}
extern void remove_search(LINKED_LIST *linked_list, int key)
{
    ITEM *temp = search(*linked_list, key);

    if(temp == NULL)
    {
        printf("Item not found!\n");
    }

    ITEM *aux_previus = get_previus(temp);
    ITEM *aux_next = get_next(temp);

    set_next(aux_previus, aux_next);
    delete_item(temp);
    linked_list->size--;
    printf("Item deleted with success!\n");
}
extern void remove_end(LINKED_LIST *linked_list)
{
    ITEM *temp = linked_list->tail;
    linked_list->tail = get_previus(temp);

    delete_item(temp);
    linked_list->size--;
    printf("Item deleted with success!\n");
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
