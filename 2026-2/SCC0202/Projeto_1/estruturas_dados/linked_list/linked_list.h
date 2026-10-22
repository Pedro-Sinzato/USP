#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

#include "../item.h"
#include "../estruturas_dados.h"

typedef struct linked_list LINKED_LIST;

/**
 * @brief Cria uma lista duplamente encadeada vazia (LINKED_LIST).
 * @return Ponteiro para a lista, ou NULL se faltar memória.
 */
extern LINKED_LIST *create_linked_list(void);

/**
 * @brief Apaga a lista e todos os ITEMs que ainda estiverem nela.
 * @param[in,out] linked_list Endereço do ponteiro da lista; vira NULL ao final.
 * @return true se conseguiu (ou se não havia lista), false caso contrário.
 */
extern bool delete_linked_list(LINKED_LIST **linked_list);

/**
 * @brief Insere o ITEM no fim da lista (a lista passa a ser dona dele).
 * @param[in,out] linked_list Ponteiro para a lista.
 * @param[in] item Ponteiro para o ITEM a inserir.
 * @return true se conseguiu, false caso contrário.
 */
extern bool linked_list_insert(LINKED_LIST *linked_list, ITEM *item);

#endif
