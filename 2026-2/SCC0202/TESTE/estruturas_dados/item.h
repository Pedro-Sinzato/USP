#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdbool.h>

typedef enum datatype
{
    ITEM_INT,
    ITEM_DOUBLE,
    ITEM_CHAR,
    ITEM_STRING,
    ITEM_BOOL
} DATATYPE;

typedef struct item ITEM;

/*
 * Cria um item com n_fields campos; types[i] define o tipo do campo i.
 * Retorna NULL em caso de erro.
 */
ITEM *create_item(int n_fields, const DATATYPE types[]);

/*
 * Libera o item e as strings armazenadas; define *item como NULL.
 * Retorna false se item ou *item for NULL.
 */
bool delete_item(ITEM **item);

/*
 * Grava um valor no campo idx, cujo tipo foi definido em create_item.
 * Para ITEM_INT, ITEM_DOUBLE, ITEM_CHAR e ITEM_BOOL, value aponta
 * para um int, double, char ou bool, respectivamente.
 * Para ITEM_STRING, value é uma string C (const char *).
 * Strings são copiadas para memória própria do item.
 */
bool item_set(ITEM *item, const void *value, int idx);
/**
 * @brief Escreve o ITEM como uma linha de CSV (campos separados por vírgula,
 * terminada por '\n'). Strings com vírgula, aspas ou quebra de linha saem
 * entre aspas.
 * @param[in] item ITEM a escrever.
 * @param[in] out Arquivo (ou stdout) onde escrever.
 * @return true se conseguiu, false caso contrário.
 */
bool item_write_csv(const ITEM *item, FILE *out);
/* Imprime os campos do item em uma linha, separados por " | " (para depuração). */
void item_print(const ITEM *item);

#endif
