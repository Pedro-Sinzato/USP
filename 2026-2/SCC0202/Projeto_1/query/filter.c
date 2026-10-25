#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "filter.h"
#include "campo.h"

union condicao
{
    bool boleano;
    double valor;
};

struct filter
{
    CAMPO campo;
    char *operador;
    CONDICAO condicao;
};

extern FILTER *create_filter(CAMPO campo, char operador[3], CONDICAO condicao)
{
    FILTER *filter = calloc(1, sizeof(FILTER));
    if(filter == NULL)
    {
        return NULL;
    }

    filter->campo = campo;
    filter->operador = operador;

    return filter;
}

extern OPERADOR parse_operador(const char *operador)
{
    if (!strcmp(operador, "==")) return OP_EQ;
    if (!strcmp(operador, "!=")) return OP_NE;
    if (!strcmp(operador, "<"))  return OP_LT;
    if (!strcmp(operador, "<=")) return OP_LE;
    if (!strcmp(operador, ">"))  return OP_GT;
    if (!strcmp(operador, ">=")) return OP_GE;
    return OP_INVALID;
}
