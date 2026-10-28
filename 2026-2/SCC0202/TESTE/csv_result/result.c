#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "result.h"
#include "../estruturas_dados/item.h"
#include "../leitor_argumentos/argument_reader.h"

#define LINE_SIZE 1024

/* Estado do percurso que grava os ITEMs */
typedef struct
{
    FILE *out;
    unsigned long long top_k;           /* 0 = todos */
    unsigned long long written;
    bool ok;
} RESULTADO;

static bool save_visit(const ITEM *item, void *ctx)
{
    RESULTADO *save = ctx;

    if (!item_write_csv(item, save->out))
    {
        save->ok = false;
        return false;
    }

    save->written++;

    return save->top_k == 0 || save->written < save->top_k;
}

/* Copia a primeira linha (cabeçalho) do arquivo input para out. */
static bool copy_header(const char *input, FILE *out)
{
    FILE *in = fopen(input, "r");
    if (in == NULL)
    {
        return false;
    }

    char line[LINE_SIZE];
    bool ok = (fgets(line, sizeof(line), in) != NULL);
    fclose(in);

    if (!ok)
    {
        return false;
    }

    line[strcspn(line, "\r\n")] = '\0';

    return fprintf(out, "%s\n", line) > 0;
}

extern bool save_result(const ED *ed, const ARGUMENTS *arg)
{
    if (ed == NULL || arg == NULL)
    {
        return false;
    }

    const char *input = get_input(arg);
    const char *output = get_output(arg);

    if (output[0] == '\0')
    {
        fprintf(stderr, "-output nao informado\n");
        return false;
    }

    if (strcmp(input, output) == 0)
    {
        fprintf(stderr, "-output nao pode ser o mesmo arquivo de -input\n");
        return false;
    }

    FILE *out = fopen(output, "w");
    if (out == NULL)
    {
        fprintf(stderr, "nao foi possivel criar '%s'\n", output);
        return false;
    }

    if (!copy_header(input, out))
    {
        fprintf(stderr, "nao foi possivel ler o cabecalho de '%s'\n", input);
        fclose(out);
        return false;
    }

    RESULTADO ctx = { out, get_top_k(arg), 0, true };
    for_each_ed(ed, save_visit, &ctx);

    bool closed = (fclose(out) == 0);

    return ctx.ok && closed;
}
