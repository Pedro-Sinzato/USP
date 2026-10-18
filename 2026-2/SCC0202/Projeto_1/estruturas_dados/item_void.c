#include <stdio.h>
#include <stdlib.h>
#include "item_void.h"

struct item_ {
    void *dados;
};

ITEM *item_criar(void *dados) {
    ITEM *item = malloc(sizeof(ITEM));
    if (item == NULL) return NULL;

    item->dados = dados;

    return item;
}

void *item_get_dados(ITEM *item) {
    if (item == NULL) return NULL;

    return item->dados;
}

void item_apagar(ITEM **item) {
    if (item == NULL || *item == NULL) return;

    free((*item)->dados);
    free(*item);
    *item = NULL;
}