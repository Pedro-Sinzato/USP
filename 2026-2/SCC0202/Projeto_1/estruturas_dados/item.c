#include <stdlib.h>
#include <string.h>

#include "item.h"

typedef struct field
{
    DATATYPE type;
    union
    {
        int i;
        double d;
        char c;
        char *s;
        bool b;
    } v;
} FIELD;

struct item
{
    int n_fields;
    FIELD *fields;
};

ITEM *create_item(int n_fields, const DATATYPE types[])
{
    if (n_fields <= 0 || types == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < n_fields; i++)
    {
        if (types[i] < ITEM_INT || types[i] > ITEM_BOOL)
        {
            return NULL;
        }
    }

    ITEM *item = malloc(sizeof(*item));
    if (item == NULL)
    {
        return NULL;
    }

    item->fields = calloc((size_t)n_fields, sizeof(*item->fields));
    if (item->fields == NULL)
    {
        free(item);
        return NULL;
    }

    item->n_fields = n_fields;

    for (int i = 0; i < n_fields; i++)
    {
        item->fields[i].type = types[i];
        /* calloc inicializa os valores; strings começam como NULL. */
    }

    return item;
}

bool delete_item(ITEM **item)
{
    if (item == NULL || *item == NULL)
    {
        return false;
    }

    for (int i = 0; i < (*item)->n_fields; i++)
    {
        if ((*item)->fields[i].type == ITEM_STRING)
        {
            free((*item)->fields[i].v.s);
        }
    }

    free((*item)->fields);
    free(*item);
    *item = NULL;

    return true;
}

ITEM *item_clone(const ITEM *item)
{
    if (item == NULL)
    {
        return NULL;
    }

    ITEM *copia = malloc(sizeof(*copia));
    if (copia == NULL)
    {
        return NULL;
    }

    copia->n_fields = item->n_fields;
    copia->fields = calloc((size_t)item->n_fields, sizeof(*copia->fields));
    if (copia->fields == NULL)
    {
        free(copia);
        return NULL;
    }

    for (int i = 0; i < item->n_fields; i++)
    {
        copia->fields[i] = item->fields[i];
        if (item->fields[i].type == ITEM_STRING)
        {
            copia->fields[i].v.s = NULL; /* delete_item já sabe liberar NULL */
        }
    }

    for (int i = 0; i < item->n_fields; i++)
    {
        const char *origem = item->fields[i].v.s;
        if (item->fields[i].type == ITEM_STRING && origem != NULL)
        {
            size_t n = strlen(origem) + 1;
            copia->fields[i].v.s = malloc(n);
            if (copia->fields[i].v.s == NULL)
            {
                delete_item(&copia);
                return NULL;
            }
            memcpy(copia->fields[i].v.s, origem, n);
        }
    }

    return copia;
}

bool item_set(ITEM *item, const void *value, int idx)
{
    if (item == NULL || value == NULL ||
        idx < 0 || idx >= item->n_fields)
    {
        return false;
    }

    FIELD *field = &item->fields[idx];

    switch (field->type)
    {
        case ITEM_INT:
            field->v.i = *(const int *)value;
            break;

        case ITEM_DOUBLE:
            field->v.d = *(const double *)value;
            break;

        case ITEM_CHAR:
            field->v.c = *(const char *)value;
            break;

        case ITEM_BOOL:
            field->v.b = *(const bool *)value;
            break;

        case ITEM_STRING:
        {
            const char *str = (const char *)value;
            size_t len = strlen(str) + 1;

            char *copy = malloc(len);
            if (copy == NULL)
            {
                return false;
            }

            memcpy(copy, str, len);

            free(field->v.s);
            field->v.s = copy;
            break;
        }

        default:
            return false;
    }

    return true;
}
/* Escreve uma string como campo CSV; entre aspas se tiver , " ou quebra de linha. */
static void write_csv_string(const char *s, FILE *out)
{
    if (s == NULL)
    {
        return;                             /* campo vazio */
    }

    if (strpbrk(s, ",\"\r\n") == NULL)
    {
        fputs(s, out);
        return;
    }

    fputc('"', out);
    for (; *s != '\0'; s++)
    {
        if (*s == '"')
        {
            fputc('"', out);                /* aspas dentro do campo são dobradas */
        }
        fputc(*s, out);
    }
    fputc('"', out);
}

bool item_write_csv(const ITEM *item, FILE *out)
{
    if (item == NULL || out == NULL)
    {
        return false;
    }

    for (int i = 0; i < item->n_fields; i++)
    {
        const FIELD *f = &item->fields[i];

        if (i > 0)
        {
            fputc(',', out);
        }

        switch (f->type)
        {
            case ITEM_INT:
                fprintf(out, "%d", f->v.i);
                break;
            case ITEM_DOUBLE:
                fprintf(out, "%.15g", f->v.d);
                break;
            case ITEM_CHAR:
            {
                char s[2] = { f->v.c, '\0' };
                write_csv_string(s, out);
                break;
            }
            case ITEM_STRING:
                write_csv_string(f->v.s, out);
                break;
            case ITEM_BOOL:
                fputs(f->v.b ? "True" : "False", out);
                break;
            case ITEM_INVALID:
                return false; /* campo sem tipo válido */
        }
    }

    fputc('\n', out);

    return ferror(out) == 0;
}
void item_print(const ITEM *item)
{
    if (item == NULL)
    {
        printf("(item nulo)\n");
        return;
    }

    for (int i = 0; i < item->n_fields; i++)
    {
        const FIELD *f = &item->fields[i];

        if (i > 0)
            fputs(" | ", stdout);

        switch (f->type)
        {
            case ITEM_INT:
                printf("%d", f->v.i);
                break;
            case ITEM_DOUBLE:
                printf("%.15g", f->v.d);
                break;
            case ITEM_CHAR:
                printf("'%c'", f->v.c);
                break;
            case ITEM_STRING:
                printf("\"%s\"", f->v.s ? f->v.s : "(null)");
                break;
            case ITEM_BOOL:
                fputs(f->v.b ? "true" : "false", stdout);
                break;
            case ITEM_INVALID:
                fputs("(invalido)", stdout);
                break;
        }
    }

    printf("\n");
}

DATATYPE item_get_type(const ITEM *item, int pos)
{
    if(pos < 0 || pos >= (item->n_fields))
    {
        return -1;
    }
    return item->fields[pos].type;
}
const void *get_field_content(const ITEM *item, int pos)
{
    if (item == NULL || pos < 0 || pos >= item->n_fields)
    {
        return NULL;
    }

    const FIELD *field = &item->fields[pos];
    switch(field->type)
    {
        case ITEM_INT:
        {
            return &field->v.i;
        }
        case ITEM_DOUBLE:
        {
            return &field->v.d;
        }
        case ITEM_CHAR:
        {
            return &field->v.c;
        }
        case ITEM_BOOL:
        {
            return &field->v.b;
        }
        case ITEM_STRING:
        {
            return field->v.s;
        }
        default:
        {
            return NULL;
        }
    }
}

int item_compare_field(const ITEM *a, const ITEM *b, int pos, bool *ok)
{
    if (ok != NULL)
    {
        *ok = true;
    }

    DATATYPE tipo = item_get_type(a, pos);
    const void *ca = get_field_content(a, pos);
    const void *cb = get_field_content(b, pos);

    if (ca == NULL || cb == NULL || tipo != item_get_type(b, pos))
    {
        if (ok != NULL)
        {
            *ok = false;
        }
        return 0;
    }

    switch (tipo)
    {
    case ITEM_INT:
    {
        int x = *(const int *)ca, y = *(const int *)cb;
        return (x > y) - (x < y);
    }
    case ITEM_DOUBLE:
    {
        double x = *(const double *)ca, y = *(const double *)cb;
        return (x > y) - (x < y);
    }
    case ITEM_CHAR:
    {
        char x = *(const char *)ca, y = *(const char *)cb;
        return (x > y) - (x < y);
    }
    case ITEM_BOOL:
    {
        bool x = *(const bool *)ca, y = *(const bool *)cb;
        return (int)x - (int)y;
    }
    case ITEM_STRING:
        return strcmp((const char *)ca, (const char *)cb);
    default:
        if (ok != NULL)
        {
            *ok = false;
        }
        return 0;
    }
}

bool item_field_equals(const ITEM *item, int pos, const void *key)
{
    const void *conteudo = get_field_content(item, pos);
    if (conteudo == NULL || key == NULL)
    {
        return false;
    }

    switch (item_get_type(item, pos))
    {
    case ITEM_INT:
        return *(const int *)conteudo == *(const int *)key;
    case ITEM_DOUBLE:
        return *(const double *)conteudo == *(const double *)key;
    case ITEM_CHAR:
        return *(const char *)conteudo == *(const char *)key;
    case ITEM_BOOL:
        return *(const bool *)conteudo == *(const bool *)key;
    case ITEM_STRING:
        return strcmp((const char *)conteudo, (const char *)key) == 0;
    default:
        return false;
    }
}
