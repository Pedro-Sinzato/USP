#ifndef LISTA_CRUZADA_H
#define LISTA_CRUZADA_H

#include "item.h"

typedef struct orthogonal_list CROSS_LIST;


/**
 * @brief Cria CROSS_LIST
 * @param[in] INTs que indicam a posicao dos campos no vetor FIELDS do item usados para a ordenacao das linha e coluna respectivamente
 * @return Ponteiro para a CROSS_LIST ou NULL no caso de erros.
 */
CROSS_LIST *create_cross_list(int lineCriteria, int columnCriteria);

/**
 * @brief Deleta CROSS_LIST
 * @param[out] Ponteiro duplo para CROSS_LIST
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
bool delete_cross_list(CROSS_LIST **cross_list);

/**
 * @brief Adicona um ITEM nas lista-linha e lista-coluna na posicao determinada pela ordenacao da lista baseado nos campos estabelecidos na criacao da lista cruzada.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] endereço do item a ser inserido nas listas.
 * @return TRUE se conseguir e FALSE se não conseguir (Lembrando que se o item nao tiver o field [campo do tipo x] usado na ordenacao da lista-linha ou da lista-coluna, isso leva a um erro).
 */
bool add_item(CROSS_LIST *cross_list, ITEM* item);

/** As funcoes de remocao foram removidas por obsolecencia


* @brief Remove o ITEM na primeira posicao da lista indicada (lista-linha ou lista-coluna).
 * @param[out] Ponteiro  para a CROSS_LIST.
 * @param[in] numero que indica se o item a ser removido e o primeiro da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.
 
bool remove_start(CROSS_LIST *cross_list, int listIndicator);
/**
 * @brief Remove ITEM correspondente a KEY de CROSS_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] key - Ponteiro para o Key (Conteúdo) do ITEM (ponteiro pois o tipo do key e indeterminado).
 * @param[in] keyType - determina qual o tipo do item a ser inserido para que um metodo de comparacao apropriado possa ser usado
 * @param[in] listIndicator - numero que indica se a chave do item a ser removido deve ser procurado a partir da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.
 
bool remove_search(CROSS_LIST *linked_list, void *key, DATATYPE keyType, int listIndicator);
/**
 * @brief Remove ITEM no fim de LINKED_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] numero que indica se o item a ser removido e o ultimo da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.
 bool remove_end(CROSS_LIST *cross_list, int listIndicator);
 */


/**
 * @brief Busca por ITEM correspondente a KEY de CROSS_LIST.
 * @param[in] Ponteiro para CROSS_LIST.
 * @param[in] key - Ponteiro para o Key (Conteúdo) do ITEM (ponteiro para void pois o tipo do key e indeterminado).
 * @param[in] field - int que indica em qual campo o valor de key esta localizado.
 * @return Ponteiro para ITEM, NULL em caso de erros (lista/item inexistente ou nao encontrado, tipo de campo invalido, etc).
 */
ITEM *search(CROSS_LIST *cross_list, void *key, int field);

#endif