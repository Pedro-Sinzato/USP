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

struct sim
{
    CAMPO campo;
    double alvo;
    double tolerancia;
    double peso;
};

struct query
{
    SIM sims[MAX_SIMS];
    int n_sims;
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

// SIM compara números, então campos de texto não servem.
static bool campo_numerico(CAMPO campo)
{
    return campo != F_INVALID && campo != F_THREAT_CATEGORY;
}

// ATENÇÃO: os nomes dos membros de ASTEROID abaixo são suposições.
// Ajuste para a sua struct real.

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

static bool add_sim(QUERY *query, CAMPO campo, double alvo, double tolerancia, double peso)
{
    if (query->n_sims >= MAX_SIMS || tolerancia <= 0.0)
    {
        return false;
    }

    SIM *sim = &query->sims[query->n_sims++];
    sim->campo = campo;
    sim->alvo = alvo;
    sim->tolerancia = tolerancia;
    sim->peso = peso;

    return true;
}

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
        if (tipo == NULL)
        {
            continue; // linha vazia
        }

        if (!strcmp(tipo, "SIM"))
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
            if (!campo_numerico(campo))
            {
                continue; // campo inválido
            }
            add_sim(query, campo,strtod(s_alvo, NULL),strtod(s_tol, NULL), strtod(s_peso, NULL));
        }
        // else if (!strcmp(tipo, "FILTER")) { ... }
    }

    fclose(query_file);

    return query;
}

//Ignorar daqui pra baixo por enquanto;
double similarity(SIM sim)
{
    int x = 0; //Não sei de onde vem esse x
    double q = sim.alvo;
    double t = sim.tolerancia;
    double result = fmax(0.0, 1 - (fabs(x-q)/t));

    return result;
}

double total_similarity(SIM *sim, int total_sim)
{
    //A similaridade total será a média ponderada dos critérios SIM
    double total_similarity = 0;
    double total_peso = 0;

    if(sim == NULL)
    {
        //Não há SIM
        return -1;
    }
    for(int i  = 0; i < total_sim; i++)
    {
        total_similarity += similarity(sim[i]) * sim[i].peso;
        total_peso+= sim[i].peso;
    }

    double media_ponderada = total_similarity / total_peso;

    return media_ponderada;
}
