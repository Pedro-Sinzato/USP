#ifndef LISTA_CRUZADA_H
#define LISTA_CRUZADA_H

#include <stdbool.h>

#include "../item.h"
#include "../../query/query.h"

typedef struct orthogonal_list CROSS_LIST;

/**
 * @brief Cria CROSS_LIST: cada ITEM entra ao mesmo tempo na lista-linha
 * (ordenada pelo primeiro campo do CROSS) e na lista-coluna (ordenada pelo
 * segundo campo), com um único nó de quatro ligações.
 * @param[in] cross_fields_pos Posições (no ITEM) dos campos de ordenação da lista-linha e da lista-coluna, nessa ordem (vêm da QUERY).
 * @param[in] cross_dir Direções de ordenação da lista-linha e da lista-coluna, nessa ordem (vêm da QUERY).
 * @return Ponteiro para a CROSS_LIST ou NULL no caso de erros.
 */
CROSS_LIST *create_cross_list(const int cross_fields_pos[2], const ORDEM cross_dir[2]);

/**
 * @brief Deleta CROSS_LIST e todos os ITEMs que ela guarda.
 * @param[in,out] cross_list Endereço do ponteiro; vira NULL ao final.
 * @return true se conseguiu (ou se não havia lista), false caso contrário.
 */
bool delete_cross_list(CROSS_LIST **cross_list);

/**
 * @brief Insere o ITEM na lista-linha e na lista-coluna, nas posições
 * determinadas pela ordenação de cada uma. Itens iguais na ordenação ficam na
 * ordem de chegada (inserção estável).
 * @param[in,out] cross_list Ponteiro para a CROSS_LIST.
 * @param[in] item ITEM a inserir (a lista passa a ser dona dele).
 * @return true se conseguiu; false se a lista/ITEM for inválido ou algum dos
 * dois campos de ordenação do ITEM não puder ser ordenado (nesse caso o ITEM
 * não é inserido em nenhuma das listas e continua sendo do chamador).
 */
bool cross_list_add_item(CROSS_LIST *cross_list, ITEM *item);

/** As funcoes de remocao foram removidas por obsolecencia */

/**
 * @brief Busca por ITEM correspondente a KEY de CROSS_LIST. Percorre a
 * lista-coluna se field for o campo da coluna; senão, a lista-linha.
 * @param[in] cross_list Ponteiro para CROSS_LIST.
 * @param[in] key Ponteiro para o valor buscado: int *, double *, char *, bool *
 * ou, para string, a própria string C (const char *).
 * @param[in] field Posição do campo onde o valor será procurado.
 * @return Ponteiro para ITEM, NULL se não encontrado ou em caso de erro.
 */
ITEM *cross_list_search(const CROSS_LIST *cross_list, const void *key, int field);

#endif
