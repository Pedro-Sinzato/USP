#ifndef ESTRUTURAS_DADOS_H
#define ESTRUTURAS_DADOS_H

#include <stdbool.h>

#include "estruturas_dados/item.h"

typedef enum estrutura_dados_types
{
    PILHA,
    FILA,
    LISTA_SEQ,
    LISTA_ENC,
    LISTA_CABECA,
    LISTA_ORD,
    LIST_GEN,
    LISTA_CRUZ
} ED_TYPES;

/* Basicamente implementando C++ manualmente em C: ver vtable */

typedef struct ed_vtable ED_VTABLE;
typedef struct estrutura_dados ED;

struct ed_vtable
{
    bool (*destroy_ed)(ED **self);
    bool (*insert_ed)(ED *self, ITEM *item);
};

/*
 * Toda estrutura concreta (STACK, ...) deve ter um ED como PRIMEIRO membro,
 * para que o cast entre ED * e o tipo concreto seja válido.
 */
struct estrutura_dados
{
    const ED_VTABLE *ed_vtable;
    ED_TYPES ed_types;
};

/* Funções genéricas */
extern ED *create_ed(ED_TYPES type);
extern void destroy_ed(ED **self);
extern bool insert_ed(ED *self, ITEM *item);

#endif
