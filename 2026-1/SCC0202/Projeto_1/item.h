#ifndef ITEM_H
#define ITEM_H

typedef struct item ITEM;

ITEM *create_item(int data);
void delete_item(ITEM *item);

void set_next(ITEM *item, ITEM *next);
void set_previus(ITEM *item, ITEM *previus);

ITEM *get_next(ITEM *item);
ITEM *get_previus(ITEM *item);
int get_data(ITEM *item);

#endif
