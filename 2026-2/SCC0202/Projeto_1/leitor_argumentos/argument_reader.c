#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "argument_reader.h"

#define ARG_MAX_LEN 32

struct arguments
{
    char reader[ARG_MAX_LEN];
    char input[ARG_MAX_LEN];
    char base[ARG_MAX_LEN];
    char result[ARG_MAX_LEN];
    char query[ARG_MAX_LEN];
    char output[ARG_MAX_LEN];
    unsigned long long int top_k;
    unsigned long long int limit;
};

extern ARGUMENTS *create_arguments(void)
{
    ARGUMENTS *arguments = calloc(1, sizeof(ARGUMENTS));
    if(arguments == NULL)
    {
        return NULL;
    }

    return arguments;
}
extern bool delete_arguments(ARGUMENTS **arguments)
{
    if(arguments == NULL || *arguments == NULL)
    {
        return false;
    }
    free(*arguments);
    *arguments = NULL;

    return true;
}
/**
 * @brief Copia uma palavra de ARG_V para DESTINO.
 * @param[out] Ponteiro para DESTINO.
 * @param[in] Ponteiro para a palavra em ARG_V
 */
static void copy_arg(char *destino, const char *arg_word)
{
    strncpy(destino, arg_word, ARG_MAX_LEN - 1);
    destino[ARG_MAX_LEN - 1] = '\0';
}

extern bool argument_parser(int argc, char *argv[], ARGUMENTS *arguments)
{
    for (int i = 1; i + 1 < argc; i += 2)
    {
        const char *flag  = argv[i];
        const char *value = argv[i + 1];

        if      (strcmp(flag, "-reader") == 0) copy_arg(arguments->reader, value);
        else if (strcmp(flag, "-input")  == 0) copy_arg(arguments->input,  value);
        else if (strcmp(flag, "-base")   == 0) copy_arg(arguments->base,   value);
        else if (strcmp(flag, "-result") == 0) copy_arg(arguments->result, value);
        else if (strcmp(flag, "-query")  == 0) copy_arg(arguments->query,  value);
        else if (strcmp(flag, "-output") == 0) copy_arg(arguments->output, value);
        else if (strcmp(flag, "-top-k") == 0) arguments->top_k = strtoull(value, NULL, 10);
        else if (strcmp(flag, "-limit") == 0) arguments->limit = strtoull(value, NULL, 10);
        else
        {
            fprintf(stderr, "flag desconhecida: '%s'\n", flag);
            return false;
        }
    }
    return true;
}

extern const char *get_reader(const ARGUMENTS *arguments)
{
    return (arguments == NULL) ? NULL : arguments->reader;
}
extern const char *get_input(const ARGUMENTS *arguments)
{
    return (arguments == NULL) ? NULL : arguments->input;
}
extern const char *get_base(const ARGUMENTS *arguments)
{
    return (arguments == NULL) ? NULL : arguments->base;
}
extern const char *get_result(const ARGUMENTS *arguments)
{
    return (arguments == NULL) ? NULL : arguments->result;
}
extern const char *get_query(const ARGUMENTS *arguments)
{
    return (arguments == NULL) ? NULL : arguments->query;
}
extern const char *get_output(const ARGUMENTS *arguments)
{
    return (arguments == NULL) ? NULL : arguments->output;
}
extern unsigned long long int get_top_k(const ARGUMENTS *arguments)
{
    return (arguments == NULL) ? 0 : arguments->top_k;
}
extern unsigned long long int get_limit(const ARGUMENTS *arguments)
{
    return (arguments == NULL) ? 0 : arguments->limit;
}
