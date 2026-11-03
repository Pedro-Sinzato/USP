#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#include "../item.h"

typedef struct queue QUEUE;

/**
 * @brief Cria QUEUE>.
 * @return Ponteiro para QUEUE.
 */
extern QUEUE *create_queue(void);

/**
 * @brief Apaga QUEUE.
 * @param[in,out] queue Endereço do ponteiro da fila; vira NULL ao final.
 * @return true se conseguiu, false caso contrário.
 */
extern bool delete_queue(QUEUE **queue);

/**
 * @brief Insere o ITEM no fim da fila (a fila passa a ser dona dele).
 * @param[in,out] queue Ponteiro para a fila.
 * @param[in] item Ponteiro para o ITEM a inserir.
 * @return true se conseguiu, false caso contrário.
 */
extern bool queue_insert(QUEUE *queue, ITEM *item);

/**
 * @brief Remove o primeiro ITEM da fila e o devolve ao cliente,
 * que deve liberá-lo com delete_item.
 * @param[in,out] queue Ponteiro para a fila.
 * @return O ITEM removido, ou NULL se a fila estiver vazia.
 */
extern ITEM *queue_remove(QUEUE *queue);

/**
 * @brief Devolve o primeiro ITEM sem removê-lo.
 * @param[in] queue Ponteiro para a fila.
 * @return O ITEM do início, ou NULL se a fila estiver vazia.
 */
extern ITEM *queue_front(const QUEUE *queue);

#endif
