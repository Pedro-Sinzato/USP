#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "estruturas_dados.h"
#include "item.h"
#include "lista_cruzada/orthogonal_list.h"
#include "lista_ordenada/ordered_list.h"
#include "lista_generalizada/gen_list.h"
#include "lista_sequencial/sequencial_list.h"
#include "../query/query.h"
#include "fila/queue.h"
#include "stack/stack.h"
#include "lista_cabeca/lista_cabeca.h"
#include "linked_list/linked_list.h"


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
    return ED_INVALID;
}

ED *create_ed(ED_TYPES type, const QUERY *query)
{
    switch (type)
    {
    case PILHA:
        return (ED *)create_stack();
    case FILA:
        return (ED *)create_queue();
    case LISTA_SEQ:
        return (ED *)create_sequencial_list();
    case LISTA_ENC:
        return (ED *)create_linked_list();
    case LISTA_CABECA:
        return (ED *)create_lista_cabeca();
    case LISTA_ORD:
        return (ED *)create_ordered_list(query); /* sem ORDER: primeiro campo, ascendente */
    case LISTA_CRUZ:
    {
        /* sem CROSS (ex.: a lista é a base): primeiro campo nas duas listas, ascendente */
        int campos[2] = {0, 0};
        ORDEM dirs[2] = {ORDEM_ASC, ORDEM_ASC};

        if (query != NULL && query_has_cross(query))
        {
            for (int i = 0; i < 2; i++)
            {
                campos[i] = query_cross(query, i);
                dirs[i] = query_cross_dir(query, i);
            }
        }
        return (ED *)create_cross_list(campos, dirs);
    }
    case LISTA_GEN:
        return (ED *)create_gen_list(query); /* lista sem agrupamento */
    default:
        return NULL;
    }
}

bool destroy_ed(ED **self)
{
    if (self == NULL || *self == NULL)
        return true;

    if ((*self)->ed_vtable == NULL || (*self)->ed_vtable->destroy_ed == NULL)
        return false;

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
