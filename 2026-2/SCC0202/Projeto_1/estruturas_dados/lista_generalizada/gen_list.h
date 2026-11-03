#ifndef GEN_LIST_H
#define GEN_LIST_H

#include <stdbool.h>

#include "../item.h"
#include "../../query/query.h"

/**
 * Lista generalizada: cada elemento é um ITEM (átomo) ou uma sublista (grupo).
 * Os níveis de agrupamento vêm do GROUP da QUERY, do mais externo ao mais
 * interno. Com GROUP|a|b, os ITEMs ficam em grupos pelo valor de a e, dentro
 * de cada grupo, em subgrupos pelo valor de b. Grupos e ITEMs aparecem na
 * ordem em que chegaram. Sem GROUP (ou sem QUERY), é uma lista simples de ITEMs.
 */
typedef struct gen_list GEN_LIST;

/**
 * @brief Cria GEN_LIST configurada pelo GROUP da QUERY.
 * @param[in] query QUERY com GROUP, ou NULL para uma lista sem agrupamento.
 * @return Ponteiro para a GEN_LIST ou NULL no caso de erros.
 */
GEN_LIST *create_gen_list(const QUERY *query);

/**
 * @brief Deleta GEN_LIST, todos os grupos e todos os ITEMs que ela guarda.
 * @param[in,out] gen_list Endereço do ponteiro; vira NULL ao final.
 * @return true se conseguiu (ou se não havia lista), false caso contrário.
 */
bool delete_gen_list(GEN_LIST **gen_list);

/**
 * @brief Insere o ITEM no grupo/subgrupo correspondente aos valores dos campos
 * do GROUP, criando os grupos que ainda não existirem.
 * @param[in,out] gen_list Ponteiro para a GEN_LIST.
 * @param[in] item ITEM a inserir (a lista passa a ser dona dele).
 * @return true se conseguiu; false se a lista/ITEM for inválido, se algum
 * campo de agrupamento do ITEM não puder ser comparado ou se faltar memória
 * (nesses casos o ITEM não é inserido e continua sendo do chamador).
 */
bool gen_list_add_item(GEN_LIST *gen_list, ITEM *item);

/**
 * @brief Busca, em todos os grupos, o primeiro ITEM cujo campo field é igual a key.
 * @param[in] gen_list Ponteiro para a GEN_LIST.
 * @param[in] key Valor buscado: int *, double *, char *, bool * ou, para string, a própria string C.
 * @param[in] field Posição do campo.
 * @return Ponteiro para o ITEM, ou NULL se não encontrado ou em caso de erro.
 */
ITEM *gen_list_search(const GEN_LIST *gen_list, const void *key, int field);

/** @brief Número de ITEMs na lista (os grupos não contam). */
int gen_list_size(const GEN_LIST *gen_list);

/** @brief Profundidade da lista: 1 sem agrupamento, 1 + número de níveis do GROUP com ele. */
int gen_list_depth(const GEN_LIST *gen_list);

/** @brief Imprime a lista com os grupos aninhados (depuração). */
void gen_list_print(const GEN_LIST *gen_list);

#endif
