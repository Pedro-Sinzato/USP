#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "query.h"

//FILTER: condição obrigatória para o registro ser aceito.
//IM: critério numérico de similaridade, no formato campo|alvo|tolerancia|peso;
//ACCEPT: limiar mínimo para a similaridade total;
//ORDER: chave usada pela lista ordenada;
//GROUP: campos usados pela lista generalizada;
//CROSS: campos usados pela lista cruzada.

typedef struct
{
    CAMPO campo;
    OPERADOR op;
    double valor;
}FILTER;
typedef struct
{
    CAMPO campo;
    double alvo;
    double tolerancia;
    double peso;
}SIM;

typedef struct query{
    FILTER *filters;
    int n_filters;

    SIM    *sims;
    int n_sims;

    double  accept;

    CAMPO order_field;
    bool order_desc;
    CAMPO   group[2];
    CAMPO   cross[2];
}QUERY;

static CAMPO parse_campo(const char *campo)
{
    if (!strcmp(campo, "year")) return F_YEAR;
    if (!strcmp(campo, "distance_au")) return F_DISTANCE_AU;
    if (!strcmp(campo, "velocity_km_s")) return F_VELOCITY_KM_S;
    if (!strcmp(campo, "risk_score")) return F_RISK_SCORE;
    if (!strcmp(campo, "is_future_event")) return F_IS_FUTURE;
    if (!strcmp(campo, "threat_category")) return F_THREAT_CATEGORY;
    if (!strcmp(campo, "threat_category")) return F_ON_SENTRY;
    if (!strcmp(campo, "threat_category")) return F_SENTRY_IMPACT_PROP;
    if (!strcmp(campo, "threat_category")) return F_SENTRY_TORINO_SCALE;
    if (!strcmp(campo, "threat_category")) return F_SENTRY_PALERMO_SCALE;
    if (!strcmp(campo, "threat_category")) return F_SENTRY_DIAMETER_KM;
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

QUERY *create_query(void)
{
    QUERY *query = calloc(1, sizeof(QUERY));
    if(query == NULL)
    {
        return NULL;
    }

    return query;
}

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

bool query_reader(const char *query_name, QUERY *query)
{
    FILE *query_file = fopen(query_name, "r");
    if (query_name == NULL)
    {
        return false;
    }
    char line[256];

    fclose(query_file);
    query_file = NULL;

    return true;
}
