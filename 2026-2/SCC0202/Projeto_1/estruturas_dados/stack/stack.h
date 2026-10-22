#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#include "../item.h"

typedef struct stack STACK;

/**
 * @brief Cria STACK.
 * @return Retorna um pontero para STACK.
 */
extern STACK *create_stack(void);
/**
 * @brief Apaga STACK.
 * @param[out] Endereço de STACK
 * @return Retorna TRUE se conseguir e false não.
 */
extern bool delete_stack(STACK **stack);

/**
 * @brief Empilha ITEM em STACK.
 * @param[out] Ponteiro para STACK.
 * @param[out] Ponteiro para o ITEM que vai ser empilhado,
 * @return Retorna TRUE se conseguir e false não.
 */
extern bool stack_insert(STACK *stack, ITEM *item);

#endif
