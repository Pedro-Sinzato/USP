#ifndef LISTA_CABECA_H
#define LISTA_CABECA_H

#include <stdbool.h>

#include "../item.h"
#include "../estruturas_dados.h"

typedef struct lista_cabeca HEAD_LIST;

/**
 * @brief Cria uma lista vazia com nó cabeça (sentinela, não guarda ITEM).
 * @return Ponteiro para a lista, ou NULL em caso de erro.
 */
extern HEAD_LIST *create_lista_cabeca(void);

/**
 * @brief Apaga a lista, liberando o nó cabeça, todos os nós e os ITEMs
 * armazenados (via delete_item).
 * @param[in,out] lista Endereço do ponteiro da lista; vira NULL ao final.
 * @return true se conseguiu (ou se não havia lista), false caso contrário.
 */
extern bool delete_lista_cabeca(HEAD_LIST **lista);

/**
 * @brief Insere o ITEM no final da lista (a lista passa a ser dona dele).
 * @param[in,out] lista Ponteiro para a lista.
 * @param[in] item Ponteiro para o ITEM a inserir.
 * @return true se conseguiu, false caso contrário.
 */
extern bool lista_cabeca_insert(HEAD_LIST *lista, ITEM *item);

#endif
