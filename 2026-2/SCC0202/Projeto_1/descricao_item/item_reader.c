#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "item_reader.h"

#define MAX_CAMPOS 20
#define MAX_LINHA 128

struct campos_nomes
{
    char *nomes[MAX_CAMPOS];
    DATATYPE tipos[MAX_CAMPOS];
    int n_campos;
};

static DATATYPE converte_tipo(const char *tipo);
static void libera_nomes(C_NOMES *c_nomes);

C_NOMES *create_c_nomes(void)
{
    /* calloc zera tudo: nomes[] = NULL e n_campos = 0 */
    return calloc(1, sizeof(C_NOMES));
}

bool destroy_c_nomes(C_NOMES **c_nomes)
{
    if (c_nomes == NULL || *c_nomes == NULL)
    {
        return false;
    }

    libera_nomes(*c_nomes);
    free(*c_nomes);
    *c_nomes = NULL;

    return true;
}

bool read_item(char *file, C_NOMES *c_nomes)
{
    if (file == NULL || c_nomes == NULL)
    {
        return false;
    }

    /* permite chamar read_item mais de uma vez sem vazar memória */
    libera_nomes(c_nomes);

    FILE *l_nomes = fopen(file, "r");
    if (l_nomes == NULL)
    {
        return false;
    }

    char linha[MAX_LINHA];
    int i = 0;

    while (fgets(linha, MAX_LINHA, l_nomes) != NULL)
    {
        linha[strcspn(linha, "\r\n")] = '\0';

        if (linha[0] == '\0')
        {
            continue;                       /* linha em branco */
        }

        if (i >= MAX_CAMPOS)                /* campos demais */
        {
            fclose(l_nomes);
            return false;
        }

        char *nome = strtok(linha, "|");
        char *tipo = strtok(NULL, "|");
        if (nome == NULL || tipo == NULL)
        {
            fclose(l_nomes);
            return false;
        }

        DATATYPE t = converte_tipo(tipo);
        if (t == ITEM_INVALID)
        {
            fclose(l_nomes);
            return false;
        }

        c_nomes->nomes[i] = malloc(strlen(nome) + 1);
        if (c_nomes->nomes[i] == NULL)
        {
            fclose(l_nomes);
            return false;
        }
        strcpy(c_nomes->nomes[i], nome);
        c_nomes->tipos[i] = t;

        i++;
        c_nomes->n_campos = i;              /* destroy já sabe o que liberar */
    }

    fclose(l_nomes);

    return i > 0;                           /* arquivo sem campos é erro */
}

DATATYPE get_tipo(const C_NOMES *c_nomes, int i)
{
    if (c_nomes == NULL || i < 0 || i >= c_nomes->n_campos)
    {
        return ITEM_INVALID;
    }
    return c_nomes->tipos[i];
}

int name_pos(const C_NOMES *c_nomes, const char *nome)
{
    if (c_nomes == NULL || nome == NULL)
    {
        return -1;
    }

    for (int i = 0; i < c_nomes->n_campos; i++)
    {
        if (strcmp(c_nomes->nomes[i], nome) == 0)
        {
            return i;
        }
    }
    return -1;
}

const DATATYPE *get_tipos(const C_NOMES *c_nomes)
{
    return (c_nomes == NULL) ? NULL : c_nomes->tipos;
}

int get_n_campos(const C_NOMES *c_nomes)
{
    return (c_nomes == NULL) ? 0 : c_nomes->n_campos;
}

static DATATYPE converte_tipo(const char *tipo)
{
    if (strcmp(tipo, "int") == 0)
    {
        return ITEM_INT;
    }
    if (strcmp(tipo, "double") == 0)
    {
        return ITEM_DOUBLE;
    }
    if (strcmp(tipo, "char") == 0)
    {
        return ITEM_CHAR;
    }
    if (strcmp(tipo, "bool") == 0)
    {
        return ITEM_BOOL;
    }
    if (strcmp(tipo, "string") == 0)
    {
        return ITEM_STRING;
    }
    return ITEM_INVALID;
}

static void libera_nomes(C_NOMES *c_nomes)
{
    for (int j = 0; j < MAX_CAMPOS; j++)
    {
        free(c_nomes->nomes[j]);
        c_nomes->nomes[j] = NULL;
    }
    c_nomes->n_campos = 0;
}
