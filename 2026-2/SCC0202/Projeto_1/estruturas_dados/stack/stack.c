#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "../item.h"
#include "../estruturas_dados.h"

typedef struct node NODE;

struct node
{
    ITEM *item;
    NODE *anterior;
};

struct stack
{
    NODE *top;
    ED base;
    int size;
};

/* Adaptadores da vtable: ED ** / ED * -> STACK ** / STACK * */
static bool stack_destroy(ED **self)
{
    if (self == NULL || *self == NULL)
    {
        return true;
    }

    STACK *stack = (STACK *)*self;      /* variável local: não dá para */
    bool ok = delete_stack(&stack);     /* converter ED ** em STACK ** */
    *self = NULL;

    return ok;
}

static bool stack_insert_ed(ED *self, ITEM *item)
{
    return stack_insert((STACK *)self, item);
}

static void stack_for_each(const ED *self, ED_VISIT visit, void *ctx)
{
    const STACK *stack = (const STACK *)self;

    for (const NODE *atual = stack->top; atual != NULL; atual = atual->anterior)
    {
        if (!visit(atual->item, ctx))
        {
            return;
        }
    }
}

static const ED_VTABLE stack_vtable =
{
    stack_destroy,
    stack_insert_ed,
    stack_for_each
};

extern STACK *create_stack(void)
{
    STACK *stack = calloc(1, sizeof(STACK));
    if (stack == NULL)
    {
        return NULL;
    }

    stack->base.ed_vtable = &stack_vtable;
    stack->base.ed_types = PILHA;

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
