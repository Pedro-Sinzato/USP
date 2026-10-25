
#ifndef FILTER_H
#define FILTER_H

#include <stdbool.h>
#include "../nasa.h"
#include "../estruturas_dados/item.h"

// Operadores de comparação disponíveis
typedef enum operador
{
    OP_EQ,
    OP_NE,
    OP_LT,
    OP_LE,
    OP_GT,
    OP_GE,
    OP_INVALID
} OPERADOR;

// TAD responsável por armazenar um critério de filtragem
typedef struct filter FILTER;

// Cria um filtro com campo, operador e valor
// Retorna NULL em caso de erro
extern FILTER *create_filter(
    CAMPO campo,
    const char *operador,
    const char *valor
);

// Libera a memória de um filtro
extern bool delete_filter(FILTER **filter);

// Converte uma string em um OPERADOR
extern OPERADOR parse_operador(const char *operador);


// ele olha se um ITEM satisfaz o filtro e  retorna true se satisfaz e false caso contrário 
extern bool apply_filter(const ITEM *item, const FILTER *filter);

#endif
