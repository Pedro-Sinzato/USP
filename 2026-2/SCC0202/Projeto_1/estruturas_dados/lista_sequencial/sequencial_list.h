#ifndef SEQUENCIAL_LIST_H
#define SEQUENCIAL_LIST_H

#include <stdbool.h>

#include "../item.h"

/**
 * Lista sequencial de ITEMs: vetor de ponteiros que cresce sozinho (dobra a
 * capacidade quando enche). A lista é dona dos ITEMs que recebe.
 */
typedef struct sequencial_list SEQUENCIAL_LIST;

/**
 * @brief Cria SEQUENCIAL_LIST.
 * @return Ponteiro para a SEQUENCIAL_LIST ou NULL no caso de erros.
 */
extern SEQUENCIAL_LIST *create_sequencial_list(void);

/**
 * @brief Apaga SEQUENCIAL_LIST e todos os ITEMs que ela guarda.
 * @param[in,out] sequencial_list Endereço do ponteiro; vira NULL ao final.
 * @return true se conseguiu (ou se não havia lista), false caso contrário.
 */
extern bool delete_sequencial_list(SEQUENCIAL_LIST **sequencial_list);

/**
 * @brief Adiciona um ITEM no começo de SEQUENCIAL_LIST (desloca os demais).
 * @param[in,out] sequencial_list Ponteiro para a SEQUENCIAL_LIST.
 * @param[in] item ITEM a inserir (a lista passa a ser dona dele).
 * @return true se conseguiu, false se a lista/ITEM for inválido ou faltar memória
 * (nesse caso o ITEM continua sendo do chamador).
 */
extern bool sl_add_start(SEQUENCIAL_LIST *sequencial_list, ITEM *item);

/**
 * @brief Adiciona um ITEM no final de SEQUENCIAL_LIST.
 * @param[in,out] sequencial_list Ponteiro para a SEQUENCIAL_LIST.
 * @param[in] item ITEM a inserir (a lista passa a ser dona dele).
 * @return true se conseguiu, false caso contrário (o ITEM continua sendo do chamador).
 */
extern bool sl_add_end(SEQUENCIAL_LIST *sequencial_list, ITEM *item);

/**
 * @brief Adiciona um ITEM de forma ordenada (ascendente pelo campo field),
 * por busca binária da posição. A lista precisa já estar ordenada por esse
 * campo (use só sl_add_ordering com o mesmo campo). Itens iguais ficam na
 * ordem de chegada.
 * @param[in,out] sequencial_list Ponteiro para a SEQUENCIAL_LIST.
 * @param[in] item ITEM a inserir (a lista passa a ser dona dele).
 * @param[in] field Posição do campo de ordenação.
 * @return true se conseguiu, false se o campo não puder ser comparado ou em
 * caso de erro (o ITEM continua sendo do chamador).
 */
extern bool sl_add_ordering(SEQUENCIAL_LIST *sequencial_list, ITEM *item, int field);

/**
 * @brief Remove e apaga o primeiro ITEM (desloca os demais).
 * @return true se conseguiu, false se a lista for inválida ou estiver vazia.
 */
extern bool sl_remove_start(SEQUENCIAL_LIST *sequencial_list);

/**
 * @brief Remove e apaga o último ITEM.
 * @return true se conseguiu, false se a lista for inválida ou estiver vazia.
 */
extern bool sl_remove_end(SEQUENCIAL_LIST *sequencial_list);

/**
 * @brief Busca binária de um ITEM pelo valor de um campo, em lista ordenada
 * (ascendente) por esse campo.
 * @param[in] sequencial_list Ponteiro para a SEQUENCIAL_LIST.
 * @param[in] key Valor buscado: int *, double *, char *, bool * ou, para
 * string, a própria string C.
 * @param[in] field Posição do campo.
 * @return Ponteiro para o primeiro ITEM encontrado, ou NULL se não achar.
 */
extern ITEM *binary_search(const SEQUENCIAL_LIST *sequencial_list, const void *key, int field);

/** @brief Número de ITEMs na lista. */
extern int sl_size(const SEQUENCIAL_LIST *sequencial_list);

#endif
