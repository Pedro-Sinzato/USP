#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#include "../item.h"
#include "../../estruturas_dados.h"

typedef struct stack STACK;

extern STACK *create_stack(void);
extern bool delete_stack(STACK **stack);

/* Empilha o item (a pilha passa a ser dona dele). */
extern bool stack_insert(STACK *stack, ITEM *item);

/* Desempilha e devolve o item ao cliente (que deve liberá-lo com delete_item). NULL se vazia. */
extern ITEM *stack_remove(STACK *stack);

/* Devolve o item do topo sem remover. NULL se vazia. */
extern ITEM *pilha_topo(STACK *stack);

#endif
