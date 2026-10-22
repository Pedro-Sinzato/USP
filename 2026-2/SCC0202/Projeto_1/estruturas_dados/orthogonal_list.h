#ifndef LISTA_CRUZADA_H
#define LISTA_CRUZADA_H

#include "item.h"

typedef struct orthogonal_list CROSS_LIST;

/*
Comentarios para os devs (retirar antes da entrega)

Como as listas podem ser ordenadas com base em nomes (strings), datas (ou outros numericos), ou flags booleanas (como se e evento futuro)
a lista usa um enumerador do item.h (DATATYPE) para poder saber qual tipo e comparacao fazer quando for inserir ordenadamente

Como os FIELD do ITEM nao tem identificador, so tipo e conteudo, a gente provavelmente vai ter q implementar um campo de string nome ou algum
outro identificador para que uma organizacao por campo especifico seja possivel, pq por enquanto eu to implementando de um jeito q eu acho
q e mais facil e explicar por meio de um exemplo:
O usuario quer procurar um item a partir de uma key string
o programa vai percorrer uma das listas (lista-linha ou lista coluna baseado no listIndicator) e ir comparando o primeiro FIELD de DATATYPE
String e comparar com esse. O problema disso e q n da pra diferenciar multiplos FIELDs de tipo string pq n tem nada q diferencia eles. Entao
um nome ou codinome sao indistinguiveis e n da pra eu fazer uma busca/insercao baseada em um campo especificado na query, apenas baseado em
tipos e portanto eu uso o primeiro FIELD do tipo indicado encontrado no array de FIELDs no item
*/


/**
 * @brief Cria CROSS_LIST
 * @param[in] tipos de dado usados para a ordenacao das linha e coluna respectivamente
 * @return Ponteiro para a CROSS_LIST ou NULL no caso de erros.
 */
CROSS_LIST *create_cross_list(DATATYPE lineCriteria, DATATYPE columnCriteria);

/**
 * @brief Deleta CROSS_LIST
 * @param[out] Ponteiro duplo para CROSS_LIST
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
bool delete_cross_list(CROSS_LIST **cross_list);

/**
 * @brief Adicona um ITEM nas lista-linha e lista-coluna na posicao determinada pela ordenacao da lista baseado nos lineCriteria e columnCriteria.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] endereço do item a ser inserido nas listas.
 * @return TRUE se conseguir e FALSE se não conseguir (Lembrando que se o item nao tiver o field [campo do tipo x] usado na ordenacao da lista-linha ou da lista-coluna, isso leva a um erro).
 */
bool add_item(CROSS_LIST *cross_list, ITEM* item);

/**
 * @brief Remove o ITEM na primeira posicao da lista indicada (lista-linha ou lista-coluna).
 * @param[out] Ponteiro  para a CROSS_LIST.
 * @param[in] numero que indica se o item a ser removido e o primeiro da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.
 */
bool remove_start(CROSS_LIST *cross_list, int listIndicator);
/**
 * @brief Remove ITEM correspondente a KEY de CROSS_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] key - Ponteiro para o Key (Conteúdo) do ITEM (ponteiro pois o tipo do key e indeterminado).
 * @param[in] keyType - determina qual o tipo do item a ser inserido para que um metodo de comparacao apropriado possa ser usado
 * @param[in] listIndicator - numero que indica se a chave do item a ser removido deve ser procurado a partir da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.
 */
bool remove_search(CROSS_LIST *linked_list, void *key, DATATYPE keyType, int listIndicator);
/**
 * @brief Remove ITEM no fim de LINKED_LIST.
 * @param[out] Ponteiro  para a LINKED_LIST.
 * @param[in] numero que indica se o item a ser removido e o ultimo da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return TRUE se conseguir e FALSE se não conseguir ou se o listIndicator for invalido.
 */
bool remove_end(CROSS_LIST *cross_list, int listIndicator);

/**
 * @brief Busca por ITEM correspondente a KEY de LINKED_LIST.
 * @param[in] Cópia de LINKED_LIST.
 * @param[in] key - Ponteiro para o Key (Conteúdo) do ITEM (ponteiro pois o tipo do key e indeterminado).
 * @param[in] keyType - determina qual o tipo do item a ser inserido para que um metodo de comparacao apropriado possa ser usado
 * @param[in] listIndicator - numero que indica se a chave do item a ser retornado deve ser procurado a partir da lista-linha (listIndicator == 0) ou da lista-coluna (listIndicador == 1)
 * @return Ponteiro para ITEM, NULL em caso de erros (lista/item inexistente ou nao encontrado, listIndicator invalido, etc).
 */
ITEM *search(CROSS_LIST *linked_list, void *key, DATATYPE keyType, int listIndicator);

#endif