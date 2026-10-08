#ifndef SEQUENCIAL_LIST_H
#define SEQUENCIAL_LIST_H

#define MAX_SIZE 100 //Tamanho Máximo de SEQUENCIAL_LIST

typedef struct sequencial_list SEQUENCIAL_LIST;

/**
 * @brief Cria SEQUENCIAL_LIST
 * @return Ponteiro para a SEQUENCIAL_LIST.
 */
extern SEQUENCIAL_LIST *create_sequencial_list(void);
/**
 * @brief Apaga SEQUENCIAL_LIST.
 * @param[out] Ponteiro duplo para a SEQUENCIAL_LIST.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool delete_sequencial_list(SEQUENCIAL_LIST **sequencial_list);

/**
 * @brief Desloca elementos de SEQUENCIAL_LIST paraa DIREITA dado um RANGE[START-END].
 * @param[out] Ponteiro duplo para a SEQUENCIAL_LIST.
 * @param[in] Inteiro Start, começo do RANGE.
 * @param[in] Inteiro End, finaldo  RANGE.
 */
static void shift_right(SEQUENCIAL_LIST *sequencial_list, int start, int end);
/**
 * @brief Desloca elementos de SEQUENCIAL_LIST paraa DIREITA dado um RANGE[START-END].
 * @param[out] Ponteiro duplo para a SEQUENCIAL_LIST.
 * @param[in] Inteiro Start, começo do RANGE.
 * @param[in] Inteiro End, finaldo  RANGE.
 */
static void shift_left(SEQUENCIAL_LIST *sequencial_list, int start, int end);

/**
 * @brief Adicona um ITEM no começo de SEQUENCIAL_LIST.
 * @param[out] Ponteiro  para a SEQUENCIAL_LIST.
 * @param[in] Data de ITEM.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool sl_add_start(SEQUENCIAL_LIST *sequencial_list, int data);
/**
 * @brief Adicona um ITEM de forma ordenada em SEQUENCIAL_LIST.
 * @param[out] Ponteiro  para a SEQUENCIAL_LIST.
 * @param[in] Data de ITEM.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool add_ordering(SEQUENCIAL_LIST *sequencial_list, int data);
/**
 * @brief Adicona um ITEM no final de SEQUENCIAL_LIST.
 * @param[out] Ponteiro  para a SEQUENCIAL_LIST.
 * @param[in] Data de ITEM.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool sl_add_end(SEQUENCIAL_LIST *sequencial_list, int data);

/**
 * @brief Remove ITEM no começo de SEQUENCIAL_LIST.
 * @param[out] Ponteiro  para a SEQUENCIAL_LIST.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool sl_remove_start(SEQUENCIAL_LIST *sequencial_list);
/**
 * @brief Remove ITEM no começo de SEQUENCIAL_LIST.
 * @param[out] Ponteiro  para a SEQUENCIAL_LIST.
 * @return TRUE se conseguir e FALSE se não conseguir.
 */
extern bool sl_remove_end(SEQUENCIAL_LIST *sequencial_list);

/**
 * @brief Faz a busca binária de ITEM em SEQUENCIAL_LIST.
 * @param[in] Ponteiro para a SEQUENCIAL_LIST.
 * @param[in] Alvo da busca.
 * @return TRUE se encontrar e FALSE se não encontrar.
 */
extern bool binary_search(SEQUENCIAL_LIST *sequencial_list, int target);
/**
 * @brief Faz a busca binária de ITEM em SEQUENCIAL_LIST para a posição à inserir ITEM.
 * @param[in] Ponteiro para a SEQUENCIAL_LIST.
 * @param[in] Data de ITEM.
 * @return Posição da inserção.
 */
static int insert_pos(SEQUENCIAL_LIST *sequencial_list, int data);
#endif
