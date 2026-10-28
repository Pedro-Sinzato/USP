#ifndef ESTRUTURAS_DADOS_H
#define ESTRUTURAS_DADOS_H

#include <stdbool.h>

#include "../estruturas_dados/item.h"

typedef enum estrutura_dados_types
{
    ED_INVALID = -1,
    PILHA,
    FILA,
    LISTA_SEQ,
    LISTA_ENC,
    LISTA_CABECA,
    LISTA_ORD,
    LISTA_GEN,
    LISTA_CRUZ
} ED_TYPES;

/* Basicamente implementando C++ manualmente em C: ver vtable */

typedef struct ed_vtable ED_VTABLE;
typedef struct estrutura_dados ED;

/* Chamada para cada ITEM durante o percurso; devolva false para parar. */
typedef bool (*ED_VISIT)(const ITEM *item, void *ctx);

struct ed_vtable
{
    bool (*destroy_ed)(ED **self);
    bool (*insert_ed)(ED *self, ITEM *item);
    void (*for_each_ed)(const ED *self, ED_VISIT visit, void *ctx);
};

/*
 * Toda estrutura concreta (STACK, QUERY, ...) deve ter um ED como PRIMEIRO
 * membro, para que o cast entre ED * e o tipo concreto seja válido.
 */
struct estrutura_dados
{
    const ED_VTABLE *ed_vtable;
    ED_TYPES ed_types;
};

/* Funções genéricas */

/**
 * @brief Converte o nome recebido em -base no ED_TYPES correspondente.
 * Nomes: pilha, fila, lista_seq, lista_enc, lista_cabeca, lista_ord,
 * lista_gen, lista_cruz.
 * @param[in] name Nome recebido em -base.
 * @return O ED_TYPES correspondente.
 */
extern ED_TYPES ed_type_from_name(const char *name);

/**
 * @brief Cria a estrutura de dados do tipo pedido.
 * @param[in] type Tipo da estrutura (PILHA, FILA, ...).
 * @return Ponteiro para a estrutura, ou NULL se o tipo ainda não foi
 * implementado ou faltou memória.
 */
extern ED *create_ed(ED_TYPES type);

/**
 * @brief Apaga a estrutura e todos os ITEMs que ainda estiverem nela.
 * @param[in,out] self Endereço do ponteiro da estrutura; vira NULL ao final.
 * @return true se conseguiu (ou se não havia estrutura), false caso contrário.
 */
extern bool destroy_ed(ED **self);

/**
 * @brief Insere o ITEM na estrutura (a estrutura passa a ser dona dele).
 * @param[in,out] self Ponteiro para a estrutura.
 * @param[in] item Ponteiro para o ITEM a inserir.
 * @return true se conseguiu, false caso contrário.
 */
extern bool insert_ed(ED *self, ITEM *item);

/* Imprime a estrutura, qualquer que seja o tipo (para depuração; apagar depois). */
extern void print_ed(const ED *self);

/**
 * @brief Percorre os ITEMs da estrutura, sem alterá-la, chamando visit para
 * cada um até acabarem ou visit devolver false.
 * @param[in] self Ponteiro para a estrutura.
 * @param[in] visit Função chamada para cada ITEM.
 * @param[in,out] ctx Ponteiro repassado a visit (estado do chamador).
 */
extern void for_each_ed(const ED *self, ED_VISIT visit, void *ctx);

#endif
