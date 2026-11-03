#ifndef ORDERED_LIST_H
#define ORDERED_LIST_H

#include <stdbool.h>

#include "../item.h"
#include "../../query/query.h"

typedef struct ordered_list ORDERED_LIST;

/**
 * @brief Cria ORDERED_LIST configurada pelo ORDER da QUERY.
 * Se o ORDER for por similaridade, usa todos os SIMs da QUERY (similaridade
 * total); senão, ordena pelo campo e direção do ORDER.
 * A QUERY precisa continuar viva enquanto a lista existir (os SIMs são
 * apenas referenciados, não copiados).
 * @param[in] query QUERY com ORDER, ou NULL / sem ORDER para ordenar pelo primeiro campo, em ordem ascendente.
 * @return Ponteiro para a ORDERED_LIST ou NULL no caso de erros.
 */
ORDERED_LIST *create_ordered_list(const QUERY *query);

/**
 * @brief Deleta ORDERED_LIST e todos os ITEMs que ela guarda.
 * @param[in,out] ordered_list Endereço do ponteiro; vira NULL ao final.
 * @return true se conseguiu (ou se não havia lista), false caso contrário.
 */
bool delete_ordered_list(ORDERED_LIST **ordered_list);

/**
 * @brief Insere o ITEM na posição determinada pela ordenação da lista.
 * Itens iguais na ordenação ficam na ordem de chegada (inserção estável).
 * @param[in,out] l Ponteiro para a ORDERED_LIST.
 * @param[in] item ITEM a inserir (a lista passa a ser dona dele).
 * @return true se conseguiu; false se a lista/ITEM for inválido ou o campo de
 * ordenação do ITEM tiver tipo que não dá para ordenar (nesse caso o ITEM não
 * é inserido e continua sendo do chamador).
 */
bool ordered_list_add_item(ORDERED_LIST *l, ITEM *item);

/** As funcoes de remocao foram removidas por obsolecencia */

/**
 * @brief Busca por ITEM correspondente a KEY de ORDERED_LIST.
 * @param[in] l Ponteiro para ORDERED_LIST.
 * @param[in] key Ponteiro para o valor buscado: int *, double *, char *, bool *
 * ou, para string, a própria string C (const char *).
 * @param[in] field Posição do campo onde o valor será procurado.
 * @return Ponteiro para ITEM, NULL se não encontrado ou em caso de erro.
 */
ITEM *ordered_list_search(const ORDERED_LIST *l, const void *key, int field);

#endif
