#ifndef FILTER_H
#define FILTER_H

#include "../nasa.h"
/**
 * @brief ENUM com os OPERADORES possíveis: ==, >=, <=, !=
 */
typedef enum operador
{
    OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE, OP_INVALID
}OPERADOR;
/**
 * @brief Union com a condição após o operador.
 */
typedef union condicao CONDICAO;
/**
 * @brief Struct com os parâmetros de FILTER
 * CAMPO
 * OPERADOR
 * CONDICAO
 */
typedef struct filter FILTER;

/**
 * @brief Cria um FILTER e faz o assigmente de seus parâmetros.
 * @param[in] String contendo o CAMPO.
 * @param[in] String contendo o OPERADOR.
 * @param[in] Union contendo a CONDICAO.
 * @return Retorna um ponteiro para o FILTER, se não conseguir, retorna NULL.
 */
extern FILTER *create_filter(CAMPO campo, char operador[3], CONDICAO condicao);


extern OPERADOR parse_operador(const char *operador);
#endif
