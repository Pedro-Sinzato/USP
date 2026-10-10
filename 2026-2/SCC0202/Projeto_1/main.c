#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define ARG_MAX_LEN 32

//Funcionando!!!
typedef struct arguments
{
    char reader[ARG_MAX_LEN];
    char input[ARG_MAX_LEN];
    char base[ARG_MAX_LEN];
    char result[ARG_MAX_LEN];
    char query[ARG_MAX_LEN];
    char output[ARG_MAX_LEN];
    unsigned long long int top_k;
    unsigned long long int limit;
}ARGUMENTS;

static void copy_arg(char *dest, const char *src)
{
    strncpy(dest, src, ARG_MAX_LEN - 1);
    dest[ARG_MAX_LEN - 1] = '\0';
}

bool argument_parser(int argc, char *argv[], ARGUMENTS *arguments)
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

int main(int argc, char *argv[])
{

    ARGUMENTS *arguments = calloc(1, sizeof(ARGUMENTS));

    if (argument_parser(argc, argv, arguments) != 0)
    {
        fprintf(stderr, "erro ao ler argumentos\n");
        free(arguments);
        return EXIT_FAILURE;
    }
    argument_parser(argc, argv, arguments);

    printf("-reader %s\n", arguments->reader);
    printf("-input %s\n", arguments->input);
    printf("-base %s\n", arguments->base);
    printf("-result %s\n", arguments->result);
    printf("-query %s\n", arguments->query);
    printf("-output %s\n", arguments->output);
    printf("-top-k %llu\n", arguments->top_k);
    printf("-limit %llu\n", arguments->limit);

    free(arguments);
    arguments = NULL;
    return EXIT_SUCCESS;
}

//-reader(1) nome(2) -input(3) nome(4)... -limit(15) value(16)

/*
 –reader leitor do dataset utilizado
 –input arquivo de entrada
 –base estrutura usada para armazenar o dataset completo
 –result estrutura usada para armazenar o resultado da busca
 –query arquivo contendo os critérios da busca
 –output arquivo que receberá os registros encontrados
 –top-k máximo de registros exportados; 0 significa todos
 –limit máximo de registros carregados; 0 significa toda a base
 */
