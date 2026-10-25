
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "filter.h"
#include "../estruturas_dados/item.h"

struct filter
{
    CAMPO campo;
    OPERADOR operador;
    char *valor;
};

// Converte o operador recebido (string) para o enum correspondente
OPERADOR parse_operador(const char *operador)
{
    if (operador == NULL) return OP_INVALID;

    if (!strcmp(operador, "==")) return OP_EQ;
    if (!strcmp(operador, "!=")) return OP_NE;
    if (!strcmp(operador, "<"))  return OP_LT;
    if (!strcmp(operador, "<=")) return OP_LE;
    if (!strcmp(operador, ">"))  return OP_GT;
    if (!strcmp(operador, ">=")) return OP_GE;

    return OP_INVALID;
}

// Cria um filtro e armazena o campo, operador e valor
FILTER *create_filter(CAMPO campo, const char *operador,
                      const char *valor)
{
    if (campo < 0 || campo >= NASA_N_CAMPOS ||
        operador == NULL || valor == NULL)
        return NULL;

    OPERADOR op = parse_operador(operador);

    if (op == OP_INVALID)
        return NULL;

    FILTER *filter = calloc(1, sizeof(FILTER));

    if (filter == NULL)
        return NULL;

    filter->valor = malloc(strlen(valor) + 1);

    if (filter->valor == NULL)
    {
        free(filter);
        return NULL;
    }

    strcpy(filter->valor, valor);

    filter->campo = campo;
    filter->operador = op;

    return filter;
}

// Libera a memoria alocada para o filtro
bool delete_filter(FILTER **filter)
{
    if (filter == NULL || *filter == NULL)
        return false;

    free((*filter)->valor);
    free(*filter);
    *filter = NULL;

    return true;
}

// Compara numeros de acordo com o operador
static bool comparar_numeros(double num1, double num2, OPERADOR operador)
{
    switch (operador)
    {
        case OP_EQ: return num1 == num2;
        case OP_NE: return num1 != num2;
        case OP_LT: return num1 < num2;
        case OP_LE: return num1 <= num2;
        case OP_GT: return num1 > num2;
        case OP_GE: return num1 >= num2;
        default: return false;
    }
}

// Compara strings de acordo com o operador
static bool comparar_strings(const char *text1, const char *text2,
                             OPERADOR operador)
{
    if (text1 == NULL || text2 == NULL)
        return false;

    int resultado = strcmp(text1, text2);

    switch (operador)
    {
        case OP_EQ: return resultado == 0;
        case OP_NE: return resultado != 0;
        case OP_LT: return resultado < 0;
        case OP_LE: return resultado <= 0;
        case OP_GT: return resultado > 0;
        case OP_GE: return resultado >= 0;
        default: return false;
    }
}

// Verifica se um ITEM satisfaz a condicao de um filtro
bool apply_filter(const ITEM *item, const FILTER *filter)
{
    if (item == NULL || filter == NULL)
        return false;

    int pos = (int)filter->campo;

    DATATYPE tipo = item_get_type(item, pos);
    const void *conteudo = get_field_content(item, pos);

    if (conteudo == NULL)
        return false;

    switch (tipo)
    {
        case ITEM_INT:
        {
            char *fim;
            double valor = strtod(filter->valor, &fim);

            if (fim == filter->valor || *fim != '\0')
                return false;

            return comparar_numeros(
                *(const int *)conteudo,
                valor,
                filter->operador
            );
        }

        case ITEM_DOUBLE:
        {
            char *fim;
            double valor = strtod(filter->valor, &fim);

            if (fim == filter->valor || *fim != '\0')
                return false;

            return comparar_numeros(
                *(const double *)conteudo,
                valor,
                filter->operador
            );
        }

        case ITEM_BOOL:
        {
            bool valor;

            if (!strcmp(filter->valor, "true") ||
                !strcmp(filter->valor, "True"))
                valor = true;
            else if (!strcmp(filter->valor, "false") ||
                     !strcmp(filter->valor, "False"))
                valor = false;
            else
                return false;

            if (filter->operador == OP_EQ)
                return *(const bool *)conteudo == valor;

            if (filter->operador == OP_NE)
                return *(const bool *)conteudo != valor;

            return false;
        }

        case ITEM_STRING:
            return comparar_strings(
                (const char *)conteudo,
                filter->valor,
                filter->operador
            );

        case ITEM_CHAR:
        {
            if (strlen(filter->valor) != 1)
                return false;

            char valor = filter->valor[0];

            return comparar_numeros(
                *(const char *)conteudo,
                valor,
                filter->operador
            );
        }

        default:
            return false;
    }
}
