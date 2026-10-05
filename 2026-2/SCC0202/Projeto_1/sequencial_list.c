#include <stdio.h>
#include <stdlib.h>
#include "sequencial_list.h"

//MUDAR int list para ITEM list[]
struct sequencial_list
{
    int end;
    int size;
    int list[MAX_SIZE];
};

extern SEQUENCIAL_LIST *create_sequencial_list(void)
{
    SEQUENCIAL_LIST *sequencial_list = calloc(1, sizeof(SEQUENCIAL_LIST));

    if(sequencial_list == NULL)
    {
        perror("Failed to create Sequencial List.\n");
        return NULL;
    }

    sequencial_list->end = 0;
    sequencial_list->size = 0;

    printf("Sequencial List created with success!\n");
    return sequencial_list;
}
extern void delete_sequencial_list(SEQUENCIAL_LIST **sequencial_list)
{
    free(*sequencial_list);
    sequencial_list = NULL;
}

static void shift_right(SEQUENCIAL_LIST *sequencial_list, int range)
{
    for(int i = range + 1; i > 0; i--)
    {
        sequencial_list->list[i] = sequencial_list->list[i-1];
    }
}
static void shift_left(SEQUENCIAL_LIST *sequencial_list, int range)
{
    for(int i = 0; i < range; i++)
    {
        sequencial_list->list[i] = sequencial_list->list[i+1];
    }
}

extern void add_start(SEQUENCIAL_LIST *sequencial_list, int data)
{
    if(sequencial_list->size == MAX_SIZE)
    {
        perror("The Sequencial List is FULL.\n");
        return;
    }
    if(sequencial_list->size == 0)
    {
        sequencial_list->list[0] = data;
    }

    int range = sequencial_list->end;
    shift_right(sequencial_list, range);

    sequencial_list->list[0] = data;

    sequencial_list->end++;
    sequencial_list->size++;

    printf("Item added with success!\n");
}
extern void add_end(SEQUENCIAL_LIST *sequencial_list, int data)
{
    if(sequencial_list->size == MAX_SIZE)
    {
        perror("The Sequencial List is FULL.\n");
        return;
    }

    int pos = sequencial_list->end;
    sequencial_list->list[pos] = data;

    sequencial_list->end++;
    sequencial_list->size++;

    printf("Item added with success!\n");
}

extern void remove_start(SEQUENCIAL_LIST *sequencial_list)
{
    int range = sequencial_list->end;
    shift_left(sequencial_list, range);

    sequencial_list->end--;
    sequencial_list->size--;

    printf("Item deleted with success!\n");
}
extern void remove_end(SEQUENCIAL_LIST *sequencial_list)
{
    int pos = sequencial_list->end;

    sequencial_list->list[pos] = 0;
    sequencial_list->end--;
    sequencial_list->size--;

    printf("Item deleted with success!\n");
}
