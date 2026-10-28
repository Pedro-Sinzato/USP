#ifndef ORDERED_LIST_H
#define ORDERED_LIST_H

#include "../item.h"

typedef struct ordered_list ORDERED_LIST;
typedef struct node NODE;

/**
 * @brief Cria ORDERED_LIST
 * @param[in] INT que indica a posicao do campo no vetor FIELDS do item usado para a ordenacao das lista
 * @return Ponteiro para a ORDERED_LIST ou NULL no caso de erros.
 */
extern ORDERED_LIST *create_ordered_list(void);

/**
 * @brief Deleta ORDERED_LIST
 * @param[out] Ponteiro duplo para ORDERED_LIST
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
bool delete_ordered_list(ORDERED_LIST **ordered_list);

/**
 * @brief Adicona um ITEM na lista na posicao determinada pela ordenacao baseada no campo estabelecido na criacao da lista ordenada.
 * @param[out] Ponteiro  para a ORDERED_LIST.
 * @param[in] endereço do item a ser inserido na lista.
 * @return TRUE se conseguir e FALSE se não conseguir (Lembrando que se o item nao tiver o field [campo do tipo x] usado na ordenacao da lista, isso leva a um erro).
 */
bool ordered_list_add_item(ORDERED_LIST *l, ITEM *item);

/** As funcoes de remocao foram removidas por obsolecencia */


/**
 * @brief Busca por ITEM correspondente a KEY de ORDERED_LIST.
 * @param[in] Ponteiro para ORDERED_LIST.
 * @param[in] key - Ponteiro para o Key (Conteúdo) do ITEM (ponteiro para void pois o tipo do key e indeterminado).
 * @param[in] field - int que indica em qual campo o valor de key esta localizado.
 * @return Ponteiro para ITEM, NULL em caso de erros (lista/item inexistente ou nao encontrado, tipo de campo invalido, etc).
 */
ITEM *ordered_list_search(ORDERED_LIST *l, void *key, int field);

#endif
