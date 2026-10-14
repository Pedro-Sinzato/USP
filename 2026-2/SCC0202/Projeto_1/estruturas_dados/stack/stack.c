#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

typedef struct node
{
    ITEM *item;
    NODE *anterior;
}NODE;

struct stack
{
    NODE *top;
    int size;
};

extern STACK *create_stack(void)
{
    STACK *stack = calloc(1, sizeof(STACK));

    if(stack == NULL)
    {
        return NULL;
    }
    stack->top = NULL;
    stack->size = 0;

    return stack;
}
extern bool delete_stack(STACK **stack)
{
    if (stack == NULL || *stack == NULL)
    {
        return true;
    }

    NODE *atual = (*stack)->top;
    while (atual != NULL)
    {
        NODE *anterior = atual->anterior;
        item_apagar(&atual->item);
        free(atual);
        atual = anterior;
    }

    free(*stack);
    *stack = NULL;
}

ITEM *pilha_topo(PILHA *pilha) {
    if (pilha == NULL || pilha_vazia(pilha)) {
        return NULL;
    }

    return pilha->topo->item;
}

extern bool stack_insert(STACK *stack, ITEM *item)
{
    if (stack == NULL || item == NULL)
    {
        return false;
    }

    NODE *novo = calloc(1, sizeof(NODE));
    if (novo == NULL)
    {
        return false;
    }

    novo->item = item;
    novo->anterior = stack->top;
    stack->top = novo;
    stack->size++;

    return true;
}
extern bool stack_remove(STACK *stack)
{
    if (stack == NULL || stack->size)
    {
        return false;
    }

    NODE *removido = stack->top;
    ITEM *item = removido->item;
    stack->top= removido->anterior;
    stack->size--;

    free(removido);
    return true;
}
