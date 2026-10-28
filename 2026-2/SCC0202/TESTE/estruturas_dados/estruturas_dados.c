#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "estruturas_dados.h"
#include "item.h"
#include "lista_cruzada/orthogonal_list.h"
#include "query/query.h"
#include "stack/stack.h"
#include "lista_cabeca/lista_cabeca.h"
#include "linked_list/linked_list.h"

/* ---------- nomes <-> tipos ---------- */

static const struct
{
    const char *name;
    ED_TYPES type;
} ED_NAMES[] = {
    {"pilha", PILHA},
    {"fila", FILA},
    {"lista_seq", LISTA_SEQ},
    {"lista_enc", LISTA_ENC},
    {"lista_cabeca", LISTA_CABECA},
    {"lista_ord", LISTA_ORD},
    {"lista_gen", LISTA_GEN},
    {"lista_cruz", LISTA_CRUZ},
};

ED_TYPES ed_type_from_name(const char *name)
{
    if (name != NULL)
    {
        for (size_t i = 0; i < sizeof ED_NAMES / sizeof ED_NAMES[0]; i++)
        {
            if (strcmp(name, ED_NAMES[i].name) == 0)
                return ED_NAMES[i].type;
        }
    }
    return ED_INVALID; /* precisa ser adicionado ao enum em estruturas_dados.h */
}

/* ---------- fábrica ---------- */

ED *create_ed(ED_TYPES type)
{
    switch (type)
    {
    case PILHA:
        return (ED *)create_stack();
    case FILA:
        return (ED *)create_query();
    case LISTA_SEQ:
    case LISTA_ENC:
        return (ED *)create_linked_list();
    case LISTA_CABECA:
        return (ED *)create_lista_cabeca();
    case LISTA_ORD:
    case LISTA_GEN:
    case LISTA_CRUZ:
        return (ED *)create_cross_list();
    default:
        return NULL;
    }
}

/* ---------- despacho pela vtable ---------- */

bool destroy_ed(ED **self)
{
    if (self == NULL || *self == NULL)
        return true;

    if ((*self)->ed_vtable == NULL || (*self)->ed_vtable->destroy_ed == NULL)
        return false;

    /* A função concreta libera tudo e põe *self em NULL. */
    return (*self)->ed_vtable->destroy_ed(self);
}

bool insert_ed(ED *self, ITEM *item)
{
    if (self == NULL || item == NULL)
        return false;

    if (self->ed_vtable == NULL || self->ed_vtable->insert_ed == NULL)
        return false;

    return self->ed_vtable->insert_ed(self, item);
}

void for_each_ed(const ED *self, ED_VISIT visit, void *ctx)
{
    if (self == NULL || visit == NULL)
        return;

    if (self->ed_vtable == NULL || self->ed_vtable->for_each_ed == NULL)
        return;

    self->ed_vtable->for_each_ed(self, visit, ctx);
}

/* ---------- depuração ---------- */

static bool print_visit(const ITEM *item, void *ctx)
{
    (void)ctx;
    item_print(item);
    return true;
}

void print_ed(const ED *self)
{
    for_each_ed(self, print_visit, NULL);
}
