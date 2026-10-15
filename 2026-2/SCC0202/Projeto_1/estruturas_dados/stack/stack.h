#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#include "item.h"
#include "../../estruturas_dados.h"

typedef struct stack STACK;
typedef struct node NODE;

extern STACK *create_stack(void);
extern bool delete_stack(STACK **stack);
extern bool stack_insert(STACK *stack);

#endif
