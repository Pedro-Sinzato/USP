#ifndef QUERY_H
#define QUERY_H

typedef struct query QUERY;

/**
 * @brief Faz o parsing do CAMPO
 * @param[in] String contendo o campo.
 * @return ENUM CAMPO correspondente.
 */
static CAMPO parse_campo(const char *campo);
/**
 * @brief Faz o parsing do OPERADOR
 * @param[in] String contendo o operador.
 * @return ENUM OPERADOR correspondente.
 */
static OPERADOR parse_operador(const char *operador);

extern bool delete_query(QUERY *query);

#endif
