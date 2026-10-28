#ifndef QUERY_H
#define QUERY_H

#include <stdbool.h>

#include "../item.h"

typedef struct query QUERY;

/**
 * @brief Cria QUERY>.
 * @return Ponteiro para QUERY.
 */
extern QUERY *create_query(void);

/**
 * @brief Apaga QUERY.
 * @param[in,out] query Endereço do ponteiro da fila; vira NULL ao final.
 * @return true se conseguiu, false caso contrário.
 */
extern bool delete_query(QUERY **query);

/**
 * @brief Insere o ITEM no fim da fila (a fila passa a ser dona dele).
 * @param[in,out] query Ponteiro para a fila.
 * @param[in] item Ponteiro para o ITEM a inserir.
 * @return true se conseguiu, false caso contrário.
 */
extern bool query_insert(QUERY *query, ITEM *item);

/**
 * @brief Remove o primeiro ITEM da fila e o devolve ao cliente,
 * que deve liberá-lo com delete_item.
 * @param[in,out] query Ponteiro para a fila.
 * @return O ITEM removido, ou NULL se a fila estiver vazia.
 */
extern ITEM *query_remove(QUERY *query);

/**
 * @brief Devolve o primeiro ITEM sem removê-lo.
 * @param[in] query Ponteiro para a fila.
 * @return O ITEM do início, ou NULL se a fila estiver vazia.
 */
extern ITEM *query_front(const QUERY *query);

#endif
