#ifndef SEARCH_H
#define SEARCH_H

#include <stdbool.h>
#include <stddef.h>

#include "../estruturas_dados/estruturas_dados.h"
#include "../query/query.h"

/**
 * @brief Percorre a estrutura base e junta os ITEMs que a QUERY aceita:
 * passam em todos os FILTER e, se a QUERY tiver SIM e ACCEPT, a similaridade
 * total é pelo menos o limiar do ACCEPT. A base não é alterada.
 * @param[in] base Estrutura percorrida.
 * @param[in] query QUERY com os critérios.
 * @param[out] found Vetor (alocado aqui; o chamador usa free) com ponteiros
 * para os ITEMs aceitos, que continuam pertencendo à base. NULL se nenhum.
 * @param[out] n_found Quantidade de ITEMs aceitos.
 * @param[out] n_scanned Quantidade de ITEMs percorridos na base. Pode ser NULL.
 * @return true se conseguiu, false se faltou memória.
 */
bool search_ed(const ED *base, const QUERY *query, ITEM ***found, size_t *n_found, size_t *n_scanned);

/**
 * @brief Insere em result uma CÓPIA de cada ITEM de found, para que base e
 * resultado não sejam donos do mesmo registro.
 * @param[in,out] result Estrutura que recebe as cópias.
 * @param[in] found Vetor devolvido por search_ed.
 * @param[in] n_found Tamanho do vetor.
 * @return true se inseriu todos, false no primeiro erro (os ITEMs já inseridos continuam em result).
 */
bool insert_found(ED *result, ITEM *const *found, size_t n_found);

#endif
