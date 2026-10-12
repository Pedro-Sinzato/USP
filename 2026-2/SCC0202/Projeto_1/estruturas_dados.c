#include <stdlib.h>
#include <stdbool.h>
#include "estruturas_dados/linked_list.h"
#include "estruturas_dados/sequencial_list.h"

//Basicamento implementando C++ manualmente em C
//Ver Vtable

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
}ED_TYPES;

typedef struct ed_vtable ED_VTABLE;
typedef struct estrutura_dados ED;

//funções genêricas
struct ed_vtable
{
    void(*create_ed)(const ED *self);
    void(*destroy_ed)(ED *self);
};
;
struct estrutura_dados
{
    const ED_VTABLE *ed_vtable;
    ED_TYPES ed_types;
};

void create_ed(const ED *self)
{
    if(self == NULL)
    {
        return;
    }
    self->ed_vtable->create_ed(self);
}
void destroy_ed(ED **self)
{
    if(self == NULL || *self == NULL)
    {
        return;
    }
    (*self)->ed_vtable->destroy_ed(*self);
    *self = NULL;
}
