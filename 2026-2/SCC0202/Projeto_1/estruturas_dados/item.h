#ifndef ITEM_H
#define ITEM_H

typedef struct item ITEM;

/**
 * @brief Cria ITEM e retorna um ponteiro para ele.
 * @param[in] Data armazenada em ITEM.
 * @return Ponteiro para ITEM.
 */
ITEM *create_item(int data);
/**
 * @brief Deleta ITEM.
 * @param[out] Ponteiro para ITEM.
 */
void delete_item(ITEM *item);
/**
 * @brief Faz ITEM item apontar para ITEM next.
 * @param[out] Ponteiro para ITEM.
 * @param[out] Ponteiro para o próximo ITEM.
 */
void set_next(ITEM *item, ITEM *next);
/**
 * @brief Faz ITEM item apontar para ITEM previus.
 * @param[out] Ponteiro para ITEM.
 * @param[out] Ponteiro para o ITEM anterior.
 */
void set_previus(ITEM *item, ITEM *previus);
/**
 * @brief Retorna o ponteiro de ITEM para o próximo ITEM.
 * @param[in] Ponteiro para ITEM.
 */
ITEM *get_next(ITEM *item);
/**
 * @brief Retorna o ponteiro de ITEM para o  ITEM interior.
 * @param[in] Ponteiro para ITEM.
 */
ITEM *get_previus(ITEM *item);
/**
 * @brief Retorna o contéudo armazendo por ITEM.
 * @param[in] Ponteiro para ITEM.
 * @return Data de ITEM.
 */
int get_data(ITEM *item);

#endif
