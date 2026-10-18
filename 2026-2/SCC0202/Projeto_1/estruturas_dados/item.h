#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>

typedef enum datatype
{
    ITEM_INT,
    ITEM_DOUBLE,
    ITEM_CHAR,
    ITEM_PCHAR,
    ITEM_BOOL
} DATATYPE;

typedef struct item ITEM;

/*
 * Cria um item com n_fields campos; types[i] define o tipo do campo i.
 * Retorna NULL em caso de erro.
 */
extern ITEM *create_item(int n_fields, const DATATYPE types[]);

/* Libera o item (e as strings dos campos ITEM_PCHAR) e zera *item. */
extern bool delete_item(ITEM **item);

/*
 * Grava um valor no campo idx.
 *  - ITEM_INT / ITEM_DOUBLE / ITEM_CHAR / ITEM_BOOL: value aponta para o valor
 *    (int *, double *, char *, bool *).
 *  - ITEM_PCHAR: value é a própria string (const char *); o item guarda uma cópia.
 */
extern bool item_set(ITEM *item, int idx, const void *value);

/*
 * Lê o campo idx.
 *  - ITEM_INT / ITEM_DOUBLE / ITEM_CHAR / ITEM_BOOL: retorna ponteiro para o valor.
 *  - ITEM_PCHAR: retorna a string (const char *).
 * Retorna NULL se o item/índice for inválido.
 */
extern const void *item_get(const ITEM *item, int idx);

#endif
