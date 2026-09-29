#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "item.h"

typedef struct linked_list LINKED_LIST;

LINKED_LIST *create_linked_list(void);

extern void add_start(LINKED_LIST *linked_list, int data);
extern void add_end(LINKED_LIST *linked_list, int data);

extern void remove_start(LINKED_LIST *linked_list);
extern void remove_search(LINKED_LIST *linked_list, int key);
extern void remove_end(LINKED_LIST *linked_list);

static ITEM *search(LINKED_LIST linked_list, int key);

#endif
