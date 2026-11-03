#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdbool.h>

typedef enum datatype
{
    ITEM_INVALID = -1,
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
 * Cria uma cópia independente do item (inclusive das strings).
 * Retorna NULL em caso de erro.
 */
ITEM *item_clone(const ITEM *item);

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

/**
 * @brief Funções de GET para o tipo de dado e conteúdo do campo
 * @param[in] Ponteiro para ITEM.
 * @param[in] Index do ITEM no array de CAMPPOS.
 * @return -1 se POS for inválido || NULL se ITEM inválido ou POS inválido
 */
DATATYPE item_get_type(const ITEM *item, int pos);
const void *get_field_content(const ITEM *item, int pos);

/**
 * @brief Compara o campo pos de dois ITEMs, em ordem ascendente natural:
 * números pelo valor, char pelo código, string por strcmp e bool com false
 * antes de true.
 * @param[in] a, b ITEMs a comparar (o campo pos precisa ter o mesmo tipo nos dois).
 * @param[in] pos Posição do campo.
 * @param[out] ok false se o campo não existe, os tipos diferem ou o tipo não
 * é comparável; nesse caso o retorno é 0. Pode ser NULL.
 * @return <0 se a < b, 0 se iguais, >0 se a > b.
 */
int item_compare_field(const ITEM *a, const ITEM *b, int pos, bool *ok);

/**
 * @brief Verifica se o campo pos do ITEM é igual a key.
 * @param[in] item ITEM a verificar.
 * @param[in] pos Posição do campo.
 * @param[in] key Valor procurado: int *, double *, char *, bool * ou, para
 * string, a própria string C (const char *).
 * @return true se o campo existe e é igual a key, false caso contrário.
 */
bool item_field_equals(const ITEM *item, int pos, const void *key);
#endif
