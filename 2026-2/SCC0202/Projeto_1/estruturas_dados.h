#ifndef ESTRUTURAS_DADOS_H
#define ESTRUTURAS_DADOS_H

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
//funções genêricas

typedef struct ed_vtable ED_VTABLE;
typedef struct estrutura_dados ED;
struct ed_vtable
{
    bool(*destroy_ed)(ED **self);
    bool(*insert_ed)(ED *self);
};
struct estrutura_dados
{
    const ED_VTABLE *ed_vtable;
    ED_TYPES ed_types;
};

#endif
