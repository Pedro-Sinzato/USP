#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "../item.h"

typedef struct node NODE;

struct node
{
    ITEM *item;
    NODE *anterior;
};

struct stack
{
    ED base;    /* PRECISA ser o primeiro membro */
    NODE *top;
    int size;
};

/* Wrappers com a assinatura que a vtable espera (ED * em vez de STACK *) */
static bool stack_destroy(ED **self)
{
    STACK *stack = (STACK *)*self;
    bool ok = delete_stack(&stack);
    if (ok) *self = NULL;
    return ok;
}

static bool stack_insert_ed(ED *self, ITEM *item)
{
    return stack_insert((STACK *)self, item);
}

static const ED_VTABLE stack_vtable =
{
    .destroy_ed = stack_destroy,
    .insert_ed  = stack_insert_ed
};

extern STACK *create_stack(void)
{
    STACK *stack = calloc(1, sizeof(STACK));
    if (stack == NULL)
    {
        return NULL;
    }

    stack->base.ed_vtable = &stack_vtable;
    stack->base.ed_types  = PILHA;

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
        delete_item(&atual->item);
        free(atual);
        atual = anterior;
    }

    free(*stack);
    *stack = NULL;

    return true;
}

extern ITEM *pilha_topo(STACK *stack)
{
    if (stack == NULL || stack->size == 0)
    {
        return NULL;
    }

    return stack->top->item;
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

extern ITEM *stack_remove(STACK *stack)
{
    if (stack == NULL || stack->size == 0)
    {
        return NULL;
    }

    NODE *removido = stack->top;
    ITEM *item = removido->item;
    stack->top = removido->anterior;
    stack->size--;

    free(removido);
    return item;
}
