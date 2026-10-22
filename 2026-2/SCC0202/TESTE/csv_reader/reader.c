#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"
#include "../estruturas_dados/estruturas_dados.h"
#include "../estruturas_dados/item.h"
#include "../leitor_argumentos/argument_reader.h"

#define LINE_SIZE 1024

// Devolve o próximo campo da linha e avança *cursor para o campo seguinte.
// Respeita aspas e campos vazios. Devolve NULL se não há mais campos
// ou se há aspas sem fechar. Quando o último campo é lido, *cursor vira NULL.
static char *next_field(char **cursor)
{
    char *start = *cursor;
    if (start == NULL)
        return NULL;

    char *comma;

    if (*start == '"')                      // campo entre aspas, tipo string "nome sobrenome"
    {
        start++;
        char *end = strchr(start, '"');
        if (end == NULL)
        {
            *cursor = NULL;
            return NULL;
        }
        *end = '\0';
        comma = strchr(end + 1, ',');
    }
    else
    {
        comma = strchr(start, ',');
    }

    if (comma != NULL)
    {
        *comma = '\0';
        *cursor = comma + 1;
    }
    else
    {
        *cursor = NULL;                     //último campo
    }

    return start;
}

static char *trim_left(char *s)
{
    while (*s == ' ')
        s++;
    return s;
}

// Converte o texto para o tipo do campo e grava no item. Vazio vira 0.
static bool set_field_from_text(ITEM *item, DATATYPE type, const char *text, int idx)
{
    switch (type)
    {
        case ITEM_INT:
        {
            int v = (int)strtol(text, NULL, 10);
            return item_set(item, &v, idx);
        }
        case ITEM_DOUBLE:
        {
            double v = strtod(text, NULL);
            return item_set(item, &v, idx);
        }
        case ITEM_CHAR:
        {
            char v = text[0];
            return item_set(item, &v, idx);
        }
        case ITEM_BOOL:
        {
            bool v = (strcmp(text, "True") == 0);
            return item_set(item, &v, idx);
        }
        case ITEM_STRING:
            return item_set(item, text, idx);
        default:
            return false;
    }
}

bool reader(ED *ed, ARGUMENTS *arg, const DATATYPE tipos[], int n_campos)
{
    if (ed == NULL || arg == NULL || tipos == NULL || n_campos <= 0)
    {
        return false;
    }

    FILE *database = fopen(get_input(arg), "r");
    if (database == NULL)
    {
        return false;
    }

    char line[LINE_SIZE];

    // Pula o cabeçalho
    if (fgets(line, sizeof(line), database) == NULL)
    {
        fclose(database);
        return true;                        // arquivo vazio
    }

    unsigned long long limit = get_limit(arg);   // 0 = todos
    unsigned long long lidos = 0;

    while ((limit == 0 || lidos < limit) && fgets(line, sizeof(line), database))
    {
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0')
            continue;                       // linha em branco (não conta)

        ITEM *item = create_item(n_campos, tipos);
        if (item == NULL)
        {
            fclose(database);
            return false;
        }

        char *cursor = line;
        bool ok = true;

        for (int i = 0; i < n_campos && ok; i++)
        {
            char *field = next_field(&cursor);
            ok = (field != NULL) &&
                 set_field_from_text(item, tipos[i], trim_left(field), i);
        }

        if (cursor != NULL)                 // sobraram campos na linha
            ok = false;

        if (!ok || !insert_ed(ed, item))
        {
            delete_item(&item);
            fclose(database);
            return false;
        }

        lidos++;
    }

    fclose(database);
    return true;
}
