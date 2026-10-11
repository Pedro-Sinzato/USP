#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "query.h"

// FILTER: condição obrigatória para o registro ser aceito.
// SIM: critério numérico de similaridade, no formato campo|alvo|tolerancia|peso;
// ACCEPT: limiar mínimo para a similaridade total;
// ORDER: chave usada pela lista ordenada;
// GROUP: campos usados pela lista generalizada;
// CROSS: campos usados pela lista cruzada.

#define MAX_SIMS 10

typedef struct
{
    CAMPO campo;
    OPERADOR operador;
    double valor;
} FILTER;

struct query
{
    SIM sims[MAX_SIMS];
    int n_sims;
};

//Usado para testar
static const char *string_campo[] = {
    [F_YEAR]                 = "year",
    [F_DISTANCE_AU]          = "distance_au",
    [F_VELOCITY_KM_S]        = "velocity_km_s",
    [F_RISK_SCORE]           = "risk_score",
    [F_IS_FUTURE]            = "is_future_event",
    [F_THREAT_CATEGORY]      = "threat_category",
    [F_ON_SENTRY]            = "on_sentry_list",
    [F_SENTRY_IMPACT_PROP]   = "sentry_impact_prob",
    [F_SENTRY_TORINO_SCALE]  = "sentry_torino_scale",
    [F_SENTRY_PALERMO_SCALE] = "sentry_palermo_scale",
    [F_SENTRY_DIAMETER_KM]   = "sentry_diameter_km",
    [F_INVALID]              = "invalid"
};

static CAMPO parse_campo(const char *campo)
{
    if (!strcmp(campo, "year")) return F_YEAR;
    if (!strcmp(campo, "distance_au")) return F_DISTANCE_AU;
    if (!strcmp(campo, "velocity_km_s")) return F_VELOCITY_KM_S;
    if (!strcmp(campo, "risk_score")) return F_RISK_SCORE;
    if (!strcmp(campo, "is_future_event")) return F_IS_FUTURE;
    if (!strcmp(campo, "threat_category")) return F_THREAT_CATEGORY;
    if (!strcmp(campo, "on_sentry_list")) return F_ON_SENTRY;
    if (!strcmp(campo, "sentry_impact_prob")) return F_SENTRY_IMPACT_PROP;
    if (!strcmp(campo, "sentry_torino_scale")) return F_SENTRY_TORINO_SCALE;
    if (!strcmp(campo, "sentry_palermo_scale")) return F_SENTRY_PALERMO_SCALE;
    if (!strcmp(campo, "sentry_diameter_km")) return F_SENTRY_DIAMETER_KM;
    return F_INVALID;
}

static OPERADOR parse_operador(const char *operador)
{
    if (!strcmp(operador, "==")) return OP_EQ;
    if (!strcmp(operador, "!=")) return OP_NE;
    if (!strcmp(operador, "<"))  return OP_LT;
    if (!strcmp(operador, "<=")) return OP_LE;
    if (!strcmp(operador, ">"))  return OP_GT;
    if (!strcmp(operador, ">=")) return OP_GE;
    return OP_INVALID;
}

static QUERY *create_query(void)
{
    QUERY *query = calloc(1, sizeof(QUERY));
    if(query == NULL)
    {
        return NULL;
    }
    return query;
}

extern bool delete_query(QUERY *query)
{
    if (query == NULL)
    {
        return false;
    }

    free(query);
    return true;
}

//Funciona!!!
QUERY *query_reader(const char *query_name)
{
    FILE *query_file = fopen(query_name, "r");
    if (query_file == NULL)
    {
        return NULL;
    }

    QUERY *query = create_query();
    if (query == NULL)
    {
        fclose(query_file);
        return NULL;
    }

    char line[128];
    while (fgets(line, sizeof(line), query_file) != NULL)
    {
        line[strcspn(line, "\r\n")] = '\0'; // remove o \n

        char *tipo = strtok(line, "|");

        if (strcmp(tipo, "SIM") == 0)
        {
            char *s_campo = strtok(NULL, "|");
            char *s_alvo  = strtok(NULL, "|");
            char *s_tol   = strtok(NULL, "|");
            char *s_peso  = strtok(NULL, "|");

            if (!s_campo || !s_alvo || !s_tol || !s_peso)
            {
                continue; // linha incompleta
            }

            CAMPO campo = parse_campo(s_campo);
            add_sim(query, campo,strtod(s_alvo, NULL),strtod(s_tol, NULL), strtod(s_peso, NULL));
        }
        else if(strcmp(tipo, "FILTER"))
        {
            char *s_campo = strtok(NULL, "|");
            char *s_operador  = strtok(NULL, "|");
            char *s_condicao   = strtok(NULL, "|"); //Pode ser true|false ou double
        }
    }

    fclose(query_file);

    return query;
}

//Teste se a leitura funcionou
void print_query(QUERY *query)
{
    int n_sims = query->n_sims;

    printf("Numero de SIMS: %d\n", n_sims);

    for(int i = 0; i < n_sims; i++)
    {
        int campo = query->sims[i].campo;
        double alvo = query->sims[i].alvo;
        double tolerancia = query->sims[i].tolerancia;
        double peso = query->sims[i].peso;
        printf("%s|%lf|%lf|%lf\n", string_campo[campo], alvo, tolerancia, peso);
    }
}
