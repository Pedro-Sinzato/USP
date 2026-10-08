#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "item.h"

typedef struct linked_list LINKED_LIST;

/**
 * @brief Cria LINKED_LIST
 * @return Ponteiro para a LINKED_LIST.
 */
extern LINKED_LIST *create_linked_list(void);
/**
 * @brief Deleta LINKED_LIST
 * @param[out] Ponteiro duplo para LINKED_LIST
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool delete_linked_list(LINKED_LIST **linked_list);

/**
 * @brief Adicona um ITEM no começo de LINKED_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] Data de ITEM.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool add_start(LINKED_LIST *linked_list, int data);
/**
 * @brief Adicona um ITEM no fim de LINKED_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] Data de ITEM.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool add_end(LINKED_LIST *linked_list, int data);

/**
 * @brief Remove ITEM no começo de LINKED_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool remove_start(LINKED_LIST *linked_list);
/**
 * @brief Remove ITEM correspondente a KEY de LINKED_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] KEY (Conteúdo) do ITEM.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool remove_search(LINKED_LIST *linked_list, int key);
/**
 * @brief Remove ITEM no fim de LINKED_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool remove_end(LINKED_LIST *linked_list);

/**
 * @brief Busca por ITEM correspondente a KEY de LINKED_LIST.
 * @param[in] Cópia de LINKED_LIST.
 * @param[in] KEY (Conteúdo) do ITEM.
 * @return Ponteiro para ITEM.
 */
static ITEM *search(LINKED_LIST linked_list, int key);

#endif
