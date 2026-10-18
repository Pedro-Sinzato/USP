#ifndef ITEM_H
#define ITEM_H

typedef struct item_ ITEM;

ITEM *item_criar(void *dados);
void *item_get_dados(ITEM *item);
void item_apagar(ITEM **item);

#endif